#include "SysInfoSever.h"

SysInfoSever::SysInfoSever()
{

}

//获取计算机名称
QString SysInfoSever::localmachineName()
{
    return QHostInfo::localHostName();
}

//本地IP地址
QString SysInfoSever::ip()
{
    QString ip = "";
    QList<QNetworkInterface> interFaceList = QNetworkInterface::allInterfaces();
    for (int i = 0; i < interFaceList.size(); i++)
    {
        QNetworkInterface myInterface = interFaceList.at(i);
        if (myInterface.flags().testFlag(QNetworkInterface::IsRunning))
        {
            QList<QNetworkAddressEntry> entryList = myInterface.addressEntries();
            foreach(QNetworkAddressEntry entry, entryList)
            {
                if (QAbstractSocket::IPv4Protocol == entry.ip().protocol() &&
                    entry.ip() != QHostAddress::LocalHost && entry.ip().toString().startsWith("192.168."))
                {
                    ip = entry.ip().toString();
                    break;
                }
            }
        }
    }
    return ip;
}

//公网IP
QString SysInfoSever::publicIp()
{
    HRESULT ret = URLDownloadToFileA(NULL, "http://whois.pconline.com.cn/", ".\\tmp.txt", 0, NULL); //下载网页到tmp.txt文件中
    if (ret != S_OK) { //如果下载失败返回NULL
        return "";
    }
    //下载成功,读取文本内容
    FILE* file;
    errno_t err = fopen_s(&file, ".\\tmp.txt", "r");
    if (err != 0) {
        return "";
    }
    fseek(file, 0, SEEK_END);
    int nSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    std::string buf;
    buf.resize(nSize + 1);
    fread((char*)buf.c_str(), sizeof(char), nSize, file);
    fclose(file);

    QString webCode = QString::fromStdString(buf);
    QString strIp;
    if (!webCode.isEmpty())
    {
        QString web = webCode.replace(" ", "");
        web = web.replace("\r", "");
        web = web.replace("\n", "");
        QStringList list = web.split("<br/>");
        QString tar = list[3];
        QStringList ip = tar.split("=");
        strIp = ip[1];
    }
    else
        strIp = "无法获取公网ip";

    return strIp;
}

//获取计算机MAC地址
QString SysInfoSever::mac()
{
    QString strMac;
    QList<QNetworkInterface> netList = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface item, netList)
    {
        if ((QNetworkInterface::IsUp & item.flags()) && (QNetworkInterface::IsRunning & item.flags()))
        {
            if (strMac.isEmpty() || strMac < item.hardwareAddress())
            {
                strMac = item.hardwareAddress();
            }
        }
    }
    return strMac;
}

//获取计算机CPU信息
QString SysInfoSever::cpuType()
{
    QSettings* CPU = new QSettings("HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", QSettings::NativeFormat);
    QString cpuTypeStr = CPU->value("ProcessorNameString").toString();
    delete CPU;
    return cpuTypeStr;
}

//获取计算机显卡信息
QString SysInfoSever::dCard()
{
    QString dcard;
    QString type;
    QString dType;
#ifdef Q_OS_WIN
    QSettings* DCard = new QSettings("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\nvlddmkm\\Device0", QSettings::NativeFormat);
    type = DCard->value("Device Description").toString();
    delete DCard;

    dType = type;
    dType.trimmed();
    if (!dType.isEmpty())
        dcard = dType;

    DCard = new QSettings("HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Services\\igfx\\Device0", QSettings::NativeFormat);
    type = DCard->value("Device Description").toString();
    delete DCard;

    dType = type;
    dType.trimmed();
    if (!dType.isEmpty())
        dcard = dcard + "\n" + dType;

    DCard = new QSettings("HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Services\\amdkmdap\\Device0", QSettings::NativeFormat);
    type = DCard->value("Device Description").toString();
    delete DCard;
#endif // Q_OS_WIN

    dType = type;
    dType.trimmed();
    if(!dType.isEmpty())
        dcard = dcard + "\n" + dType;

    dcard.trimmed();
    if(dcard.isEmpty()) dcard = "Graphics card information cannot be searched";
    return dcard;
}

