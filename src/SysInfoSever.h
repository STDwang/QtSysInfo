#ifndef SYSINFOSEVER_H
#define SYSINFOSEVER_H

#include <Windows.h>
#pragma comment(lib,"Urlmon.lib")

#include<qmap.h>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSysInfo>
#include <QSettings>
#include <QDebug>
#include <QDesktopWidget>
#include <QFileInfoList>
#include <QDir>
#include <QLibrary>
#include <QTimer>
#include <QHostInfo>
#include <QApplication>

class SysInfoSever
{
public:
    SysInfoSever();
    //获取计算机名称
    QString localmachineName();
    //本地IP地址
    QString ip();
    //公网IP
    QString publicIp();
    //获取计算机MAC地址
    QString mac();
    //获取计算机CPU信息
    QString cpuType();
    //获取计算机显卡信息
    QString dCard();
    //获取计算机内存
    QString memory();
    //获取计算机操作系统
    QString osVersion();
    //获取计算机屏幕个数以及像素
    QString screen();
    //获取计算机硬盘信息
    QString disk();
    //判断计算机是否可以联网
    QString ipLive();
};

#endif // SYSINFOSEVER_H
