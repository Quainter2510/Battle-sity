#include "enemytank.h"

EnemyTank::EnemyTank(int startX, int startY, QTimer* tmr, QGraphicsScene* sc, types tankTypes, std::vector<std::vector<int>> tmpField)
{
    images = {
        {"up", QPixmap(":images\\images\\tank3_up.png")},
        {"left", QPixmap(":images\\images\\tank3_left.png")},
        {"down", QPixmap(":images\\images\\tank3_down.png")},
        {"right", QPixmap(":images\\images\\tank3_right.png")}
             };
    image = images["down"];
    scene = sc;
    scene->addItem(this);
    percentageBestMove = 70;
    percentageShot = 5;
    field = tmpField;
    x = startX;
    y = startY;
    dir = direction::UP;
    type = tankTypes;
    speed = 6;
    bulletSpeed = 8;
    damage = 1;
    hp = 1;
    cd = 700;
    timer = tmr;
    cooldown = false;
    srand(time(0));
    setPos(x, y);
    QObject::connect(timer, &QTimer::timeout, this, &EnemyTank::timerSlot);
    setZValue(25);
}

EnemyTank::~EnemyTank() {
    emit destruction();
}

void EnemyTank::takeDamage(int dmg) {
    hp -= dmg;
    if (hp <= 0)
        this->~EnemyTank();
}

int EnemyTank::getDamage() {
    return 1;
}

types EnemyTank::getType() const {
    return type;
}

bool EnemyTank::checkHero() {
    int tmpX = x + 16;
    int tmpY = y + 16;
    if (rand() % 100 < 10) {
        return false;
    }

    while (tmpX > 0) {
        tmpX -= 32;
        QList<QGraphicsItem *> foundItems = scene->items(QRectF(tmpX, tmpY, 1, 1));
        for (auto elem : foundItems) {
            auto element = static_cast<GameObject*>(elem);
            if (element->getType() == types::HERO) {
                this->moveLeft();
                this->shot();
                return true;
            }
            else
                tmpX = 0;
        }
    }
    tmpX = x + 16;
    tmpY = y + 16;
    while (tmpX < WIDTH) {
        tmpX += 32;
        QList<QGraphicsItem *> foundItems = scene->items(QRectF(tmpX, tmpY, 1, 1));
        for (auto elem : foundItems) {
            auto element = static_cast<GameObject*>(elem);
            if (element->getType() == types::HERO) {
                this->moveRight();
                this->shot();
                return true;
            }
            else
                tmpX = WIDTH;
        }
    }
    tmpX = x + 16;
    tmpY = y + 16;
    while (tmpY > 0) {
        tmpY -= 32;
        QList<QGraphicsItem *> foundItems = scene->items(QRectF(tmpX, tmpY, 1, 1));
        for (auto elem : foundItems) {
            auto element = static_cast<GameObject*>(elem);
            if (element->getType() == types::HERO) {
                this->moveUp();
                this->shot();
                return true;
            }
            else
                tmpY = 0;;
        }
    }
    tmpX = x + 16;
    tmpY = y + 16;
    while (tmpY < HEIGHT) {
        tmpY += 32;
        QList<QGraphicsItem *> foundItems = scene->items(QRectF(tmpX, tmpY, 1, 1));
        for (auto elem : foundItems) {
            auto element = static_cast<GameObject*>(elem);
            if (element->getType() == types::HERO) {
                this->moveDown();
                this->shot();
                return true;
            }
            else
                tmpY = HEIGHT;
        }
    }
    return false;
}

void EnemyTank::moveLeft() {
    dir = direction::LEFT;
    image = images["left"];
    int oldX = x;
    x = std::max(0, x - speed);
    setPos(x, y);
    QList<QGraphicsItem*> foundItems = scene->items(mapRectToScene(image.rect()));
    for (QGraphicsItem* element : foundItems) {
        GameObject* obj = static_cast<GameObject*>(element);
        if (obj != this && obj->getType() != types::BONUS && obj->getType() != types::BUSHES) {
            x = oldX;
            setPos(x, y);
        }
    }
}

