#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//#include "../3rd_party/marble-product/include/marble/MarbleWidget.h"
#include "../../3rd_party/marble-product/include/marble/MarbleWidget.h"
#include "../../3rd_party/marble-product/include/marble/GeoPainter.h"
#include "../../3rd_party/marble-product/include/marble/MarbleMap.h"
#include "../../3rd_party/marble-product/include/marble/GeoDataLineString.h"
#include "../../3rd_party/marble-product/include/marble/LayerInterface.h"
#include "../../3rd_party/marble-product/include/marble/RouteRequest.h"


//#include "marble/MarbleWidget.h"
//#include <marble/GeoPainter.h>
//#include <marble/MarbleMap.h>
//#include <marble/MarbleModel.h>
//#include <marble/GeoPainter.h>
//#include <marble/GeoDataLineString.h>
//#include <marble/LayerInterface.h>
//#include <marble/RouteRequest.h>
//#include <marble/RoutingManager.h>
//#include <marble/MarbleModel.h>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


using namespace Marble;

class MainWindow : public MarbleWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    double ugol = 13.9585;
    int count = 360.0 / ugol;

    QVector < double > schir1, dol1;
    QVector < QVector < double > > vecSchir, vecDol;

    // виртуальная функция для нследвоания метода из базового класса
    virtual void customPaint(GeoPainter* painter);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
