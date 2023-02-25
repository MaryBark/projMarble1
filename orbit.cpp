#include "orbit.h"
#include <iostream>
#include <cstdlib>
#include <climits>

//using namespace orbit;

#define ICD16
//---------------------------------------------------------------------------
#ifdef ICD08
    #ifdef ICD02
        #undef ICD02
    #endif
#endif
//---------------------------------------------------------------------------
#ifdef ICD16
    #ifdef ICD02
        #undef ICD02
    #endif
    #ifdef ICD08
        #undef ICD08
    #endif
#endif
//---------------------------------------------------------------------------
#ifdef ICD02
    #define MU  (398600.44e9)
    #define AE  (6378136.0)
    #define J20 (1082625.7e-9)
    #define WZ  (7.292115e-5)
#endif
//---------------------------------------------------------------------------
#ifdef ICD08
    #define MU  (398600.4418e9)
    #define AE  (6378136.0)
    #define J20 (1082625.75e-9)
    #define WZ  (7.292115e-5)
#endif
//---------------------------------------------------------------------------
#ifdef ICD16
    #define MU  (398600441.8e6)
    #define AE  (6378136.0)
    #define J20 (1082625.75e-9)
    #define WZ  (7.2921151467e-5)
#endif

//  Большая полуось "орбиты" Солнца, км
#define A_S (1.49598e8)

//  Эксцентриситет "орбиты" Солнца
#define E_S (0.016719)

//  Большая полуось орбиты Луны, км
#define A_M (3.84385243e5)

//  Эксцентриситет лунной орбиты
#define E_M (0.054900489)

//  Среднее наклонение орбиты Луны к плоскости эклиптики, рад.
#define I_M (0.0898041080)

//  Константа гравитационного поля Луны, м^3/c^2
#define G_M (4902.799e9)

//  Константа гравитационного поля Солнца, м^3/c^2
#define G_S (13271244.0e13)


coordVectorXYZ<double> orbit::calc_dV(const coordVectorXYZ<double> &pos,
                               const coordVectorXYZ<double> &v,
                               const coordVectorXYZ<double> &a)
{
    coordVectorXYZ<double> res;

    double r = pos.abs();

    double A1 = (-MU) / POW3(r);

    double A2 = (-3.0 * J20 * MU * POW2(AE)) / (2.0 * POW5(r));

    double A2_xy = A2 * (1.0 - ((5.0 * POW2(pos.z)) / POW2(r)));
    double A2_z = A2 * (3.0 - ((5.0 * POW2(pos.z)) / POW2(r)));

    double A3 = POW2(WZ);

    double A4 = 2 * WZ;

    res.x = (A1 + A2_xy) * pos.x + A3 * pos.x + A4 * v.y + a.x;
    res.y = (A1 + A2_xy) * pos.y + A3 * pos.y - A4 * v.x + a.y;
    res.z = (A1 + A2_z) * pos.z + a.z;

    return res;
}

coordVectorXYZ<double> orbit::calc_dV(const coordVectorXYZ<double> &pos, const coordVectorXYZ<double> &a)
{
    double r = pos.abs();

    double A1 = (-MU) / POW3(r);

    double A2 = (-3.0 * J20 * MU * POW2(AE)) / (2.0 * POW5(r));

    double A2_xy = A2 * (1.0 - ((5.0 * POW2(pos.z)) / POW2(r)));
    double A2_z = A2 * (3.0 - ((5.0 * POW2(pos.z)) / POW2(r)));

    return coordVectorXYZ<double>((A1 + A2_xy) * pos.x,
                              (A1 + A2_xy) * pos.y,
                              (A1 + A2_z) * pos.z) + a;
}

