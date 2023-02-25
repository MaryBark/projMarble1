#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <math.h>


// забыла, чем можно заменить define в 17 стандарте(((( ааааа

#define ERROR_OUT_RANGE ("Выход за границы массива")

#define STRUCT_FIELD_EQ(x, y, z) (x.z == y.z)
#define STRUCT_FIELD_NEQ(x, y, z) (x.z != y.z)

#define POW2(x) ((x) * (x))
#define POW3(x) ((x) * (x) * (x))
#define POW4(x) ((x) * (x) * (x) * (x))
#define POW5(x) ((x) * (x) * (x) * (x) * (x))
#define POW6(x) ((x) * (x) * (x) * (x) * (x) * (x))

#define D2PI (6.283185307179586476925287)

#define VABSPTR(x) sqrt(POW2((x)[0]) + POW2((x)[1]) + POW2((x)[2]))

//double fMz = 398600.4415;// (км3/с2)
//double fML = 4902.65910;

#define QS(x) (QString::fromUtf8(x))


#endif // DEFINITIONS_H
