#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QPushButton>
#include<QLabel>
#include "qrightclickbutton.h"
#include <ctime>
#include <cstdlib>
#include <QMessageBox>
const int numOfMines = 9;
int numOfFlags = 0,numOfMinesLeft = numOfMines;
bool gameOver = false;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(300,330);
    QGridLayout* gridLayout = new QGridLayout();
    QPushButton* resetButton = new QPushButton("Reset", this);
    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetGame()));
    minesLeft = new QLabel(QString("Mines Left: %1").arg(numOfMines));
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    const int BUTTON_SIZE = 30;
    for(int row = 0; row < 9; row++)
    {
        for(int col = 0; col < 9; col++)
        {
            QRightClickButton* pushButton = new QRightClickButton();
            //QPushButton* pushButton = new QPushButton();
            pushButton->setFixedSize(BUTTON_SIZE, BUTTON_SIZE);
            QString buttonName = QString("button_%1_%2").arg(row).arg(col);
            pushButton->setObjectName(buttonName);
            gridLayout->addWidget(pushButton, row, col);
            connect(pushButton, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
            connect(pushButton, SIGNAL(rightClicked()), this, SLOT(onRightClicked()));
        }
    }

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(resetButton);
    mainLayout->addWidget(minesLeft);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    spawnMines();

}
void MainWindow::resetGame()
{
    // Clear all buttons
    for(int row = 0; row < 9; row++)
    {
        for(int col = 0; col < 9; col++)
        {
            QString buttonName = QString("button_%1_%2").arg(row).arg(col);
            QPushButton* pushButton = findChild<QPushButton*>(buttonName);
            pushButton->setText("");
            pushButton->setEnabled(true);
            pushButton->setStyleSheet("");
            pushButton->setProperty("isMine", QVariant());
        }
    }

    // Reset game state
    gameOver = false;
    numOfFlags = 0,numOfMinesLeft = numOfMines;
    minesLeft->setText(QString("Mines Left: %1").arg(numOfMines));
    // Respawn mines
    spawnMines();
}
void MainWindow::openSurroundingCells(int row, int col)
{
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue; // Skip the current cell

            int newRow = row + i;
            int newCol = col + j;

            // Check if the new coordinates are within the grid
            if (newRow >= 0 && newRow < 9 && newCol >= 0 && newCol < 9) {
                QString buttonName = QString("button_%1_%2").arg(newRow).arg(newCol);
                QPushButton* nearbyButton = findChild<QPushButton*>(buttonName);

                if (nearbyButton && nearbyButton->isEnabled()) {
                    // Simulate a click on this button
                    nearbyButton->click();

                }
            }
        }
    }
}


bool MainWindow::isMine(int row, int col) {
    QString buttonName = QString("button_%1_%2").arg(row).arg(col);
    QPushButton* button = findChild<QPushButton*>(buttonName);
    return button && button->property("isMine").toBool();
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::disableAllButtons()
{
    for(int row = 0; row < 9; row++)
    {
        for(int col = 0; col < 9; col++)
        {
            QString buttonName = QString("button_%1_%2").arg(row).arg(col);
            QPushButton* pushButton = findChild<QPushButton*>(buttonName);
            pushButton->setEnabled(false);
        }
    }
}
int MainWindow::nearbyMines(QPushButton* button)
{
    if (!button) return -1;

    QString buttonName = button->objectName();
    QStringList parts = buttonName.split("_");
    if (parts.size() != 3) return -1;

    int row = parts[1].toInt();
    int col = parts[2].toInt();
    int mineCount = 0;

    // Check all 8 surrounding cells
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            if (isMine(row + i, col + j)) {
                mineCount++;
            }
        }
    }

    return mineCount;
}
bool MainWindow::gameWon() {
    for(int row = 0; row < 9; row++) {
        for(int col = 0; col < 9; col++) {
            QString buttonName = QString("button_%1_%2").arg(row).arg(col);
            QPushButton* button = findChild<QPushButton*>(buttonName);

            // If a non-mine button is still enabled (not clicked), game isn't won
            if(!button->property("isMine").toBool() && button->isEnabled()) {
                return false;
            }
        }
    }
    return true; // All non-mine squares have been revealed
}
void MainWindow::spawnMines()
{
    int mineLocX, mineLocY;
    int minesPlaced = 0;
    QString buttonName;

    std::srand(std::time(nullptr));

    while (minesPlaced < numOfMines)
    {
        mineLocX = std::rand() % 9;
        mineLocY = std::rand() % 9;
        buttonName = QString("button_%1_%2").arg(mineLocX).arg(mineLocY);

        QPushButton *button = findChild<QPushButton*>(buttonName);
        if (button && button->property("isMine").isNull())
        {
            button->setProperty("isMine", true);
            //button->setText("M");
            minesPlaced++;
        }
    }
}
void MainWindow::onRightClicked()
{
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
    QFont font = clickedButton->font();
    font.setFamily("Segoe UI Symbol");
    clickedButton->setFont(font);
    if(!clickedButton) return;
    if(clickedButton->text().isEmpty())
    {
        clickedButton->setText("🚩");
        numOfFlags++;
    }
        else if(clickedButton->text() == QString::fromUtf8("🚩"))
    {
        clickedButton->setText("");
        numOfFlags--;
    }
    numOfMinesLeft = numOfMines - numOfFlags;
    if(numOfMinesLeft >= 0)
        minesLeft->setText(QString("Mines Left: %1").arg(numOfMinesLeft));
    else
        minesLeft->setText(QString("Mines Left: 0"));
}
void MainWindow::onButtonClicked()
{
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());

    if(!clickedButton) return;
    else if(clickedButton->property("isMine").toBool())
    {
        gameOver = true;
        clickedButton->setStyleSheet("background-color: red;");
        disableAllButtons();

    }
    else
    {
        int numOfMines = nearbyMines(clickedButton);
        clickedButton->setText(QString::number(numOfMines));
        clickedButton->setEnabled(false);
        if(gameWon()) {
            gameOver = true;
            disableAllButtons();
            QMessageBox::information(this, "Game Won!", "Congratulations! You won!");
        }
        if (numOfMines == 0)
        {
            QStringList parts = clickedButton->objectName().split("_");
            int row = parts[1].toInt();
            int col = parts[2].toInt();

            // Open surrounding cells
            openSurroundingCells(row, col);
        }
    }

}


