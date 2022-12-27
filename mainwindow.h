#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "herotank.h"
#include "enemytank.h"
#include "enemylighttank.h"
#include "enemymediumtank.h"
#include "enemyheavytank.h"
#include "block.h"
#include <QTimer>
#include <QTime>
#include <QFile>
#include <ctime>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void stopwatchUpdate();
    void leftMapBtnClicked();
    void rightMapBtnClicked();
    void leftLevelBtnClicked();
    void rightLevelBtnClicked();
    void addEnemy();
    void startGame();
    void addBonus();
    void applyBonus();


private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QTimer *gameTimer;
    QTimer *enemyTimer;
    QTimer *stopwatchTimer;
    QTimer *bonusTimer;
    std::vector<std::vector<int>> field;
    std::map<int, std::string> choiseMap;
    std::map<int, std::string> choiseLevel;
    int indexMap;
    int indexLevel;
    int kills;
    int pointers;
    QTime stopwatch;
    HeroTank *tank;
    BaseBlock *base;

    void setMap(const std::string &path);
    void generateMap(const std::string &path);
    void endGame();
    void changeHeroHp();
    void uploadMap();
    void changeKillNumber();
};
#endif // MAINWINDOW_H
