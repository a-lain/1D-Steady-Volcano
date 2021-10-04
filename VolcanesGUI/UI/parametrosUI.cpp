#include "parametrosUI.h"
#include "ui_parametrosUI.h"


ParametrosUI::ParametrosUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ParametrosUI)
{
    ui->setupUi(this);
    connect(ui->BotonIniciar, SIGNAL(clicked()), this, SLOT(BotonIniciarSimulacion()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), this, SLOT(Cierra()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(CierraPrograma()));
    ui->modo_box->setCurrentIndex(1);
    ui->modo_box->setCurrentIndex(0);
    ui->cri_box->setCurrentIndex(1);
    ui->cri_box->setCurrentIndex(0);
}

ParametrosUI::~ParametrosUI()
{
    delete ui;
}

void ParametrosUI::Cierra()
{
    this->close();
}

void ParametrosUI::CierraPrograma()
{
    QCoreApplication::quit();
}

void ParametrosUI::BotonIniciarSimulacion()
{
    Datos datos;
    datos.T0 = ui->T0_box->value();
    datos.rho_l = ui->rhol_box->value();
    datos.eta0 = ui->eta0_box->value();
    datos.s = ui->s_box->value() / 1e6;
    datos.n0 = ui->n_box->value();
    datos.Rg = ui->R_box->value();
    datos.A = ui->A_box->value();
    datos.f0 = ui->f_box->value();
    datos.g = ui->g_box->value();
    datos.P0 = ui->P0_box->value() * 1e6;
    datos.z0 = -ui->z0_box->value();
    datos.dP = ui->dP_box->value();
    datos.eps = pow(10, ui->eps_box->value());
    datos.Ndatos = ui->N_box->value();
    datos.criterio = ui->cri_box->currentIndex();
    datos.phi_critico = ui->vcrit_box->value();
    datos.modo = ui->modo_box->currentIndex();
    datos.u0 = ui->u0_box->value();
    datos.u01 = ui->u01_box->value();
    datos.u02 = ui->u02_box->value();
    datos.Nu = ui->Nu0_box->value();
    datos.Pb = ui->Pb_box->value();
    datos.koyaguchi_analitico = ui->KA_box->isChecked();
    if (datos.modo != 2)
    {
        datos.koyaguchi_analitico = 0;
    }
    ProgresoUI* progreso = new ProgresoUI();
    progreso->datos = datos;
    progreso->show();
    progreso->EmpiezaSimulacion();
    vprogreso.push_back(progreso);
}


void ParametrosUI::on_modo_box_currentIndexChanged(int index)
{
    switch (index)
    {
        case 0:
            ui->u0_label->setVisible(true);
            ui->u0_box->setVisible(true);
            ui->u0_image->setVisible(true);
            ui->u01_label->setVisible(false);
            ui->u01_box->setVisible(false);
            ui->u01_image->setVisible(false);
            ui->u02_label->setVisible(false);
            ui->u02_box->setVisible(false);
            ui->u02_image->setVisible(false);
            ui->Nu0_label->setVisible(false);
            ui->Nu0_box->setVisible(false);
            ui->Pb_label->setVisible(false);
            ui->Pb_box->setVisible(false);
            ui->Pb_image->setVisible(false);
            ui->KA_box->setVisible(false);
            ui->KA_label->setVisible(false);
            break;
        case 1:
            ui->u0_label->setVisible(false);
            ui->u0_box->setVisible(false);
            ui->u0_image->setVisible(false);
            ui->u01_label->setVisible(true);
            ui->u01_box->setVisible(true);
            ui->u01_image->setVisible(true);
            ui->u02_label->setVisible(true);
            ui->u02_box->setVisible(true);
            ui->u02_image->setVisible(true);
            ui->Nu0_label->setVisible(true);
            ui->Nu0_box->setVisible(true);
            ui->Pb_label->setVisible(false);
            ui->Pb_box->setVisible(false);
            ui->Pb_image->setVisible(false);
            ui->KA_box->setVisible(false);
            ui->KA_label->setVisible(false);
            break;
        case 2:
            ui->u0_label->setVisible(false);
            ui->u0_box->setVisible(false);
            ui->u0_image->setVisible(false);
            ui->u01_label->setVisible(false);
            ui->u01_box->setVisible(false);
            ui->u01_image->setVisible(false);
            ui->u02_label->setVisible(false);
            ui->u02_box->setVisible(false);
            ui->u02_image->setVisible(false);
            ui->Nu0_label->setVisible(false);
            ui->Nu0_box->setVisible(false);
            ui->Pb_label->setVisible(true);
            ui->Pb_box->setVisible(true);
            ui->Pb_image->setVisible(true);
            ui->KA_box->setVisible(true);
            ui->KA_label->setVisible(true);
            break;
        default:
            break;
    }
}

void ParametrosUI::on_cri_box_currentIndexChanged(int index)
{
    switch (index)
    {
        case 0:
            ui->vcrit_box->setVisible(true);
            ui->vcrit_label->setVisible(true);
            break;
        case 1:
            ui->vcrit_box->setVisible(false);
            ui->vcrit_label->setVisible(false);
            break;
        default:
            break;
    }
}
