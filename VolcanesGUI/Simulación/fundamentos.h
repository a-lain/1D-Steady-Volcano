#ifndef FUNDAMENTOS_H
#define FUNDAMENTOS_H

#include "solveDiff.h"
#include "útil.h"

struct PropiedadesMagma
{
    double T0; // Temperatura del magma
    double rho_l; // Densidad del magma
    double s; // Valor de s de la curva de solubilidad de Koyaguchi
    double n0; // Fracción másica inicial de gas
    double eta0; // Visosidad de la mezcla
    double Rg; // Constante del gas (R gases ideales / Masa molar gas)
    double phiCriticoKoyaguchi; // Densidad volumétrica crítica de gas
};


struct PropiedadesConducto
{
    std::function<double(double)> A; // Área Koyaguchi del conducto
    double f0; // Coeficiente de rozamiento Koyaguchi
    std::function<double(double)> dAdz; // Derivada de la sección del conducto respecto a z
};


class Estado
{
    public:
    double A; // Área del conducto
    double z; // Profundidad;
    double P; // Presión
    double u; // velocidad
    double rho; // Densidad de la mezcla
    double rho_g; // Densidad del gas
    double n; // Fracción másica de gas
    double c; // Velocidad del sonido en la mezcla
    double M; // Número de Mach
    double F; // Fuerza de rozamiento
    double phi; // Densidad volumétrica de gas

    string ATexto();
    void GuardaEnFichero(FILE* F);
    void EscribePorPantalla();
};

class Datos
{
    public:
    double T0;
    double rho_l;
    double eta0;
    double s;
    double n0;
    double Rg;
    double A;
    double f0;
    double g;
    double P0;
    double z0;
    double dP;
    double eps;
    int Ndatos;
    int criterio;
    double phi_critico;
    int modo;
    double u0;
    double u01;
    double u02;
    int Nu;
    double Pb;
    int koyaguchi_analitico;

    Datos(FILE *F);
    Datos();
    string ATexto();
    void GuardaEnFichero(FILE* F);
    void EscribePorPantalla();

};

#endif
