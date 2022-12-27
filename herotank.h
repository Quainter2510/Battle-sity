#ifndef HEROTANK_H
#define HEROTANK_H
#include <QGraphicsItem>
#include <windows.h>
#include <QPainter>
#include <QTimer>
#include <QObject>
#include <cmath>
#include <QGraphicsScene>
#include <QSound>
#include "bullet.h"
#include "GameObject.h"
#include "block.h"


class Tank: public GameObject
{
    Q_OBJECT
public:
    virtual void takeDamage(int dmg) = 0;
    virtual int getDamage() = 0;
    virtual ~Tank() = default;

public slots:
    virtual void timerSlot() = 0;

protected:
    int x, y, speed, bulletSpeed, hp, damage, cd;
    bool cooldown;
    std::map<std::string, QPixmap> images;
    direction dir;
    QPixmap image;
    QTimer* timer;
    QGraphicsScene* scene;

    virtual QRectF boundingRect() const = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;
    virtual types getType() const = 0;
};




class HeroTank: public Tank {
    Q_OBJECT
public:
    explicit HeroTank(int hp, QTimer* timer, QGraphicsScene* sc, types tankTypes);
    ~HeroTank();
    void takeDamage(int dmg) override;
    int getDamage() override;
    void update();
    int getHp() const;
    int getLevel() const;
    void levelUp();

public slots:
    void timerSlot() override;

signals:
    void gameOver();
    void changeHp();

private:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    types getType() const override;
    int level;



};


#endif // HEROTANK_H
