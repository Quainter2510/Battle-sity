#include "mainwindow.h"
#include "./ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1250, 900);

    choiseMap = {{0, "Случайная"}, {1, "Последняя"}, {2, "Карта 1"}, {3, "Карта 2"}, {4, "Карта 3"}};
    choiseLevel = {{0, "Легко"}, {1, "Средне"}, {2, "Сложно"}};
    indexMap = 0;
    indexLevel = 1;
    kills = 0;
    pointers = 0;
    field.resize(16, std::vector<int> (21, 1e2));
    srand(time(0));
    scene = new QGraphicsScene();
    gameTimer = new QTimer();
    enemyTimer = new QTimer();
    stopwatchTimer = new QTimer();
    bonusTimer = new QTimer();

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scene->setSceneRect(0, 0, WIDTH, HEIGHT);
    scene->setBackgroundBrush(Qt::black);
    ui->stacked->setCurrentWidget(ui->mainMenu);

    stopwatch = QTime(0, 0);
    this->addEnemy();
    connect(stopwatchTimer, &QTimer::timeout, this, &MainWindow::stopwatchUpdate);
    connect(bonusTimer, &QTimer::timeout, this, &MainWindow::addBonus);
    connect(ui->btn, &QPushButton::clicked, this, &MainWindow::startGame);
    connect(ui->leftMapBtn, &QPushButton::clicked, this, &MainWindow::leftMapBtnClicked);
    connect(ui->leftLevelBtn, &QPushButton::clicked, this, &MainWindow::leftLevelBtnClicked);
    connect(ui->rightMapBtn, &QPushButton::clicked, this, &MainWindow::rightMapBtnClicked);
    connect(ui->rightLevelBtn, &QPushButton::clicked, this, &MainWindow::rightLevelBtnClicked);
    connect(ui->mainMenuBtn, &QPushButton::clicked, this, [=](){ui->stacked->setCurrentWidget(ui->mainMenu);});
    connect(ui->quitBtn, &QPushButton::clicked, this, [=](){this->close();});
}

MainWindow::~MainWindow() {
    delete ui;
    delete gameTimer;
    delete enemyTimer;
    delete stopwatchTimer;
    delete bonusTimer;
    delete tank;
    delete base;
    scene->clear();
    delete scene;
}

void MainWindow::addBonus() {
    int x = rand() % (WIDTH - 100) + 50;
    int y = rand() % (HEIGHT - 100) + 50;
    int bonusType = rand() % 3;
    BonusBlock* bonusBlock;
    switch(bonusType) {
    case 0:
        bonusBlock = new BonusBlock(x, y, bonusImages.at("level"), bonusTypes::LEVELBONUS);
        break;
    case 1:
        bonusBlock = new BonusBlock(x, y, bonusImages.at("base"), bonusTypes::BASEBONUS);
        break;
    case 2:
        bonusBlock = new BonusBlock(x, y, bonusImages.at("time"), bonusTypes::TIMEBONUS);
        break;
    }
    scene->addItem(bonusBlock);
    connect(bonusBlock, &BonusBlock::applyBonus, this, &MainWindow::applyBonus);
}


void MainWindow::applyBonus() {
    BonusBlock* bonusBlock = static_cast<BonusBlock*>(sender());
    if (bonusBlock->getBonusType() == bonusTypes::BASEBONUS) {
        scene->addItem(new Block(9 * IMAGE_SIZE, 15 * IMAGE_SIZE, 1, 1, blockImages["destr"]));
        scene->addItem(new Block(11 * IMAGE_SIZE, 15 * IMAGE_SIZE, 1, 1, blockImages["destr"]));
        scene->addItem(new Block(9 * IMAGE_SIZE, 14 * IMAGE_SIZE, 1, 1, blockImages["destr"]));
        scene->addItem(new Block(10 * IMAGE_SIZE, 14 * IMAGE_SIZE, 1, 1, blockImages["destr"]));
        scene->addItem(new Block(11 * IMAGE_SIZE, 14 * IMAGE_SIZE, 1, 1, blockImages["destr"]));
    }
    else if (bonusBlock->getBonusType() == bonusTypes::LEVELBONUS) {
        tank->levelUp();
        ui->labelLevel->setText(QString::number(tank->getLevel()));
    }
    else if (bonusBlock->getBonusType() == bonusTypes::TIMEBONUS) {
        enemyTimer->stop();
        QTimer::singleShot(2500, this, [=](){enemyTimer->start(1000 / FPS);});
    }
}


void MainWindow::rightMapBtnClicked() {
    indexMap = (indexMap + 1) % 5;
    ui->mapLabel->setText(QString::fromStdString(choiseMap.at(indexMap)));
}

