#include "bullet.h"

Bullet::Bullet(QGraphicsScene* sc,
               QTimer* tmr,
               direction directoin,
               types bulletType,
               int bulletSpeed,
               int b_damage,
               int bullet_x,
               int bullet_y) {
    images = {
        {"up", QPixmap(":/bigimages\\bigImages\\bullet_up.png")},
        {"left", QPixmap(":/bigimages\\bigImages\\bullet_left.png")},
        {"down", QPixmap(":/bigimages\\bigImages\\bullet_down.png")},
        {"right", QPixmap(":/bigimages\\bigImages\\bullet_right.png")}
             };
    x = bullet_x;
    y = bullet_y;
    setPos(x, y);
    timer = tmr;
    scene = sc;
    dir = directoin;
    type = bulletType;
    speed = bulletSpeed;
    damage = b_damage;
    hp = 1;
    setZValue(75);
    QObject::connect(timer, &QTimer::timeout, this, &Bullet::timerSlot);
    if (dir == direction::DOWN) {
        image = images["down"];
        dp = std::make_pair(0, 1);
    }
    if (dir == direction::LEFT) {
        image = images["left"];
        dp = std::make_pair(-1, 0);
    }
    if (dir == direction::UP) {
        image = images["up"];
        dp = std::make_pair(0, -1);
    }
    if (dir == direction::RIGHT) {
        image = images["right"];
        dp = std::make_pair(1, 0);
    }
}

void Bullet::takeDamage(int dmg) {
    hp -= dmg;
    if (hp <= 0)
        this->~Bullet();
}

int Bullet::getDamage() {
    return 1;
}


void Bullet::timerSlot() {
    x += dp.first * speed;
    y += dp.second * speed;
    setPos(x, y);
    if (x < -100 || x > WIDTH + 100 || y < -100 || y > HEIGHT + 100) {
        this->~Bullet();
        return;
    }
    QList<QGraphicsItem *> foundItems = scene->items(mapRectToScene(image.rect()));
    for (auto item : foundItems) {
        auto element = static_cast<GameObject*>(item);
        if (element->getType() != this->type) {
            element->takeDamage(damage);
            this->takeDamage(element->getDamage());
            return;
        }
    }

}

types Bullet::getType() const {
    return type;
}

QRectF Bullet::boundingRect() const {
    return QRectF(image.rect());
}

void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->drawPixmap(0, 0, image);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}




