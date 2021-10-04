#ifndef PROGRESO_H
#define PROGRESO_H

#include <string>
#include <vector>
#include <mutex>

using namespace std;

class Progreso
{
    private:
    int enEllo;
    vector<double> p; // Un número entre 0 y 1.
    vector<string> tareas; // El nombre de las tareas que se están realizando.
    mutex M;

    public:
    Progreso();
    void Comienza();
    void Acaba();
    void AgnadeTarea(string nombreTarea);
    void ActualizaTarea(double p);
    void EliminaTarea();

    int EstasEnEllo();
    void InformaDelProgreso(vector<double> &p, vector<string>& tareas);
    
};

#endif // PROGRESO_H