//获取计算机内存
QString SysInfoSever::memory()
{
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    double m_totalMem = statex.ullTotalPhys * 1.0 / 1073741824;
    double m_freeMem = statex.ullAvailPhys * 1.0 / 1073741824;

    QString m_memDescribe = QString("free %1 GB / total %2 GB").
        arg(QString::asprintf("%.2f", m_freeMem)).arg(QString::asprintf("%.2f", m_totalMem));

    return m_memDescribe;
}

//获取计算机操作系统
QString SysInfoSever::osVersion()
{
    typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
    LPFN_ISWOW64PROCESS fnIsWow64Process;
    BOOL bIsWow64 = FALSE;
    fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
    if (NULL != fnIsWow64Process)
    {
        fnIsWow64Process(GetCurrentProcess(), &bIsWow64);
    }
    QString sysBit = "unknown";
    if (bIsWow64) sysBit = "64bit";
    else sysBit = "32bit";

    QString m_osDescirbe = QSysInfo::prettyProductName() + " " + sysBit;
    return m_osDescirbe;
}

//获取计算机屏幕个数以及像素
QString SysInfoSever::screen()
{
    QString m_screenDescribe = "";
    QList<QSize> screenSizeList;
    QList <int> screenCountList;

    for (int i = 0; i < QApplication::desktop()->screenCount(); i++)
    {
        QRect screenRect = QApplication::desktop()->screenGeometry(i);
        QSize size(screenRect.width(), screenRect.height());

        bool bExist = false;
        for (int j = 0; j < screenSizeList.length(); j++)
        {
            QSize existSize = screenSizeList.at(j);
            if (size == existSize)
            {
                screenCountList[j]++;
                bExist = true;
                break;
            }
        }

        if (!bExist)
        {
            screenSizeList.append(size);
            screenCountList.append(1);
        }
    }

    QSize m_maxScreenSize = screenSizeList.at(0);
    for (int i = 0; i < screenSizeList.length(); i++)
    {
        int width = screenSizeList.at(i).width();
        int height = screenSizeList.at(i).height();

        if (width > m_maxScreenSize.width() && height > m_maxScreenSize.height())
            m_maxScreenSize = screenSizeList.at(i);

        m_screenDescribe += QString("(%1px x %2px) x %3piece").arg(width).arg(height).arg(screenCountList.at(i));
        if (i != screenSizeList.length() - 1) m_screenDescribe += ", ";
    }

    return m_screenDescribe;
}

//获取计算机硬盘信息
QString SysInfoSever::disk()
{
    double m_maxFreeDisk = 4;
    QString m_diskDescribe = "";
    QFileInfoList list = QDir::drives();
    foreach(QFileInfo dir, list)
    {
        QString dirName = dir.absolutePath();
        dirName.remove("/");
        LPCWSTR lpcwstrDriver = (LPCWSTR)dirName.utf16();
        ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
        if (GetDiskFreeSpaceEx(lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes))
        {
            QString free = QString::number((double)liTotalFreeBytes.QuadPart / 1073741824, 'f', 1);
            free += "G";
            QString all = QString::number((double)liTotalBytes.QuadPart / 1073741824, 'f', 1);
            all += "G";

            QString str = QString("%1 %2/%3       ").arg(dirName, free, all);
            m_diskDescribe += str;

            double freeMem = (double)liTotalFreeBytes.QuadPart / 1073741824;

            if (freeMem > m_maxFreeDisk)
                m_maxFreeDisk = freeMem;
        }
    }

    return m_diskDescribe;
}

//判断计算机是否可以联网
QString SysInfoSever::ipLive()
{
    //能接通百度IP说明可以通外网
    QTcpSocket tcpClient;
    tcpClient.abort();
    tcpClient.connectToHost("202.108.22.5", 80);
    //100毫秒没有连接上则判断不在线
    QString intnetLive("false");
    if (tcpClient.waitForConnected(100)) intnetLive = "true";
    return intnetLive;
}
