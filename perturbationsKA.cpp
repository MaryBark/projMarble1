#include "perturbationsKA.h"
#include "definitions.h"

perturbationsKA::perturbationsKA()    : m_pB(nullptr),
    m_pA0(nullptr),
    m_pF(nullptr),
    cnmGPZ_v(nullptr),
    dnmGPZ_v(nullptr)
{

}

bool perturbationsKA::pertubations(const QDateTime &dateTime, double *YASK, double *F, double *A0, double *DFDP)
{
    // как это оформить по-нормальному?????
    double drxl[3], drxs[3], drerp[3], drsrp[3], drdu[3], drstohastic[3], drplan[3], dr_rel[3],dratm[3];
    double dgr, dgm, dgl,s0,t0,w0;

    double SZ(0.0);
    double xp(0.0);
    double yp(0.0);


    double F0[3]={0,0,0};
    double F1[3]={0,0,0};
    double A0SUN[9],A0MOON[9],A0PLAN[9];
    double r_ka, rmtep[3],YGSK[3],SUN[3],MOON[3],vmtep[3],VGSK[3],YASK0[6];

    double ASK2GSK[9],GSK2ASK[9],KAPPA[9],NUT[9],NUTt[9],PRE[9],PREt[9],
            NUTPRE[9],NUTPREt[9],SID[9],SIDt[9],HELP36[36],HELP9[9];

    r_ka = sqrt(POW2(YASK[0])+POW2(YASK[1])+POW2(YASK[2]));
    for (int i=0; i<3; i++)
    {
        rmtep[i]=YASK[i];
        vmtep[i]=YASK[i+3];
        SUN[i]  =  xs[i];
        MOON[i] =  xl[i];
        F[i]=0.0;
        drplan[i] = dratm[i] = drxl[i] = drxs[i] = dr_rel[i] = drerp[i] =
        drstohastic[i] = drdu[i] = YASK0[i] = YASK0[i+3] = 0.0;
    }

    for (int i=0; i<9; i++)
        A0[i]=A0PLAN[i]=A0SUN[i]=A0MOON[i]=0;

    for (int i=0; i<45; i++)
        DFDP[i]=0;

    s0=t0=w0=0;
    dgl=dgm=dgr=0.0;


    // времая

    // матрица прецесии и нутации


    // притяжение солнца, Луны
    drx1(fML, YASK, MOON, drxl,A0MOON );

    s0+=drxl[0];
    t0+=drxl[1];
    w0+=drxl[2];
    // возмущения от планет
    // световое давление
    // релятивистская поправка

    F1[0]=s0; F1[1]=t0; F1[2]=w0;

    for (int i = 0; i < 3; i++)
    {
        F0[i] = -fMz*YASK[i]/pow(r_ka,3);
        for (int k = 0; k < 3; k++)
        {
            A0[i*3+k] += -F0[i] * 3 * YASK[k] / pow(r_ka,2);
            A0[i*3+k] += A0SUN[i*3+k] + A0MOON[i*3+k] + A0PLAN[i*3+k];
            if(i == k)
                A0[i*3+k] -= fMz / pow(r_ka,3);
        }
    }

    return true;
}

void perturbationsKA::drx1(const double nst, double *rx, double *rp, double *drx, double *A0)
{
    double dr[3],RBody,RBody3,DELTA,DELTA2,DELTA3;

    for (int i=0; i<3; i++)
        dr[i]=rp[i]-rx[i];

    DELTA=VABSPTR(dr);

    DELTA2=pow(DELTA,2);
    DELTA3=pow(DELTA,3);

    RBody=VABSPTR(rp);

    RBody3=pow(RBody,3);
    for (int i=0; i<3; i++) /// как в таком случае преобразовать цикл под 17 стандарт
        drx[i]=nst*(dr[i]/DELTA3-rp[i]/RBody3);

    for (int i=0; i<3; i++)
        for (int k=0; k<3; k++)
        {
            A0[i*3+k]=3.0*nst/DELTA3*(dr[i])*(dr[k])/DELTA2;
            if(i==k)
                A0[i*3+k]=A0[i*3+k]-nst/DELTA3;
        }
}
