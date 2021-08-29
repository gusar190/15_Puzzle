#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <algorithm>
#include <random>
#include <chrono>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->geometry().width(),this->geometry().height());

    secondsTimer = new QTimer(this);
    gameTimeInSeconds = 0;
    connect(secondsTimer, &QTimer::timeout, this, &MainWindow::incrementSecondsTimer);
    secondsTimer->start(1000);

    newGameButton = new QPushButton("NEW GAME", this);
    newGameButton->setGeometry((QRect(QPoint(58, 320), QSize(140, 32))));
    connect(newGameButton, SIGNAL(released()), this, SLOT(newGame()));

    gameStateLabel = new QLabel(this);
    gameStateLabel->setGeometry(QRect(QPoint(58, 0),QSize(140, 32)));
    gameStateLabel->setScaledContents(true);
    gameState.load(":/images/ThinkPlay.jpg");
    gameStateLabel->setPixmap(gameState);


    gameTimeLabel = new QLabel(this);
    gameTimeLabel->setGeometry(QRect(QPoint(207, 305),QSize(34, 10)));
    gameTimeLabel->setText("00:00");
    updateGameTimeLabel();

    tileMovesLabel = new QLabel(this);
    tileMovesLabel->setGeometry(QRect(QPoint(3, 305),QSize(100, 10)));
    tileMovesLabel->setText("Your moves: 0");
    tileMoves = 0;
    updateTileMovesLabel();

    gameTimeInSeconds = 0;

    makeField();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*********************SIGNALS*********************/
void MainWindow::newGame(){
    shuffleField();
    drawField();
    tileMoves = 0;
    updateTileMovesLabel();
    gameState.load(":/images/ThinkPlay.jpg");
    gameStateLabel->setPixmap(gameState);
    gameTimeInSeconds = 0;
    updateGameTimeLabel();
    secondsTimer->start();
}

void MainWindow::tilePressed(){
    QPushButton *button = (QPushButton*)sender();
    int x1, y1, x2, y2;
    button->geometry().getCoords(&x1, &y1, &x2, &y2);
    uint8_t i = y1 / TILE_HEIGHT;
    uint8_t j = x1 / TILE_WIDTH;
    while(true){
        if( i != 0 ){
            if( tiles[i-1][j] == nullptr ){
                tiles[i-1][j] = button;
                tiles[i][j] = nullptr;
                ++tileMoves;
                break;
            }
        }
        if( i + 1 < ROWS_NUMBER ){
            if( tiles[i+1][j] == nullptr){
                tiles[i+1][j] = button;
                tiles[i][j] = nullptr;
                ++tileMoves;
                break;
            }
        }
        if( j != 0 ){
            if( tiles[i][j-1] == nullptr ){
                tiles[i][j-1] = button;
                tiles[i][j] = nullptr;
                ++tileMoves;
                break;
            }
        }
        if( j + 1 < COLS_NUMBER ){
            if( tiles[i][j+1] == nullptr){
                tiles[i][j+1] = button;
                tiles[i][j] = nullptr;
                ++tileMoves;
                break;
            }
        }
        break;
    }
    drawField();
    updateTileMovesLabel();
    if( checkFieldState() == fieldStateArranged ){
        secondsTimer->stop();
        gameState.load(":/images/UWinCongrats.jpg");
        gameStateLabel->setPixmap(gameState);
    }
}
/******************************************/

void MainWindow::makeField(){
    uint8_t tilesCounter(0);
    for( uint8_t i = 0 ; i < ROWS_NUMBER ; i++ ){
        for( uint8_t j = 0 ; j < COLS_NUMBER ; j++ ){
            if(tilesCounter == TILES_NUMBER){
                tiles[i][j] = nullptr;
                break;
            }
            tiles[i][j] = new QPushButton(this);
            //this->addDockWidget()
            tiles[i][j]->setGeometry(QRect(QPoint(GAME_FIELD_LEFT_UPPER_X + j * TILE_WIDTH, GAME_FIELD_LEFT_UPPER_Y + i * TILE_HEIGHT), QSize(TILE_WIDTH, TILE_HEIGHT)));
            tiles[i][j]->setText(std::to_string(tilesCounter + 1).c_str());
            connect(tiles[i][j], SIGNAL(released()), this, SLOT(tilePressed()));
            ++tilesCounter;
        }
    }
}

void MainWindow::drawField(){
    for( uint8_t i = 0 ; i < ROWS_NUMBER ; i++ ){
        for( uint8_t j = 0 ; j < COLS_NUMBER ; j++ ){
            if( tiles[i][j] != nullptr){
                tiles[i][j]->setGeometry(QRect(QPoint(GAME_FIELD_LEFT_UPPER_X + j * TILE_WIDTH, GAME_FIELD_LEFT_UPPER_Y + i * TILE_HEIGHT), QSize(TILE_WIDTH, TILE_HEIGHT)));
            }
        }
    }
}

void MainWindow::shuffleField(){
    std::vector<int> numberArray;
    for( uint16_t i = 1 ; i <= TILES_NUMBER ; i++){
        numberArray.push_back(i);
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();  // To obtain a time-based seed
    std::shuffle( numberArray.begin(), numberArray.end(), std::default_random_engine(seed));
    for( uint8_t i = 0 ; i < ROWS_NUMBER ; i++ ){
        for( uint8_t j = 0 ; j < COLS_NUMBER ; j++ ){
            if( tiles[i][j] != nullptr ){
                tiles[i][j]->setText(QString::number(numberArray[i * COLS_NUMBER + j]));
            } else {
                tiles[i][j] = tiles[ROWS_NUMBER - 1][COLS_NUMBER - 1];
                tiles[ROWS_NUMBER - 1][COLS_NUMBER - 1] = nullptr;
            }
        }
    }
}

fieldState MainWindow::checkFieldState(){
    for( uint8_t i = 0 ; i < ROWS_NUMBER ; i++ ){
        for( uint8_t j = 0 ; j < COLS_NUMBER ; j++ ){
            if( tiles[i][j] != nullptr){;
                if((i * COLS_NUMBER + j + 1) != tiles[i][j]->text().toInt() )
                {
                    return fieldStateNotArranged;
                }

            }
        }
    }
    return fieldStateArranged;
}

void MainWindow::updateTileMovesLabel(){
    tileMovesLabel->setText("Your moves: " + QString::number(tileMoves));
}

void MainWindow::incrementSecondsTimer(){
    ++gameTimeInSeconds;
}

void MainWindow::updateGameTimeLabel(){
    gameTimeLabel->setText(QString::number(gameTimeInSeconds / (3600)) + ":" + QString::number(gameTimeInSeconds % (3600) / 60));
}
