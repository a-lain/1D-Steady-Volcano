#ifndef RESULTADOSUI_H
#define RESULTADOSUI_H

#include <QMainWindow>
#include "Simulación/fundamentos.h"
#include "Simulación/útil.h"
#include "Simulación/analyticalKoyaguchi.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QHeaderView>
#include <QFileDialog>
#include "UI/seleccionGraficaUI.h"

namespace Ui {
class ResultadosUI;
}

class ResultadosUI : public QMainWindow
{
    Q_OBJECT

public:
    vector<Estado> resSim, resSimAnalitico;
    int modo;
    double q, u0max;
    Estado psiFrag;
    ParametrosKoyaguchiAnalitico param;
    void Actualiza();
    explicit ResultadosUI(QWidget *parent = nullptr);
    ~ResultadosUI();    

private:
    Ui::ResultadosUI *ui;

public slots:
    void GuardaEnFichero();
    void NuevaGrafica();
    void Cierra();
    void CierraPrograma();

};

#endif // RESULTADOSUI_H
