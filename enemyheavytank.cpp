#include "enemytank.h"

EnemyHeavyTank::EnemyHeavyTank(int tmpX, int tmpY, QTimer* timer,
                               QGraphicsScene* sc, types tankTypes, std::vector<std::vector<int>> field):
EnemyTank(tmpX, tmpY, timer, sc, tankTypes, field){
    images = {
        {"up", QPixmap(":images\\images\\tank3_up.png")},
        {"left", QPixmap(":images\\images\\tank3_left.png")},
        {"down", QPixmap(":images\\images\\tank3_down.png")},
        {"right", QPixmap(":images\\images\\tank3_right.png")}
             };
    image = images["down"];
    percentageBestMove = 55;
    percentageShot = 4;
    speed = 2;
    bulletSpeed = 5;
    damage = 1;
    hp = 3;
    cd = 1000;
}
