#pragma execution_character_set("utf-8")
#include "enemy.h"
#include "waypoint.h"
#include "tower.h"
#include "cannon.h"
#include "judge.h"
#include "gamewidgeta.h"
#include "audioplayer.h"
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMatrix>
#include <QVector2D>
#include <QtMath>

static const int Health_Bar_Width = 30;
const QSize Enemy::ms_fixedSize(52, 52);

static const int Health_Bar_Width1 = 30;
const QSize Enemy::ms_fixedSize1(52, 52);

Enemy::Enemy(WayPoint *startWayPoint, GameWidgetA *game, const QPixmap &sprite/* = QPixmap(":/image/enemy.png")*/)
    : QObject(0)
    , m_active(false)
    , m_maxHp(800)
    , m_currentHp(800)
    , m_walkingSpeed(1.0)
    , m_rotationSprite(0.0)
    , m_pos(startWayPoint->pos())
    , m_destinationWayPoint(startWayPoint->nextWayPoint())
    , m_game(game)
    , m_sprite(sprite)
    , m_sprite1(sprite)
{
}

Enemy::~Enemy()
{
    m_attackedTowersList.clear();
    m_attackedCannonList.clear();
    m_destinationWayPoint = NULL;
    m_game = NULL;
}

void Enemy::doActivate()
{
    m_active = true;
}

void Enemy::move()
{
    if (!m_active)
        return;

    if (collisionWithCircle(m_pos, 1, m_destinationWayPoint->pos(), 1))
    {
        // 敌人抵达了一个航点
        if (m_destinationWayPoint->nextWayPoint())
        {
            // 还有下一个航点
            m_pos = m_destinationWayPoint->pos();
            m_destinationWayPoint = m_destinationWayPoint->nextWayPoint();
        }
        else
        {
            // 表示进入基地
            m_game->getHpDamage();
            m_game->removedEnemy(this);
            return;
        }
    }

    // 还在前往航点的路上
    // 目标航点的坐标
    QPoint targetPoint = m_destinationWayPoint->pos();
    // 未来修改这个可以添加移动状态,加快,减慢,m_walkingSpeed是基准值

    // 向量标准化
    qreal movementSpeed = m_walkingSpeed;
    QVector2D normalized(targetPoint - m_pos);
    normalized.normalize();
    m_pos = m_pos + normalized.toPoint() * movementSpeed;
}

void Enemy::draw(QPainter *painter) const
{
    if (!m_active)
        return;

    painter->save();

    QPoint healthBarPoint = m_pos + QPoint(-Health_Bar_Width / 2 - 5, -ms_fixedSize.height() / 2);
    // 绘制血条
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(healthBarPoint, QSize(Health_Bar_Width, 2));
    painter->drawRect(healthBarBackRect);

    painter->setBrush(Qt::yellow);
    QRect healthBarRect(healthBarPoint, QSize((double)m_currentHp / m_maxHp * Health_Bar_Width, 2));
    painter->drawRect(healthBarRect);

    // 绘制偏转坐标,由中心+偏移=左上
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    painter->translate(m_pos);
    painter->rotate(m_rotationSprite);
    // 绘制敌人
    painter->drawPixmap(offsetPoint, m_sprite);

    painter->restore();
}

void Enemy::getRemoved()
{
    if (m_attackedTowersList.empty() && m_attackedCannonList.empty())
        return;

    foreach (Tower *attacker, m_attackedTowersList)
        attacker->targetKilled();

    foreach (Cannon *attacker, m_attackedCannonList)
        attacker->targetKilled1();
    // 通知game,此敌人已经阵亡
    m_game->removedEnemy(this);
}


void Enemy::getDamage(int damage)
{
    m_currentHp -= damage;
    // 阵亡,需要移除
    if (m_currentHp <= 0)
    {
        m_game->awardGold(150);
        getRemoved();
    }
}

void Enemy::getAttacked(Tower *attacker)
{
    m_attackedTowersList.push_back(attacker);
}

void Enemy::getAttacked(Cannon *attacker)
{
    m_attackedCannonList.push_back(attacker);
}
// 表明敌人已经逃离了攻击范围
void Enemy::gotLostSight(Tower *attacker)
{
    m_attackedTowersList.removeOne(attacker);
}

void Enemy::gotLostSight(Cannon *attacker)
{
    m_attackedCannonList.removeOne(attacker);
}

QPoint Enemy::pos() const
{
    return m_pos;
}
