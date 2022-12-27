#ifndef ENAMYTANK_H
#define ENAMYTANK_H
#include <QGraphicsItem>
#include <windows.h>
#include <QPainter>
#include <QTimer>
#include <QObject>
#include <cmath>
#include <ctime>
#include <queue>
#include <QGraphicsScene>
#include "bullet.h"
#include "herotank.h"

class EnemyTank: public Tank {
    Q_OBJECT
public:
    explicit EnemyTank(int, int, QTimer* timer, QGraphicsScene* sc, types tankTypes, std::vector<std::vector<int>>);
    ~EnemyTank();
    virtual void takeDamage(int dmg) override;
    virtual int getDamage() override;

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual types getType() const override;

signals:
    void destruction();

public slots:
    virtual void timerSlot() override;

protected:
    virtual void moveLeft();
    virtual void moveRight();
    virtual void moveUp();
    virtual void moveDown();
    virtual void shot();
    virtual bool checkHero();
    virtual void addMoves();

    std::vector<std::vector<int>> field;
    std::queue<int> moveQueue;
    int percentageBestMove;
    int percentageShot;

};

class EnemyMediumTank: public EnemyTank {
public:
    EnemyMediumTank(int, int, QTimer* timer, QGraphicsScene* sc, types tankTypes, std::vector<std::vector<int>>);
    ~EnemyMediumTank() = default;
};


class EnemyHeavyTank: public EnemyTank {
    Q_OBJECT

public:
    EnemyHeavyTank(int, int, QTimer* timer, QGraphicsScene* sc, types tankTypes, std::vector<std::vector<int>>);
    ~EnemyHeavyTank() = default;
};


class EnemyLightTank: public EnemyTank {
    Q_OBJECT
public:
    EnemyLightTank(int, int, QTimer* timer, QGraphicsScene* sc, types tankTypes, std::vector<std::vector<int>>);
    ~EnemyLightTank() = default;
};

#endif // ENAMYTANK_H
