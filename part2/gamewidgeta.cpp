#pragma execution_character_set("utf-8")
#include "gamewidgeta.h"
#include "ui_gamewidgeta.h"
#include "waypoint.h"
#include "enemy.h"
#include "bullet.h"
#include "cannonbullet.h"
#include "audioplayer.h"
#include "plistreader.h"
#include "money.h"
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

GameWidgetA::GameWidgetA(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::GameWidgetA)
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
    setWindowTitle("Qt塔防游戏之冰雪传奇");



    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);
}

GameWidgetA::~GameWidgetA()
{
    delete ui;
}

void GameWidgetA::loadTowerPositions()
{
    m_towerPositionsList.clear();
    QFile file(":/config/TowersPosition.plist");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
        return;
    }

    PListReader reader;
    reader.read(&file);

    QList<QVariant> array = reader.data();
    foreach (QVariant dict, array)
    {
        QMap<QString, QVariant> point = dict.toMap();
        int x = point.value("x").toInt();
        int y = point.value("y").toInt();
        m_towerPositionsList.push_back(QPoint(x, y));
    }

    file.close();
}
//手机修复城堡的药水
void GameWidgetA::act()
{
    money* mymoney;
    foreach(mymoney, moneylist)
    {
        mymoney->act();
    }

}
void GameWidgetA::paintEvent(QPaintEvent *)
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
            timer->stop();
            QMessageBox msgBox;
            msgBox.setText("游戏结束.");
            msgBox.exec();
            m_gameEnded = false;
            done(0);
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
            timer->stop();
            QMessageBox msgBox;
            msgBox.setText("重新开始.");
            msgBox.exec();
            done(0);
        }
        while (!moneylist.empty())
        {
            delete moneylist[0];
            moneylist.removeAt(0);
        }
        return;
    }

    QPixmap cachePix(":/image/Bg4.png");
    QPainter cachePainter(&cachePix);
    castle.draw(&cachePainter);

    foreach (const TowerPosition &towerPos, m_towerPositionsList)
        towerPos.draw(&cachePainter);

    foreach (const Tower *tower, m_towersList)
        tower->draw(&cachePainter);

    foreach (const TowerPosition &cannonPos, m_towerPositionsList)
        cannonPos.draw(&cachePainter);

    foreach (const Cannon *cannon, m_cannonList)
        cannon->draw(&cachePainter);

    foreach (const Enemy *enemy, m_enemyList)
        enemy->draw(&cachePainter);

    foreach (const Enemy *enemy1, m_enemyList1)
        enemy1->draw(&cachePainter);


    foreach (const Bullet *bullet, m_bulletList)
        bullet->draw(&cachePainter);

    foreach (const CannonBullet *cannonbullet, m_cannonbulletList)
        cannonbullet->draw(&cachePainter);

    drawWave(&cachePainter);
    drawHP(&cachePainter);
    drawPlayerGold(&cachePainter);
    drawXT(&cachePainter);

    QPainter painter(this);
    painter.drawPixmap(0, 0, cachePix);//QPixmap cachePix(":/image/Bg4.png");
}

void GameWidgetA::flagChange1()
{
    flag = 1;
}

void GameWidgetA::flagChange2()
{
    flag = 2;
}

void GameWidgetA::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu(this);

    menu->setStyleSheet("QMenu::item{ padding:45px;font-size:20px;}");
    QAction *pNew1 = new QAction("建造地狱之塔", this);
    pNew1->setIcon(QIcon(":/image/helltower.png"));
    QAction *pNew2 = new QAction("建造加农炮", this);
    pNew2->setIcon(QIcon(":/image/tower.png"));
    menu->addAction(pNew1);
    menu->addAction(pNew2);
    menu->resize(200,200);
    connect(pNew1, SIGNAL(triggered()), this, SLOT(flagChange1()));
    connect(pNew2, SIGNAL(triggered()), this, SLOT(flagChange2()));

    menu->move(cursor().pos());
    menu->show();
}

