#ifndef MYSERVER_H
#define MYSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>


QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class MyServer : public QObject
{
    Q_OBJECT
public:
    explicit MyServer(quint16 port, bool debug, QObject *parent = nullptr);
    ~MyServer() override; //Destroys the websocket server (not sure why we want this)

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void socketDisconnected();

private:
    QWebSocketServer *server;
    QList<QWebSocket *> clients;
    bool m_debug;

};

#endif // MYSERVER_H
