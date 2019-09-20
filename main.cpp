#include <QApplication>

#include "batterymeter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    BatteryMeter bm;
    bm.show();

    return a.exec();
}
