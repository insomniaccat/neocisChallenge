#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setFixedSize(420,500);
    this->setMouseTracking(true);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, myPixmap);
    int gX = 10;
    int gY = 10;

    for(int i = 0;i<20;++i){
        for(int j = 0;j<20;++j){
            QRect gridRect(gX, gY, gW, gH);
            if(gridColor[i][j] == false){
                QPen framepen(Qt::gray);
                QBrush brush(Qt::gray, Qt::SolidPattern);
                painter.fillRect(gridRect, brush);
                painter.setPen(framepen);
            }else{
                QPen framepen(Qt::blue);
                QBrush brush(Qt::blue, Qt::SolidPattern);
                painter.fillRect(gridRect, brush);
                painter.setPen(framepen);
            }
            painter.drawRect(gridRect);

            gY = gY + gap;
          // qDebug() << gridColor[i][j];
        }
        gX = gX + gap;
        gY = 10;
    }

}

void Widget::resizeEvent(QResizeEvent *event)
{
    auto newRect = myPixmap.rect().united(rect());
    if (newRect == myPixmap.rect()) return;
    QPixmap newPixmap{newRect.size()};
    QPainter painter{&newPixmap};
    painter.fillRect(newPixmap.rect(), Qt::white);
    painter.drawPixmap(0, 0, myPixmap);
    myPixmap = newPixmap;
}
void Widget::mousePressEvent(QMouseEvent *event)
{
    if(is_user_done == false){
        QPainter painter{&myPixmap};
        is_mouse_down = true;
        mousePos = event->pos();
        c_x = mousePos.x();
        c_y = mousePos.y();
    }
    //update();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(is_mouse_down==true && is_user_done == false){
        QPainter painter{&myPixmap};
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen({Qt::blue, 1.0});
        mousePos = event->pos();
        int tempW = qFabs(mousePos.x() - c_x);
        int tempH = qFabs(mousePos.y() -c_y);
        if(tempW > tempH){
                w = tempW;
                h = tempW;
        }else{
            w = tempH;
            h = tempH;
        }
        r = w/2;
        QRect myRect(c_x-(w/2),c_y-(h/2),w, h);
        //myRect.moveCenter(QPoint(width()/2,height()/2));
        painter.fillRect(myPixmap.rect(), Qt::white);
        painter.drawEllipse(myRect);
        update();
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    is_mouse_down = false;
    is_user_done = true;
    QPainter painter{&myPixmap};
    painter.setRenderHint(QPainter::Antialiasing);

    QRect myRect(c_x-(w/2),c_y-(h/2),w, h);
    int helpW = w-0.25;
    int helpH = h-0.25;
    QRect helpRect(c_x-(helpW/2),c_y-(helpH/2),helpW, helpH);

    QPainterPath path;
    path.addEllipse(myRect);
    path.addEllipse(helpRect);


    int gX = 10;
    int gY = 10;
    int rmax = 0;
    int rmin = 200;
    int rtempmin;
    int rtempmax;
    qDebug() << "actual radius: "<<r;
    for(int i = 0;i<20;++i){
        for(int j = 0;j<20;++j){
            QRect gridRect(gX, gY, gW, gH);

            bool result = path.intersects(gridRect);
            gridColor[i][j] = result;
            if(result == false){
                QPen framepen(Qt::gray);
                QBrush brush(Qt::gray, Qt::SolidPattern);
                painter.fillRect(gridRect, brush);
                painter.setPen(framepen);
            }else{
                QPen framepen(Qt::blue);
                QBrush brush(Qt::blue, Qt::SolidPattern);
                painter.fillRect(gridRect, brush);
                painter.setPen(framepen);
                if((gX+(gW/2) > c_x) && (gY+(gW/2) > c_y)){
                    rtempmax =qSqrt(qPow(gX+gW-c_x,2) + qPow(gY+gH-c_y,2));
                    rtempmin =qSqrt(qPow(gX-c_x,2) + qPow(gY-c_y,2));
                }else if((gX+(gW/2) > c_x) && (gY+(gW/2) < c_y)){
                    rtempmax =qSqrt(qPow(gX+gW-c_x,2) + qPow(gY-c_y,2));
                    rtempmin =qSqrt(qPow(gX-c_x,2) + qPow(gY+gH-c_y,2));
                }else if((gX+(gW/2) < c_x) && (gY+(gW/2) < c_y)){
                    rtempmax =qSqrt(qPow(gX-c_x,2) + qPow(gY-c_y,2));
                    rtempmin =qSqrt(qPow(gX-c_x+gW,2) + qPow(gY+gH-c_y,2));
                }else if((gX+(gW/2) < c_x) && (gY+(gW/2) > c_y)){
                    rtempmax =qSqrt(qPow(gX-c_x,2) + qPow(gY+gH-c_y,2));
                    rtempmin =qSqrt(qPow(gX-c_x+gW,2) + qPow(gY-c_y,2));
                }else{
                    rtempmax =qSqrt(qPow(gX-c_x,2) + qPow(gY-c_y,2));
                    rtempmin =qSqrt(qPow(gX-c_x+gW,2) + qPow(gY-c_y,2));
                }
                //qDebug() << "rtempmax values: "<<rtempmax;
                if (rtempmax >rmax){
                    rmax = rtempmax;
                }
                if (rtempmin <rmin){
                    rmin = rtempmin;
                }
            }
            painter.drawRect(gridRect);

            gY = gY + gap;
           //qDebug() << gridColor[i][j];
        }
        gX = gX + gap;
        gY = 10;
    }

    painter.setPen({Qt::blue, 1.0});
    painter.fillRect(myPixmap.rect(), Qt::white);

//    QBrush fillbrush;
   // fillbrush.setColor(Qt::green);
   // fillbrush.setStyle(Qt::SolidPattern);
    //painter.fillPath(path,fillbrush);
    painter.drawEllipse(myRect);

    painter.setPen({Qt::red, 1.2});
    int maxW = 2*rmax;
    int maxH = 2*rmax;
    QRect maxRect(c_x-(maxW/2),c_y-(maxH/2),maxW, maxH);
    painter.drawEllipse(maxRect);

    painter.setPen({Qt::red, 1.2});
    int minW = 2*rmin;
    int minH = 2*rmin;
    QRect minRect(c_x-(minW/2),c_y-(minH/2),minW, minH);
    painter.drawEllipse(minRect);

    update();

}

