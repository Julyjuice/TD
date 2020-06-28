#ifndef CASTLE_H
#define CASTLE_H

#include <QPoint>
#include <QSize>
#include <QPixmap>

class QPainter;

class Castle
{
public:
    Castle(const QPixmap &sprite = QPixmap(":/image/laocao.png"));


    void draw(QPainter *painter) const;

private:

    QPixmap		m_sprite;

    static const QSize ms_fixedSize;
};

#endif // CASTLE_H
