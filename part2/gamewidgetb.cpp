#include "gamewidgetb.h"
#include "ui_gamewidgetb.h"
#include "audioplayer.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtGlobal>
#include <QMessageBox>
#include <QTimer>
#include <QXmlStreamReader>
#include <QtDebug>
#include <QColor>
static const int TowerCost= 300;
static int flag = 0;

GameWidgetB::GameWidgetB(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::GameWidgetB)
  , m_waves(0)
  , m_playerHp(5)
  , m_playrGold(1000)
  , m_gameEnded(false)
  , m_gameWin(false)
  ,attackedcastle_music(_attackedcastle_music)
  ,collectpotion_music(_collectpotion_music)
  ,enemydie_music(_enemydie_music)
  ,neverend_music(_neverend_music)
  ,settower_music(_settower_music)
  ,win_music(_win_music)
{
    ui->setupUi(this);
    preLoadWavesInfo();
    loadTowerPositions();
    addWayPoints();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);
    gameStart();
}

GameWidgetB::~GameWidgetB()
{
    delete ui;
}

void GameWidgetB::loadTowerPositions()
{
    QFile file(":/config/TowersPosition.plist");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
        return;
    }
    file.close();
}

void GameWidgetB::act()
{

}

void GameWidgetB::paintEvent(QPaintEvent *)
{
    if (m_gameEnded || m_gameWin)
    {

        if(m_gameEnded==true)
        {
            //lost
            attackedcastle_music.stop();
            collectpotion_music.stop();
            enemydie_music.stop();
            settower_music.stop();

            win_music.setLoops(3);
            win_music.play();
            QPixmap cachePix(":/image/snowworld.jpg");
            QPainter cachePainter(&cachePix);
            QPainter painter(this);
            painter.drawPixmap(0, 0, cachePix);

        }
        if(m_gameEnded==false)
        {
            //win
            attackedcastle_music.stop();
            collectpotion_music.stop();
            enemydie_music.stop();
            neverend_music.stop();
            settower_music.stop();

            win_music.setLoops(3);
            win_music.play();
            QPixmap cachePix(":/image/hotworld.jpg");
            QPainter cachePainter(&cachePix);
            QPainter painter(this);
            painter.drawPixmap(0, 0, cachePix);

        }

        return;
    }



}

void GameWidgetB::flagChange1()
{
    flag = 1;
}

void GameWidgetB::flagChange2()
{
    flag = 2;
}

void GameWidgetB::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu(this);
    menu->setStyleSheet("QMenu::item{ padding:25px;font-size:20px;}");
    QAction *pNew1 = new QAction("建造地狱之塔", this);
    pNew1->setIcon(QIcon(":/image/helltower.png"));
    QAction *pNew2 = new QAction("建造加农炮", this);
    pNew2->setIcon(QIcon(":/image/tower.png"));
    menu->addAction(pNew1);
    menu->addAction(pNew2);
    connect(pNew1, SIGNAL(triggered()), this, SLOT(flagChange1()));
    connect(pNew2, SIGNAL(triggered()), this, SLOT(flagChange2()));
    menu->move(cursor().pos());
    menu->show();
}

void GameWidgetB::mousePressEvent(QMouseEvent *event)
{

}

bool GameWidgetB::canBuyTower() const
{
    if (m_playrGold >= TowerCost)
        return true;
    return false;
}

void GameWidgetB::drawWave(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(800, 5, 100, 25), QString("WAVE : %1").arg(m_waves + 1));
}

void GameWidgetB::drawHP(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(30, 5, 100, 25), QString("HP : %1").arg(m_playerHp));
}

void GameWidgetB::drawXT(QPainter *painter)
{
    painter->save();
    QPoint healthBarPoint = QPoint(560,135) + QPoint(0, -5);
    // 绘制血条
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(169,169,169));
    QRect healthBarBackRect(healthBarPoint, QSize(50, 6));
    painter->drawRect(healthBarBackRect);

    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint, QSize((double)m_playerHp / 5 * 50, 6));
    //attackedcastle_music.setLoops(1);
    //attackedcastle_music.play();
    painter->drawRect(healthBarRect);
    painter->restore();
}


void GameWidgetB::drawPlayerGold(QPainter *painter)
{
    painter->setPen(QPen(Qt::yellow));
    painter->drawText(QRect(400, 5, 200, 25), QString("GOLD : %1").arg(m_playrGold));
}

void GameWidgetB::doGameOver()
{
    if (!m_gameEnded)
    {
        m_gameEnded = true;

    }
   /* if(m_gameEnded==false)
    {
        //lost
        attackedcastle_music.stop();
        collectpotion_music.stop();
        enemydie_music.stop();
        neverend_music.stop();
        settower_music.stop();

        win_music.setLoops(1);
        win_music.play();
        QPixmap cachePix(":/image/snowworld.png");
        QPainter cachePainter(&cachePix);
        QPainter painter(this);
        painter.drawPixmap(0, 0, cachePix);

    }*/

}

void GameWidgetB::awardGold(int gold)
{
    m_playrGold += gold;
    //收集到金币攻击敌人的声音
   //collectpotion_music.setLoops(1);
  // collectpotion_music.play();
    update();
}

AudioPlayer *GameWidgetB::audioPlayer() const
{
    return m_audioPlayer;
}

void GameWidgetB::addWayPoints()
{


}

void GameWidgetB::getHpDamage(int damage/* = 1*/)
{
    attackedcastle_music.setLoops(1);
    attackedcastle_music.play();
    m_playerHp -= damage;
    if (m_playerHp <= 0)
        doGameOver();
}

void GameWidgetB::removedEnemy(Enemy *enemy)
{
    Q_ASSERT(enemy);

    m_enemyList.removeOne(enemy);
    m_enemyList1.removeOne(enemy);
    delete enemy;

    if (m_enemyList.empty()&&m_enemyList1.empty())
    {
        ++m_waves;
        if (!loadWave())
        {
            m_gameWin = true;
            //win_music.setLoops(1);
            //win_music.play();
            // 游戏胜利转到游戏胜利场景
            // 这里暂时以打印处理
        }
    }
}
void GameWidgetB::removeDeath()
{

}
void GameWidgetB::removedBullet(Bullet *bullet)
{
    Q_ASSERT(bullet);

    m_bulletList.removeOne(bullet);
    delete bullet;
}

void GameWidgetB::addBullet(Bullet *bullet)
{
    Q_ASSERT(bullet);

    m_bulletList.push_back(bullet);
}

void GameWidgetB::removedCannonBullet(CannonBullet *cannonbullet)
{
    Q_ASSERT(cannonbullet);

    m_cannonbulletList.removeOne(cannonbullet);
    delete cannonbullet;
}

void GameWidgetB::addCannonBullet(CannonBullet *cannonbullet)
{
    Q_ASSERT(cannonbullet);

    m_cannonbulletList.push_back(cannonbullet);
}

void GameWidgetB::updateMap()
{

   // this->exit->raise();
    update();
}

void GameWidgetB::preLoadWavesInfo()
{

}

bool GameWidgetB::loadWave()
{

    return true;
}

QList<Enemy *> GameWidgetB::enemyList() const
{
    return m_enemyList;

}
QList<Enemy *> GameWidgetB::enemyList1() const
{

    return m_enemyList1;
}

void GameWidgetB::gameStart()
{
    loadWave();
}
