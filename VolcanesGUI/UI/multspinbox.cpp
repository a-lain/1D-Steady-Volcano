#include "multspinbox.h"

MultSpinBox::MultSpinBox(QWidget *parent) : QSpinBox(parent)
{

}


void MultSpinBox::stepBy(int steps)
{
    double val = value();
    if (steps > 0)
    {
        for (int i = 0; i < steps; i ++)
        {
            val *= multFactor;
        }
    }
    else if (steps < 0)
    {
        for (int i = 0; i < -steps; i ++)
        {
            val /= multFactor;
        }
    }
    setValue(val);
}
