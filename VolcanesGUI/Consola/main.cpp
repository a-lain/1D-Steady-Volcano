#include "main.h"

int main(int argc, char** argv)
{
    if (argc == 2)
    {
        if (string(argv[1]) == "-h")
        {
            printf("Use la siguiente sintaxis 'volcanes <nombre del fichero de los datos> <nombre del fichero en el que guardar los resultados>'.\n");
        }     
    }
    else if (argc == 3)
    {
        FILE *Flee; // Fichero F Lee, no «huye»
        Flee = fopen(argv[1], "r");
        Datos datos = Datos(Flee);
        fclose(Flee);

        // Koyaguchi sólo tiene sentido en el modo 2
        if (datos.modo != 2)
        {
            datos.koyaguchi_analitico = 0;
        }

        // Iniciamos la Simulación en paralelo.
        Progreso progreso;
        progreso.Comienza();
        Estado psiFrag;
        double q, u0max;
        int error = 0;
        future<vector<Estado>> vfuturo = 
            async(launch::async, [&]{return Simula(datos, psiFrag, q, u0max, progreso, error);});

        // Obtenemos información de la Simulación.
        vector<double> p;
        vector<string> tareas;
        int Ntareas = 0;
        auto t0 = chrono::steady_clock::now();
        while (progreso.EstasEnEllo() == 1)
        {
            progreso.InformaDelProgreso(p, tareas);
            for (int i = 0; i < Ntareas + 2; i++)
            {
                printf("\33[2K\r\033[A"); // Borramos una línea.
            }
            Ntareas = p.size(); // Téngase en cuenta que el número de tareas activas puede cambiar de instante a instante.
            auto t = chrono::steady_clock::now();
            double dt = chrono::duration_cast<chrono::seconds>(t - t0).count();
            int dHoras = chrono::duration_cast<chrono::hours>(t - t0).count();
            int dMinutos = chrono::duration_cast<chrono::minutes>(t - t0).count() - 60*dHoras;
            int dSegundos = chrono::duration_cast<chrono::seconds>(t - t0).count() - 60*dMinutos;
            printf("Tiempo transcurrido: %02d:%02d:%02d\n", dHoras, dMinutos, dSegundos);
            double v = (p.size() == 0 || p[0] == 0 || dt == 0) ? 0: p[0] / dt;
            int srestantes = (v == 0) ? 0 : (int) (1/v - dt);
            int mrestantes =  srestantes / 60;
            int hrestantes = mrestantes / 60;
            mrestantes %= 60; srestantes %= 60;
            printf("Tiempo restante estimado: %02d:%02d:%02d\n", hrestantes, mrestantes, srestantes);
            for (int i = 0; i < Ntareas; i++)
            {
                printf("%s: %2.lf\%\n", tareas[i].c_str(), 100*p[i]);
            }
            this_thread::sleep_for(chrono::milliseconds(250));
        }

        vector<Estado> v = vfuturo.get();
        vector<Estado> vAnalitico;
        ParametrosKoyaguchiAnalitico param;

        // Calculamos la solución analítica de Koyaguchi
        if (datos.koyaguchi_analitico)
        {
            Progreso progresoAnalitico;
            progresoAnalitico.Comienza();
            future<vector<Estado>> vfuturoAnalitico = 
                async(launch::async, [&]{return KoyaguchiAnalitico(datos, v, param, progresoAnalitico, error);});

            // Obtenemos información de la Simulación.
            vector<double> pAnalitico;
            vector<string> tareasAnalitico;
            int Ntareas = 0;
            auto t0 = chrono::steady_clock::now();
            while (progresoAnalitico.EstasEnEllo() == 1)
            {
                progresoAnalitico.InformaDelProgreso(pAnalitico, tareasAnalitico);
                for (int i = 0; i < Ntareas + 2; i++)
                {
                    printf("\33[2K\r\033[A"); // Borramos una línea.
                }
                Ntareas = pAnalitico.size(); // Téngase en cuenta que el número de tareas activas puede cambiar de instante a instante.
                auto t = chrono::steady_clock::now();
                double dt = chrono::duration_cast<chrono::seconds>(t - t0).count();
                int dHoras = chrono::duration_cast<chrono::hours>(t - t0).count();
                int dMinutos = chrono::duration_cast<chrono::minutes>(t - t0).count() - 60*dHoras;
                int dSegundos = chrono::duration_cast<chrono::seconds>(t - t0).count() - 60*dMinutos;
                printf("Tiempo transcurrido: %02d:%02d:%02d\n", dHoras, dMinutos, dSegundos);
                double v = (pAnalitico.size() == 0 || pAnalitico[0] == 0 || dt == 0) ? 0: pAnalitico[0] / dt;
                int srestantes = (v == 0) ? 0 : (int) (1/v - dt);
                int mrestantes =  srestantes / 60;
                int hrestantes = mrestantes / 60;
                mrestantes %= 60; srestantes %= 60;
                printf("Tiempo restante estimado: %02d:%02d:%02d\n", hrestantes, mrestantes, srestantes);
                for (int i = 0; i < Ntareas; i++)
                {
                    printf("%s: %2.lf\%\n", tareasAnalitico[i].c_str(), 100*pAnalitico[i]);
                }
                this_thread::sleep_for(chrono::milliseconds(250));
            }

            vAnalitico = vfuturoAnalitico.get();
        }

        // Mostramos información importante por pantalla

        printf("\n--- RESULTADOS ---\n\n");

        if (error == 20)
        {
            printf("Error: ¡Solución sin sentido físico! No hay solución para la velocidad inicial indicada.\n");
            return -1;
        }
        else if (error == 21)
        {
            printf("Error: La velocidad inicial mínima proporcionada es superior a la máxima velocidad inicial para la cual hay solución con sentido físico.\n");
            return -1;
        }
        else
        {
            if (error == 22)
            {
                printf("Advertencia: La velocidad inicial máxima proporcionada es superior a la máxima velocidad inicial para la cual existe solución con sentido físico. Se ha cambiado automáticamente el valor de la máxima velocidad inicial por el valor de la máxima velocidad inicial para la cual hay solución con sentido físico.\n");
            }

            if (datos.modo == 0 || datos.modo == 2)
            {
                printf("q = %lf (kg/s)\n\n", q);
                printf("Estado del nivel de fragmentación\n");
                psiFrag.EscribePorPantalla();
                printf("\nEstado en la boca\n");
                v[v.size() - 1].EscribePorPantalla();
            }
            if (datos.modo == 2 && datos.koyaguchi_analitico)
            {
                printf("\n");
                param.EscribePorPantalla();
            }
            if (datos.modo == 1)
            {
                printf("u0máx = %lf (m/s)\n", u0max);
            }

            // Guardamos en fichero
            FILE *Fguarda;
            Fguarda = fopen(argv[2], "w");
            if (datos.koyaguchi_analitico)
            {
                fprintf(Fguarda, "# profundidad z (m) / velocidad inicial (m/s), profundidad z (m) / velocidad inicial (m/s) [Analítico], Presión (Pa), Presión (Pa) [Analítico],  velocidad (m/s),  velocidad (m/s) [Analítico], densidad del magma (kg/m^3), densidad del magma (kg/m^3) [Analítico], densidad del gas (kg/m^3), densidad del gas (kg/m^3) [Analítico], área (m^2), área (m^2) [Analítico], fracción másica de gas (adimensional), fracción másica de gas (adimensional) [Analítico], fracción volumétrica de gas (adimensional), fracción volumétrica de gas (adimensional) [Analítico], velocidad del sonido (m/s), velocidad del sonido (m/s) [Analítico], número de Mach (adimensional), número de Mach (adimensional) [Analítico], fuerza de rozamiento (N), fuerza de rozamiento (N) [Analítico]\n");
                for (int i = 0; i < v.size(); i++)
                {
                    fprintf(Fguarda, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n",
                        v[i].z, vAnalitico[i].z, v[i].P, vAnalitico[i].P, v[i].u, vAnalitico[i].u, v[i].rho_g, 
                        vAnalitico[i].rho_g, v[i].rho, vAnalitico[i].rho, v[i].A, vAnalitico[i].A, v[i].n, vAnalitico[i].n,
                        v[i].phi, vAnalitico[i].phi, v[i].c, vAnalitico[i].c, v[i].M, vAnalitico[i].M,
                        v[i].F, vAnalitico[i].F);
                }
            }
            else
            {
                fprintf(Fguarda, "# profundidad z (m) / velocidad inicial (m/s), Presión (Pa), velocidad (m/s), densidad del magma (kg/m^3), densidad del gas (kg/m^3), área (m^2), fracción másica de gas (adimensional), fracción volumétrica de gas (adimensional), velocidad del sonido (m/s), número de Mach (adimensional), fuerza de rozamiento (N)\n");
                for (int i = 0; i < v.size(); i++)
                {
                    v[i].GuardaEnFichero(Fguarda);
                }
            }
            fclose(Fguarda);
            
            return 0;
            }
    }
    else
    {
        printf("Error: Número de argumentos suministrado inválido. Use '-h' para obtener ayuda.");
    }

    return 0; 
}
