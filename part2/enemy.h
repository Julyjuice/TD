#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QPoint>
#include <QSize>
#include <QPixmap>

class WayPoint;
class QPainter;
class GameWidgetA;
class Tower;
class Cannon;

class Enemy : public QObject
{
    Q_OBJECT
public:
    Enemy(WayPoint *startWayPoint, GameWidgetA *game, const QPixmap &sprite = QPixmap(":/image/princess.png"));
    ~Enemy();

    void draw(QPainter *painter) const;
    void move();
    void getDamage(int damage);
    void getRemoved();
    void getAttacked(Tower *attacker);
    void gotLostSight(Tower *attacker);
    void getAttacked(Cannon *attacker);
    void gotLostSight(Cannon *attacker);
    QPoint pos() const;

public slots:
    void doActivate();

private:
    bool			m_active;
    int				m_maxHp;
    int				m_currentHp;
    qreal			m_walkingSpeed;
    qreal			m_rotationSprite;

    QPoint			m_pos;
    WayPoint *		m_destinationWayPoint;
    GameWidgetA *	m_game;
    QList<Tower *>	m_attackedTowersList;
    QList<Cannon *> m_attackedCannonList;

    const QPixmap	m_sprite,m_sprite1;
    static const QSize ms_fixedSize;
    static const QSize ms_fixedSize1;
};
#endif // ENEMY_H
