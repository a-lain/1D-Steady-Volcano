#ifndef CUSTOMSPINBOX_H
#define CUSTOMSPINBOX_H

#include <QSpinBox>

class MultSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit MultSpinBox(QWidget *parent = nullptr);
    double multFactor = 10;

public slots:
    void stepBy(int steps);

signals:

};

#endif // CUSTOMSPINBOX_H
