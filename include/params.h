#ifndef OPTIMALLANDING_PARAMS_H
#define OPTIMALLANDING_PARAMS_H

#include <iostream>
#include <cmath>

#include <Eigen/Dense>
#include <Eigen/Core>
#include <boost/numeric/odeint.hpp>
#include <boost/numeric/odeint/external/eigen/eigen_algebra.hpp>

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Vector3d;
using Eigen::Vector4d;
using Eigen::Matrix;

typedef Matrix<double, 14, 1> Vector14d;
typedef Matrix<double, 14, 14> Matrix14d;
typedef Matrix<double, 14, 3> Matrix14x3d;

//trajectory points
unsigned int K = 50;
double dt = 1 / 50;

int iterations = 15;

double sigma_guess = 2.;

//initial state
double m_wet = 2;
Vector3d r_I_init(4., 4., 0.);
Vector3d v_I_init(0., -2., -2.);
Vector4d q_B_I_init(1.0, 0.0, 0.0, 0.0);
Vector3d w_B_init(0., 0., 0.);
VectorXd x_init(14);

//final state
double m_dry = 1;
Vector3d r_I_final(0., 0., 0.);
Vector3d v_I_final(-1e-1, 0., 0.);
Vector4d q_B_I_final(1.0, 0.0, 0.0, 0.0);
Vector3d w_B_final(0., 0., 0.);
VectorXd x_final(14);

//gravity vector
Vector3d g_I(-1, 0, 0);
Vector3d J_B(1e-2, 1e-2, 1e-2);
Vector3d r_T_B(-1e-2, 0, 0);

double alpha_m = 0.02;