void GameWidgetA::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        QPoint pressPos = event->pos();
        //flag为1，建造地狱之塔
        if(flag == 1)
        {
            flag = 0;
            auto it = m_towerPositionsList.begin();
            while (it != m_towerPositionsList.end())
            {
                if (canBuyTower() && it->containPoint(pressPos) && !it->hasTower())
                {
                    settower_music.setLoops(1);
                    settower_music.play();
                    m_playrGold -= TowerCost;
                    it->setHasTower();

                    Tower *tower = new Tower(it->centerPos(), this);
                    m_towersList.push_back(tower);
                    update();
                    break;
                }
                ++it;
            }
        }

        if(flag == 2)
        {
            flag = 0;
            auto it = m_towerPositionsList.begin();
            while (it != m_towerPositionsList.end())
            {
                if (canBuyTower() && it->containPoint(pressPos) && !it->hasTower())
                {
                    settower_music.setLoops(1);
                    settower_music.play();
                    m_playrGold -= TowerCost;
                    it->setHasTower();

                    Cannon *cannon = new Cannon(it->centerPos(), this);
                    m_cannonList.push_back(cannon);
                    update();
                    break;
                }

                ++it;
            }
        }
    }
}

bool GameWidgetA::canBuyTower() const
{
    if (m_playrGold >= TowerCost)
        return true;
    return false;
}

void GameWidgetA::drawWave(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(800, 5, 100, 25), QString("WAVE : %1").arg(m_waves + 1));
}

void GameWidgetA::drawHP(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(30, 5, 100, 25), QString("HP : %1").arg(m_playerHp));
}

void GameWidgetA::drawXT(QPainter *painter)
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


void GameWidgetA::drawPlayerGold(QPainter *painter)
{
    painter->setPen(QPen(Qt::yellow));
    painter->drawText(QRect(400, 5, 200, 25), QString("GOLD : %1").arg(m_playrGold));
}

void GameWidgetA::doGameOver()
{
    if (!m_gameEnded)
    {
        m_gameEnded = true;

    }

}

void GameWidgetA::awardGold(int gold)
{
    m_playrGold += gold;
    //收集到金币攻击敌人的声音
    //collectpotion_music.setLoops(1);
    // collectpotion_music.play();
    update();
}

AudioPlayer *GameWidgetA::audioPlayer() const
{
    return m_audioPlayer;
}

void GameWidgetA::addWayPoints()
{
    m_wayPointsList.clear();
    WayPoint *wayPoint1 = new WayPoint(QPoint(600,160));
    m_wayPointsList.push_back(wayPoint1);

    WayPoint *wayPoint2 = new WayPoint(QPoint(417,160));
    m_wayPointsList.push_back(wayPoint2);
    wayPoint2->setNextWayPoint(wayPoint1);

    WayPoint *wayPoint3 = new WayPoint(QPoint(417,545));
    m_wayPointsList.push_back(wayPoint3);
    wayPoint3->setNextWayPoint(wayPoint2);

    WayPoint *wayPoint4 = new WayPoint(QPoint(160,545));
    m_wayPointsList.push_back(wayPoint4);
    wayPoint4->setNextWayPoint(wayPoint3);

    WayPoint *wayPoint5 = new WayPoint(QPoint(160,231));
    m_wayPointsList.push_back(wayPoint5);
    wayPoint5->setNextWayPoint(wayPoint4);

    WayPoint *wayPoint6 = new WayPoint(QPoint(35,231));
    m_wayPointsList.push_back(wayPoint6);
    wayPoint6->setNextWayPoint(wayPoint5);

    //同窗口的第二波怪物
    WayPoint *wayPoint11 = new WayPoint(QPoint(600,160));
    m_wayPointsList1.push_back(wayPoint11);

    WayPoint *wayPoint12 = new WayPoint(QPoint(863, 160));
    m_wayPointsList1.push_back(wayPoint12);
    wayPoint12->setNextWayPoint(wayPoint11);

    WayPoint *wayPoint13 = new WayPoint(QPoint(863, 675));
    m_wayPointsList1.push_back(wayPoint13);
    wayPoint13->setNextWayPoint(wayPoint12);

    WayPoint *wayPoint14 = new WayPoint(QPoint(900, 675));
    m_wayPointsList1.push_back(wayPoint14);
    wayPoint14->setNextWayPoint(wayPoint13);
}

