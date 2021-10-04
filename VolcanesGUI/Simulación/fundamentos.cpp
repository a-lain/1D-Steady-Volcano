#include "fundamentos.h"

void Estado::GuardaEnFichero(FILE* F)
{
    fprintf(F, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n",
        this->z, this->P, this->u, this->rho_g, this->rho, this->A, this->n,
        this->phi, this->c, this->M, this->F);
}

string Estado::ATexto()
{
    string texto;
    texto += printfToString("Estado:\n");
    texto += printfToString("\tz = %lf (m)\n", z);
    texto += printfToString("\tP = %lf (MPa)\n", P/1e6);
    texto += printfToString("\tu = %lf (m/s)\n", u);
    texto += printfToString("\tρ = %lf (kg/m³)\n", rho);
    texto += printfToString("\tρ_g = %lf (kg/m³)\n", rho_g);
    texto += printfToString("\tA = %lf (m²)\n", A);
    texto += printfToString("\tn = %lf\n", n);
    texto += printfToString("\tϕ = %lf\n", phi);
    texto += printfToString("\tc = %lf (m/s)\n", c);
    texto += printfToString("\tM = %lf\n", M);
    texto += printfToString("\tF = %lf (N)\n", F);
    texto += printfToString("Fin Estado.\n");
    return texto;
}

void Estado::EscribePorPantalla()
{
    printf("%s", ATexto().c_str());
}

Datos::Datos(FILE* F)
{
    rewind(F);
    fscanf(F, "%lf\n", &this->T0);
    fscanf(F, "%lf\n", &this->rho_l);
    fscanf(F, "%lf\n", &this->eta0);
    fscanf(F, "%lf\n", &this->s);
    fscanf(F, "%lf\n", &this->n0);
    fscanf(F, "%lf\n", &this->Rg);
    fscanf(F, "%lf\n", &this->A);
    fscanf(F, "%lf\n", &this->f0);
    fscanf(F, "%lf\n", &this->g);
    fscanf(F, "%lf\n", &this->P0);
    fscanf(F, "%lf\n", &this->z0);
    fscanf(F, "%lf\n", &this->dP);
    fscanf(F, "%lf\n", &this->eps);
    fscanf(F, "%d\n", &this->Ndatos);
    fscanf(F, "%d\n", &this->criterio);
    fscanf(F, "%lf\n", &this->phi_critico);
    fscanf(F, "%d\n", &this->modo);
    fscanf(F, "%lf\n", &this->u0);
    fscanf(F, "%lf\n", &this->u01);
    fscanf(F, "%lf\n", &this->u02);
    fscanf(F, "%d\n", &this->Nu);
    fscanf(F, "%lf\n", &this->Pb);
    fscanf(F, "%d\n", &this->koyaguchi_analitico);
}

Datos::Datos()
{

}

void Datos::GuardaEnFichero(FILE* F)
{
    fprintf(F, "%lf\n", this->T0);
    fprintf(F, "%lf\n", this->rho_l);
    fprintf(F, "%lf\n", this->eta0);
    fprintf(F, "%lf\n", this->s);
    fprintf(F, "%lf\n", this->n0);
    fprintf(F, "%lf\n", this->Rg);
    fprintf(F, "%lf\n", this->A);
    fprintf(F, "%lf\n", this->f0);
    fprintf(F, "%lf\n", this->g);
    fprintf(F, "%lf\n", this->P0);
    fprintf(F, "%lf\n", this->z0);
    fprintf(F, "%lf\n", this->dP);
    fprintf(F, "%lf\n", this->eps);
    fprintf(F, "%d\n", this->Ndatos);
    fprintf(F, "%d\n", this->criterio);
    fprintf(F, "%lf\n", this->phi_critico);
    fprintf(F, "%d\n", this->modo);
    fprintf(F, "%lf\n", this->u0);
    fprintf(F, "%lf\n", this->u01);
    fprintf(F, "%lf\n", this->u02);
    fprintf(F, "%d\n", this->Nu);
    fprintf(F, "%lf\n", this->Pb);
    fprintf(F, "%d\n", this->koyaguchi_analitico);
}


string Datos::ATexto()
{
    string texto;
    texto += printfToString("Datos:\n");
    texto += printfToString("\tT0 = %lf\n", T0);
    texto += printfToString("\tρ_l = %lf\n", rho_l);
    texto += printfToString("\tη0 = %lf\n", eta0);
    texto += printfToString("\tRg = %lf\n", Rg);
    texto += printfToString("\tA = %lf\n", A);
    texto += printfToString("\tf0 = %lf\n", f0);
    texto += printfToString("\tg = %lf\n", g);
    texto += printfToString("\tP0 = %lf\n", P0);
    texto += printfToString("\tz0 = %lf\n", z0);
    texto += printfToString("\tdP = %lf\n", dP);
    texto += printfToString("\tε = %lf\n", eps);
    texto += printfToString("\tNdatos = %d\n", Ndatos);
    texto += printfToString("\tcriterio = %d\n", criterio);
    texto += printfToString("\tφ crítico = %lf\n", phi_critico);
    texto += printfToString("\tmodo = %d\n", modo);
    texto += printfToString("\tu0 = %lf\n", u0);
    texto += printfToString("\tu01 = %lf\n", u01);
    texto += printfToString("\tu02 = %lf\n", u02);
    texto += printfToString("\tNu = %d\n", Nu);
    texto += printfToString("\tPb = %lf\n", Pb);
    texto += printfToString("\tKoyaguchi Analítico = %d\n", koyaguchi_analitico);
    texto += printfToString("Fin Datos.\n");
    return texto;
}

void Datos::EscribePorPantalla()
{
    printf("%s", ATexto().c_str());
}
