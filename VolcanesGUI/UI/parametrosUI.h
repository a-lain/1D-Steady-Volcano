#ifndef PARAMETROSUI_H
#define PARAMETROSUI_H

#include "progresoUI.h"
#include "resultadosUI.h"
#include "./Simulación/fundamentos.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ParametrosUI; }
QT_END_NAMESPACE

class ParametrosUI : public QMainWindow
{
    Q_OBJECT

public:
    ParametrosUI(QWidget *parent = nullptr);
    ~ParametrosUI();
    QVector<ProgresoUI*> vprogreso;

public slots:
    void BotonIniciarSimulacion();
    void on_modo_box_currentIndexChanged(int index);
    void on_cri_box_currentIndexChanged(int index);
    void Cierra();
    void CierraPrograma();

private:
    Ui::ParametrosUI *ui;
};

#endif // PARAMETROSUI_H
