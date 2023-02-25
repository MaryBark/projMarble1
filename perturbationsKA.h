#ifndef PERTURBATIONSKA_H
#define PERTURBATIONSKA_H

// класс предназначен для учета возмущающих ускорений, кт влияют на движение КА по орбите
// тема феерически огромная!!!!!! это только композиция, кт будт состоять из большого количества классвов\
// кстати, если есть идеи, как это все лучше организовать, буду рада и КУДА ВСУНУТЬ АЛЛОКАТОР?????????

#include <QDateTime>

double fMz = 398600.4415;// (км3/с2)
double fML = 4902.65910;

class perturbationsKA
{
public:
    perturbationsKA();
    ~perturbationsKA(){}

    // вычисление правых частей уравнений
    bool pertubations(const QDateTime& dateTime,double *YASK, double *F,double *A0,double *DFDP);

    // притяжение Луны
    void drx1(const double nst, double *rx, double *rp,double *drx,double *A0);

private:
    double* m_pB;
    double* m_pA0;
    double* m_pF;
    double m_H;
    double XSAT[6];
    double XSAVE[6];
    double DBDP[3*21*19];
    double DFDP[3*21*21];
    double FAK[11];
    double TEMPSAT[3*10];

    double *cnmGPZ_v, *dnmGPZ_v;

    double xl[6], xs[6];

};

#endif // PERTURBATIONSKA_H