void orbit::calc_next_step(const coordVectorXYZ<double> &pos,
                           const coordVectorXYZ<double> &v,
                           const coordVectorXYZ<double> &a, const double h,
                           coordVectorXYZ<double> &new_pos,
                           coordVectorXYZ<double> &new_v)
{
    // расчет K1
    coordVectorXYZ<double> K1_p = h * v;
    coordVectorXYZ<double> K1_v = h * calc_dV(pos, v, a);

    // расчет K2
    coordVectorXYZ<double> K2_p = h * (v + 0.5 * K1_v);
    coordVectorXYZ<double> K2_v = h * calc_dV(pos + 0.5 * K1_p,
                                          v + 0.5 * K1_v, a);

    // расчет K3
    coordVectorXYZ<double> K3_p = h * (v + 0.5 * K2_v);
    coordVectorXYZ<double> K3_v = h * calc_dV(pos + 0.5 * K2_p,
                                          v + 0.5 * K2_v, a);

    // расчет K4
    coordVectorXYZ<double> K4_p = h * (v + K3_v);
    coordVectorXYZ<double> K4_v = h
            * calc_dV(pos + K3_p, v + K3_v, a);

    new_pos = pos + (K1_p + (2.0 * K2_p) + (2.0 * K3_p) + K4_p) / 6.0;
    new_v = v + (K1_v + (2.0 * K2_v) + (2.0 * K3_v) + K4_v) / 6.0;
}

void orbit::calc_next_step(const coordVectorXYZ<double> &pos,
                           const coordVectorXYZ<double> &v,
                           const coordVectorXYZ<double> &a,
                           const coordVectorXYZ<double> *da,
                           const double hBase, const double h,
                           coordVectorXYZ<double> &new_pos, coordVectorXYZ<double> &new_v)
{
    // расчет K1
    coordVectorXYZ<double> K1_p = h * v;
    coordVectorXYZ<double> K1_v = h * calc_dV(pos, v,
                                          a + calcDeltaAcc(da, hBase));

    // расчет K2
    coordVectorXYZ<double> K2_p = h * (v + 0.5 * K1_v);
    coordVectorXYZ<double> K2_v = h * calc_dV(
                pos + 0.5 * K1_p,
                v + 0.5 * K1_v,
                a + calcDeltaAcc(da, hBase + 0.5 * h));

    // расчет K3
    coordVectorXYZ<double> K3_p = h * (v + 0.5 * K2_v);
    coordVectorXYZ<double> K3_v = h * calc_dV(
                pos + 0.5 * K2_p,
                v + 0.5 * K2_v,
                a + calcDeltaAcc(da, hBase + 0.5 * h));

    // расчет K4
    coordVectorXYZ<double> K4_p = h * (v + K3_v);
    coordVectorXYZ<double> K4_v = h
            * calc_dV(pos + K3_p,
                      v + K3_v,
                      a + calcDeltaAcc(da, hBase + h));

    new_pos = pos + (K1_p + (2.0 * K2_p) + (2.0 * K3_p) + K4_p) / 6.0;
    new_v = v + (K1_v + (2.0 * K2_v) + (2.0 * K3_v) + K4_v) / 6.0;
}

void orbit::calc_next_step(const coordVectorXYZ<double> &pos,
                           const coordVectorXYZ<double> &v,
                           const rungeKutta &sun,
                           const rungeKutta &moon,
                           const double h,
                           coordVectorXYZ<double> &new_pos,
                           coordVectorXYZ<double> &new_v)
{
    // расчет K1
    coordVectorXYZ<double> K1_p = h * v;
    coordVectorXYZ<double> K1_v = h * calc_dV(pos,
                                          acc(sun, moon, pos));

    // расчет K2
    coordVectorXYZ<double> K2_p = h * (v + 0.5 * K1_v);
    coordVectorXYZ<double> K2_v = h * calc_dV(pos + 0.5 * K1_p,
                                          acc(sun, moon, pos + 0.5 * K1_p));

    // расчет K3
    coordVectorXYZ<double> K3_p = h * (v + 0.5 * K2_v);
    coordVectorXYZ<double> K3_v = h * calc_dV(pos + 0.5 * K2_p,
                                          acc(sun, moon, pos + 0.5 * K2_p));

    // расчет K4
    coordVectorXYZ<double> K4_p = h * (v + K3_v);
    coordVectorXYZ<double> K4_v = h * calc_dV(pos + K3_p,
                                          acc(sun, moon, pos + K3_p));

    new_pos = pos + (K1_p + (2.0 * K2_p) + (2.0 * K3_p) + K4_p) / 6.0;
    new_v = v + (K1_v + (2.0 * K2_v) + (2.0 * K3_v) + K4_v) / 6.0;
}

