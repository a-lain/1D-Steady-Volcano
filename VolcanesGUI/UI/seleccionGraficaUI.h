#ifndef SELECCIONGRAFICAUI_H
#define SELECCIONGRAFICAUI_H

#include <QDialog>
#include "Simulación/fundamentos.h"
#include "UI/graficaUI.h"
#include <QDebug>
#include <QFile>
#include <QtCharts>
using namespace QtCharts;
#include <QLineSeries>

namespace Ui {
class SeleccionGraficaUI;
}

class SeleccionGraficaUI : public QDialog
{
    Q_OBJECT

public:
    vector<Estado> resSim, resSimAnalitico;
    int modo;
    explicit SeleccionGraficaUI(QWidget *parent = nullptr);
    ~SeleccionGraficaUI();

    void ConfiguraDesplegables();

public slots:
    void Representa();

private:
    Ui::SeleccionGraficaUI *ui;
};

#endif // SELECCIONGRAFICAUI_H
