#include "progreso.h"

Progreso::Progreso()
{
    enEllo = 1;
}

void Progreso::Comienza()
{
    M.lock();
    this->enEllo = 1;
    M.unlock();
}

void Progreso::Acaba()
{
    M.lock();
    this->enEllo = 0;
    M.unlock();
}


void Progreso::AgnadeTarea(string nombreTarea)
{
    M.lock();
    this->p.push_back(0);
    this->tareas.push_back(nombreTarea);
    M.unlock();
}

void Progreso::ActualizaTarea(double p)
{
    M.lock();
    this->p[this->p.size() - 1] = p;
    M.unlock();
}

void Progreso::EliminaTarea()
{
    M.lock();
    this->p.pop_back();
    this->tareas.pop_back();
    M.unlock();
}

int Progreso::EstasEnEllo()
{
    M.lock();
    int temp = this->enEllo;
    M.unlock();
    return temp;
}

void Progreso::InformaDelProgreso(vector<double> &p, vector<string>& tareas)
{
    M.lock();
    p = vector<double>(this->p);
    tareas = vector<string>(this->tareas);
    M.unlock();
}
