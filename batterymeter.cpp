#include "batterymeter.h"
#include "ui_batterymeter.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QStandardPaths>
#include <QDir>
#include <QIODevice>
#include <QTextStream>

#include <windows.h>
#include <iostream>

using namespace std;

BatteryMeter::BatteryMeter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BatteryMeter)
{
    ui->setupUi(this);

    createActions();
    createTrayIcon();
    loadConfig();

    trayIcon->show();

    resetAlarmFlag();
    checkBattery();
}

BatteryMeter::~BatteryMeter()
{
    delete ui;
}

void BatteryMeter::loadConfig()
{
    //ToDo: Need for exception handling

    QString configFilePath = QDir::homePath() + "/.battery_meter";
    // Check config existance
    QFile configFile(configFilePath);
    if(configFile.exists()) {
        QString line;
        if (configFile.open(QIODevice::ReadOnly))
        {
           QTextStream in(&configFile);
           while (!in.atEnd())
           {
              line = in.readLine();
           }
           configFile.close();
        }
        QStringList params = line.split(",");
        lowAlarm = params[0].toInt();
        highAlarm = params[1].toInt();
    } else {
        if (configFile.open(QIODevice::WriteOnly))
        {
           QTextStream out(&configFile);
           out << "40,80";
           configFile.close();
        }
        lowAlarm = 40;
        highAlarm = 80;
    }
    // Read values if exist
}

void BatteryMeter::createActions()
{
    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void BatteryMeter::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    QIcon icon = QIcon(":/images/battery.png");
    trayIcon->setIcon(icon);
    trayIcon->setToolTip("Text");
}

void BatteryMeter::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        QIcon icon = QIcon(":/images/battery.png");
        trayIcon->showMessage("Battery Meter", "The program will keep running in the system tray. To terminate the program, choose Quit in the context menu of the system tray entry.", icon, 5000);
        hide();
        event->ignore();
    }
}

void BatteryMeter::checkBattery()
{
    SYSTEM_POWER_STATUS spsPwr;

    if( GetSystemPowerStatus(&spsPwr) ) {
        double percent = static_cast<double>(spsPwr.BatteryLifePercent);
        unsigned int batteryFlag = static_cast<unsigned int>(spsPwr.BatteryFlag);

        QIcon icon;

        if(batteryFlag == 128) {
            icon = QIcon(":/images/no_battery.png");
            trayIcon->setToolTip("No Battery");
        } else if(batteryFlag != 255) {
            if(percent < 10) {
                icon = QIcon(":/images/battery_10.png");
            } else if (percent < 20) {
                icon = QIcon(":/images/battery_20.png");
            } else if (percent < 30) {
                icon = QIcon(":/images/battery_30.png");
            } else if (percent < 40) {
                icon = QIcon(":/images/battery_40.png");
            } else if (percent < 50) {
                icon = QIcon(":/images/battery_50.png");
            } else if (percent < 60) {
                icon = QIcon(":/images/battery_60.png");
            } else if (percent < 70) {
                icon = QIcon(":/images/battery_70.png");
            } else if(percent < 80) {
                icon = QIcon(":/images/battery_80.png");
            } else if (percent < 90) {
                icon = QIcon(":/images/battery_90.png");
            } else {
                icon = QIcon(":/images/battery_100.png");
            }

            trayIcon->setToolTip(QString("Battery: %1\%").arg(percent));

            if(alarmFlag) {
                if(percent <= lowAlarm) {
                    trayIcon->showMessage("Battery Status", QString("Low level has been reached.(%1\%)").arg(percent), icon, 5000);
                    alarmFlag = false;
                }

                if(percent >= highAlarm) {
                    trayIcon->showMessage("Battery Status", QString("High level has been reached. (%1\%)").arg(percent), icon, 5000);
                    alarmFlag = false;
                }
            }
        }

        trayIcon->setIcon(icon);
    } else {
        QIcon icon = QIcon(":/images/alert_128.png");
        trayIcon->setIcon(icon);
        trayIcon->setToolTip("Error in battery info...");
        trayIcon->showMessage("Battery Status", "Error in getting battery info...", icon, 60000);
    }

    QTimer::singleShot(60000, this, SLOT(checkBattery())); // Check battery status every minutes
}


void BatteryMeter::resetAlarmFlag()
{
    alarmFlag = true;
    QTimer::singleShot(300000, this, SLOT(resetAlarmFlag())); // 5 minutes alarm
}