void GameWidgetA::getHpDamage(int damage/* = 1*/)
{
    attackedcastle_music.setLoops(1);
    attackedcastle_music.play();
    m_playerHp -= damage;
    if (m_playerHp <= 0)
        doGameOver();
}

void GameWidgetA::removedEnemy(Enemy *enemy)
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
        }
    }
}
void GameWidgetA::removeDeath()
{
    int p = 0;
    while (p < moneylist.count())
    {
        if (!(moneylist[p]->alive))
        {
            enemydie_music.setLoops(1);
            enemydie_music.play();
            delete (moneylist[p]);
            moneylist.removeAt(p);
        }
        else
        {
            p++;
        }
    }
}
void GameWidgetA::removedBullet(Bullet *bullet)
{
    Q_ASSERT(bullet);

    m_bulletList.removeOne(bullet);
    delete bullet;
}

void GameWidgetA::addBullet(Bullet *bullet)
{
    Q_ASSERT(bullet);

    m_bulletList.push_back(bullet);
}

void GameWidgetA::removedCannonBullet(CannonBullet *cannonbullet)
{
    Q_ASSERT(cannonbullet);

    m_cannonbulletList.removeOne(cannonbullet);
    delete cannonbullet;
}

void GameWidgetA::addCannonBullet(CannonBullet *cannonbullet)
{
    Q_ASSERT(cannonbullet);

    m_cannonbulletList.push_back(cannonbullet);
}

void GameWidgetA::updateMap()
{
    foreach (Enemy *enemy, m_enemyList)
        enemy->move();
    foreach (Enemy *enemy1, m_enemyList1)
        enemy1->move();
    foreach (Tower *tower, m_towersList)
        tower->checkEnemyInRange();
    foreach (Cannon *cannon, m_cannonList)
        cannon->checkEnemyInRange();

    this->removeDeath();
    this->act();
    if (qrand() % 200 < 1)
    {
        money* sun = new zSunFall(this);
        collectpotion_music.setLoops(1);
        collectpotion_music.play();
        sun->setgame(this);
        moneylist.append(sun);
    }
    update();
}

void GameWidgetA::preLoadWavesInfo()
{
    m_wavesInfo.clear();
    QFile file(":/config/Waves.plist");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
        return;
    }

    PListReader reader;
    reader.read(&file);

    // 获取波数信息
    m_wavesInfo = reader.data();

    file.close();
}

bool GameWidgetA::loadWave()
{
    if (m_waves >= m_wavesInfo.size())
        return false;

    WayPoint *startWayPoint = m_wayPointsList.back();
    WayPoint *startWayPoint1 = m_wayPointsList1.back();
    QList<QVariant> curWavesInfo = m_wavesInfo[m_waves].toList();

    for (int i = 0; i < curWavesInfo.size(); ++i)
    {
        QMap<QString, QVariant> dict = curWavesInfo[i].toMap();
        int spawnTime = dict.value("spawnTime").toInt();

        Enemy *enemy = new Enemy(startWayPoint, this);
        Enemy *enemy1 = new Enemy(startWayPoint1, this);
        m_enemyList.push_back(enemy);
        m_enemyList1.push_back(enemy1);
        QTimer::singleShot(spawnTime, enemy, SLOT(doActivate()));
        QTimer::singleShot(spawnTime, enemy1, SLOT(doActivate()));
    }

    return true;
}

QList<Enemy *> GameWidgetA::enemyList() const
{
    return m_enemyList;

}
QList<Enemy *> GameWidgetA::enemyList1() const
{

    return m_enemyList1;
}

void GameWidgetA::gameStart()
{
     m_gameEnded = false;
     m_gameWin = false;
    preLoadWavesInfo();
    loadTowerPositions();
    addWayPoints();
    loadWave();
}
