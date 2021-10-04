#include "Koyaguchi.h"

double RozamientoAntesFragmentKoyaguchi(Estado psi,
    PropiedadesMagma magma, PropiedadesConducto conducto)
{
    return 8*magma.eta0*psi.u/psi.rho/(conducto.A(psi.z)/M_PI);
}

double RozamientoDespuesFragmentKoyaguchi(Estado psi,
    PropiedadesMagma magma, PropiedadesConducto conducto)
{
    return conducto.f0*psi.u*psi.u/4/sqrt(conducto.A(psi.z)/M_PI);
}

int AlcanzadoNivelFragmentacionKoyaguchiVolumenCritico(Estado psi,
    PropiedadesMagma magma, PropiedadesConducto conducto)
{
    if (psi.phi < magma.phiCriticoKoyaguchi) return 0;
    else return 1;
}

Estado ActualizaEstadoKoyaguchi(double P, double q, double z,
    PropiedadesMagma magma, PropiedadesConducto conducto, 
    int alcanzadoNivelFragmentacion)
{
    Estado psi;
    psi.z = z;
    psi.A = conducto.A(psi.z);
    psi.P = P;
    psi.n = (magma.n0 - magma.s*sqrt(psi.P)) / (1. - magma.s*sqrt(psi.P));
    psi.n = (psi.n >= 0) ? psi.n : 0.;
    psi.rho_g = psi.P / (magma.Rg*magma.T0);
    psi.rho = 1. / ((1. - psi.n) / magma.rho_l + psi.n / psi.rho_g);
    psi.u = q / psi.rho / psi.A;
    psi.c = psi.P / sqrt(psi.n*magma.Rg*magma.T0) / psi.rho;
    psi.M = psi.u / psi.c;
    psi.phi = psi.n * psi.rho / psi.rho_g;
    double (*F) (Estado psi, PropiedadesMagma magma, PropiedadesConducto conducto);
    F = alcanzadoNivelFragmentacion ? RozamientoDespuesFragmentKoyaguchi : RozamientoAntesFragmentKoyaguchi;
    psi.F = F(psi, magma, conducto);
    return psi;
}

Estado Psi0Koyaguchi(double z0, double P0, double u0, 
    PropiedadesMagma magma, PropiedadesConducto conducto)
{
    Estado psi;
    psi.z = z0;
    psi.A = conducto.A(psi.z);
    psi.P = P0;
    psi.n = (magma.n0 - magma.s*sqrt(psi.P)) / (1. - magma.s*sqrt(psi.P));
    psi.n = (psi.n >= 0) ? psi.n : 0.;
    psi.rho_g = psi.P / (magma.Rg*magma.T0);
    psi.rho = 1. / ((1. - psi.n) / magma.rho_l + psi.n / psi.rho_g);
    psi.u = u0;
    psi.c = psi.P / sqrt(psi.n*magma.Rg*magma.T0) / psi.rho;
    psi.M = psi.u / psi.c;
    psi.phi = psi.n * psi.rho / psi.rho_g;
    psi.F = RozamientoAntesFragmentKoyaguchi(psi, magma, conducto);
    return psi;
}

void EvolucionaKoyaguchi(Estado& psi, std::function<VectorXd(VectorXd, double)> f, PropiedadesMagma magma, PropiedadesConducto conducto, 
    double q, int& alcanzadoNivelFragmentacion, double epsilon, double& dP,
    int (*AlcanzadoNivelFragmentacion) (Estado, PropiedadesMagma, PropiedadesConducto), double tol, Progreso &progreso)
{
    double dPSig = dP;
    VectorXd Zv(1); Zv << psi.z;
    RungeKuttaAdaptativo(Zv, psi.P, epsilon, f, dP, dPSig, 0);
    if (f(Zv, psi.P + dP)[0] > 0) // Miramos si nos hemos pasado de largo al aplicar Runge Kutta de paso variable.
    {
        double Pmax = Biseccion(psi.P, psi.P + dP, [&] (double x) {return f(Zv, x)[0];}, tol, 1000, progreso);
        Zv = RungeKuttaOrden4(Zv, psi.P, f, Pmax - psi.P);
        psi = ActualizaEstadoKoyaguchi(Pmax, q, Zv[0], magma, conducto, alcanzadoNivelFragmentacion);
        if (psi.z < 0)
        {
            throw 20;
        }
    }
    else
    {
        psi = ActualizaEstadoKoyaguchi(psi.P + dP, q, Zv[0], magma, conducto, alcanzadoNivelFragmentacion);
    }
    
    if (!alcanzadoNivelFragmentacion && AlcanzadoNivelFragmentacion(psi, magma, conducto))
    {
        alcanzadoNivelFragmentacion = 1;
    }
    dP = dPSig;
}

