#include "Widget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QResizeEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    resize(720/5,1280/5);

    main = new QLabel("X");
    main->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    auto palette = main->palette();
    palette.setColor(QPalette::Background, Qt::blue);
    main->setAutoFillBackground(true);
    main->setPalette(palette);

    aux = new QLabel("OO");
    aux->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    palette = aux->palette();
    palette.setColor(QPalette::Background, Qt::yellow);
    aux->setAutoFillBackground(true);
    aux->setPalette(palette);

    auto layout = new QVBoxLayout;
    layout->addWidget(main);
    layout->addWidget(aux);
    setLayout(layout);
}

void Widget::resizeEvent(QResizeEvent *event)
{
    qDebug() << width() << height();

    auto conRect = this->layout()->contentsRect();
    qDebug() << conRect;

    auto f = main->font();
    QFontMetrics fm(f);

    auto curh = fm.horizontalAdvance(main->text());
    float factor = (float)(conRect.width()-5) / curh;

    qDebug() << main->width() << main->height()
         << "font" << fm.horizontalAdvance(main->text()) << fm.height() << f.pointSizeF() << factor;

    f.setPointSizeF(f.pointSizeF()*factor);
    if(factor > 1.1){
        main->setFont(f);
        fm = QFontMetrics(f);
        qDebug() << main->size() << fm.height();
        main->resize(conRect.width(), fm.height()+2);
    }

    QWidget::resizeEvent(event);
}

Widget::~Widget()
{
}
