#include "resultadosUI.h"
#include "ui_resultadosUI.h"


ResultadosUI::ResultadosUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ResultadosUI)
{
    ui->setupUi(this);
    QObject::connect(ui->actionCerrar, SIGNAL(triggered()), this, SLOT(Cierra()));
    QObject::connect(ui->actionGuardar_datos, SIGNAL(triggered()), this, SLOT(GuardaEnFichero()));
    QObject::connect(ui->actionNueva, SIGNAL(triggered()), this, SLOT(NuevaGrafica()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(CierraPrograma()));
}

ResultadosUI::~ResultadosUI()
{
    delete ui;
}

// Extraído de https://stackoverflow.com/questions/42458735/how-do-i-adjust-a-qtableview-height-according-to-contents
void verticalResizeTableViewToContents(QTableView *tableView)
{
    int rowTotalHeight=0;

    // Rows height
    int count=tableView->verticalHeader()->count();
    for (int i = 0; i < count; ++i) {
        // 2018-03 edit: only account for row if it is visible
        if (!tableView->verticalHeader()->isSectionHidden(i)) {
            rowTotalHeight+=tableView->verticalHeader()->sectionSize(i);
        }
    }

    // Check for scrollbar visibility
    if (!tableView->horizontalScrollBar()->isHidden())
    {
         rowTotalHeight+=tableView->horizontalScrollBar()->height();
    }

    // Check for header visibility
    if (!tableView->horizontalHeader()->isHidden())
    {
         rowTotalHeight+=tableView->horizontalHeader()->height();
    }
    tableView->setMinimumHeight(rowTotalHeight);
    tableView->setMaximumHeight(rowTotalHeight);
}


void ResultadosUI::Actualiza()
{
    // Mostramos la información de antes de la tabla
    if (modo == 0 || modo == 2)
    {
        ui->info->setText(printfToString("Flujo másico por unidad de tiempo: q = %lf (kg/s)", q).c_str());
        QStringList EncabezadosFrag;
        EncabezadosFrag << "profundidad z (m)" <<
                          "presión (Pa)" <<
                          "velocidad (m/s)" <<
                          "densidad del magma (kg/m^3)" <<
                          "densidad del gas (kg/m^3)" <<
                          "área (m^2)" <<
                          "fracción másica de gas (adimensional)" <<
                          "fracción volumétrica de gas (adimensional)" <<
                          "velocidad del sonido (m/s)" <<
                          "número de Mach (adimensional)" <<
                          "fuerza de rozamiento (N)";
        ui->tablaFrag->setRowCount(1);
        ui->tablaFrag->setColumnCount(11);
        ui->tablaFrag->setHorizontalHeaderLabels(EncabezadosFrag);
        ui->tablaFrag->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tablaFrag->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tablaFrag->setItem(0, 0, new QTableWidgetItem(printfToString("%lf", psiFrag.z).c_str()));
        ui->tablaFrag->setItem(0, 1, new QTableWidgetItem(printfToString("%lf", psiFrag.P).c_str()));
        ui->tablaFrag->setItem(0, 2, new QTableWidgetItem(printfToString("%lf", psiFrag.u).c_str()));
        ui->tablaFrag->setItem(0, 3, new QTableWidgetItem(printfToString("%lf", psiFrag.rho).c_str()));
        ui->tablaFrag->setItem(0, 4, new QTableWidgetItem(printfToString("%lf", psiFrag.rho_g).c_str()));
        ui->tablaFrag->setItem(0, 5, new QTableWidgetItem(printfToString("%lf", psiFrag.A).c_str()));
        ui->tablaFrag->setItem(0, 6, new QTableWidgetItem(printfToString("%lf", psiFrag.n).c_str()));
        ui->tablaFrag->setItem(0, 7, new QTableWidgetItem(printfToString("%lf", psiFrag.phi).c_str()));
        ui->tablaFrag->setItem(0, 8, new QTableWidgetItem(printfToString("%lf", psiFrag.c).c_str()));
        ui->tablaFrag->setItem(0, 9, new QTableWidgetItem(printfToString("%lf", psiFrag.M).c_str()));
        ui->tablaFrag->setItem(0, 10, new QTableWidgetItem(printfToString("%lf", psiFrag.F).c_str()));
        verticalResizeTableViewToContents(ui->tablaFrag);

        ui->tablaBoca->setRowCount(1);
        ui->tablaBoca->setColumnCount(11);
        ui->tablaBoca->setHorizontalHeaderLabels(EncabezadosFrag);
        ui->tablaBoca->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tablaBoca->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tablaBoca->setItem(0, 0, new QTableWidgetItem(printfToString("%lf", resSim[resSim.size() - 1].z).c_str()));
        ui->tablaBoca->setItem(0, 1, new QTableWidgetItem(printfToString("%lf", resSim[resSim.size() - 1].P).c_str()));
        ui->tablaBoca->setItem(0, 2, new QTableWidgetItem(printfToString("%lf", resSim[resSim.size() - 1].u).c_str()));
        ui->tablaBoca->setItem(0, 3, new QTableWidgetItem(printfToString("%lf", resSim[resSim.size() - 1].rho).c_str()));
        ui->tablaBoca->setItem(0, 4, new QTableWidgetItem(printfToString("%lf", resSim[resSim.size() - 1].rho_g).c_str()));
        ui->tablaBoca->setItem(0, 5, new QTableWidgetItem(printfToString("%lf", resSim[resSim.size() - 1].A).c_str()));
        ui->tablaBoca->setItem(0, 6, new QTableWidgetItem(printfToString("%lf", resSim[resSim.size() - 1].n).c_str()));
        ui->tablaBoca->setItem(0, 7, new QTableWidgetItem(printfToString("%lf", resSim[resSim.size() - 1].phi).c_str()));
        ui->tablaBoca->setItem(0, 8, new QTableWidgetItem(printfToString("%lf", resSim[resSim.size() - 1].c).c_str()));
        ui->tablaBoca->setItem(0, 9, new QTableWidgetItem(printfToString("%lf", resSim[resSim.size() - 1].M).c_str()));
        ui->tablaBoca->setItem(0, 10, new QTableWidgetItem(printfToString("%lf", resSim[resSim.size() - 1].F).c_str()));
        verticalResizeTableViewToContents(ui->tablaBoca);

        if (modo == 0 || resSimAnalitico.size() == 0)
        {
            ui->tablaKoyaguchiAnalitico->setVisible(0);
            ui->KoyaguchiAnalitico_label->setVisible(0);
        }
        else
        {
            QStringList EncabezadosKoyaguchiAnalitico;
            EncabezadosKoyaguchiAnalitico << "longitud total del conducto (m)" <<
                                             "presión en el nivel de fragmentación (Pa)" <<
                                             "fracción másica de gas en el nivel de fragmentación" <<
                                             "escala característica de la longitud de la región de flujo burbujeante (m)" <<
                                             "α" <<
                                             "β" <<
                                             "γ" <<
                                             "δ" <<
                                             "λ" <<
                                             "longitud de la región del flujo de gas-piroclastos (m)" <<
                                             "longitud de la región del flujo burbujeante (m)" <<
                                             "flujo másico por unidad de tiempo (kg/s)" <<
                                             "fuerza de rozamiento en la región de flujo burbujeante (N)" <<
                                             "presión en la boca (Pa)";

            ui->tablaKoyaguchiAnalitico->setRowCount(1);
            ui->tablaKoyaguchiAnalitico->setColumnCount(14);
            ui->tablaKoyaguchiAnalitico->setHorizontalHeaderLabels(EncabezadosKoyaguchiAnalitico);
            ui->tablaKoyaguchiAnalitico->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->tablaKoyaguchiAnalitico->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
            ui->tablaKoyaguchiAnalitico->setItem(0, 0, new QTableWidgetItem(printfToString("%lf", param.Lt).c_str()));
            ui->tablaKoyaguchiAnalitico->setItem(0, 1, new QTableWidgetItem(printfToString("%lf", param.P_f).c_str()));
            ui->tablaKoyaguchiAnalitico->setItem(0, 2, new QTableWidgetItem(printfToString("%lf", param.n_f).c_str()));
            ui->tablaKoyaguchiAnalitico->setItem(0, 3, new QTableWidgetItem(printfToString("%lf", param.z_b).c_str()));
            ui->tablaKoyaguchiAnalitico->setItem(0, 4, new QTableWidgetItem(printfToString("%lf", param.alpha).c_str()));
            ui->tablaKoyaguchiAnalitico->setItem(0, 5, new QTableWidgetItem(printfToString("%lf", param.beta).c_str()));
            ui->tablaKoyaguchiAnalitico->setItem(0, 6, new QTableWidgetItem(printfToString("%lf", param.gamma).c_str()));
            ui->tablaKoyaguchiAnalitico->setItem(0, 7, new QTableWidgetItem(printfToString("%lf", param.delta).c_str()));
            ui->tablaKoyaguchiAnalitico->setItem(0, 8, new QTableWidgetItem(printfToString("%lf", param.lambda).c_str()));
            ui->tablaKoyaguchiAnalitico->setItem(0, 9, new QTableWidgetItem(printfToString("%lf", param.L_g).c_str()));
            ui->tablaKoyaguchiAnalitico->setItem(0, 10, new QTableWidgetItem(printfToString("%lf", param.L_b).c_str()));
            ui->tablaKoyaguchiAnalitico->setItem(0, 11, new QTableWidgetItem(printfToString("%lf", param.q).c_str()));
            ui->tablaKoyaguchiAnalitico->setItem(0, 12, new QTableWidgetItem(printfToString("%lf", param.F).c_str()));
            ui->tablaKoyaguchiAnalitico->setItem(0, 13, new QTableWidgetItem(printfToString("%lf", param.P_v).c_str()));
            verticalResizeTableViewToContents(ui->tablaKoyaguchiAnalitico);
        }

    }
    else if (modo == 1)
    {
        ui->info->setText(printfToString("Velocidad inicial máxima para la cual hay solución con sentido físico: u0máx = %lf (kg/s)",
                                         u0max).c_str());
        ui->tablaBoca->setVisible(0);
        ui->tablaFrag->setVisible(0);
        ui->boca_label->setVisible(0);
        ui->frag_label->setVisible(0);
        ui->tablaKoyaguchiAnalitico->setVisible(0);
        ui->KoyaguchiAnalitico_label->setVisible(0);
    }

    // Mostramos los resultados en la tabla
    if (resSimAnalitico.size() == 0)
    {
        ui->tabla->setRowCount(resSim.size());
        ui->tabla->setColumnCount(11);
        QStringList Encabezados;
        if (modo == 1)
        {
            Encabezados << "velocidad inicial (m/s)" <<
                           "presión en la boca (Pa)" <<
                           "velocidad en la boca (m/s)" <<
                           "densidad del magma en la boca (kg/m^3)" <<
                           "densidad del gas en la boca (kg/m^3)" <<
                           "área en la boca (m^2)" <<
                           "fracción másica de gas en la boca (adimensional)" <<
                           "fracción volumétrica de gas en la boca (adimensional)" <<
                           "velocidad del sonido en la boca (m/s)" <<
                           "número de Mach en la boca (adimensional)" <<
                           "fuerza de rozamiento en la boca (N)";
        }
        else
        {
            Encabezados << "profundidad z (m)" <<
                           "presión (Pa)" <<
                           "velocidad (m/s)" <<
                           "densidad del magma (kg/m^3)" <<
                           "densidad del gas (kg/m^3)" <<
                           "área (m^2)" <<
                           "fracción másica de gas (adimensional)" <<
                           "fracción volumétrica de gas (adimensional)" <<
                           "velocidad del sonido (m/s)" <<
                           "número de Mach (adimensional)" <<
                           "fuerza de rozamiento (N)";
        }
        ui->tabla->setHorizontalHeaderLabels(Encabezados);
        ui->tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        for (int i = 0; i < resSim.size(); i++)
        {
            ui->tabla->setItem(i, 0, new QTableWidgetItem(printfToString("%lf", resSim[i].z).c_str()));
            ui->tabla->setItem(i, 1, new QTableWidgetItem(printfToString("%lf", resSim[i].P).c_str()));
            ui->tabla->setItem(i, 2, new QTableWidgetItem(printfToString("%lf", resSim[i].u).c_str()));
            ui->tabla->setItem(i, 3, new QTableWidgetItem(printfToString("%lf", resSim[i].rho).c_str()));
            ui->tabla->setItem(i, 4, new QTableWidgetItem(printfToString("%lf", resSim[i].rho_g).c_str()));
            ui->tabla->setItem(i, 5, new QTableWidgetItem(printfToString("%lf", resSim[i].A).c_str()));
            ui->tabla->setItem(i, 6, new QTableWidgetItem(printfToString("%lf", resSim[i].n).c_str()));
            ui->tabla->setItem(i, 7, new QTableWidgetItem(printfToString("%lf", resSim[i].phi).c_str()));
            ui->tabla->setItem(i, 8, new QTableWidgetItem(printfToString("%lf", resSim[i].c).c_str()));
            ui->tabla->setItem(i, 9, new QTableWidgetItem(printfToString("%lf", resSim[i].M).c_str()));
            ui->tabla->setItem(i, 10, new QTableWidgetItem(printfToString("%lf", resSim[i].F).c_str()));
        }
    }
    else
    {
        ui->tabla->setRowCount(resSim.size());
        ui->tabla->setColumnCount(22);
        QStringList Encabezados;
        Encabezados << "profundidad z (m)" <<
                       "profundidad z (m) [Analítico]" <<
                       "presión (Pa)" <<
                       "presión (Pa) [Analítico]" <<
                       "velocidad (m/s)" <<
                       "velocidad (m/s) [Analítico]" <<
                       "densidad del magma (kg/m^3)" <<
                       "densidad del magma (kg/m^3) [Analítico]" <<
                       "densidad del gas (kg/m^3)" <<
                       "densidad del gas (kg/m^3) [Analítico]" <<
                       "área (m^2)" <<
                       "área (m^2) [Analítico]" <<
                       "fracción másica de gas (adimensional)" <<
                       "fracción másica de gas (adimensional) [Analítico]" <<
                       "fracción volumétrica de gas (adimensional)" <<
                       "fracción volumétrica de gas (adimensional) [Analítico]" <<
                       "velocidad del sonido (m/s)" <<
                       "velocidad del sonido (m/s) [Analítico]" <<
                       "número de Mach (adimensional)" <<
                       "número de Mach (adimensional) [Analítico]" <<
                       "fuerza de rozamiento (N)" <<
                       "fuerza de rozamiento (N) [Analítico]";
        ui->tabla->setHorizontalHeaderLabels(Encabezados);
        ui->tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        for (int i = 0; i < resSim.size(); i++)
        {
            ui->tabla->setItem(i, 0, new QTableWidgetItem(printfToString("%lf", resSim[i].z).c_str()));
            ui->tabla->setItem(i, 1, new QTableWidgetItem(printfToString("%lf", resSimAnalitico[i].z).c_str()));
            ui->tabla->setItem(i, 2, new QTableWidgetItem(printfToString("%lf", resSim[i].P).c_str()));
            ui->tabla->setItem(i, 3, new QTableWidgetItem(printfToString("%lf", resSimAnalitico[i].P).c_str()));
            ui->tabla->setItem(i, 4, new QTableWidgetItem(printfToString("%lf", resSim[i].u).c_str()));
            ui->tabla->setItem(i, 5, new QTableWidgetItem(printfToString("%lf", resSimAnalitico[i].u).c_str()));
            ui->tabla->setItem(i, 6, new QTableWidgetItem(printfToString("%lf", resSim[i].rho).c_str()));
            ui->tabla->setItem(i, 7, new QTableWidgetItem(printfToString("%lf", resSimAnalitico[i].rho).c_str()));
            ui->tabla->setItem(i, 8, new QTableWidgetItem(printfToString("%lf", resSim[i].rho_g).c_str()));
            ui->tabla->setItem(i, 9, new QTableWidgetItem(printfToString("%lf", resSimAnalitico[i].rho_g).c_str()));
            ui->tabla->setItem(i, 10, new QTableWidgetItem(printfToString("%lf", resSim[i].A).c_str()));
            ui->tabla->setItem(i, 11, new QTableWidgetItem(printfToString("%lf", resSimAnalitico[i].A).c_str()));
            ui->tabla->setItem(i, 12, new QTableWidgetItem(printfToString("%lf", resSim[i].n).c_str()));
            ui->tabla->setItem(i, 13, new QTableWidgetItem(printfToString("%lf", resSimAnalitico[i].n).c_str()));
            ui->tabla->setItem(i, 14, new QTableWidgetItem(printfToString("%lf", resSim[i].phi).c_str()));
            ui->tabla->setItem(i, 15, new QTableWidgetItem(printfToString("%lf", resSimAnalitico[i].phi).c_str()));
            ui->tabla->setItem(i, 16, new QTableWidgetItem(printfToString("%lf", resSim[i].c).c_str()));
            ui->tabla->setItem(i, 17, new QTableWidgetItem(printfToString("%lf", resSimAnalitico[i].c).c_str()));
            ui->tabla->setItem(i, 18, new QTableWidgetItem(printfToString("%lf", resSim[i].M).c_str()));
            ui->tabla->setItem(i, 19, new QTableWidgetItem(printfToString("%lf", resSimAnalitico[i].M).c_str()));
            ui->tabla->setItem(i, 20, new QTableWidgetItem(printfToString("%lf", resSim[i].F).c_str()));
            ui->tabla->setItem(i, 21, new QTableWidgetItem(printfToString("%lf", resSimAnalitico[i].F).c_str()));
        }
    }
}

void ResultadosUI::GuardaEnFichero()
{
    try
    {
        QString fichero = QFileDialog::getSaveFileName(this, "Guardar resultados", "resultados.txt");
        FILE *Fguarda;
        Fguarda = fopen(fichero.toUtf8().constData(), "w");
        if (resSimAnalitico.size() != 0)
        {
            fprintf(Fguarda, "# profundidad z (m) / velocidad inicial (m/s), profundidad z (m) / velocidad inicial (m/s) [Analítico], Presión (Pa), Presión (Pa) [Analítico],  velocidad (m/s),  velocidad (m/s) [Analítico], densidad del magma (kg/m^3), densidad del magma (kg/m^3) [Analítico], densidad del gas (kg/m^3), densidad del gas (kg/m^3) [Analítico], área (m^2), área (m^2) [Analítico], fracción másica de gas (adimensional), fracción másica de gas (adimensional) [Analítico], fracción volumétrica de gas (adimensional), fracción volumétrica de gas (adimensional) [Analítico], velocidad del sonido (m/s), velocidad del sonido (m/s) [Analítico], número de Mach (adimensional), número de Mach (adimensional) [Analítico], fuerza de rozamiento (N), fuerza de rozamiento (N) [Analítico]\n");
            for (int i = 0; i < resSim.size(); i++)
            {
                fprintf(Fguarda, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n",
                    resSim[i].z, resSimAnalitico[i].z, resSim[i].P, resSimAnalitico[i].P, resSim[i].u, resSimAnalitico[i].u, resSim[i].rho_g,
                    resSimAnalitico[i].rho_g, resSim[i].rho, resSimAnalitico[i].rho, resSim[i].A, resSimAnalitico[i].A, resSim[i].n, resSimAnalitico[i].n,
                    resSim[i].phi, resSimAnalitico[i].phi, resSim[i].c, resSimAnalitico[i].c, resSim[i].M, resSimAnalitico[i].M,
                    resSim[i].F, resSimAnalitico[i].F);
            }
        }
        else
        {
            fprintf(Fguarda, "# profundidad z (m) / velocidad inicial (m/s), Presión (Pa), velocidad (m/s), densidad del magma (kg/m^3), densidad del gas (kg/m^3), área (m^2), fracción másica de gas (adimensional), fracción volumétrica de gas (adimensional), velocidad del sonido (m/s), número de Mach (adimensional), fuerza de rozamiento (N)\n");
            for (int i = 0; i < resSim.size(); i++)
            {
                resSim[i].GuardaEnFichero(Fguarda);
            }
        }
        fclose(Fguarda);
    }
    catch (int e)
    {

    }

}

void ResultadosUI::NuevaGrafica()
{
    SeleccionGraficaUI* graficaDiag = new SeleccionGraficaUI();
    graficaDiag->resSim = resSim;
    graficaDiag->resSimAnalitico = resSimAnalitico;
    graficaDiag->modo = modo;
    graficaDiag->ConfiguraDesplegables();
    graficaDiag->show();
}

void ResultadosUI::Cierra()
{
    this->close();
}

void ResultadosUI::CierraPrograma()
{
    QCoreApplication::quit();
}