void EnemyTank::moveRight() {
    dir = direction::RIGHT;
    image = images["right"];
    int oldX = x;
    x = std::min(WIDTH - image.width(), x + speed);
    setPos(x, y);
    QList<QGraphicsItem *> foundItems = scene->items(mapRectToScene(image.rect()));
    for (QGraphicsItem* element : foundItems) {
        GameObject* obj = static_cast<GameObject*>(element);
        if (obj != this && obj->getType() != types::BONUS && obj->getType() != types::BUSHES) {
            x = oldX;
            setPos(x, y);
        }
    }
}

void EnemyTank::moveUp() {
    dir = direction::UP;
    image = images["up"];
    int oldY = y;
    y = std::max(0, y - speed);
    setPos(x, y);
    QList<QGraphicsItem *> foundItems = scene->items(mapRectToScene(image.rect()));
    for (QGraphicsItem* element : foundItems) {
        GameObject* obj = static_cast<GameObject*>(element);
        if (obj != this && obj->getType() != types::BONUS && obj->getType() != types::BUSHES) {
            y = oldY;
            setPos(x, y);
        }
    }
}

void EnemyTank::moveDown() {
    dir = direction::DOWN;
    image = images["down"];
    int oldY = y;
    y = std::min(HEIGHT - image.height(), y + speed);
    setPos(x, y);
    QList<QGraphicsItem *> foundItems = scene->items(mapRectToScene(image.rect()));
    for (QGraphicsItem* element : foundItems) {
        GameObject* obj = static_cast<GameObject*>(element);
        if (obj != this && obj->getType() != types::BONUS && obj->getType() != types::BUSHES) {
            y = oldY;
            setPos(x, y);
        }
    }
}

void EnemyTank::shot() {
//    QSound::play(":/sounds/sounds/shot.wav");
    if (cooldown)
        return;
    Bullet* bullet = new Bullet(scene, timer, dir, type, bulletSpeed, damage, x + image.width() / 2, y + image.height() / 2);
    cooldown = true;
    scene->addItem(bullet);
    QTimer::singleShot(cd, this, [=](){cooldown = false;});
}

void EnemyTank::addMoves() {
    int min = 1e2, index;
    int currX = x / IMAGE_SIZE;
    int currY = y / IMAGE_SIZE;
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    for (int i = 0; i < 4; i++) {
        if (currY + dy[i] >= 0 && currY + dy[i] < 16 &&
                currX + dx[i] >= 0 && currX + dx[i] < 21 &&
                field[currY + dy[i]][currX + dx[i]] < min) {
            min = field[currY + dy[i]][currX + dx[i]];
            index = i;
        }
    }
    int numMoves = rand() % 25 + 5;
    for (int i = 0; i < numMoves; i++) {
        moveQueue.push(index);
    }
}

void EnemyTank::timerSlot() {
    if (x < 0) {
        this->moveDown();
        return;
    }
    if (this->checkHero())
        return;

    int move = rand() % 100;
    if (moveQueue.empty()) {
        if (move < percentageBestMove) {
            this->addMoves();
        }
        else {
            int direct = rand() % 4;
            int numMoves = rand() % 25 + 5;
            for (int i = 0; i < numMoves; i++) {
                moveQueue.push(direct);
            }
        }
    }
    if (move < percentageShot)
        this->shot();



    switch (moveQueue.front()) {
       case 0:
        this->moveRight();
        break;
       case 1:
        this->moveLeft();
        break;
       case 2:
        this->moveDown();
        break;
       case 3:
        this->moveUp();
        break;
    }

    moveQueue.pop();
}

QRectF EnemyTank::boundingRect() const {
    return QRectF(image.rect());
}

void EnemyTank::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->drawPixmap(0, 0, image);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
