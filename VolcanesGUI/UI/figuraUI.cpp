#include "figuraUI.h"
#include "ui_figuraUI.h"

figuraUI::figuraUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::figuraUI)
{
    ui->setupUi(this);
    QFile* F = new QFile("tmp.png");
    F->open(QIODevice::ReadOnly);
    pix = new QPixmap();
    pix->loadFromData(F->readAll());
    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsView* view = new QGraphicsView(scene);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(*pix);
    scene->addItem(item);

    F->remove();
}

figuraUI::~figuraUI()
{
    delete ui;
}