coordVectorXYZ<double> orbit::calcDeltaAcc(const coordVectorXYZ<double> *da, const double tgtTime)
{
    coordVectorXYZ<double> res(NULL_VECTOR_XYZ);

    for(int i = 0; i <= 4; i++)
        res += da[i] * pow(tgtTime, i);

    return res;
}

void orbit::gsk2ask(const coordVectorXYZ<double> &posIn,
                    const coordVectorXYZ<double> &velIn,
                    const double sTime,
                    coordVectorXYZ<double> &posOut,
                    coordVectorXYZ<double> &velOut)
{
    posOut = coordVectorXYZ<double>(posIn.x * cos(sTime) - posIn.y * sin(sTime),
                                posIn.x * sin(sTime) + posIn.y * cos(sTime),
                                posIn.z);

    velOut = coordVectorXYZ<double>(
                velIn.x * cos(sTime) - velIn.y * sin(sTime) - WZ * posOut.y,
                velIn.x * sin(sTime) + velIn.y * cos(sTime) + WZ * posOut.x,
                velIn.z);
}

void orbit::ask2gsk(const coordVectorXYZ<double> &posIn,
                    const coordVectorXYZ<double> &velIn,
                    const double sTime,
                    coordVectorXYZ<double> &posOut,
                    coordVectorXYZ<double> &velOut)
{
    posOut = coordVectorXYZ<double>( posIn.x * cos(sTime) + posIn.y * sin(sTime),
                                -posIn.x * sin(sTime) + posIn.y * cos(sTime),
                                 posIn.z);

    velOut = coordVectorXYZ<double>(
                 velIn.x * cos(sTime) + velIn.y * sin(sTime) + WZ * posOut.y,
                -velIn.x * sin(sTime) + velIn.y * cos(sTime) - WZ * posOut.x,
                 velIn.z);
}

coordVectorXYZ<double> orbit::ask2gsk(const coordVectorXYZ<double> &v, const double s)
{
    return coordVectorXYZ<double>(v.x * cos(s) + v.y * sin(s),
                              -v.x * sin(s) + v.y * cos(s),
                              v.z);

}

double orbit::gmst(const double jd0)
{
    double dt((jd0 - 2451545.0) / 36525.0);

    return D2PI * (0.7790572732640 + 1.00273781191135448 * (jd0 - 2451545.0))
            + 0.0000000703270726
            + 0.0223603658710194 * dt
            + 0.0000067465784654 * POW2(dt)
            + 0.0000000000021332 * POW3(dt)
            + 0.0000000001452308 * POW4(dt)
            + 0.0000000000001784 * POW5(dt);
}

double orbit::starTime(const double gst, const double tb)
{
    return gst + WZ * (tb - 10800.0);
}

orbit::rungeKutta orbit::sun(const double jd0, const double tb)
{
    double T = ephT(jd0, tb);

    double q_c = 6.2400601269 + 628.3019551714 * T - 0.0000026820 * POW2(T);

    double Ecc_s = solveKeplerEquation(q_c, E_S);

    double sinTeta;
    double cosTeta;

    sinCosTeta(E_S, Ecc_s, sinTeta, cosTeta);

    double w_s = -7.6281824375 + 0.0300101976 * T + 0.0000079741 * POW2(T);

    double eps = meanEcliptic(T);

    return rungeKutta(cosTeta * cos(w_s) - sinTeta * sin(w_s),
                     (sinTeta * cos(w_s) + cosTeta * sin(w_s)) * cos(eps),
                     (sinTeta * cos(w_s) + cosTeta * sin(w_s)) * sin(eps),
                     A_S * (1.0 - E_S * cos(Ecc_s)));
}