void ResuelveKoyaguchi(vector<Estado>& v, Estado& psiFrag, double& q, 
    Estado psi, PropiedadesMagma magma, PropiedadesConducto conducto,
    double g, double epsilon, double dP, double tol,
    int (*AlcanzadoNivelFragmentacion) (Estado, PropiedadesMagma, PropiedadesConducto), Progreso &progreso)
{
    progreso.AgnadeTarea("Resolviendo la ecuación diferencial..."); // Añadimos la nueva tarea.
    double z0 = psi.z;
    int alcanzadoNivelFragmentacion = 0;
    int antiguoAlcanzadoNivelFragmentacion = 0;
    v.push_back(psi);

    q = psi.rho * psi.A * psi.u;
    auto f = [q, conducto, magma, &alcanzadoNivelFragmentacion, g] (VectorXd X, double P) -> VectorXd
    {
        Estado psif = ActualizaEstadoKoyaguchi(P, q, X[0], magma, conducto, alcanzadoNivelFragmentacion);
        double dndp = psif.n > 0 ? 
            magma.s*(magma.n0-1.)/(2.*sqrt(psif.P)*(1.-magma.s*sqrt(psif.P))*(1.-magma.s*sqrt(psif.P))) : 0;
        double drhodp = -psif.rho*psif.rho*(-dndp/magma.rho_l+(psif.P*dndp-psif.n)/psif.rho_g/psif.P);
        VectorXd vec(1);
        vec << -(1.-psif.u*psif.u*drhodp) / 
            (psif.rho*g+psif.rho*psif.F-psif.rho*psif.u*psif.u/conducto.A(psif.z)*conducto.dAdz(psif.z));
        return vec;
    };

    
    while(psi.z < 0)
    {
        try
        {
            EvolucionaKoyaguchi(psi, f, magma, conducto, q, alcanzadoNivelFragmentacion, epsilon, dP,
            AlcanzadoNivelFragmentacion, tol, progreso);
        }
        catch(int e)
        {
            if (e == 20)
            {
                progreso.EliminaTarea();
                throw 20;
            }
        }
        v.push_back(psi);
        if (alcanzadoNivelFragmentacion && !antiguoAlcanzadoNivelFragmentacion)
        {
            progreso.AgnadeTarea("Determinando el nivel de fragmentación...");
            Estado psi1, psi2, psim;
            psi2 = v[v.size() - 1];
            psi1 = v[v.size() - 2];
            int N = ceil(log((psi2.z - psi1.z) / tol) / log(2)); // Número de iteracciones total que vamos a necesitar.
            int i = 0;
            while (fabs(psi1.z - psi2.z) > tol)
            {
                double Pm = (psi2.P + psi1.P) / 2;
                VectorXd Zv(1); Zv << psi1.z;
                alcanzadoNivelFragmentacion = 0; // Si no pusiéramos esto, se calcularía el rozamiento mal, ya que se consideraría alcanzadoNivelFragmentacion=1
                Zv = RungeKuttaOrden4(Zv, psi1.P, f, Pm - psi1.P);
                psim = ActualizaEstadoKoyaguchi(Pm, q, Zv[0], magma, conducto, alcanzadoNivelFragmentacion);
                if (AlcanzadoNivelFragmentacion(psim, magma, conducto))
                {
                    psi2 = psim;
                }
                else
                {
                    psi1 = psim;
                }
                progreso.ActualizaTarea((double) i / N);
                i++;
            }
            v[v.size() - 1] = psim;
            psi = psim;
            psiFrag = psi;
            alcanzadoNivelFragmentacion = 1; // Si no pusiéramos esto, bucle infinito
            progreso.EliminaTarea();
        }
        
        antiguoAlcanzadoNivelFragmentacion = alcanzadoNivelFragmentacion;
        progreso.ActualizaTarea(1 - psi.z/z0);
    }

    // Interpolación lineal para obtener P en z=0
    Estado psi1, psi2;     
    psi2 = v[v.size() - 1];
    psi1 = v[v.size() - 2];
    double Pb = psi1.P - (psi2.P - psi1.P) / (psi2.z - psi1.z)*psi1.z;
    v[v.size() - 1] = ActualizaEstadoKoyaguchi(Pb, q, 0, magma, conducto, alcanzadoNivelFragmentacion);
    progreso.EliminaTarea();
}