//A matrix
Matrix14d A(Vector14d x, Vector3d u, double sigma){
    double t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24;
    double t25, t26, t27, t28, t29, t30, t31, t32;
    Matrix14d A;
    t2 = 1.0/(x[0]*x[0]);
    t3 = 1.0/x[0];
    t4 = x[10]*x[10];
    t5 = t4*2.0;
    t6 = x[7]*x[10]*2.0;
    t7 = x[8]*x[9]*2.0;
    t8 = t3*u[2]*x[8]*2.0;
    t9 = t3*u[2]*x[10]*2.0;
    t10 = x[8]*x[8];
    t11 = t10*2.0;
    t12 = x[9]*x[9];
    t13 = t12*2.0;
    t14 = x[7]*x[9]*2.0;
    t15 = x[7]*x[8]*2.0;
    t16 = x[9]*x[10]*2.0;
    t17 = t3*u[0]*x[9]*2.0;
    t18 = t3*u[1]*x[8]*2.0;
    t19 = t3*u[1]*x[7]*2.0;
    t20 = t3*u[1]*x[10]*2.0;
    t21 = t3*u[0]*x[8]*2.0;
    t22 = t3*u[1]*x[9]*2.0;
    t23 = x[13]*(1.0/2.0);
    t24 = x[11]*(1.0/2.0);
    t25 = x[7]*(1.0/2.0);
    t26 = x[12]*(1.0/2.0);
    t27 = x[9]*(1.0/2.0);
    t28 = 1.0/J_B[0];
    t29 = 1.0/J_B[1];
    t30 = J_B[1]*x[12];
    t31 = 1.0/J_B[2];
    t32 = J_B[0]*x[11];
    A(1, 4) = 1.0;
    A(2, 5) = 1.0;
    A(3, 6) = 1.0;
    A(4, 0) = t2*u[2]*(t14-x[8]*x[10]*2.0)-t2*u[1]*(t6+t7)+t2*u[0]*(t5+t13-1.0);
    A(4, 7) = t20-t3*u[2]*x[9]*2.0;
    A(4, 8) = t9+t22;
    A(4, 9) = t18-t3*u[0]*x[9]*4.0-t3*u[2]*x[7]*2.0;
    A(4, 10) = t8+t19-t3*u[0]*x[10]*4.0;
    A(5, 0) = t2*u[1]*(t5+t11-1.0)-t2*u[2]*(t15+t16)+t2*u[0]*(t6-t7);
    A(5, 7) = t8-t3*u[0]*x[10]*2.0;
    A(5, 8) = t17-t3*u[1]*x[8]*4.0+t3*u[2]*x[7]*2.0;
    A(5, 9) = t9+t21;
    A(5, 10) = t3*u[0]*x[7]*-2.0-t3*u[1]*x[10]*4.0+t3*u[2]*x[9]*2.0;
    A(6, 0) = -t2*u[0]*(t14+x[8]*x[10]*2.0)+t2*u[2]*(t11+t13-1.0)+t2*u[1]*(t15-t16);
    A(6, 7) = t17-t18;
    A(6, 8) = -t19+t3*u[0]*x[10]*2.0-t3*u[2]*x[8]*4.0;
    A(6, 9) = t20+t3*u[0]*x[7]*2.0-t3*u[2]*x[9]*4.0;
    A(6, 10) = t21+t22;
    A(7, 8) = x[11]*(-1.0/2.0);
    A(7, 9) = x[12]*(-1.0/2.0);
    A(7, 10) = x[13]*(-1.0/2.0);
    A(7, 11) = x[8]*(-1.0/2.0);
    A(7, 12) = x[9]*(-1.0/2.0);
    A(7, 13) = x[10]*(-1.0/2.0);
    A(8, 7) = t24;
    A(8, 9) = t23;
    A(8, 10) = x[12]*(-1.0/2.0);
    A(8, 11) = t25;
    A(8, 12) = x[10]*(-1.0/2.0);
    A(8, 13) = t27;
    A(9, 7) = t26;
    A(9, 8) = -t23;
    A(9, 10) = t24;
    A(9, 11) = x[10]*(1.0/2.0);
    A(9, 12) = t25;
    A(9, 13) = x[8]*(-1.0/2.0);
    A(10, 7) = t23;
    A(10, 8) = t26;
    A(10, 9) = -t24;
    A(10, 11) = -t27;
    A(10, 12) = x[8]*(1.0/2.0);
    A(10, 13) = t25;
    A(11, 12) = t28*(J_B[1]*x[13]-J_B[2]*x[13]);
    A(11, 13) = t28*(t30-J_B[2]*x[12]);
    A(12, 11) = -t29*(J_B[0]*x[13]-J_B[2]*x[13]);
    A(12, 13) = -t29*(t32-J_B[2]*x[11]);
    A(13, 11) = -t31*(t30-J_B[0]*x[12]);
    A(13, 12) = t31*(t32-J_B[1]*x[11]);
    return sigma * A;
}
//B matrix
Matrix14x3d B(Vector14d x, Vector3d u, double sigma){
    double t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24;
    Matrix14x3d B;
    t2 = fabs(u[0]);
    t3 = fabs(u[1]);
    t4 = fabs(u[2]);
    t5 = t2*t2;
    t6 = t3*t3;
    t7 = t4*t4;
    t8 = t5+t6+t7;
    t9 = 1.0/sqrt(t8);
    t10 = 1.0/x[0];
    t11 = x[7]*x[10]*2.0;
    t12 = x[8]*x[9]*2.0;
    t13 = x[10]*x[10];
    t14 = t13*2.0;
    t15 = x[7]*x[9]*2.0;
    t16 = x[7]*x[8]*2.0;
    t17 = x[9]*x[10]*2.0;
    t18 = x[8]*x[8];
    t19 = t18*2.0;
    t20 = x[9]*x[9];
    t21 = t20*2.0;
    t22 = 1.0/J_B[0];
    t23 = 1.0/J_B[1];
    t24 = 1.0/J_B[2];
    B(0, 0) = -alpha_m*t2*t9*((u[0]/fabs(u[0])));
    B(0, 1) = -alpha_m*t3*t9*((u[1]/fabs(u[1])));
    B(0, 2) = -alpha_m*t4*t9*((u[2]/fabs(u[2])));
    B(4, 0) = -t10*(t14+t21-1.0);
    B(4, 1) = t10*(t11+t12);
    B(4, 2) = -t10*(t15-x[8]*x[10]*2.0);
    B(5, 0) = -t10*(t11-t12);
    B(5, 1) = -t10*(t14+t19-1.0);
    B(5, 2) = t10*(t16+t17);
    B(6, 0) = t10*(t15+x[8]*x[10]*2.0);
    B(6, 1) = -t10*(t16-t17);
    B(6, 2) = -t10*(t19+t21-1.0);
    B(11, 1) = -r_T_B[2]*t22;
    B(11, 2) = r_T_B[1]*t22;
    B(12, 0) = r_T_B[2]*t23;
    B(12, 2) = -r_T_B[0]*t23;
    B(13, 0) = -r_T_B[1]*t24;
    B(13, 1) = r_T_B[0]*t24;

    return sigma * B;
}
//f vector
Vector14d f(Vector14d x, Vector3d u){
    Vector14d f;
    f <<
      -alpha_m * u.norm(),
            x[4],
            x[5],
            x[6],
            g_I[0] - (u[0] * (2 * pow(x[9], 2) + 2 * pow(x[10], 2) - 1)) / x[0] + (u[1] * (2 * x[7] * x[10] + 2 * x[8] * x[9])) / x[0] - (
                                                                                                                                                 u[2] * (2 * x[7] * x[9] - 2 * x[8] * x[10])) / x[0],
            g_I[1] - (u[1] * (2 * pow(x[8], 2) + 2 * pow(x[10], 2) - 1)) / x[0] - (u[0] * (2 * x[7] * x[10] - 2 * x[8] * x[9])) / x[0] + (
                                                                                                                                                 u[2] * (2 * x[7] * x[8] + 2 * x[9] * x[10])) / x[0],
            g_I[2] - (u[2] * (2 * pow(x[8], 2) + 2 * pow(x[9], 2) - 1)) / x[0] + (u[0] * (2 * x[7] * x[9] + 2 * x[8] * x[10])) / x[0] - (
                                                                                                                                                u[1] * (2 * x[7] * x[8] - 2 * x[9] * x[10])) / x[0],
            - (x[8] * x[11]) / 2 - (x[9] * x[12]) / 2 - (x[10] * x[13]) / 2,
            (x[7] * x[11]) / 2 + (x[9] * x[13]) / 2 - (x[10] * x[12]) / 2,
            (x[7] * x[12]) / 2 - (x[8] * x[13]) / 2 + (x[10] * x[11]) / 2,
            (x[7] * x[13]) / 2 + (x[8] * x[12]) / 2 - (x[9] * x[11]) / 2,
            (r_T_B[1] * u[2] - r_T_B[2] * u[1] + J_B[1] * x[12] * x[13] - J_B[2] * x[12] * x[13]) / J_B[0],
            - (r_T_B[0] * u[2] - r_T_B[2] * u[0] + J_B[0] * x[11] * x[13] - J_B[2] * x[11] * x[13]) / J_B[1],
            (r_T_B[0] * u[1] - r_T_B[1] * u[0] + J_B[0] * x[11] * x[12] - J_B[1] * x[11] * x[12]) / J_B[2];

    return f;
}

#endif //OPTIMALLANDING_PARAMS_H