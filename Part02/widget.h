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
#include <Eigen>
#include <cmath>
#include <iostream>

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
    //virtual void mouseMoveEvent(QMouseEvent *event)Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *event)Q_DECL_OVERRIDE;
    virtual bool solveLeastSquaresCircleKasa(const  std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d> > &points, Eigen::Vector2d &midpoint, double &radius);
    bool is_mouse_down = false;
    bool is_user_done = false;
    bool gridColor[20][20] = { {false} };
    QPoint mousePos;
    QPixmap myPixmap;
    int c_x;
    int c_y;
    //int w;
    //int h;
    int gW = 10;
    int gH = 10;
    //int r;
    int gap = 20;
    int N = 0;
private slots:
    void on_pushButton_clicked();
};



#endif // WIDGET_H
