#include "Widget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    main = new QLabel("X");
    aux = new QLabel("y");
    auto layout = new QVBoxLayout;
    layout->addWidget(main);
    layout->addWidget(aux);
    setLayout(layout);
}

void Widget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "resize event:" << width() << height();
    QWidget::resizeEvent(event);
}

Widget::~Widget()
{
}
