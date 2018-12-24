#include "displayconf.h"

#include <QFormLayout>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QRadioButton>
#include <QFrame>
#include <QTimer>
#include <QMetaEnum>
#include <QHostAddress>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
//#include <QTextToSpeech>

DisplayConf::DisplayConf(QWidget *parent)
    : QWidget(parent)
{
    auto vl = new QVBoxLayout;
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
    connect(toSpeech, &QCheckBox::clicked, this, &DisplayConf::enableSpeech);
    soundPlayer = new QCheckBox;
    fl->addRow("Play Sounds", soundPlayer);
    connect(soundPlayer, &QCheckBox::clicked, [this](bool checked){
        features.setFlag(scoreboard::Protocol::DisplayFeature::PlaySound, checked);
        sendFeatures();
    });

    vl->addLayout(fl);

    auto newHLine = []{
        auto hLine = new QFrame;
        hLine->setFrameShadow(QFrame::Shadow::Sunken);
        hLine->setFrameShape(QFrame::Shape::HLine);
        return hLine;
    };
    vl->addWidget(newHLine());

    plusA = new QPushButton("Plus A");
    plusB = new QPushButton("Plus B");

    vl->addWidget(plusA);
    vl->addWidget(plusB);
    vl->addWidget(newHLine());

    {
        auto scoreCorrectionFrame = new QGroupBox;
        scoreCorrectionFrame->setTitle("Score Corrections");
        auto gl = new QGridLayout;
        scoreL = new QSpinBox;
        scoreR = new QSpinBox;
        setL = new QSpinBox;
        setR = new QSpinBox;
        gl->addWidget(new QLabel("Score"), 0, 0);
        gl->addWidget(scoreL, 0, 1);
        gl->addWidget(scoreR, 0, 2);
        gl->addWidget(new QLabel("Set"), 1, 0);
        gl->addWidget(setL, 1, 1);
        gl->addWidget(setR, 1, 2);
        servisNon = new QRadioButton("ServisNon");
        servisL = new QRadioButton("ServisL");
        servisR = new QRadioButton("ServisR");
        gl->addWidget(servisNon, 2, 0);
        gl->addWidget(servisL, 2, 1);
        gl->addWidget(servisR, 2, 2);
        scoreCorrectionFrame->setLayout(gl);
        vl->addWidget(scoreCorrectionFrame);
        apply = new QPushButton("Apply Corrections");
        vl->addWidget(apply);
    }

    setLayout(vl);

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

    connect(&socket, &QTcpSocket::readyRead, [this]{in.process(socket.readAll())});

    reconnect.setSingleShot(true);
    connect(&reconnect, &QTimer::timeout, [this]{
        if(enabled->isChecked())
        {
            qDebug() << "reconnect";
            socket.connectToHost(host->text(), scoreboard::port);
        }
    });
}

DisplayConf::~DisplayConf()
{

}


void DisplayConf::scheduleReconnect()
{
    qDebug() << "schedule reconnect";
    reconnect.start(std::chrono::milliseconds(5000));
}

void DisplayConf::enableSpeech(bool on)
{
    if(on)
    {
    //    speech = std::make_unique<QTextToSpeech>(this);
    //    speech->setVolume(1);
    //    speech->say("check 1 2 match ball");
    }
    else
    {
    //    speech = nullptr;
    }
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
