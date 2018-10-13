#include "scoreboard.h"

#include <QDebug>
#include <QTextStream>

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

    if( ! listener.listen(QHostAddress::Any, 9233))
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
    qDebug() << "new socket from:" << socket.peerAddress();
}

Client::~Client()
{
    qDebug() << "client destuct";
}

void Client::onReadyRead()
{
    in.append(socket.readAll());
    qDebug() << "data:" << in;
    QTextStream ts(&in);
    while(1)
    {
        auto line =  ts.readLine();

        if(line.isNull())
        {
            in = ts.readAll();
            break;
        }
        else
        {
            if(ts.atEnd())
            {
                in = line;
                break;
            }
            process(std::move(line));
        }
    }

    qDebug() << "check" << in;
}

void Client::process(QString line)
{
    qDebug() << "process line:" << line;
}
