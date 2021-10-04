#ifndef analyticalKoyaguchi_h
#define analyticalKoyaguchi_h

#include "fundamentos.h"
#include "solveDiff.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "progreso.h"

using namespace std;


class ParametrosKoyaguchiAnalitico
{
    public:
    double Lt;
    double P_f;
    double n_f;
    double z_b;
    double alpha;
    double beta;
    double gamma;
    double delta;
    double lambda;
    double L_g;
    double L_b;
    double q;
    double F;
    double P_v;

    string ATexto();
    void EscribePorPantalla();
};


ParametrosKoyaguchiAnalitico CalculaParametros(double P0, PropiedadesMagma magma, PropiedadesConducto conducto, double Lt, double g, 
    double tol, Progreso &progreso);
double Burbujeante(PropiedadesMagma magma, double P0, double z, ParametrosKoyaguchiAnalitico param, double g);
double GasPiroclasto(PropiedadesMagma magma, double z, ParametrosKoyaguchiAnalitico param, double CTE, double estimP,  double g, double tol,
                    Progreso &progreso);
double integral(double p, PropiedadesMagma magma, ParametrosKoyaguchiAnalitico param);
double Presion(PropiedadesMagma magma, double P0, double z, ParametrosKoyaguchiAnalitico param, double CTE, double estimP, double g,
                double tol, Progreso& progreso);



#endif // KoyaguchiAnalítico_h
