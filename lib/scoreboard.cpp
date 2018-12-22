#include "scoreboard.h"

#include <QDebug>
#include <QMetaEnum>

Scoreboard::Scoreboard()
{
    connect(&listener, &QTcpServer::newConnection, this, [this]{
        do{
            auto tcpcon = listener.nextPendingConnection();
            if(!tcpcon)
                break;
            clients.emplace_back(std::make_unique<Client>(*tcpcon));
            connect(clients.back().get(), &Client::disconnected, this, &Scoreboard::removeClient);
            connect(clients.back().get(), &Client::newLine, this, &Scoreboard::processLine);
        }
        while(listener.hasPendingConnections());
    });

    if( ! listener.listen(QHostAddress::Any, scoreboard::port))
    {
        qDebug() << listener.errorString();
    }

    qDebug() << "listening";
}

void Scoreboard::removeClient(Client *client)
{
    auto remove = std::find_if(std::begin(clients),
                 std::end(clients), [client](decltype(clients.back()) p){
       return p.get() == client;
    });

    clients.erase(remove);
}

void Scoreboard::processLine(Client *client, QString line)
{
    qDebug() << client << line;

    QTextStream ts(&line);
    QString argTxt;
    ts >> argTxt;

    using scoreboard::Protocol;
    switch((Protocol::ServerRequest)QMetaEnum::fromType<Protocol::ServerRequest>().keyToValue(argTxt.toStdString().c_str()))
    {
    case Protocol::ServerRequest::InfoDisplayRole:
      {
        ts >> argTxt;
        auto role = (Protocol::DisplayRole)QMetaEnum::fromType<Protocol::DisplayRole>().keyToValue(argTxt.toStdString().c_str());
        updateRole(client, role);
      }
        break;
    case Protocol::ServerRequest::AcceptEvent:
      {
        ts >> argTxt;
        auto e = (Protocol::Event)QMetaEnum::fromType<Protocol::Event>().keyToValue(argTxt.toStdString().c_str());
        processEvent(e);
      }
        break;

    default:
        qDebug() << "unknown request" << line;
        break;
    }
}

void Scoreboard::updateRole(Client *client, scoreboard::Protocol::DisplayRole role)
{
    qDebug() << client << role;
}

void Scoreboard::processEvent(scoreboard::Protocol::Event)
{

}

Client::Client(QTcpSocket& socket)
    :socket(socket)
{
    connect(&socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(&socket, &QTcpSocket::disconnected, this, [this]{
        in.clear();
        emit disconnected(this);
    });
    connect(&in, &LineParser::newLine, [this](QString line){
       emit newLine(this, std::move(line));
    });
    qDebug() << "new socket from:" << socket.peerAddress();
}

Client::~Client()
{
    qDebug() << "client destuct";
}

void Client::onReadyRead()
{
    in.process(socket.readAll());
}
