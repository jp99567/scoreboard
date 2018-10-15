#include "displayconf.h"

#include <QFormLayout>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QTimer>
#include <QMetaEnum>
#include <QHostAddress>

#include "protocol.h"

DisplayConf::DisplayConf(QWidget *parent) : QWidget(parent)
{
    auto fl = new QFormLayout;
    connStatus = new QLabel;
    fl->addRow(connStatus);
    enabled = new QCheckBox;
    fl->addRow("Enable", enabled);
    host = new QLineEdit;
    host->setText("localhost");
    fl->addRow("Server", host);
    role = new QComboBox;
    role->addItem("No Role");
    role->addItem("Display A1");
    role->addItem("Display A2");
    role->addItem("Display B1");
    role->addItem("Display B2");
    fl->addRow("Role", role);
    setLayout(fl);

    connect(enabled, &QCheckBox::clicked, this, [this](bool checked){
        if(checked)
        {
            if(socket.state() == QAbstractSocket::SocketState::UnconnectedState)
            {
                socket.connectToHost(host->text(), scoreboard::port);
            }
        }
        else
        {
            if(   socket.state() == QAbstractSocket::SocketState::ConnectedState
               || socket.state() == QAbstractSocket::SocketState::ConnectingState
               || socket.state() == QAbstractSocket::SocketState::HostLookupState )
            {
                socket.disconnectFromHost();
            }
        }
    });

    connect(&socket, &QTcpSocket::stateChanged, [this](QAbstractSocket::SocketState state){
        auto txt = QMetaEnum::fromType<QAbstractSocket::SocketState>();

        switch(state)
        {
          case QAbstractSocket::SocketState::UnconnectedState:
          {
            if(enabled->isChecked())
            {
                scheduleReconnect();
            }
            connStatus->setText(txt.valueToKey(state));
          }
            break;
          case QAbstractSocket::SocketState::ConnectedState:
          {
            connStatus->setText( QString("Connected to %1(%2)")
                                 .arg(socket.peerName())
                                 .arg(socket.peerAddress().toString()));
          }
            break;
          default:
            connStatus->setText(txt.valueToKey(state));
            break;
        }
    });

    connect(&socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
          [this](QAbstractSocket::SocketError socketError){
        qDebug() << "connection error" << socketError;
        connStatus->setText(socket.errorString());
        scheduleReconnect();
    });
}

void DisplayConf::scheduleReconnect()
{
    qDebug() << "schedule reconnect";
    QTimer::singleShot(5e3, [this]{
        if(enabled->isChecked())
            qDebug() << "reconnect";
            socket.connectToHost(host->text(), scoreboard::port);
    });
}
