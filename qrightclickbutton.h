// qrightclickbutton.h
#ifndef QRIGHTCLICKBUTTON_H
#define QRIGHTCLICKBUTTON_H

#include <QPushButton>
#include <QMouseEvent>

class QRightClickButton : public QPushButton
{
    Q_OBJECT

public:
    explicit QRightClickButton(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *e) override;

signals:
    void rightClicked();
};

#endif // QRIGHTCLICKBUTTON_H
