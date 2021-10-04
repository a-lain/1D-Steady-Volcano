#ifndef PROGRESOUI_H
#define PROGRESOUI_H

#include <QDialog>
#include "../Simulación/progreso.h"
#include "../Simulación/fundamentos.h"
#include "../Simulación/volcanes.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>
#include <vector>
#include <QTimer>
#include <QLabel>
#include <QProgressBar>
#include "resultadosUI.h"
#include <QFutureWatcher>
#include <QMessageBox>
#include <QUnhandledException>
#include <QtConcurrent/QtConcurrent>

namespace Ui {
class ProgresoUI;
}

class ProgresoUI : public QDialog
{
    Q_OBJECT

public:
    explicit ProgresoUI(QWidget *parent = nullptr);
    ~ProgresoUI();
    Datos datos;
    Progreso progreso;
    chrono::time_point<chrono::steady_clock> t0;
    int Ntareas;
    QTimer *timer;
    QVector<QLabel*> vetiquetas;
    QVector<QProgressBar*> vbarras;
    QFuture<vector<Estado>> vfuturo, vfuturoAnalitico;
    int error;
    double q, u0max;
    ParametrosKoyaguchiAnalitico param;
    Estado psiFrag;

    void EmpiezaSimulacion();

public slots:
    void ActualizaInformacion();
    void EmpiezaKoyaguchiAnalitico();
    void MuestraResultados();


private:
    Ui::ProgresoUI *ui;
};

#endif // PROGRESOUI_H
