#include "seleccionGraficaUI.h"
#include "ui_seleccionGraficaUI.h"

SeleccionGraficaUI::SeleccionGraficaUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SeleccionGraficaUI)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(Representa()));
}

SeleccionGraficaUI::~SeleccionGraficaUI()
{
    delete ui;
}

void SeleccionGraficaUI::ConfiguraDesplegables()
{
    if (modo == 1)
    {
        ui->ejeXComboBox->addItems(QStringList() << "velocidad inicial (m/s)"
                                                 << "Presión en la boca (Pa)"
                                                 << "velocidad en la boca (m/s)"
                                                 << "densidad del magma en la boca (kg/m^3)"
                                                 << "densidad del gas en la boca (kg/m^3)"
                                                 << "área en la boca (m^2)"
                                                 << "fracción másica de gas en la boca (adimensional)"
                                                 << "fracción volumétrica de gas en la boca (adimensional)"
                                                 << "velocidad del sonido en la boca (m/s)"
                                                 << "número de Mach en la boca (adimensional)"
                                                 << "fuerza de rozamiento en la boca (N)");
        ui->ejeYComboBox->addItems(QStringList() << "velocidad inicial (m/s)"
                                                 << "Presión en la boca (Pa)"
                                                 << "velocidad en la boca (m/s)"
                                                 << "densidad del magma en la boca (kg/m^3)"
                                                 << "densidad del gas en la boca (kg/m^3)"
                                                 << "área en la boca (m^2)"
                                                 << "fracción másica de gas en la boca (adimensional)"
                                                 << "fracción volumétrica de gas en la boca (adimensional)"
                                                 << "velocidad del sonido en la boca (m/s)"
                                                 << "número de Mach en la boca (adimensional)"
                                                 << "fuerza de rozamiento en la boca (N)");
    }
    else
    {
        ui->ejeXComboBox->addItems(QStringList() << "profundidad z (m)"
                                                 << "Presión (Pa)"
                                                 << "velocidad (m/s)"
                                                 << "densidad del magma (kg/m^3)"
                                                 << "densidad del gas (kg/m^3)"
                                                 << "área (m^2)"
                                                 << "fracción másica de gas (adimensional)"
                                                 << "fracción volumétrica de gas (adimensional)"
                                                 << "velocidad del sonido (m/s)"
                                                 << "número de Mach (adimensional)"
                                                 << "fuerza de rozamiento (N)");
        ui->ejeYComboBox->addItems(QStringList() << "profundidad z (m)"
                                                 << "Presión (Pa)"
                                                 << "velocidad (m/s)"
                                                 << "densidad del magma (kg/m^3)"
                                                 << "densidad del gas (kg/m^3)"
                                                 << "área (m^2)"
                                                 << "fracción másica de gas (adimensional)"
                                                 << "fracción volumétrica de gas (adimensional)"
                                                 << "velocidad del sonido (m/s)"
                                                 << "número de Mach (adimensional)"
                                                 << "fuerza de rozamiento (N)");
    }
}

void SeleccionGraficaUI::Representa()
{
    QLineSeries *series = new QLineSeries();
    series->setName("Sol. numérica");
    for (int i = 0; i < resSim.size(); i++)
    {
        double x = 0, y = 0;
        switch (ui->ejeXComboBox->currentIndex())
        {
        case 0:
            x = resSim[i].z;
            break;
        case 1:
            x = resSim[i].P;
            break;
        case 2:
            x = resSim[i].u;
            break;
        case 3:
            x = resSim[i].rho;
            break;
        case 4:
            x = resSim[i].rho_g;
            break;
        case 5:
            x = resSim[i].A;
            break;
        case 6:
            x = resSim[i].n;
            break;
        case 7:
            x = resSim[i].phi;
            break;
        case 8:
            x = resSim[i].c;
            break;
        case 9:
            x = resSim[i].M;
            break;
        case 10:
            x = resSim[i].F;
            break;
        default:
            break;
        }
        switch (ui->ejeYComboBox->currentIndex())
        {
        case 0:
            y = resSim[i].z;
            break;
        case 1:
            y = resSim[i].P;
            break;
        case 2:
            y = resSim[i].u;
            break;
        case 3:
            y = resSim[i].rho;
            break;
        case 4:
            y = resSim[i].rho_g;
            break;
        case 5:
            y = resSim[i].A;
            break;
        case 6:
            y = resSim[i].n;
            break;
        case 7:
            y = resSim[i].phi;
            break;
        case 8:
            y = resSim[i].c;
            break;
        case 9:
            y = resSim[i].M;
            break;
        case 10:
            y = resSim[i].F;
            break;
        default:
            break;
        }
        series->append(x, y);
    }
    QLineSeries *seriesAnalitico = new QLineSeries();
    seriesAnalitico->setName("Sol. analítica Koyaguchi");
    for (int i = 0; i < resSimAnalitico.size(); i++)
    {
        double x = 0, y = 0;
        switch (ui->ejeXComboBox->currentIndex())
        {
        case 0:
            x = resSimAnalitico[i].z;
            break;
        case 1:
            x = resSimAnalitico[i].P;
            break;
        case 2:
            x = resSimAnalitico[i].u;
            break;
        case 3:
            x = resSimAnalitico[i].rho;
            break;
        case 4:
            x = resSimAnalitico[i].rho_g;
            break;
        case 5:
            x = resSimAnalitico[i].A;
            break;
        case 6:
            x = resSimAnalitico[i].n;
            break;
        case 7:
            x = resSimAnalitico[i].phi;
            break;
        case 8:
            x = resSimAnalitico[i].c;
            break;
        case 9:
            x = resSimAnalitico[i].M;
            break;
        case 10:
            x = resSimAnalitico[i].F;
            break;
        default:
            break;
        }
        switch (ui->ejeYComboBox->currentIndex())
        {
        case 0:
            y = resSimAnalitico[i].z;
            break;
        case 1:
            y = resSimAnalitico[i].P;
            break;
        case 2:
            y = resSimAnalitico[i].u;
            break;
        case 3:
            y = resSimAnalitico[i].rho;
            break;
        case 4:
            y = resSimAnalitico[i].rho_g;
            break;
        case 5:
            y = resSimAnalitico[i].A;
            break;
        case 6:
            y = resSimAnalitico[i].n;
            break;
        case 7:
            y = resSimAnalitico[i].phi;
            break;
        case 8:
            y = resSimAnalitico[i].c;
            break;
        case 9:
            y = resSimAnalitico[i].M;
            break;
        case 10:
            y = resSimAnalitico[i].F;
            break;
        default:
            break;
        }
        seriesAnalitico->append(x, y);
    }

    graficaUI* grafica = new graficaUI();
    grafica->modo = modo;
    grafica->comboBoxX = ui->ejeXComboBox->currentIndex();
    grafica->comboBoxY = ui->ejeYComboBox->currentIndex();
    grafica->Numerico = series;
    grafica->KoyaguchiAnalitico = seriesAnalitico;
    grafica->Actualiza();
    grafica->show();
}


