#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    tcpServer = new QTcpServer(this);
    this->setWindowTitle("服务端");
    // 新的连接
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(mNewConnetion()));
    ui->pushButton_2->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mNewConnetion()
{
    // 与客户端连接
    QTcpSocket *tmpTcpSocket = tcpServer->nextPendingConnection();

    // 客户端的ip地址
    QString ipaddr = tmpTcpSocket->peerAddress().toString();

    quint16 port = tmpTcpSocket->peerPort();

    // 打印客户端连接的ip信息
    ui->textBrowser->append("服务端：客户端的ip地址：" + ipaddr);
    // 打印客户端连接的端口信息
    ui->textBrowser->append("服务端：客户端的端口：" + QString::number(port));

    connect(tmpTcpSocket, SIGNAL(readyRead()), this, SLOT(receiveMessages()));
    connect(tmpTcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(mStateChanged(QAbstractSocket::SocketState)));

}

void Widget::receiveMessages()
{
    QTcpSocket *tmpTcpSocket = (QTcpSocket *)sender();
    // 接收消息
    ui->textBrowser->append("客户端：" + tmpTcpSocket->readAll());
}

void Widget::mStateChanged(QAbstractSocket::SocketState state)
{
    QTcpSocket *tmpTcpSocket = (QTcpSocket *)sender();
    switch (state) {
    // 已断开
    case QAbstractSocket::UnconnectedState:
        ui->textBrowser->append("服务端：客户端断开连接");
        tmpTcpSocket->deleteLater();
        break;
        // 已连接
    case QAbstractSocket::ConnectedState:
        ui->textBrowser->append("服务端：客户端已连接");
        break;
    default:
        break;
    }
}

void Widget::on_pushButton_3_clicked()
{
    QList <QTcpSocket *> socketList = tcpServer->findChildren<QTcpSocket *>();
    qDebug() << "tcpSocket 数量：" << socketList.count() << endl;

    if (socketList.count() == 0) {
        ui->textBrowser->append("当前没有客户端连接，请先与客户端连接！");
        return;
    }
    foreach (QTcpSocket *tmpTcpSocket, socketList) {
        // 服务端向每个客户端发送消息
        tmpTcpSocket->write(ui->lineEdit->text().toUtf8());
    }
    ui->textBrowser->append("服务端：" + ui->lineEdit->text());
}

void Widget::on_pushButton_clicked()
{
    // 绑定ip地址和端口
    tcpServer->listen(QHostAddress("192.168.1.59"), 9999);
    ui->textBrowser->append("服务端:监听的ip地址和端口：192.168.1.59，9999");
    ui->pushButton_2->setEnabled(true);
    ui->pushButton->setEnabled(false);
}

void Widget::on_pushButton_2_clicked()
{
    // 停止监听
    tcpServer->close();
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
}
