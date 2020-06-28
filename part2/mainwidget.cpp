#pragma execution_character_set("utf-8")
#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "bgmusic.h"
#include <QSound>
#include <QPainter>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget),
    neverend_music(_neverend_music)
{
    ui->setupUi(this);
    neverend_music.setLoops(1);
    neverend_music.play();
    setWindowTitle("Qt塔防游戏之冰雪传奇");
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::paintEvent(QPaintEvent *)
{
    QPixmap cachePix(":/image/Start.png");
    cachePix = cachePix.scaled(this->width(),this->height());
    QPainter painter(this);
    painter.drawPixmap(0, 0, cachePix);
}

void MainWidget::on_pushButton_a_clicked()
{
    this->hide();
    neverend_music.stop();
    GameWidgetA m_GameWidgetA;
    m_GameWidgetA.gameStart();
    m_GameWidgetA.exec();
    this->show();
}

void MainWidget::on_pushButton_b_clicked()
{
    this->hide();
    neverend_music.stop();
    GameWidgetA m_GameWidgetA;
    m_GameWidgetA.gameStart();
    m_GameWidgetA.exec();
    this->show();
}
