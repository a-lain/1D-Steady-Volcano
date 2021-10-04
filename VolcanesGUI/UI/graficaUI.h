#ifndef GRAFICAUI_H
#define GRAFICAUI_H

#include <QMainWindow>
#include <QtCharts>
using namespace QtCharts;
#include <QLineSeries>

namespace Ui {
class graficaUI;
}

class graficaUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit graficaUI(QWidget *parent = nullptr);
    ~graficaUI();

    QLineSeries *Numerico, *KoyaguchiAnalitico;
    int modo;
    int comboBoxX;
    int comboBoxY;

    void Actualiza();

public slots:
    void Cierra();
    void CierraPrograma();
    void Guarda();
    void CopiaAlPortapapeles();

private:
    Ui::graficaUI *ui;
};

#endif // GRAFICAUI_H
