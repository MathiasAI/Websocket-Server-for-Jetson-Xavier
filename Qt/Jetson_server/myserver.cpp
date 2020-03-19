#include "myserver.h"
#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <QtCore/QDebug>
#include <QtCore/QFile>

// Constructor
MyServer::MyServer(quint16 port, bool debug, QObject *parent) :
    QObject(parent),
    server(nullptr),
    m_debug(debug)
{
    server = new QWebSocketServer(QStringLiteral("Jetson Server"),
                                  QWebSocketServer::NonSecureMode,
                                  this);

    //Starting the server
    if(!server->listen(QHostAddress::Any, port))
    {
        qDebug() << "Server could not start!";
    }
    else
    {
        qDebug() << "Server started- \r\nListening on port:" << port;
        // If listening is successful we connect the newConnection() signal to the onNewConnecton slot
        connect(server, &QWebSocketServer::newConnection, this, &MyServer::onNewConnection);
        connect(server, &QWebSocketServer::closed, this, &MyServer::closed);
    }
}

MyServer::~MyServer()
{
    // Closing connection and deleting the connected clients
    server->close();
    qDeleteAll(clients.begin(), clients.end());
}

// New Connection
void MyServer::onNewConnection()
{
    QWebSocket *socket = server->nextPendingConnection();

    // Connecting the textMessageRecieved signal to our slot
    connect(socket, &QWebSocket::textMessageReceived, this, &MyServer::processTextMessage);
    connect(socket, &QWebSocket::disconnected, this, &MyServer::socketDisconnected);
    qDebug() << "Client connected:";
    //socket->close();
}

// Process TextMessage
void MyServer::processTextMessage(QString message)
{
    //QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Command recieved from client" << message;
    if (message == "Object Detection")
    {
        system("cd /opt/nvidia/deepstream/deepstream-4.0/sources/objectDetector_Yolo && deepstream-app -c RTSP_stream_infer.txt &");
    }
    else if (message == "Object Tracking")
    {
        system("cd /opt/nvidia/deepstream/deepstream-4.0/sources/objectDetector_Yolo && deepstream-app -c RTSP_stream_infer_tracker.txt &");
    }
    else if (message == "Run pipeline")
    {
    system("cd /opt/nvidia/deepstream/deepstream-4.0/sources/objectDetector_Yolo && deepstream-app -c RTSP_stream.txt &");
    }
    else if (message == "Stop")
    {
        system("killall deepstream-app");
    }
    else
    {
        qDebug() << "Non valid command recieved: " << message;
    }
}


// Socket Disconnected
void MyServer::socketDisconnected()
{
   // QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Client Disconnected";
}

