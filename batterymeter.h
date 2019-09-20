#ifndef BATTERYMETER_H
#define BATTERYMETER_H

#include <QDialog>
#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QTimer>

namespace Ui {
class BatteryMeter;
}

class BatteryMeter : public QDialog
{
    Q_OBJECT

public:
    explicit BatteryMeter(QWidget *parent = nullptr);
    ~BatteryMeter();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void loadConfig();
    void createActions();
    void createTrayIcon();

private slots:
    void checkBattery();
    void resetAlarmFlag();

private:
    Ui::BatteryMeter *ui;

    QAction *quitAction;
    QMenu *trayIconMenu;
    QSystemTrayIcon *trayIcon;

    int lowAlarm;
    int highAlarm;
    bool alarmFlag;
};

#endif // BATTERYMETER_H
