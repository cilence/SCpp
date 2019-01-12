#pragma once

#include <cppad/cppad.hpp>
#include <cppad/example/cppad_eigen.hpp>

#include "optimizationProblem.hpp"

template <size_t STATE_DIM, size_t INPUT_DIM, size_t K>
class SystemModel
{
  public:
    typedef double default_t;
    typedef Eigen::Matrix<default_t, STATE_DIM, 1> state_vector_t;
    typedef Eigen::Matrix<default_t, STATE_DIM, STATE_DIM> state_matrix_t;
    typedef Eigen::Matrix<default_t, INPUT_DIM, 1> input_vector_t;
    typedef Eigen::Matrix<default_t, INPUT_DIM, INPUT_DIM> input_matrix_t;
    typedef Eigen::Matrix<default_t, STATE_DIM, INPUT_DIM> control_matrix_t;
    typedef Eigen::Matrix<default_t, Eigen::Dynamic, 1> dynamic_vector_t;

    typedef CppAD::AD<default_t> scalar_ad_t;
    typedef Eigen::Matrix<scalar_ad_t, STATE_DIM, 1> state_vector_ad_t;
    typedef Eigen::Matrix<scalar_ad_t, STATE_DIM, STATE_DIM> state_matrix_ad_t;
    typedef Eigen::Matrix<scalar_ad_t, INPUT_DIM, 1> input_vector_ad_t;
    typedef Eigen::Matrix<scalar_ad_t, INPUT_DIM, INPUT_DIM> input_matrix_ad_t;
    typedef Eigen::Matrix<scalar_ad_t, STATE_DIM, INPUT_DIM> control_matrix_ad_t;
    typedef Eigen::Matrix<scalar_ad_t, Eigen::Dynamic, 1> dynamic_vector_ad_t;

    enum : size_t
    {
        state_dim_ = STATE_DIM,
        input_dim_ = INPUT_DIM,
        domain_dim_ = (STATE_DIM + INPUT_DIM),
    };

    SystemModel();

    virtual void systemFlowMap(
        const state_vector_ad_t &x,
        const input_vector_ad_t &u,
        state_vector_ad_t &f) = 0;

    void initializeModel();
    void computef(const state_vector_t &x, const input_vector_t &u, state_vector_t &f);
    void computeA(const state_vector_t &x, const input_vector_t &u, state_matrix_t &A);
    void computeB(const state_vector_t &x, const input_vector_t &u, control_matrix_t &B);

    void initializeTrajectory(Eigen::Matrix<default_t, STATE_DIM, K> &X,
                              Eigen::Matrix<default_t, INPUT_DIM, K> &U);

    virtual void addApplicationConstraints(
        optimization_problem::SecondOrderConeProgram &socp,
        Eigen::Matrix<default_t, STATE_DIM, K> &X0,
        Eigen::Matrix<default_t, INPUT_DIM, K> &U0) = 0;

  private:
    string modelName_;

    CppAD::ADFun<default_t> f_;
    vector<bool> sparsityA_, sparsityB_;
    vector<size_t> rowsA_, rowsB_;
    vector<size_t> colsA_, colsB_;
    CppAD::sparse_jacobian_work workA_, workB_;
};

template <size_t STATE_DIM, size_t INPUT_DIM, size_t K>
SystemModel<STATE_DIM, INPUT_DIM, K>::SystemModel()
{
}

template <size_t STATE_DIM, size_t INPUT_DIM, size_t K>
void SystemModel<STATE_DIM, INPUT_DIM, K>::initializeModel()
{
    // set up spasity
    sparsityA_.resize((STATE_DIM + INPUT_DIM) * STATE_DIM);
    sparsityB_.resize((STATE_DIM + INPUT_DIM) * STATE_DIM);
    rowsA_.reserve(STATE_DIM * STATE_DIM);
    colsA_.reserve(STATE_DIM * STATE_DIM);
    rowsB_.reserve(INPUT_DIM * STATE_DIM);
    colsB_.reserve(INPUT_DIM * STATE_DIM);
    for (size_t i = 0; i < (STATE_DIM + INPUT_DIM) * STATE_DIM; i++)
    {
        if (i < STATE_DIM * STATE_DIM)
        {
            sparsityA_[i] = true;
            sparsityB_[i] = false;
            rowsA_.emplace_back(int(i / STATE_DIM));
            colsA_.emplace_back(i % STATE_DIM);
        }
        else
        {
            sparsityA_[i] = false;
            sparsityB_[i] = true;
            rowsB_.emplace_back(int(i / STATE_DIM));
            colsB_.emplace_back(i % STATE_DIM);
        }
    }

    // record the model
    {
        // input vector
        dynamic_vector_ad_t x(STATE_DIM + INPUT_DIM);
        x.setRandom();

        // declare x as independent
        CppAD::Independent(x);

        // create fixed size types since CT uses fixed size types
        state_vector_ad_t xFixed = x.template head<STATE_DIM>();
        input_vector_ad_t uFixed = x.template tail<INPUT_DIM>();

        state_vector_ad_t dxFixed;

        systemFlowMap(xFixed, uFixed, dxFixed);

        // output vector, needs to be dynamic size
        dynamic_vector_ad_t dx(STATE_DIM);
        dx = dxFixed;

        // store operation sequence in f: x -> dx and stop recording
        CppAD::ADFun<default_t> f(x, dx);

        f.optimize();

        f_ = f;
    }
}

template <size_t STATE_DIM, size_t INPUT_DIM, size_t K>
void SystemModel<STATE_DIM, INPUT_DIM, K>::computef(const state_vector_t &x, const input_vector_t &u, state_vector_t &f)
{
    f.setOnes();
}

template <size_t STATE_DIM, size_t INPUT_DIM, size_t K>
void SystemModel<STATE_DIM, INPUT_DIM, K>::computeA(const state_vector_t &x, const input_vector_t &u, state_matrix_t &A)
{
    dynamic_vector_t input(STATE_DIM + INPUT_DIM);
    input << x, u;

    dynamic_vector_t jac(STATE_DIM * STATE_DIM);
    f_.SparseJacobianForward(input, sparsityA_, rowsA_, colsA_, jac, workA_);

    Eigen::Map<state_matrix_t> out(jac.data());

    A = out;
}

template <size_t STATE_DIM, size_t INPUT_DIM, size_t K>
void SystemModel<STATE_DIM, INPUT_DIM, K>::computeB(const state_vector_t &x, const input_vector_t &u, control_matrix_t &B)
{
    dynamic_vector_t input(STATE_DIM + INPUT_DIM);
    input << x, u;

    dynamic_vector_t jac(STATE_DIM * INPUT_DIM);
    f_.SparseJacobianForward(input, sparsityB_, rowsB_, colsB_, jac, workB_);

    Eigen::Map<control_matrix_t> out(jac.data());

    B = out;
}