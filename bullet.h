#ifndef BULLET_H
#define BULLET_H
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>
#include <QTimer>
#include <QPainter>
#include "GameObject.h"
#include <iostream>

class Bullet: public GameObject
{
    Q_OBJECT
public:
    Bullet(QGraphicsScene* scene, QTimer* tmr, direction directoin, types bullet_type, int bullet_speed, int damage, int x, int y);
    ~Bullet() = default;
    virtual void takeDamage(int dmg) override;
    virtual int getDamage() override;

public slots:
    void timerSlot();

private:
    QTimer* timer;
    direction dir;
    int speed;
    int damage;
    int hp;
    int x, y;
    std::pair<int, int> dp;
    QPixmap image;
    QGraphicsScene* scene;
    std::map<std::string, QPixmap> images;
    virtual types getType() const override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // BULLET_H