orbit::rungeKutta orbit::moon(const double jd0, const double tb)
{
    double T = ephT(jd0, tb);

    double q_m = 2.3555557435 + 8328.6914257190 * T + 0.0001545547 * POW2(T);

    double omega_m = 2.1824391966 - 33.7570459536 * T + 0.0000362262 * POW2(T);
    double gamma_m = 1.4547885346 + 71.0176852437 * T - 0.0001801481 * POW2(T);

    double eps = meanEcliptic(T);

    double Ecc_m = solveKeplerEquation(q_m, E_M);

    double sinTeta;
    double cosTeta;

    sinCosTeta(E_M, Ecc_m, sinTeta, cosTeta);

    double xi_ast = 1.0 - POW2(cos(omega_m)) * (1.0 - cos(I_M));
    double eta_ast = sin(omega_m) * sin(I_M);
    double iot_ast = cos(omega_m) * sin(I_M);

    double xi_11 = sin(omega_m) * cos(omega_m) * (1.0 - cos(I_M));

    coordVectorXYZPair<double> coeff(
                coordVectorXYZ<double>(
                    xi_11,
                    xi_ast * cos(eps) - iot_ast * sin(eps),
                    xi_ast * sin(eps) + iot_ast * cos(eps)),
                coordVectorXYZ<double>(
                    1.0 - POW2(sin(omega_m)) * (1.0 - cos(I_M)),
                    xi_11 * cos(eps) + eta_ast * sin(eps),
                    xi_11 * sin(eps) - eta_ast * cos(eps)
                    ));

    double mult1 = (sinTeta * cos(gamma_m) + cosTeta * sin(gamma_m));
    double mult2 = (cosTeta * cos(gamma_m) - sinTeta * sin(gamma_m));

    return rungeKutta( mult1 * coeff.first + mult2 * coeff.second,
                      A_M * (1.0 - E_M * cos(Ecc_m)));
}

double orbit::solveKeplerEquation(const double q, const double e, const double eps)
{
    double res = q + e * sin(q);

    forever
    {
        double newRes = q + e * sin(res);

        if(qAbs(newRes - res) < eps)
            return newRes;

        res = newRes;
    }
}

void orbit::sinCosTeta(const double ecc, const double E, double &sinTeta, double &cosTeta)
{
    double help = (1.0 - ecc * cos(E));

    sinTeta = (sqrt(1.0 - POW2(ecc)) * sin(E)) / help;
    cosTeta = (cos(E) - ecc) / help;
}

coordVectorXYZ<double> orbit::acc(const double G, const rungeKutta &body, const coordVectorXYZ<double> &sat)
{
    coordVectorXYZ<double> deltaCos = (body.cos - sat / body.r);

    return (G / POW2(body.r))
            * (deltaCos / POW3(deltaCos.abs()) - body.cos);
}

coordVectorXYZ<double> orbit::acc(const rungeKutta &sun, const rungeKutta &moon, const coordVectorXYZ<double> &sat)
{
    return acc(G_S, sun, sat) + acc(G_M, moon, sat);
}

coordVectorXYZ<double> orbit::accSun(const rungeKutta &sun, const coordVectorXYZ<double> &sat)
{
    return acc(G_S, sun, sat);
}

coordVectorXYZ<double> orbit::accMoon(const rungeKutta &moon, const coordVectorXYZ<double> &sat)
{
    return acc(G_M, moon, sat);
}
