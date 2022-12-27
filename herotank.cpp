#include "herotank.h"





HeroTank::HeroTank(int heroHp, QTimer* tmr, QGraphicsScene* sc, types tankTypes)
{
    images = {
        {"up", QPixmap(":images\\images\\tank1_up.png")},
        {"left", QPixmap(":images\\images\\tank1_left.png")},
        {"down", QPixmap(":images\\images\\tank1_down.png")},
        {"right", QPixmap(":images\\images\\tank1_right.png")}
             };
    image = images["up"];
    scene = sc;
    scene->addItem(this);
    x = 410;
    y = 760;
    dir = direction::UP;
    type = tankTypes;
    speed = 6;
    bulletSpeed = 7;
    damage = 1;
    hp = heroHp;
    cd = 300;
    timer = tmr;
    cooldown = false;
    level = 1;

    setPos(x, y);
    QObject::connect(timer, &QTimer::timeout, this, &HeroTank::timerSlot);
}

HeroTank::~HeroTank() {}

void HeroTank::takeDamage(int dmg) {
    QSound::play(":/sounds/sounds/destroy.wav");
    if (hp > 0) {
        hp -= dmg;
        emit changeHp();
    }
    if (hp == 0){
        QSound::play(":/sounds/sounds/dead.wav");
        emit gameOver();
    }
}

int HeroTank::getDamage() {
    return 1;
}

int HeroTank::getHp() const {
    return hp;
}

int HeroTank::getLevel() const {
    return level;
}

types HeroTank::getType() const {
    return type;
}

void HeroTank::levelUp() {
    level = level > 9 ? 10 : level + 1;
    if (level == 2 || level == 5 || level == 8)
        speed++;
    if (level == 3 || level == 7)
        bulletSpeed++;
    if (level == 6)
        damage++;
    if (level == 4 || level == 9)
        cd *= 0.9;
    if (level == 10 && hp > 0) {
        hp = 3;
        emit changeHp();
    }
}


void HeroTank::update() {
    image = images["up"];
    hp = 3;
    x = 410;
    y = 760;
    setPos(x, y);
}

void HeroTank::timerSlot() {
    if (GetAsyncKeyState(0x41)) { // left = a
        dir = direction::LEFT;
        image = images["left"];
        int oldX = x;
        x = std::max(0, x - speed);
        setPos(x, y);
        QList<QGraphicsItem *> foundItems = scene->items(mapRectToScene(image.rect()));

        for (auto element : foundItems) {
            auto objType = static_cast<GameObject*>(element)->getType();
            if (objType == types::BLOCK || objType == types::ENEMY) {
                x= oldX;
                setPos(x, y);
            }
            else if (objType == types::BONUS) {
                auto bonus = static_cast<BonusBlock*>(element);
                emit bonus->applyBonus();
//                this->takeBonus();
                scene->removeItem(element);
            }
        }
    }
    else if (GetAsyncKeyState(0x44)) { // right = d
        dir = direction::RIGHT;
        image = images["right"];
        int oldX = x;
        x = std::min(WIDTH - image.width(), x + speed);
        setPos(x, y);
        QList<QGraphicsItem *> foundItems = scene->items(mapRectToScene(image.rect()));
        for (auto element : foundItems) {
            auto objType = static_cast<GameObject*>(element)->getType();
            if (objType == types::BLOCK || objType == types::ENEMY) {
                x = oldX;
                setPos(x, y);
            }
            else if (objType == types::BONUS) {
                auto bonus = static_cast<BonusBlock*>(element);
                emit bonus->applyBonus();
//                this->takeBonus();
                scene->removeItem(element);
            }
        }
    }
    else if (GetAsyncKeyState(0x57)) { // up = w
        dir = direction::UP;
        image = images["up"];
        int oldY = y;
        y = std::max(0, y - speed);
        setPos(x, y);
        QList<QGraphicsItem *> foundItems = scene->items(mapRectToScene(image.rect()));
        for (auto element : foundItems) {
            auto objType = static_cast<GameObject*>(element)->getType();
            if (objType == types::BLOCK || objType == types::ENEMY) {
                y = oldY;
                setPos(x, y);
            }
            else if (objType == types::BONUS) {
                auto bonus = static_cast<BonusBlock*>(element);
                emit bonus->applyBonus();
//                this->takeBonus();
                scene->removeItem(element);
            }
        }
    }
    else if (GetAsyncKeyState(0x53)) { //down = s
        dir = direction::DOWN;
        image = images["down"];
        int oldY = y;
        y = std::min(HEIGHT - image.height(), y + speed);
        setPos(x, y);
        QList<QGraphicsItem *> foundItems = scene->items(mapRectToScene(image.rect()));
        for (auto element : foundItems) {
            auto objType = static_cast<GameObject*>(element)->getType();
            if (objType == types::BLOCK || objType == types::ENEMY) {
                y = oldY;
                setPos(x, y);
            }
            else if (objType == types::BONUS) {
                auto bonus = static_cast<BonusBlock*>(element);
                emit bonus->applyBonus();
//                this->takeBonus();
                scene->removeItem(element);
            }
        }
    }
    if (GetAsyncKeyState(VK_SPACE) and !cooldown) {
//        QSound::play(":/sounds/sounds/shot.wav");
        Bullet* bullet = new Bullet(scene, timer, dir, type, bulletSpeed, damage, x + image.width() / 2, y + image.height() / 2);
        cooldown = true;
        scene->addItem(bullet);
        QTimer::singleShot(cd, this, [=](){cooldown = false;});
    }
}

QRectF HeroTank::boundingRect() const {
    return QRectF(image.rect());
}

void HeroTank::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->drawPixmap(0, 0, image);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
