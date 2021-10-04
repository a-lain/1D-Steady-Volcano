#include "graficaUI.h"
#include "ui_graficaUI.h"

graficaUI::graficaUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::graficaUI)
{
    ui->setupUi(this);
    connect(ui->actionCerrar, SIGNAL(triggered()), this, SLOT(Cierra()));
    connect(ui->actionGuardar, SIGNAL(triggered()), this, SLOT(Guarda()));
    connect(ui->actionCopiar_al_portapapeles, SIGNAL(triggered()), this, SLOT(CopiaAlPortapapeles()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(CierraPrograma()));
}

graficaUI::~graficaUI()
{
    delete ui;
}

void graficaUI::Cierra()
{
    this->close();
}

void graficaUI::CierraPrograma()
{
    QCoreApplication::quit();
}

void graficaUI::Actualiza()
{
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(Numerico);
    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY1 = new QValueAxis();
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY1, Qt::AlignLeft);
    Numerico->attachAxis(axisX);
    Numerico->attachAxis(axisY1);
    if (KoyaguchiAnalitico->count() != 0)
    {
        QValueAxis *axisY2 = new QValueAxis();
        chart->addAxis(axisY2, Qt::AlignLeft);
        KoyaguchiAnalitico->attachAxis(axisX);
        KoyaguchiAnalitico->attachAxis(axisY2);
        chart->addSeries(KoyaguchiAnalitico);
        chart->legend()->setVisible(true);
    }


    QString labelX, labelY;
    switch (comboBoxX)
    {
    case 0:
        labelX = (modo == 1) ? "u₀ (m/s)" : "z(m)";
        break;
    case 1:
        labelX = (modo == 1) ? "P_b (Pa)" : "P (Pa)";
        break;
    case 2:
        labelX = (modo == 1) ? "u_b (m/s)" : "u (m/s)";
        break;
    case 3:
        labelX = (modo == 1) ? "ρ_b (kg/m³)" : "ρ (kg/m³)";
        break;
    case 4:
        labelX = (modo == 1) ? "ρ_g_b (kg/m³)" : "ρ_g (kg/m³)";
        break;
    case 5:
        labelX = (modo == 1) ? "A_b (m²)" : "A (m²)";
        break;
    case 6:
        labelX = (modo == 1) ? "n_b" : "n";
        break;
    case 7:
        labelX = (modo == 1) ? "φ_b" : "φ";
        break;
    case 8:
        labelX = (modo == 1) ? "c_b (m/s)" : "c (m/s)";
        break;
    case 9:
        labelX = (modo == 1) ? "M_b" : "M";
        break;
    case 10:
        labelX = (modo == 1) ? "F_b (N)" : "F (N)";
        break;
    default:
        break;
    }
    switch (comboBoxY)
    {
    case 0:
        labelY = (modo == 1) ? "u₀ (m/s)" : "z(m)";
        break;
    case 1:
        labelY = (modo == 1) ? "P_b (Pa)" : "P (Pa)";
        break;
    case 2:
        labelY = (modo == 1) ? "u_b (m/s)" : "u (m/s)";
        break;
    case 3:
        labelY = (modo == 1) ? "ρ_b (kg/m³)" : "ρ (kg/m³)";
        break;
    case 4:
        labelY = (modo == 1) ? "ρ_g_b (kg/m³)" : "ρ_g (kg/m³)";
        break;
    case 5:
        labelY = (modo == 1) ? "A_b (m²)" : "A (m²)";
        break;
    case 6:
        labelY = (modo == 1) ? "n_b" : "n";
        break;
    case 7:
        labelY = (modo == 1) ? "φ_b" : "φ";
        break;
    case 8:
        labelY = (modo == 1) ? "c_b (m/s)" : "c (m/s)";
        break;
    case 9:
        labelY = (modo == 1) ? "M_b" : "M";
        break;
    case 10:
        labelY = (modo == 1) ? "F_b (N)" : "F (N)";
        break;
    default:
        break;
    }

    axisX->setTitleText(labelX);
    axisY1->setTitleText(labelY);


    ui->graphicsView->setChart(chart);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->show();
}

void graficaUI::Guarda()
{
    try
    {
        QString fichero = QFileDialog::getSaveFileName(this, tr("Guardar gráfica"), tr("gráfica.pdf"), tr("PDF (*.pdf)"));
        if (fichero.lastIndexOf(".pdf") == -1)
        {
            fichero += ".pdf";
        }
        QPdfWriter writer(fichero);
        writer.setPageSize(QPageSize(ui->graphicsView->sceneRect().size(), QPageSize::Unit::Millimeter));
        QPainter painter(&writer);
        ui->graphicsView->render(&painter);
        painter.end();
    }
    catch (int e)
    {

    }
}

void graficaUI::CopiaAlPortapapeles()
{
    QPixmap p = ui->graphicsView->grab();
    QApplication::clipboard()->setPixmap(p);
}
