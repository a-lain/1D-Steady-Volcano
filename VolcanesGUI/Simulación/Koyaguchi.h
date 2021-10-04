#ifndef KOYAGUCHI_H
#define KOYAGUCHI_H

#include "fundamentos.h"
#include "solveDiff.h"
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include "progreso.h"

using namespace std;

double RozamientoAntesFragmentKoyaguchi(Estado psi,
    PropiedadesMagma magma, PropiedadesConducto conducto);
double RozamientoDespuesFragmentKoyaguchi(Estado psi,
    PropiedadesMagma magma, PropiedadesConducto conducto);
int AlcanzadoNivelFragmentacionKoyaguchiVolumenCritico(Estado psi,
    PropiedadesMagma magma, PropiedadesConducto conducto);
Estado ActualizaEstadoKoyaguchi(double P, double q, double z,
    PropiedadesMagma magma, PropiedadesConducto conducto,
    int alcanzadoNivelFragmentacion);
Estado Psi0Koyaguchi(double z0, double P0, double u0,
    PropiedadesMagma magma, PropiedadesConducto conducto);
void EvolucionaKoyaguchi(Estado& psi, std::function<VectorXd(VectorXd, double)> f, PropiedadesMagma magma, PropiedadesConducto conducto,
    double q, int& alcanzadoNivelFragmentacion, double epsilon, double& dP,
    int (*AlcanzadoNivelFragmentacion) (Estado, PropiedadesMagma, PropiedadesConducto),
    double tol, Progreso &progreso);
void ResuelveKoyaguchi(vector<Estado>& v, Estado& psiFrag, double& q, Estado psi, PropiedadesMagma magma, PropiedadesConducto conducto,
    double g, double epsilon, double dP, double tol,
    int (*AlcanzadoNivelFragmentacion) (Estado, PropiedadesMagma, PropiedadesConducto),
    Progreso &progreso);
double u0Koyaguchi(double z0, double P0, PropiedadesMagma magma, PropiedadesConducto conducto,
    double g, double Pb, double epsilon, double dP, double tol,
    int (*AlcanzadoNivelFragmentacion) (Estado, PropiedadesMagma, PropiedadesConducto),
    Progreso &progreso);


#endif // KOYAGUCHI_H
