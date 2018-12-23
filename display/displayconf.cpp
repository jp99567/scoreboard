#include "displayconf.h"

#include <QFormLayout>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QTimer>
#include <QMetaEnum>
#include <QHostAddress>

DisplayConf::DisplayConf(QWidget *parent)
    : QWidget(parent)
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
    {
        using scoreboard::Protocol;
        auto roleTxt = QMetaEnum::fromType<Protocol::DisplayRole>();
        role->addItem(roleTxt.valueToKey((int)Protocol::DisplayRole::non));
        role->addItem(roleTxt.valueToKey((int)Protocol::DisplayRole::L1));
        role->addItem(roleTxt.valueToKey((int)Protocol::DisplayRole::L2));
        role->addItem(roleTxt.valueToKey((int)Protocol::DisplayRole::R1));
        role->addItem(roleTxt.valueToKey((int)Protocol::DisplayRole::R2));
    }
    fl->addRow("Role", role);

    connect(role, &QComboBox::currentTextChanged, [this]{
        sendRole();
    });

    toSpeech = new QCheckBox;
    fl->addRow("Say Score", toSpeech);
    soundPlayer = new QCheckBox;
    fl->addRow("Play Sounds", soundPlayer);
    connect(soundPlayer, &QCheckBox::clicked, [this](bool checked){
        features.setFlag(scoreboard::Protocol::DisplayFeature::PlaySound, checked);
        sendFeatures();
    });

    setLayout(fl);

    connect(enabled, &QCheckBox::clicked, this, [this](bool checked){
        qDebug() << "conn enabled" << checked;
        if(checked)
        {
            if(socket.state() == QAbstractSocket::SocketState::UnconnectedState)
            {
                socket.connectToHost(host->text(), scoreboard::port);
            }
        }
        else
        {
            reconnect.stop();
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

        qDebug() << txt.valueToKey(state);

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
            sendRole();
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
    });

    reconnect.setSingleShot(true);

    connect(&reconnect, &QTimer::timeout, [this]{
        if(enabled->isChecked())
        {
            qDebug() << "reconnect";
            socket.connectToHost(host->text(), scoreboard::port);
        }
    });
}

void DisplayConf::scheduleReconnect()
{
    qDebug() << "schedule reconnect";
    reconnect.start(std::chrono::milliseconds(5000));
}

void DisplayConf::sendLine(QString line)
{
    auto data = line + '\n';
    auto len = socket.write(data.toUtf8());
    qDebug() << "written" << len << line;
}

void DisplayConf::sendRole()
{
    auto reqTxt = QMetaEnum::fromType<scoreboard::Protocol::ServerRequest>();
    sendLine(reqTxt.valueToKey((int)scoreboard::Protocol::ServerRequest::InfoDisplayRole)
             +QString(" ") + role->currentText());
}

void DisplayConf::sendFeatures()
{
    auto featuresTxt = QMetaEnum::fromType<scoreboard::Protocol::DisplayFeature>();
    sendLine(featuresTxt.valueToKeys(features));
}
