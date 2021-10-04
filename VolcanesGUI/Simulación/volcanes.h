#ifndef VOLCANES_H
#define VOLCANES_H

#include "fundamentos.h"
#include "Koyaguchi.h"
#include "progreso.h"
#include "analyticalKoyaguchi.h"

using namespace std;

vector<Estado> Simula(Datos datos, Estado &psiFrag, double &q, double &u0max, Progreso &progreso, int& error);
vector<Estado> EntradaVelocidadInicialSalidaPerfil(Datos datos, PropiedadesMagma magma,
                                                   PropiedadesConducto conducto,
                                                   Estado &psiFrag, double&q, Progreso &progreso, int& error);
vector<Estado> EntradaRangoVelocidadesInicialesSalidaValoresEnLaBoca(Datos datos, PropiedadesMagma magma,
                                                                     PropiedadesConducto conducto,
                                                                     double &u0max, Progreso &progreso, int& error);
vector<Estado> EntradaPresionEnLaBocaSalidaPerfil(Datos datos, PropiedadesMagma magma,
                                                  PropiedadesConducto conducto,
                                                  Estado &psiFrag, double &u0max, Progreso &progreso, int& error);
vector<Estado> KoyaguchiAnalitico(Datos datos, vector<Estado> v, ParametrosKoyaguchiAnalitico &param,
                                    Progreso &progeso, int& error);

#endif // VOLCANES_H