double u0Koyaguchi(double z0, double P0, PropiedadesMagma magma, PropiedadesConducto conducto,
    double g, double Pb, double epsilon, double dP, double tol,
    int (*AlcanzadoNivelFragmentacion) (Estado, PropiedadesMagma, PropiedadesConducto), Progreso &progreso)
{
    progreso.AgnadeTarea("Hallando la máxima velocidad inicial posible...");
    // Estimación inicial del máximo valor de u0
    double u1 = 0, u2 = 0.001, q, umax, M1, M2, F1, F2, a, b, dP0=dP;
    Estado psi = Psi0Koyaguchi(z0, P0, u1, magma, conducto);
    Estado psiFrag;
    vector<Estado> v1, v2, v;
    dP = dP0;
    ResuelveKoyaguchi(v1, psiFrag, q, psi, magma, conducto, g, epsilon, dP, tol, AlcanzadoNivelFragmentacion, progreso);
    psi = Psi0Koyaguchi(z0, P0, u2, magma, conducto);
    dP = dP0;
    ResuelveKoyaguchi(v2, psiFrag, q, psi, magma, conducto, g, epsilon, dP, tol, AlcanzadoNivelFragmentacion, progreso);
    dP = dP0;
    M1 = v1[v1.size() - 1].M;
    M2 = v2[v2.size() - 1].M;
    F1 = 1 - M1;
    F2 = 1 - M2;
    a = (F2 - F1) / (u2 - u1);
    b = F1 - a*u1;
    umax = -b/a;

    auto h = [z0, P0, magma, conducto, epsilon, dP0, tol, AlcanzadoNivelFragmentacion, g, &progreso] (double u0)->int
    {
        // Devuelve 1 o -1. -1 a la izquierda de la solución. 1 a la derecha.
        Estado psi;
        double q;
        psi = Psi0Koyaguchi(z0, P0, u0, magma, conducto);
        vector<Estado> v;
        Estado psiFrag;
        try
        {
            double dP = dP0;
            ResuelveKoyaguchi(v, psiFrag, q, psi, magma, conducto, g, epsilon, dP, tol, AlcanzadoNivelFragmentacion, progreso);
            return -1;
        }
        catch(int e)
        {
            if (e == 20)
            {
                return 1;
            }
        } 
    };

    umax = Biseccion(0, umax, h, tol, 1000, progreso) - 2*tol; // Restamos dos veces la tolerancia para asegurarnos de que hay solución.
    progreso.ActualizaTarea(1);
    progreso.EliminaTarea();
    psi = Psi0Koyaguchi(z0, P0, umax, magma, conducto);
    dP = dP0;
    ResuelveKoyaguchi(v, psiFrag, q, psi, magma, conducto, g, epsilon, dP, tol, AlcanzadoNivelFragmentacion, progreso);
    if (v[v.size() - 1].P > Pb)
    {
        return umax;
    }
    else
    {
        progreso.AgnadeTarea("Encontrando la velocidad inicial que satisface la condición de presión a la salida...");
        auto h = [z0, P0, magma, conducto, epsilon, dP0, tol, AlcanzadoNivelFragmentacion, umax, Pb, g, &progreso] (double u0)->double
        {
            Estado psi;
            double q;
            psi = Psi0Koyaguchi(z0, P0, u0, magma, conducto);
            vector<Estado> v;
            Estado psiFrag;
            double dP = dP0;
            ResuelveKoyaguchi(v, psiFrag, q, psi, magma, conducto, g, epsilon, dP, tol, AlcanzadoNivelFragmentacion, progreso);
            return v[v.size() - 1].P - Pb;
        };
        double res = Biseccion(0, umax, h, tol, 1000, progreso);
        progreso.ActualizaTarea(1);
        progreso.EliminaTarea();
        return res;
    }
}
