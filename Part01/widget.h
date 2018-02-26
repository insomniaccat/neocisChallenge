#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QtGui>
#include <QtCore>
#include <QGraphicsScene>
#include <QApplication>
#include <QTimer>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QRubberBand>
#include <QtMath>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

protected:
    virtual void paintEvent(QPaintEvent *event)Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event)Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent *event)Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *event)Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *event)Q_DECL_OVERRIDE;

    bool is_mouse_down = false; //flag
    bool is_user_done = false;// flag
    bool gridColor[20][20] = { {false} }; //0 for gray, 1 for blue
    QPoint mousePos; //(x, y) for mouse position
    QPixmap myPixmap;
    int c_x, c_y, r; //coordinates for circle's center and radius
    int w, h; //circle QRect object's
    int gW = 10;
    int gH = 10; //grid rect width and height
    int gap = 20; //gap between grid rects
};



#endif // WIDGET_H
