#ifndef SOLVEDIFF_H
#define SOLVEDIFF_H
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/LU>
#include <math.h>
#include "progreso.h"

using namespace Eigen;

VectorXd EulerExplicito(VectorXd y, double t, std::function<VectorXd(VectorXd,double)> f, double dt);
VectorXd RungeKuttaOrden4(VectorXd y, double t, std::function<VectorXd(VectorXd, double)> f, double dt);
void RungeKuttaAdaptativo(VectorXd& y, double t, double epsilon,
    std::function<VectorXd(VectorXd, double)> f, double& dt1, double& dzSig, double dtmin);
double NewtonRaphson(double x0, std::function<Vector2d(double)> fYdfdx, double tol, int iterMax, int debug, Progreso& progreso);
double Biseccion(double a, double b, std::function<double(double)> f, double tol, int iterMax,
                 Progreso &progreso);
double Secante(double x1, double x2, std::function<double(double)> f, double tol, int iterMax);
double Brent(double x1, double x2, std::function<double(double)> f, double tol, int iterMax);
VectorXd Broyden(std::function<VectorXd(VectorXd)> F, VectorXd x0, MatrixXd Jx0, double tol, int iterMax);
std::function<double(double)> InterpolacionCuadratica(double f1, double x1, double f2, double x2, double f3, double x3);

#endif
