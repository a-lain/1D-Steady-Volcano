#include "progresoUI.h"
#include "ui_progresoUI.h"

ProgresoUI::ProgresoUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgresoUI)
{
    ui->setupUi(this);
    error = 0;
}

void ProgresoUI::EmpiezaSimulacion()
{
    // Iniciamos la Simulación en paralelo.
    progreso.Comienza();

    // Obtenemos información de la Simulación.
    timer = new QTimer(this);
    Ntareas = 0;
    t0 = chrono::steady_clock::now();
    timer->start(250);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(ActualizaInformacion()));
    vfuturo = QtConcurrent::run([&]{return Simula(datos, psiFrag, q, u0max, progreso, error);});

    QFutureWatcher<vector<Estado>> *observadorFuturo = new QFutureWatcher<vector<Estado>>(this);
    if (datos.koyaguchi_analitico)
    {
        QObject::connect(observadorFuturo, SIGNAL(finished()), this, SLOT(EmpiezaKoyaguchiAnalitico()));
    }
    else
    {
        QObject::connect(observadorFuturo, SIGNAL(finished()), this, SLOT(MuestraResultados()));
    }
    observadorFuturo->setFuture(vfuturo);
}

void ProgresoUI::EmpiezaKoyaguchiAnalitico()
{
    progreso.Comienza();
    vfuturoAnalitico = QtConcurrent::run([&]{return KoyaguchiAnalitico(datos, vfuturo.result(), param, progreso, error);});
    QFutureWatcher<vector<Estado>> *observadorFuturoAnalitico = new QFutureWatcher<vector<Estado>>(this);
    QObject::connect(observadorFuturoAnalitico, SIGNAL(finished()), this, SLOT(MuestraResultados()));
    observadorFuturoAnalitico->setFuture(vfuturoAnalitico);
}

ProgresoUI::~ProgresoUI()
{
    delete ui;
    delete timer;
}


void ProgresoUI::ActualizaInformacion()
{
    if (!progreso.EstasEnEllo())
    {
        timer->stop();
    }
    vector<double> p;
    vector<string> tareas;
    progreso.InformaDelProgreso(p, tareas);
    for (int i = 0; i < Ntareas; i++)
    {
        vetiquetas[i]->deleteLater();
        vbarras[i]->deleteLater();
    }
    vetiquetas = QVector<QLabel*>();
    vbarras = QVector<QProgressBar*>();
    Ntareas = p.size(); // Téngase en cuenta que el número de tareas activas puede cambiar de instante a instante.
    auto t = chrono::steady_clock::now();
    double dt = chrono::duration_cast<chrono::seconds>(t - t0).count();
    int dHoras = chrono::duration_cast<chrono::hours>(t - t0).count();
    int dMinutos = chrono::duration_cast<chrono::minutes>(t - t0).count() - 60*dHoras;
    int dSegundos = chrono::duration_cast<chrono::seconds>(t - t0).count() - 60*dMinutos;
    char texto[100];
    sprintf(texto, "Tiempo transcurrido: %02d:%02d:%02d", dHoras, dMinutos, dSegundos);
    ui->transcurrido_label->setText(texto);
    double v = (p.size() == 0 || p[0] == 0 || dt == 0) ? 0: p[0] / dt;
    int srestantes = (v == 0) ? 0 : (int) (1/v - dt);
    int mrestantes =  srestantes / 60;
    int hrestantes = mrestantes / 60;
    mrestantes %= 60; srestantes %= 60;
    sprintf(texto, "Tiempo restante estimado: %02d:%02d:%02d", hrestantes, mrestantes, srestantes);
    ui->restante_label->setText(texto);
    for (int i = 0; i < Ntareas; i++)
    {
        QLabel *label = new QLabel("");
        label->setText(tareas[i].c_str());
        QProgressBar *progressBar = new QProgressBar();
        progressBar->setValue(100*p[i]);
        sprintf(texto, "%d%% completado.", (int)(100*p[i]));
        progressBar->setFormat(texto);
        ui->verticalLayout_2->addWidget(label);
        ui->verticalLayout_2->addWidget(progressBar);
        label->show();
        progressBar->show();
        vetiquetas.push_back(label);
        vbarras.push_back(progressBar);
    }
}

void ProgresoUI::MuestraResultados()
{
    if (error == 20)
    {
        QMessageBox::critical(this, "¡Error!", "¡Solución sin sentido físico! No hay solución para la velocidad inicial indicada.");
        this->close();
    }
    else if (error == 21)
    {
        QMessageBox::critical(this, "¡Error!", "La velocidad inicial mínima proporcionada es superior a la máxima velocidad inicial para la cual hay solución con sentido físico.");
        this->close();
    }
    else
    {
        if (error == 22)
        {
            QMessageBox::warning(this, "¡Advertencia!", "La velocidad inicial máxima proporcionada es superior a la máxima velocidad inicial para la cual existe solución con sentido físico. Se ha cambiado automáticamente el valor de la máxima velocidad inicial por el valor de la máxima velocidad inicial para la cual hay solución con sentido físico.");
        }
        ResultadosUI* resultados = new ResultadosUI();
        resultados->resSim = vfuturo.result();
        resultados->modo = datos.modo;
        resultados->q = q;
        resultados->u0max = u0max;
        resultados->psiFrag = psiFrag;
        resultados->param = param;
        if (datos.koyaguchi_analitico)
        {
            resultados->resSimAnalitico = vfuturoAnalitico.result();
        }
        resultados->show();
        resultados->Actualiza();
        resultados->activateWindow();
        this->close();
    }
}
