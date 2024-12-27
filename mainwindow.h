#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QLabel>
#include <QMainWindow>
#include<QPushButton>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onButtonClicked();
    void resetGame();
    void onRightClicked();

private:
    Ui::MainWindow *ui;
    void spawnMines();
    void gameLogic();
    void disableAllButtons();
    int nearbyMines(QPushButton* button);
    bool isMine(int row,int col);
    void openSurroundingCells(int row, int col);
    bool gameWon();
    QLabel* minesLeft;
};
#endif // MAINWINDOW_H
