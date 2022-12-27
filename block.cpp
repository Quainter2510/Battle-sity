#include "block.h"


Block::Block(int x, int y, int blockDamage, int blockHp, std::string path){
    setPos(x, y);
    image = QPixmap(QString::fromStdString(path));
    hp = blockHp;
    type = types::BLOCK;
    damage = blockDamage;
    setZValue(50);
}

QRectF Block::boundingRect() const {
    return QRectF(image.rect());
}

void Block::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->drawPixmap(0, 0, image);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Block::takeDamage(int dmg) {
    if (hp == -1)
        return;
    hp -= dmg;
    if (hp <= 0)
        this->~Block();
}

types Block::getType() const {
    return type;
}

int Block::getDamage() {
    return damage;
}

BaseBlock::BaseBlock(int x, int y):
    Block(x, y, 1, 1, blockImages["base"]) {
    type = types::HERO;
}

void BaseBlock::takeDamage(int dmg) {
    emit endGame();
}


BonusBlock::BonusBlock(int x, int y, std::string image, bonusTypes bonus):
    Block(x, y, 0, -1, image){
    bonusType = bonus;
    type = types::BONUS;
    QTimer::singleShot(5000, this, &BonusBlock::destructor);
}

bonusTypes BonusBlock::getBonusType() const {
    return bonusType;
}

void BonusBlock::destructor() {
    this->~BonusBlock();
}


BushesBlock::BushesBlock(int x, int y, std::string image):
    Block(x, y, 0, 0, image){
    type = types::BUSHES;
    this->setZValue(100);
}

void BushesBlock::takeDamage(int dmg) {
    return;
}

