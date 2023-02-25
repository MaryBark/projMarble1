#ifndef ORBIT_H
#define ORBIT_H

#include "coordinateVectors.h"
#include "definitions.h"


#define NULL_VECTOR_XYZ (coordVectorXYZ<double>(0.0, 0.0, 0.0))


namespace orbit
{
    class rungeKutta
    {

    public:
        coordVectorXYZ<double> cos;
        double r;


        inline rungeKutta()
            : cos(NULL_VECTOR_XYZ),
              r(0.0)
        {}

        inline rungeKutta(const double x,
                         const double y,
                         const double z,
                         const double _r)
            : cos(x, y, z),
              r(_r)
        {}


        inline rungeKutta(const coordVectorXYZ<double> _cos, const double _r)
            : cos(_cos),
              r(_r)
        {}

        inline coordVectorXYZ<double> pos() const
        {   return r * cos; }

        inline friend rungeKutta operator*(const double left,
                                          const rungeKutta& right)
        {   return rungeKutta(right.cos, left * right.r);    }

        inline friend rungeKutta operator*(const rungeKutta& left,
                                          const double right)
        {   return rungeKutta(left.cos, left.r * right); }

    };

    coordVectorXYZ<double> calc_dV(const coordVectorXYZ<double>&,
                               const coordVectorXYZ<double>&,
                               const coordVectorXYZ<double>&);

    coordVectorXYZ<double> calc_dV(const coordVectorXYZ<double>&,
                               const coordVectorXYZ<double> &a);

    void calc_next_step(const coordVectorXYZ<double>&,
                        const coordVectorXYZ<double>&,
                        const coordVectorXYZ<double>&,
                        const double,
                        coordVectorXYZ<double>&,
                        coordVectorXYZ<double>&);

    void calc_next_step(const coordVectorXYZ<double>&,
                        const coordVectorXYZ<double>&,
                        const coordVectorXYZ<double>&,
                        const coordVectorXYZ<double>*,
                        const double,
                        const double,
                        coordVectorXYZ<double>&,
                        coordVectorXYZ<double>&);

    void calc_next_step(const coordVectorXYZ<double>&,
                        const coordVectorXYZ<double>&,
                        const rungeKutta &sun,
                        const rungeKutta &moon,
                        const double,
                        coordVectorXYZ<double>&,
                        coordVectorXYZ<double>&);

    coordVectorXYZ<double> calcDeltaAcc(const coordVectorXYZ<double> *,
                                    const double);

    void gsk2ask(const coordVectorXYZ<double>&, const coordVectorXYZ<double>&,
                 const double,
                 coordVectorXYZ<double>&, coordVectorXYZ<double>&);

    void ask2gsk(const coordVectorXYZ<double>&, const coordVectorXYZ<double>&,
                 const double,
                 coordVectorXYZ<double>&, coordVectorXYZ<double>&);

    coordVectorXYZ<double> ask2gsk(const coordVectorXYZ<double>&, const double);

    inline double jd0(const int N4, const int Nt)
    {   return 1461 * (N4 - 1) + Nt + 2450082.5;    }

    double gmst(const double);


    inline double gmst(const int N4, const int Nt)
    {   return gmst(jd0(N4, Nt));   }

    double starTime(const double gst, const double tb);


    inline double starTime(const int N4, const int Nt, const double tb)
    {   return starTime(gmst(N4, Nt), tb);  }

    rungeKutta sun(const double jd0, const double tb);
    rungeKutta moon(const double jd0, const double tb);


    inline rungeKutta sun(const int N4, const int Nt, const double tb)
    {   return sun(jd0(N4, Nt), tb);    }


    inline rungeKutta moon(const int N4, const int Nt, const double tb)
    {   return moon(jd0(N4, Nt), tb);   }

    inline double ephT(const double jd0, const double tb)
    {   return (jd0 + ((tb - 10800.0) / 86400.0) - 2451545.0) / 36525.0;    }


    inline double meanEcliptic(const double T)
    {
        return 0.4090926006 - 0.0002270711 * T;
    }

    double solveKeplerEquation(const double q, const double e,
                               const double eps = 1e-8);

    void sinCosTeta(const double ecc, const double E, double& sinTeta,
                    double& cosTeta);

    coordVectorXYZ<double> acc(const double G,
                           const rungeKutta &body,
                           const coordVectorXYZ<double> &sat);

    coordVectorXYZ<double> acc(const rungeKutta &sun,
                           const rungeKutta &moon,
                           const coordVectorXYZ<double> &sat);


    coordVectorXYZ<double> accSun(const rungeKutta &sun,
                            const coordVectorXYZ<double> &sat);

    coordVectorXYZ<double> accMoon(const rungeKutta &moon,
                            const coordVectorXYZ<double> &sat);



}




#endif // ORBIT_H
