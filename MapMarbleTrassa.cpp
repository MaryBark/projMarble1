#include "MapMarbleTrassa.h"

MapMarbleTrassa::MapMarbleTrassa()
{


//    if (!ui->cB_showSat->isChecked())
//        updateSourceSatPos();

//    SNCode::Map<QMap<QDateTime, MVectorBLH<double> > > mapCoordSat;
//    SNCode::Map<QColor> mapColorOrbit;

//    showSatellitesSettings sets = panelSatWgt->settings();

//    QDateTime dt_begin = sets.dtInterval.first;
//    QDateTime dt_end = sets.dtInterval.second;

//    foreach (QString sat, panelSatWgt->satChecked())
//    {
//        SNCode id_sat = SNCode::fromString(sat);

//        int step = sets.stepTrackGnss;
//        if (id_sat.isLEO() || id_sat.isMCC())
//            step = sets.stepTrackLeo;

//        QString sourceType = panelSatWgt->sourceTypeGnss()[SNCode::fromString(sat).type()];

//        int countIter = static_cast<int>((NTime(dt_end) - NTime(dt_begin))/step);

//        for (int i = 0; i <= countIter; ++i)
//        {
//            QDateTime dt = dt_begin.addSecs(i*step);
//            mapCoordSat[sat][dt] = satData->getBLH(sourceType, sat, NTime(dt), coordSys);
//        }

//        mapColorOrbit[sat] = panelSatWgt->getColor(id_sat);
//    }

//    w->setMapCoordSat(mapCoordSat);
//    w->setMapColorOrbit(mapColorOrbit);
}
