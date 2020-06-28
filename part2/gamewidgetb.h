#ifndef GAMEWIDGETB_H
#define GAMEWIDGETB_H

#include <QDialog>
#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QList>
#include <bgmusic.h>
#include <QSound>
#include <QString>
class WayPoint;
class Enemy;
class Bullet;
class CannonBullet;
class AudioPlayer;

namespace Ui {
class GameWidgetB;
}

class GameWidgetB : public QDialog
{
    Q_OBJECT

public:
    explicit GameWidgetB(QDialog *parent = nullptr);
    ~GameWidgetB();

private:
    Ui::GameWidgetB *ui;
public:
    void getHpDamage(int damage = 1);
    void removedEnemy(Enemy *enemy);
    void removedBullet(Bullet *bullet);
    void addBullet(Bullet *bullet);
    void removedCannonBullet(CannonBullet *cannonbullet);
    void addCannonBullet(CannonBullet *cannonbullet);
    void awardGold(int gold);
    void contextMenuEvent(QContextMenuEvent *event);

    AudioPlayer* audioPlayer() const;
    QList<Enemy *> enemyList() const;
    QList<Enemy *> enemyList1() const;

public slots:
    void updateMap();
    void gameStart();
    void flagChange1();
    void flagChange2();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    void loadTowerPositions();
    void addWayPoints();
    bool loadWave();
    bool canBuyTower() const;
    void drawWave(QPainter *painter);
    void drawHP(QPainter *painter);
    void drawXT(QPainter *painter);
    void drawPlayerGold(QPainter *painter);
    void doGameOver();
    void preLoadWavesInfo();
    void act();
    void removeDeath();

    //数据成员
    int						m_waves;
    int						m_playerHp;
    int						m_playrGold;
    bool					m_gameEnded;
    bool					m_gameWin;

    AudioPlayer *			m_audioPlayer;
    QList<QVariant>			m_wavesInfo;

    QList<WayPoint *>		m_wayPointsList;
    QList<WayPoint *>		m_wayPointsList1;

    QList<Enemy *>			m_enemyList;
    QList<Enemy *>			m_enemyList1;
    QList<Bullet *>			m_bulletList;
    QList<CannonBullet *>   m_cannonbulletList;
    BgMusic attackedcastle_music;
    BgMusic collectpotion_music ;
    BgMusic enemydie_music ;
    BgMusic neverend_music;
    BgMusic settower_music ;
    BgMusic win_music;
};

#endif // GAMEWIDGETB_H
