#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "gamewidgeta.h"
#include "gamewidgetb.h"
#include "bgmusic.h"
#include <QSound>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void on_pushButton_a_clicked();

    void on_pushButton_b_clicked();

private:
    Ui::MainWidget *ui;

    BgMusic neverend_music;//出场音乐
};

#endif // MAINWIDGET_H
