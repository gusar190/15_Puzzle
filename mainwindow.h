#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define ROWS_NUMBER 4
#define COLS_NUMBER 4
#define TILES_NUMBER 15
#define TILE_WIDTH 64
#define TILE_HEIGHT 64
#define GAME_FIELD_LEFT_UPPER_X 0
#define GAME_FIELD_LEFT_UPPER_Y 48

#include <QMainWindow>
#include <QPushButton>
#include <vector>
#include <QTimer>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum fieldState { fieldStateArranged, fieldStateNotArranged };
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void incrementGameTime();

private slots:
    void newGame();
    void tilePressed();

private:
    Ui::MainWindow *ui;
    QPushButton* tiles[ROWS_NUMBER][COLS_NUMBER];
    QPushButton *newGameButton;

    QTimer *secondsTimer;
    uint32_t gameTimeInSeconds;  //in seconds
    QLabel *gameTimeLabel;

    void makeField(void);
    //void destroyField(void);
    void drawField(void);
    void shuffleField(void);
    fieldState checkFieldState(void);
    void updateTileMovesLabel(void);
    void incrementSecondsTimer();
    void updateGameTimeLabel(void);

    QLabel *gameStateLabel;
    QPixmap gameState;
    uint32_t tileMoves;
    QLabel *tileMovesLabel;


};
#endif // MAINWINDOW_H
