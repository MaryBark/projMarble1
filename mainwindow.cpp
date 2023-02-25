#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : MarbleWidget(parent)
    , ui(new Ui::MainWindow)
{
//    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::customPaint(GeoPainter *painter)
{
    GeoDataCoordinates kam(158.438854, 54.49744, 0.0, GeoDataCoordinates::Degree  );
    painter->setPen(QPen(QBrush(QColor::fromRgb(255, 0, 0)), 5.0, Qt::SolidLine,Qt::RoundCap ));
    painter->drawEllipse(kam, 7,7);
    painter->setPen(QPen(QBrush(QColor::fromRgb(255, 255, 255)), 1.5, Qt::SolidLine,Qt::RoundCap ));
    painter->drawText(kam, QString("Kamchatka"));

    // отрисовка трассы
    painter->setPen( Qt::red );

    GeoDataLineString shapeLatitudeCircle(RespectLatitudeCircle | Tessellate);
    for(int k = 0; k < vecSchir.count(); k++)
    {
        shapeLatitudeCircle.clear();
        for(int i = 0; i < vecSchir[0].count(); i++)
        {
            //GeoDataCoordinates trass( dol1[i], schir1[i], 0.0, GeoDataCoordinates::Degree );
            GeoDataCoordinates trass( vecSchir[k][i], vecDol[k][i], 0.0, GeoDataCoordinates::Degree );
            //painter->drawLine(trass);
            shapeLatitudeCircle << trass;
            //painter->drawPolyline(GeoDataLineString() << trass);
        }
        //painter->drawLine(trass);
        painter->drawPolyline(shapeLatitudeCircle);
    }



    // тупо пробник в лоб
    QVector < double > schiro(23), dol(23);
    schiro[0] = 108.6364556;       dol[0] = 0.34654575674;
    schiro[1] = 118.0464367;       dol[1] = -43.564756657;
    schiro[2] = 145.0876547;       dol[2] = -76.736586587;
    schiro[3] = 162.0365834;       dol[3] = -80.237493776;
    schiro[4] = 179.0563677;       dol[4] = -81.657588688;
    schiro[5] = -157.4565875;      dol[5] = -82.009786553;
    schiro[6] = -117.3356747;      dol[6] = -80.000064367;
    schiro[7] = -88.865463373;     dol[7] = -70.875454323;
    schiro[8] = -77.364767437;     dol[8] = -60.475848849;
    schiro[9] = -68.456758678;     dol[9] = -47.456473778;
    schiro[10] = -62.036874036;    dol[10] = -22.436747487;
    schiro[11] = -56.457785778;    dol[11] = 0.65474647463;
    schiro[12] = -33.837659346;    dol[12] = 58.456575787;
    schiro[13] = -26.459674764;    dol[13] = 67.364757873;
    schiro[14] = 0.000645767;      dol[14] = 80.346735783;
    schiro[15] = 15.345647547;     dol[15] = 81.635647757;
    schiro[16] = 30.467575775;     dol[16] = 82.456575478;
    schiro[17] = 44.546577588;     dol[17] = 81.957556564;
    schiro[18] = 60.866647545;     dol[18] = 81.00046466;
    schiro[19] = 65.238765879;     dol[19] = 80.475854884;
    schiro[20] = 90.873566746;     dol[20] = 76.364758659;
    schiro[21] = 110.948576446;    dol[21] = 60.000675666;
    schiro[22] = 132.567755865;    dol[22] = 0.3464636474;



    QVector < QVector < double > > vecSchir1(count, QVector < double >(23)), vecDol1(count, QVector < double >(23));
    vecSchir1[0] = schiro;
    vecDol1[0] = dol;



    int konezMassiva = vecSchir1[0].count();

        for(int i = 0; i < vecSchir1.count()-1; i++)
            {
                for(int j = 0; j < vecSchir1[0].count(); j++)
                {
                    vecSchir1[i+1][j] = vecSchir1[i][j] + ugol;
                    //qDebug() << "nachalka = " << i+1 << "  " << j << "  " << vecSchir1[i+1][j];


                    if( vecSchir1[i+1][j] >= 180.00000000000)
                    {
                        vecSchir1[i+1][j] = (vecSchir1[i+1][j] - 180.0) - 180.0;
                        //qDebug() << "i = " << i+1 << " j  = " << j << "  " << vecSchir1[i+1][j];
                        //int y = 120;
                    }
                }
                vecSchir1[i+1][0] = vecSchir1[i][konezMassiva-1];
                vecDol1[i+1] = dol;

                //qDebug() << "konez " << i+1 << "trassi";
            }

        vecSchir1[0][0] = vecSchir1[vecSchir1.count()-1][konezMassiva-1];
        vecSchir1[0][1] = vecSchir1[0][1] - 10.0;
        //vecSchir1[0][2] = vecSchir1[0][2] - 5.0;

    vecSchir = vecSchir1;
    vecDol   = vecDol1;
}

