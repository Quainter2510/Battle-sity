#include "enemytank.h"

EnemyLightTank::EnemyLightTank(int tmpX, int tmpY, QTimer* timer,
                               QGraphicsScene* sc, types tankTypes, std::vector<std::vector<int>> field):
EnemyTank(tmpX, tmpY, timer, sc, tankTypes, field){
    images = {
        {"up", QPixmap(":images\\images\\tank4_up.png")},
        {"left", QPixmap(":images\\images\\tank4_left.png")},
        {"down", QPixmap(":images\\images\\tank4_down.png")},
        {"right", QPixmap(":images\\images\\tank4_right.png")}
             };
    image = images["down"];
    percentageBestMove = 50;
    percentageShot = 2;
    speed = 7;
    bulletSpeed = 8;
    damage = 1;
    hp = 1;
    cd = 1100;
}
