#include "scoreboard.h"

#include <QDebug>

#include "protocol.h"

Scoreboard::Scoreboard()
{
    connect(&listener, &QTcpServer::newConnection, this, [this]{
        do{
            auto tcpcon = listener.nextPendingConnection();
            if(!tcpcon)
                break;
            clients.emplace_back(std::make_unique<Client>(*tcpcon));
            connect(clients.back().get(), &Client::disconnected, this, &Scoreboard::removeClient);
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

Client::Client(QTcpSocket& socket)
    :socket(socket)
{
    connect(&socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(&socket, &QTcpSocket::disconnected, this, [this]{
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

