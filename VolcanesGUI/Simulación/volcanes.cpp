#include "volcanes.h"

vector<Estado> Simula(Datos datos, Estado &psiFrag, double &q, double &u0max, Progreso &progreso, int& error)
{
    PropiedadesMagma magma;
    PropiedadesConducto conducto;
    magma.s = datos.s;
    magma.Rg = datos.Rg;
    magma.T0 = datos.T0;
    magma.n0 = datos.n0;
    magma.eta0 = datos.eta0;
    magma.rho_l = datos.rho_l;
    magma.phiCriticoKoyaguchi = datos.phi_critico;
    conducto.f0 = datos.f0;
    conducto.dAdz = [] (double z)->double {return 0;};
    double A = datos.A;
    conducto.A = [A] (double z)->double {return A;};

    vector<Estado> datosBrutos, datosFinales;

    switch (datos.modo)
    {
        case 0:
            datosBrutos = EntradaVelocidadInicialSalidaPerfil(datos, magma, conducto, psiFrag, q, progreso, error);
            break;
        case 1:
            datosBrutos = EntradaRangoVelocidadesInicialesSalidaValoresEnLaBoca(datos, magma, conducto, u0max, progreso, error);
            break;
        case 2:
            datosBrutos = EntradaPresionEnLaBocaSalidaPerfil(datos, magma, conducto, psiFrag, q, progreso, error);
            break;
        default:
            break;
    }

    // Sólo devolvemos el número de datos pedidos por el usuario
    if (datos.modo != 1)
    {
        double z0 = datos.z0;
        int inc = (int) -z0 / datos.Ndatos;
        double z = z0;
        int j = 0;
        while (z < 0 && j < datosBrutos.size())
        {
            while (datosBrutos[j].z < z && j < datosBrutos.size())
            {
                j++;
            }
            datosFinales.push_back(datosBrutos[j]);
            z += inc;
            j++;
        }
    }
    else
    {
        datosFinales = datosBrutos;
    }

    progreso.Acaba();
    return datosFinales;
}

vector<Estado> EntradaVelocidadInicialSalidaPerfil(Datos datos, PropiedadesMagma magma,
                                                   PropiedadesConducto conducto, Estado &psiFrag, double &q, Progreso &progreso, int& error)
{
    Estado psi = Psi0Koyaguchi(datos.z0, datos.P0, datos.u0, magma, conducto);
    vector<Estado> v;
    try
    {
        ResuelveKoyaguchi(v, psiFrag, q, psi, magma, conducto, datos.g, datos.eps, datos.dP, datos.eps,
                            AlcanzadoNivelFragmentacionKoyaguchiVolumenCritico, progreso);
    }
    catch (int e)
    {
        if (e == 20)
        {
            error = 20;
        }
    }

    return v;
}

vector<Estado> EntradaPresionEnLaBocaSalidaPerfil(Datos datos, PropiedadesMagma magma,
                                                   PropiedadesConducto conducto, Estado &psiFrag, double &q, Progreso &progreso, int& error)
{
    double u0 = u0Koyaguchi(datos.z0, datos.P0, magma, conducto, datos.g, datos.Pb, datos.eps, datos.dP, datos.eps,
                            AlcanzadoNivelFragmentacionKoyaguchiVolumenCritico, progreso);
    Estado psi = Psi0Koyaguchi(datos.z0, datos.P0, u0, magma, conducto);
    vector<Estado> v;
    try
    {
        ResuelveKoyaguchi(v, psiFrag, q, psi, magma, conducto, datos.g, datos.eps, datos.dP, datos.eps,
                          AlcanzadoNivelFragmentacionKoyaguchiVolumenCritico, progreso);
    }
    catch (int e)
    {
        if (e == 20)
        {
            error = 20;
        }
    }
    return v;
}

vector<Estado> EntradaRangoVelocidadesInicialesSalidaValoresEnLaBoca(Datos datos, PropiedadesMagma magma,
                                                   PropiedadesConducto conducto, double &u0max, Progreso &progreso, int& error)
{
    u0max = u0Koyaguchi(datos.z0, datos.P0, magma, conducto, datos.g, 0, datos.eps, datos.dP, datos.eps,
                               AlcanzadoNivelFragmentacionKoyaguchiVolumenCritico, progreso);
    progreso.AgnadeTarea("Resolviendo para diferentes velocidades iniciales...");
    vector<Estado> psiBoca;
    double u01 = datos.u01;
    double u02 = datos.u02;

    // Comprobamos que el rango de velocidades suministrado está contenido en el intervalo para el cual hay solución con sentido físico.
    if (u01 >= u0max)
    {
        error = 21;
        vector<Estado> v;
        return v;
    }
    else if (u02 >= u0max)
    {
        error = 22;
        u02 = u0max;
    }

    int Nu0 = datos.Nu;
    double inc = (u02 - u01) / Nu0;
    double u0 = u01;
    while (u0 < u02)
    {
        Estado psiFrag;
        double q;
        Estado psi = Psi0Koyaguchi(datos.z0, datos.P0, u0, magma, conducto);
        vector<Estado> v;
        ResuelveKoyaguchi(v, psiFrag, q, psi, magma, conducto, datos.g, datos.eps, datos.dP, datos.eps,
                          AlcanzadoNivelFragmentacionKoyaguchiVolumenCritico,
                          progreso);
        psi = v[v.size() - 1];
        psi.z = u0;
        psiBoca.push_back(psi);
        progreso.ActualizaTarea((u0 - u01) / (u02 - u01));
        u0 += inc;
    }
    progreso.EliminaTarea();
    return psiBoca;
}

vector<Estado> KoyaguchiAnalitico(Datos datos, vector<Estado> v, ParametrosKoyaguchiAnalitico &param,
        Progreso &progreso, int& error)
{
    progreso.AgnadeTarea("Calculando solución «analítica» de Koyaguchi...");

    PropiedadesMagma magma;
    PropiedadesConducto conducto;
    magma.s = datos.s;
    magma.Rg = datos.Rg;
    magma.T0 = datos.T0;
    magma.n0 = datos.n0;
    magma.eta0 = datos.eta0;
    magma.rho_l = datos.rho_l;
    magma.phiCriticoKoyaguchi = datos.phi_critico;
    conducto.f0 = datos.f0;
    conducto.dAdz = [] (double z)->double {return 0;};
    double Ad = datos.A;
    conducto.A = [Ad] (double z)->double {return Ad;};

    double z0 = datos.z0;
    double P0 = datos.P0;
    double u0 = datos.u0;
    double g = datos.g;
    double Lt = -z0, CTE;
    param = CalculaParametros(P0, magma, conducto, Lt, g, datos.eps, progreso); 
    CTE = -integral(param.P_f, magma, param);

    vector<Estado> vAnalitico;
    double estimP = param.P_f;
    for (int i = 0; i < v.size(); i++)
    {   
        // printf("estimP = %lf\n", estimP);
        double P = Presion(magma, P0, v[i].z, param, CTE, estimP, g, datos.eps, progreso);
        vAnalitico.push_back(ActualizaEstadoKoyaguchi(P, param.q, v[i].z, magma, conducto, (v[i].P > param.P_f) ? 0: 1));
        estimP = (P < param.P_f) ? P : param.P_f;
        progreso.ActualizaTarea((double) i / v.size());
    }
    progreso.Acaba();
    return vAnalitico;
}