void MainWindow::rightLevelBtnClicked() {
    indexLevel = (indexLevel + 1) % 3;
    ui->levelLabel->setText(QString::fromStdString(choiseLevel.at(indexLevel)));
}

void MainWindow::leftMapBtnClicked() {
    indexMap = indexMap ? (indexMap - 1) % 5 : 4;
    ui->mapLabel->setText(QString::fromStdString(choiseMap.at(indexMap)));
}

void MainWindow::leftLevelBtnClicked() {
    indexLevel = indexLevel ? (indexLevel - 1) % 3 : 2;
    ui->levelLabel->setText(QString::fromStdString(choiseLevel.at(indexLevel)));
}

void MainWindow::changeHeroHp() {
    int hp = tank->getHp();
    if (hp >= 0)
        ui->labelHp->setText(QString::number(hp));
   else
        ui->labelHp->setText(QString::fromStdString("inf"));

}

void MainWindow::changeKillNumber() {
    kills++;
    ui->labelKillNum->setText(QString::number(kills));
    pointers += 50;
    ui->labelPointers->setText(QString::number(pointers));
}

void MainWindow::uploadMap() {
    if (indexMap < 2) {
        int repeat = 0;
        do {
            if (repeat == 30){
                std::cout << "many repeat" << std::endl;
                setMap(":/maps/maps/map1.txt");
                return;
            }
            if (indexMap == 0)
                generateMap("maps/randomMap.txt");
            setMap("maps/randomMap.txt");
            repeat++;
        } while (field[0][0] == 1e2);
    }
    else if (indexMap == 2)
        setMap(":/maps/maps/map1.txt");
    else if (indexMap == 3)
        setMap(":/maps/maps/map2.txt");
    else if (indexMap == 4)
        setMap(":/maps/maps/map3.txt");
}

void MainWindow::addEnemy() {
    int position = rand() % WIDTH;
    int repeat = 0;
    QList<QGraphicsItem *> foundItems = scene->items(QRectF(position, -50, IMAGE_SIZE, IMAGE_SIZE));
    while (!foundItems.empty()) {
        if (repeat == 15)
            return;
        repeat++;
        position = rand() % WIDTH;
        foundItems.clear();
        foundItems = scene->items(QRectF(position, -50, IMAGE_SIZE, IMAGE_SIZE));
    }

    EnemyTank *enemy;
    switch (rand() % 3) {
       case 0:
        enemy = new EnemyLightTank(position, -50, enemyTimer, scene, types::ENEMY, field);
        break;
       case 1:
        enemy = new EnemyMediumTank(position, -50, enemyTimer, scene, types::ENEMY, field);
        break;
       case 2:
        enemy = new EnemyHeavyTank(position, -50, enemyTimer, scene, types::ENEMY, field);
        break;
    }

    connect(enemy, &EnemyTank::destruction, this, &MainWindow::changeKillNumber);
    QTimer::singleShot(8000, this, &MainWindow::addEnemy);
}

void MainWindow::startGame() {
    ui->stacked->setCurrentWidget(ui->play);
    this->uploadMap();
    kills = 0;
    pointers = 0;

    int heroHp = 0;
    if (indexLevel == 0)
        heroHp = -1;
    else if (indexLevel == 1)
        heroHp = 3;
    else if (indexLevel == 2)
        heroHp = 1;

    ui->labelTimer->setText("00:00");
    ui->labelLevel->setText("1");
    ui->labelKillNum->setText(QString::number(kills));
    ui->labelPointers->setText(QString::number(pointers));
    tank = new HeroTank(heroHp, gameTimer, scene, types::HERO);
    base = new BaseBlock(500, 750);
    this->changeHeroHp();
    scene->addItem(base);
    connect(base, &BaseBlock::endGame, this, &MainWindow::endGame);
    connect(tank, &HeroTank::gameOver, this, &MainWindow::endGame);
    connect(tank, &HeroTank::changeHp, this, &MainWindow::changeHeroHp);
    gameTimer->start(1000 / FPS);
    enemyTimer->start(1000 / FPS);
    stopwatchTimer->start(1000);
    bonusTimer->start(10000);
    for (int i = 1; i < 4; i++) {
        EnemyTank *enemy = new EnemyMediumTank(300 * i, -50, enemyTimer, scene, types::ENEMY, field);
        connect(enemy, &EnemyTank::destruction, this, &MainWindow::changeKillNumber);
    }
}

