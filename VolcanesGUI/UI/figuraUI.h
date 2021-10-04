#ifndef FIGURAUI_H
#define FIGURAUI_H

#include <QWidget>
#include <QFile>
#include <QProcess>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>

namespace Ui {
class figuraUI;
}

class figuraUI : public QWidget
{
    Q_OBJECT

public:
    QPixmap* pix;
    explicit figuraUI(QWidget *parent = nullptr);
    ~figuraUI();


private:
    Ui::figuraUI *ui;
};

#endif // FIGURAUI_H
