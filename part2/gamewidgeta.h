#ifndef GAMEWIDGETA_H
#define GAMEWIDGETA_H

#include <QMenu>
#include <QAction>
#include <QDialog>
#include <QList>
#include <bgmusic.h>
#include <QSound>
#include <QString>
#include "money.h"
#include "audioplayer.h"
#include "bgmusic.h"
#include "bullet.h"
#include "cannon.h"
#include "cannonbullet.h"
#include "castle.h"
#include "enemy.h"
#include "judge.h"
#include "money.h"
#include "plistreader.h"
#include "tower.h"
#include "towerposition.h"
#include "waypoint.h"

namespace Ui {
class GameWidgetA;
}

class GameWidgetA : public QDialog
{
    Q_OBJECT

public:
    explicit GameWidgetA(QDialog *parent = nullptr);
    ~GameWidgetA();

public:
    Ui::GameWidgetA *ui;

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

public:
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
    QList<money*> moneylist;



    //数据成员
    int						m_waves;
    int						m_playerHp;
    int						m_playrGold;
    bool					m_gameEnded;
    bool					m_gameWin;
    Castle                  castle;
    AudioPlayer *			m_audioPlayer;
    QList<QVariant>			m_wavesInfo;
    QList<TowerPosition>	m_towerPositionsList;
    QList<Tower *>			m_towersList;
    QList<Cannon *>         m_cannonList;
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
        QTimer *timer;
};

#endif // GAMEWIDGETA_H
