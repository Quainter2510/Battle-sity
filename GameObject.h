#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <QObject>
#include <QGraphicsItem>
#include "config.h"

class GameObject: public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    virtual void takeDamage(int dmg) = 0;
    virtual types getType() const = 0;
    virtual int getDamage() = 0;
    virtual ~GameObject() = default;

protected:
    virtual QRectF boundingRect() const = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;

    types type;
};

#endif // GAMEOBJECT_H
