#include "enemytank.h"

EnemyMediumTank::EnemyMediumTank(int tmpX, int tmpY, QTimer* timer,
                               QGraphicsScene* sc, types tankTypes, std::vector<std::vector<int>> field):
EnemyTank(tmpX, tmpY, timer, sc, tankTypes, field){
    images = {
        {"up", QPixmap(":images\\images\\tank5_up.png")},
        {"left", QPixmap(":images\\images\\tank5_left.png")},
        {"down", QPixmap(":images\\images\\tank5_down.png")},
        {"right", QPixmap(":images\\images\\tank5_right.png")}
             };
    image = images["down"];
    percentageBestMove = 70;
    percentageShot = 3;
    speed = 6;
    bulletSpeed = 7;
    damage = 1;
    hp = 1;
    cd = 800;
}
