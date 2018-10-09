#include "Widget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QResizeEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    main = new QLabel("O");
    aux = new QLabel("y");
    auto layout = new QVBoxLayout;
    layout->addWidget(main);
    layout->addWidget(aux);
    setLayout(layout);
}

void Widget::resizeEvent(QResizeEvent *event)
{
        auto f = main->font();
        QFontMetrics fm(f);

        auto cur = fm.horizontalAdvance(main->text());
        float factor = (float)(width()-1) / cur;

        qDebug() << width() << height()
             << main->width() << main->height()
             << "font" << fm.horizontalAdvance(main->text()) << fm.height() << f.pointSizeF() << factor
             << "eventprop" << event->spontaneous() << event->isAccepted();

        f.setPointSizeF(f.pointSizeF()*factor);
        if(factor < 0.9 || factor > 1.1)
            main->setFont(f);

    QWidget::resizeEvent(event);
}

Widget::~Widget()
{
}
