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
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(is_user_done == false){
    QPainter painter{&myPixmap};
    painter.setRenderHint(QPainter::Antialiasing);

    int gX = 10;
    int gY = 10;

    for(int i = 0;i<20;++i){
        for(int j = 0;j<20;++j){
            QRect gridRect(gX, gY, gW, gH);

            QPen framepen;
            QBrush brush;
            (Qt::gray, Qt::SolidPattern);


            bool result = gridRect.contains(c_x, c_y);

            if(result == true){
                gridColor[i][j] = !gridColor[i][j];
            }
            bool myColor = gridColor[i][j];
            if(myColor == false){
                framepen.setColor("gray");
                brush.setColor("gray");
            }else{
                framepen.setColor("blue");
                brush.setColor("blue");
            }
            brush.setStyle(Qt::SolidPattern);
            painter.fillRect(gridRect, brush);
            painter.setPen(framepen);
            painter.drawRect(gridRect);

            gY = gY + gap;
           //qDebug() << gridColor[i][j];
        }
        gX = gX + gap;
        gY = 10;
    }
    painter.fillRect(myPixmap.rect(), Qt::white);
    }


    update();

}

bool Widget::solveLeastSquaresCircleKasa(const  std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d> > &points, Eigen::Vector2d &midpoint, double &radius)
{
    /* Code from https://github.com/DLuensch/Least-Squares-Circle-Fitting-Kasa-Method-.git was used.
     * ---------------------------------------------------------------------------
    * Revision			: 1.0
    * Modul				: Least Square Circle Kasa C++
    * Creation			: 10.01.2015
    * Recent changes	: 10.01.2015
    * ----------------------------------------------------------------------------
    * LOG:
    * ----------------------------------------------------------------------------
    * Author			: Dennis Luensch
    * Contact			: dennis.luensch@gmail.com
    * ----------------------------------------------------------------------------
    * Tabsize			: 4
    * Charset			: Windows
    * ------------------------------------------------------------------------- */
    int length = points.size();
    double x1;
    double x2;
    double x3;
    Eigen::MatrixXd AFill(3, length);
    Eigen::MatrixXd A(length, 3);
    Eigen::VectorXd AFirst(length);
    Eigen::VectorXd ASec(length);
    Eigen::VectorXd AFirstSquared(length);
    Eigen::VectorXd ASecSquared(length);
    Eigen::VectorXd ASquaredRes(length);
    Eigen::VectorXd b(length);
    Eigen::VectorXd c(3);
    bool ok = true;

    if (length > 1)
    {
        for (int i = 0; i < length; i++)
        {
            AFill(0, i) = points[i](0);
            AFill(1, i) = points[i](1);
            AFill(2, i) = 1;
        }

        A = AFill.transpose();

        for (int i = 0; i < length; i++)
        {
            AFirst(i) = A(i, 0);
            ASec(i) = A(i, 1);
        }

        for (int i = 0; i < length; i++)
        {
            AFirstSquared(i) = AFirst(i) * AFirst(i);
            ASecSquared(i) = ASec(i) * ASec(i);
        }

        b = AFirstSquared + ASecSquared;

        c = A.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);

        x1 = c(0);
        midpoint(0) = x1 * 0.5;
        x2 = c(1);
        midpoint(1) = x2 * 0.5;
        x3 = c(2);
        radius = sqrt((x1 * x1 + x2 * x2) / 4 + x3);
    }
    else
    {
        ok = false;
    }

    return ok;
}


void Widget::on_pushButton_clicked()
{
    //if (is_user_done = false){
    QPainter painter{&myPixmap};
    painter.setRenderHint(QPainter::Antialiasing);

    Eigen::MatrixXf XY;
    XY.resize(1,2);

    std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d> > samplePoints;
    Eigen::Vector2d midpoint;
    //midpoint = XY.rowwise().mean();
    double radius;
    bool ok;

    is_user_done = true;
    int gX = 10;
    int gY = 10;
    for(int i = 0;i<20;++i){
        for(int j = 0;j<20;++j){
            if(gridColor[i][j] == true){
                samplePoints.push_back(Eigen::Vector2d((double)(gX + (gW/2)), (double) (gY + (gH/2))));
            }
            gY = gY + gap;
        }
        gX = gX + gap;
        gY = 10;
    }
    ok = solveLeastSquaresCircleKasa(samplePoints, midpoint, radius);
    if (ok)
    {
        //cout << "x: " << midpoint(0) << "  |  y: " << midpoint(1) << "  |  radius: " << radius << std::endl;
        QRect myRect(midpoint(0)-radius,midpoint(1)-radius,radius*2, radius*2);
        painter.setPen({Qt::red, 1.2});
        painter.fillRect(myPixmap.rect(), Qt::white);
        painter.drawEllipse(myRect);
        update();
    }else{
        qDebug() << "Error - Unable to create circle of best fit";
    }

   // }
}
