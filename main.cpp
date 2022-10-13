#include <QApplication>
#include "./src/SysInfoSever.h"
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    SysInfoSever info;
    qDebug() << QString("The computer name: ").append(info.localmachineName());
    qDebug() << QString("operating system:  ").append(info.osVersion());
    qDebug() << QString("CPU type:          ").append(info.cpuType());
    qDebug() << QString("memory:            ").append(info.memory());
    qDebug() << QString("hard disk:         ").append(info.disk());
    qDebug() << QString("Graphics:          ").append(info.dCard());
    qDebug() << QString("Network status:    ").append(info.ipLive());
    qDebug() << QString("Local IP address:  ").append(info.ip());
    qDebug() << QString("Public IP address: ").append(info.publicIp());
    qDebug() << QString("MAC address:       ").append(info.mac());
    qDebug() << QString("Display parameters:").append(info.screen());
    return a.exec();
}