void MainWindow::endGame() {
    ui->stacked->setCurrentWidget(ui->endGame);
    ui->labelResult->setText(QString::number(pointers));
    stopwatch = QTime(0, 0);
    gameTimer->stop();
    enemyTimer->stop();
    stopwatchTimer->stop();
    bonusTimer->stop();
}


void MainWindow::stopwatchUpdate() {
    stopwatch = stopwatch.addSecs(1);
    ui->labelTimer->setText(stopwatch.toString("mm:ss"));
    pointers += 10;
    ui->labelPointers->setText(QString::number(pointers));
}

void MainWindow::setMap(const std::string &path) {
    scene->clear();
    QFile file(QString::fromStdString(path));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
    for (int i = 0; i < 16; i++) {
        char line[42];
        file.readLine(line, sizeof(line));
        for (size_t j = 0; j < 21; j++) {
            if (line[j] == '#') {
                scene->addItem(new Block(j * 50, i * 50, 1, -1, blockImages["indestr"]));
            }
            if (line[j] == '*') {
                scene->addItem(new Block(j * 50, i * 50, 1, 1, blockImages["destr"]));
            }
            if (line[j] == '$') {
                scene->addItem(new Block(j * 50, i * 50, 0, -1, blockImages["water"]));
            }
            if (line[j] == '&') {
                scene->addItem(new BushesBlock(j * 50, i * 50, blockImages["bushes"]));
            }
        }
    }
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 21; j++) {
            field[i][j] = 100;
        }
    }
    field[15][10] = 0;
    std::queue<std::pair<int, int>> q;
    q.push(std::make_pair(15, 10));
    int nowX, nowY;
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};
    while (!q.empty()) {
        for (int i = 0; i < 4; i++) {
            nowY = q.front().first;
            nowX = q.front().second;
            QList<QGraphicsItem *> foundItems = scene->items(QRectF((nowX + dx[i]) * IMAGE_SIZE, (nowY + dy[i]) * IMAGE_SIZE, 1, 1));
            if (nowX + dx[i] >= 0 && nowX + dx[i] < 21 &&
                    nowY + dy[i] >= 0 && nowY + dy[i] < 16 &&
                    field[nowY][nowX] + 1 < field[nowY + dy[i]][nowX + dx[i]] &&
                    (foundItems.size() == 0 || static_cast<GameObject*>(foundItems[0])->getType() == types::BUSHES)) {
                field[nowY + dy[i]][nowX + dx[i]] = field[nowY][nowX] + 1;
                q.push(std::make_pair(nowY + dy[i], nowX + dx[i]));

            }
        }
        q.pop();
    }

    base = new BaseBlock(10 * IMAGE_SIZE, 15 * IMAGE_SIZE);
    scene->addItem(new Block(9 * IMAGE_SIZE, 15 * IMAGE_SIZE, 1, 1, blockImages["destr"]));
    scene->addItem(new Block(11 * IMAGE_SIZE, 15 * IMAGE_SIZE, 1, 1, blockImages["destr"]));
    scene->addItem(new Block(9 * IMAGE_SIZE, 14 * IMAGE_SIZE, 1, 1, blockImages["destr"]));
    scene->addItem(new Block(10 * IMAGE_SIZE, 14 * IMAGE_SIZE, 1, 1, blockImages["destr"]));
    scene->addItem(new Block(11 * IMAGE_SIZE, 14 * IMAGE_SIZE, 1, 1, blockImages["destr"]));
    scene->addItem(base);

    file.close();
}

void MainWindow::generateMap(const std::string& path) {
    QFile file(QString::fromStdString(path));
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
            return;
    int num;
    file.write(QByteArray("000000000000000000000'\n"));
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 21; j++) {
            if ((i == 14 || i == 13) && j > 7 && j < 12) {
                file.putChar('0');
                continue;
            }
            num = rand() % 100;
            if (num < PERCENTAGE_INDESRT_BLOCK) {
                file.putChar('#');
            }
            else if (num > 100 - PERCENTAGE_DESTR_BLOCK) {
                file.putChar('*');
            }
            else if (num > PERCENTAGE_INDESRT_BLOCK && num < PERCENTAGE_INDESRT_BLOCK + PERCENTAGE_WATER_BLOCK) {
                file.putChar('$');
            }
            else if (num > PERCENTAGE_INDESRT_BLOCK + PERCENTAGE_WATER_BLOCK &&
                     num < PERCENTAGE_INDESRT_BLOCK + PERCENTAGE_WATER_BLOCK + PERCENTAGE_BUSHES_BLOCK) {
                file.putChar('&');
            }
            else {
                file.putChar('0');
            }
        }
        file.putChar('\n');
    }
    file.close();
}








