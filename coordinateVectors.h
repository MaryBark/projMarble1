#ifndef COORDINATEVECTORS_H
#define COORDINATEVECTORS_H

#include <math.h>
#include <QtCore>
#include "definitions.h"


template < typename T > class coordVectorBase
{
public:
    virtual ~coordVectorBase(){}

    virtual T& operator[](const unsigned i) = 0;
    virtual unsigned length() const = 0;
};


template < typename T > class coordVectorXYZ : public coordVectorBase<T>
{
public:

    union
    {
        struct
        {
            T x;
            T y;
            T z;
        };
        T data[3];
    };

    coordVectorXYZ() {}
    coordVectorXYZ(const coordVectorXYZ<T> &old)
        : x(old.x), y(old.y), z(old.z)
    {}
    coordVectorXYZ(T _x, T _y, T _z)
        : x(_x), y(_y), z(_z)
    {}

    coordVectorXYZ(const T * p)
    {
        if(IS_NULL(p))
            return;

        for(int i = 0; i < length(); i++)
            data[i] = p[i];
    }


    virtual unsigned length() const
    {   return 3;   }

    T& operator[](const unsigned i)
    {
        switch (i)
        {
        case 0:
            return x;

        case 1:
            return y;

        case 2:
            return z;

        default:
            throw ERROR_OUT_RANGE;
        }
    }

    const T& operator[](const unsigned i) const
    {
        switch (i)
        {
        case 0:
            return x;

        case 1:
            return y;

        case 2:
            return z;

        default:
            throw ERROR_OUT_RANGE;
        }
    }

    const T& at(const unsigned i) const
    {
        switch (i)
        {
        case 0:
            return x;

        case 1:
            return y;

        case 2:
            return z;

        default:
            throw ERROR_OUT_RANGE;
        }
    }


    T abs() const
    {   return sqrt(POW2(x) + POW2(y) + POW2(z));   }

    coordVectorXYZ<T> normalize() const
    {   return coordVectorXYZ<T>(x / abs(), y / abs(), z / abs());  }

    coordVectorXYZ<T> invert() const
    {   return coordVectorXYZ<T>(-x, -y, -z);   }

    coordVectorXYZ<T> cross(const coordVectorXYZ<T>& v)
    {
        return coordVectorXYZ<T>(y * v.z - z * v.y,
                             z * v.x - x * v.z,
                             x * v.y - y * v.x);
    }

    coordVectorXYZ<T> toHEN() const
    {   return coordVectorXYZ<T>(x, y, z);  }

    coordVectorXYZ<T> toBLH() const
    {   return coordVectorXYZ<T>(x, y, z);  }

    coordVectorXYZ<T> toXYZ() const
    {   return *this;   }

    coordVectorXYZ<T>& operator=(const coordVectorXYZ<T>& old)
    {
        x = old.x;
        y = old.y;
        z = old.z;
        return *this;

    }

    coordVectorXYZ<T> xy() const
    {   return coordVectorXYZ<T>(x, y);  }


    friend coordVectorXYZ<T> operator+(const coordVectorXYZ<T>& left,
                                       const coordVectorXYZ<T>& right)
    {
        return coordVectorXYZ<T>(left.x + right.x,
                             left.y + right.y,
                             left.z + right.z);

    }

    friend coordVectorXYZ<T> operator-(const coordVectorXYZ<T>& left,
                                   const coordVectorXYZ<T>& right)
    {
        return coordVectorXYZ<T>(left.x - right.x,
                             left.y - right.y,
                             left.z - right.z);

    }

    friend const coordVectorXYZ<T> operator-(const coordVectorXYZ<T>& right)
    {   return coordVectorXYZ<T>(-right.x, -right.y, -right.z); }

    friend coordVectorXYZ<T> operator/(const coordVectorXYZ<T>& left, const T& right)
    {
        return coordVectorXYZ<T>(left.x / right,
                             left.y / right,
                             left.z / right);

    }

    friend coordVectorXYZ<T> operator*(const T& left, const coordVectorXYZ<T>& right)
    {
        return coordVectorXYZ<T>(left * right.x,
                             left * right.y,
                             left * right.z);

    }

    friend coordVectorXYZ<T> operator*(const coordVectorXYZ<T>& left, const T& right)
    {
        return coordVectorXYZ<T>(left.x * right,
                             left.y * right,
                             left.z * right);

    }

    friend T operator*(const coordVectorXYZ<T>& left, const coordVectorXYZ<T>& right)
    {	return left.x * right.x + left.y * right.y + left.z * right.z;	}


    friend coordVectorXYZ<T> operator%(const coordVectorXYZ<T>& left,
                                   const coordVectorXYZ<T>& right)
    {
        return coordVectorXYZ<T>(left.y * right.z - left.z * right.y,
                             left.z * right.x - left.x * right.z,
                             left.x * right.y - left.y * right.x);
    }

    friend int operator==(const coordVectorXYZ<T>& left,
                          const coordVectorXYZ<T>& right)
    {
        return STRUCT_FIELD_EQ(left, right, x) &&
                STRUCT_FIELD_EQ(left, right, y) &&
                STRUCT_FIELD_EQ(left, right, z);

    }

    friend int operator!=(const coordVectorXYZ<T>& left,
                          const coordVectorXYZ<T>& right)
    {
        return STRUCT_FIELD_NEQ(left, right, x) ||
                STRUCT_FIELD_NEQ(left, right, y) ||
                STRUCT_FIELD_NEQ(left, right, z);
    }

    friend coordVectorXYZ<T> operator*=(coordVectorXYZ<T>& left, const T& right)
    {	return left = (left * right); }

    friend coordVectorXYZ<T> operator/=(coordVectorXYZ<T>& left, const T& right)
    {	return left = left / right; }

    friend coordVectorXYZ<T> operator+=(coordVectorXYZ<T>& left,
                                    const coordVectorXYZ<T>& right)
    {	return left = left + right; }

    friend coordVectorXYZ<T> operator-=(coordVectorXYZ<T>& left,
                                    const coordVectorXYZ<T>& right)
    {	return left = left - right; }

};


template<class T>
class coordVectorXYZPair : public QPair<coordVectorXYZ<T>, coordVectorXYZ<T> >
{
public:
    coordVectorXYZPair() {}
    coordVectorXYZPair(const coordVectorXYZ<T> &t1, const coordVectorXYZ<T> &t2)
        : QPair<coordVectorXYZ<T>, coordVectorXYZ<T> >(t1, t2)
    {}
};


template < typename T > class coordVectorBLH : public coordVectorBase<T>
{
public:
    union
    {
        struct
        {
            T b;
            T l;
            T h;
        };
        T data[3];
    };


    coordVectorBLH()
    {}


    coordVectorBLH(T _b, T _l, T _h)
        : b(_b), l(_l), h(_h)
    {}

    T& operator[](const unsigned i)
    {
        switch (i)
        {
        case 0:
            return b;

        case 1:
            return l;

        case 2:
            return h;

        default:
            throw ERROR_OUT_RANGE;
        }
    }

    virtual unsigned length() const {   return 3;   }

};

#endif // COORDINATEVECTORS_H
