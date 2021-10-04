#include "analyticalKoyaguchi.h"


string ParametrosKoyaguchiAnalitico::ATexto()
{
    string texto;
    texto += printfToString("Paramétros Modelo Koyaguchi:\n");
    texto += printfToString("\tLt = %lf (m)\n", Lt);
    texto += printfToString("\tP_f = %lf (MPa)\n", P_f/1e6);
    texto += printfToString("\tn_f = %lf\n", n_f);
    texto += printfToString("\tz_b = %lf (m)\n", z_b);
    texto += printfToString("\tα = %lf\n", alpha);
    texto += printfToString("\tβ = %lf\n", beta);
    texto += printfToString("\tγ = %lf\n", gamma);
    texto += printfToString("\tδ = %lf\n", delta);
    texto += printfToString("\tλ = %lf\n", lambda);
    texto += printfToString("\tL_g = %lf (m)\n", L_g);
    texto += printfToString("\tL_b = %lf (m)\n", L_b);
    texto += printfToString("\tq = %lf (kg/s)\n", q);
    texto += printfToString("\tF = %lf (N)\n", F);
    texto += printfToString("\tP_v = %lf (MPa)\n", P_v/1e6);
    texto += printfToString("Fin paramétros Modelo Koyaguchi.\n");
    return texto;
}

void ParametrosKoyaguchiAnalitico::EscribePorPantalla()
{
    printf("%s", ATexto().c_str());
}

ParametrosKoyaguchiAnalitico CalculaParametros(double P0, PropiedadesMagma magma, PropiedadesConducto conducto, double Lt, double g, 
        double tol, Progreso &progreso){
    progreso.AgnadeTarea("Calculando parámetros...");
    ParametrosKoyaguchiAnalitico param;
    param.Lt = Lt;
    double B=(magma.rho_l*(1-magma.phiCriticoKoyaguchi))*magma.Rg*magma.T0/magma.phiCriticoKoyaguchi/(1-magma.n0);
    double P_f=param.P_f=pow(((sqrt(pow((magma.s*B),2)+4*magma.n0*B)-magma.s*B)/2),2);
    double n_f=param.n_f=(magma.n0-magma.s*sqrt(P_f))/(1-magma.s*sqrt(P_f));
    double z_b=param.z_b=(P0-P_f)/magma.rho_l/g;
    double gamma=param.gamma=magma.rho_l*magma.rho_l*g*conducto.A(0)*sqrt(n_f*magma.Rg*magma.T0)/8/magma.eta0/P_f/M_PI;
    double delta=param.delta=(z_b-Lt)/Lt;
    double lambda=param.lambda=g*Lt/n_f/magma.Rg/magma.T0;

    auto fYdfdx = [&] (double beta) -> Vector2d{
        Vector2d v;
        double f0 = exp(lambda*(gamma*((1+delta)/(gamma+beta))-1))-beta;
        v << f0, 
            -(lambda*gamma*(f0+beta)*((1+delta)/(gamma+beta)/(gamma+beta)))-1;
        return v;
    };
    double beta = param.beta = NewtonRaphson(exp(-lambda), fYdfdx, tol, 1000, 0, progreso);
    double L_g=param.L_g=-n_f*magma.Rg*magma.T0/g*(log(beta)-(1-magma.phiCriticoKoyaguchi)/magma.phiCriticoKoyaguchi*(1-beta)*(1-beta)+(1-beta*beta)/2);
    double L_b=param.L_b=Lt-L_g;
    double q=param.q=beta*P_f/sqrt(n_f*magma.Rg*magma.T0)*conducto.A(0);
    double F=param.F=M_PI*8*magma.eta0*q/magma.rho_l/magma.rho_l/conducto.A(0)/conducto.A(0);
    double P_v=param.P_v=beta*P_f;
    double alpha=param.alpha=beta/gamma;
    progreso.ActualizaTarea(1);
    return param;
}

double Burbujeante(PropiedadesMagma magma, double P0, double z, ParametrosKoyaguchiAnalitico param, double g){
    double P;
    P=-magma.rho_l*(g+param.F)*(z+param.Lt)+P0;
//    P=(-(magma.rho_l*(g+F)*(z+Lt)-P0-P_v*P_v/P0)+
//       sqrt((magma.rho_l*(g+F)*(z+Lt)-P0-P_v*P_v/P0)*(magma.rho_l*(g+F)*(z+Lt)-P0-P_v*P_v/P0)-4*P_v*P_v))/2;
    return P;
}

double GasPiroclasto(PropiedadesMagma magma, double z, ParametrosKoyaguchiAnalitico param, double CTE, double estimP, double g,
                    double tol, Progreso &progreso){
    auto fYdfdx = [&] (double P) -> Vector2d{
        Vector2d v;
        v << g*(z+param.L_g)-integral(P, magma, param)-CTE, 
            (1-param.n_f)/magma.rho_l+param.n_f*magma.Rg*magma.T0/P-(1-param.n_f)/magma.rho_l*param.P_v*param.P_v/P/P
            -param.n_f*magma.Rg*magma.T0*param.P_v*param.P_v/P/P/P;
        return v;
    };
    return NewtonRaphson(estimP, fYdfdx, tol, 1000, 0, progreso);
}

double integral(double p, PropiedadesMagma magma, ParametrosKoyaguchiAnalitico param){
    return -(1-param.n_f)/magma.rho_l*p-param.n_f*magma.Rg*magma.T0*log(p)-(1-param.n_f)/magma.rho_l*param.P_v*param.P_v/p
        -param.n_f*magma.Rg*magma.T0*param.P_v*param.P_v/p/p/2;
}

double Presion(PropiedadesMagma magma, double P0, double z, ParametrosKoyaguchiAnalitico param, double CTE, double estimP, double g,
                double tol, Progreso& progreso){ 
    double P=Burbujeante(magma, P0, z, param, g);
    if(P<param.P_f)
        P=GasPiroclasto(magma, z, param, CTE, estimP, g, tol, progreso);
    return P;
}

   
