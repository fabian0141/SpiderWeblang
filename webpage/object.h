#ifndef OBJECT_H
#define OBJECT_H

#include<QString>
#include<QDebug>

class LoxInstance;
class LoxCallable;
class Array;
class Style;

class Object {

public:
    enum LiteralType{
          STRING, DOUBLE, LOXINST, LOXCALL, BOOLEAN, ARRAY, STYLE
    };

    QString sObj;
    double dObj;
    LoxInstance* liObj;
    LiteralType lt;
    LoxCallable* lcObj;
    bool bObj;
    Array* arObj;
    Style* stObj;


    Object(QString obj) : sObj(obj){
        lt = STRING;
    }

    Object(double obj) : dObj(obj){
        lt = DOUBLE;
    }

    Object(LoxInstance* obj) : liObj(obj){
        lt = LOXINST;
    }

    Object(LoxCallable* obj) : lcObj(obj){
        lt = LOXCALL;
    }

    Object(bool obj) : bObj(obj){
        lt = BOOLEAN;
    }

    Object(Array* obj) : arObj(obj){
        lt = ARRAY;
    }

    Object(Style* obj) : stObj(obj){
        lt = STYLE;
    }

    template<typename Obj>
    inline Obj operator=(const Object &e1)
    {
        if(e1.lt == Object::DOUBLE)
            return e1.dObj;

        if(e1.lt == Object::LOXINST)
            return e1.liObj;

        if(e1.lt == Object::LOXCALL)
            return e1.lcObj;

        if(e1.lt == Object::BOOLEAN)
            return e1.lcObj;

        return e1.sObj;
    }

};

inline uint qHash(const Object &key, uint seed)
{
    if(key.lt == Object::DOUBLE)
        return qHash(key.dObj,seed);

    if(key.lt == Object::BOOLEAN)
        return qHash(key.bObj,seed);

    if(key.lt == Object::STRING)
        return qHash(key.sObj, seed);

    return qHash(&key, seed);
}

template<typename Obj>
inline QString operator<<(const Obj &e1, const Object &e2)
{
    if(e1.lt == Object::DOUBLE)
        return *e1 << QString::number(e2.dObj);

    return *e1 << e2.sObj;
}

inline double operator-(const Object &e1, const Object &e2)
{
    if(e1.lt == Object::DOUBLE && e2.lt == Object::DOUBLE)
        return e1.dObj - e2.dObj;

    return 0;
}

inline Object* operator+(const Object &e_1, const Object &e_2)
{
    Object e1(QString("nil")),e2(QString("nil"));

    if(&e_1 != NULL)
        e1 = e_1;

    if(&e_2 != NULL)
        e2 = e_2;

    if(e1.lt == Object::DOUBLE && e2.lt == Object::DOUBLE)
        return new Object(e1.dObj + e2.dObj);

    if(e1.lt == Object::STRING && e2.lt == Object::STRING)
        return new Object(QString(e1.sObj + e2.sObj));

    if(e1.lt == Object::STRING && e2.lt == Object::DOUBLE)
        return new Object(QString(e1.sObj + QString::number(e2.dObj)));

    if(e1.lt == Object::DOUBLE && e2.lt == Object::STRING)
        return new Object(QString(QString::number(e1.dObj) + e2.sObj));

    return 0;
}

inline double operator*(const Object &e1, const Object &e2)
{
    if(e1.lt == Object::DOUBLE && e2.lt == Object::DOUBLE)
        return e1.dObj * e2.dObj;

    return 0;
}

inline double operator/(const Object &e1, const Object &e2)
{
    if(e1.lt == Object::DOUBLE && e2.lt == Object::DOUBLE)
        return e1.dObj / e2.dObj;

    return 0;
}

inline bool operator<(const Object &e1, const Object &e2)
{
    if(e1.lt == Object::DOUBLE && e2.lt == Object::DOUBLE)
        return e1.dObj < e2.dObj;

    if(e1.lt == Object::STRING && e2.lt == Object::STRING)
        return e1.sObj.toLatin1() < e2.sObj.toLatin1();

    return &e1 < &e2;
}

inline bool operator>(const Object &e1, const Object &e2)
{
    if(e1.lt == Object::DOUBLE && e2.lt == Object::DOUBLE)
        return e1.dObj > e2.dObj;

    return false;
}

inline bool operator<=(const Object &e1, const Object &e2)
{
    if(e1.lt == Object::DOUBLE && e2.lt == Object::DOUBLE)
        return e1.dObj <= e2.dObj;

    return false;
}

inline bool operator>=(const Object &e1, const Object &e2)
{
    if(e1.lt == Object::DOUBLE && e2.lt == Object::DOUBLE)
        return e1.dObj >= e2.dObj;

    return false;
}

inline bool operator==(const Object &e1, const Object &e2)
{
    if(e1.lt == Object::DOUBLE && e2.lt == Object::DOUBLE)
        return e1.dObj == e2.dObj;

    if(e1.lt == Object::BOOLEAN && e2.lt == Object::BOOLEAN)
        return e1.bObj == e2.bObj;

    if(e1.lt == Object::STRING && e2.lt == Object::STRING)
        return e1.sObj == e2.sObj;

    return &e1 == &e2;
}

inline bool operator!=(const Object &e1, const Object &e2)
{
    if(e1.lt == Object::DOUBLE && e2.lt == Object::DOUBLE)
        return e2.dObj != e2.dObj;

    if(e1.lt == Object::BOOLEAN && e2.lt == Object::BOOLEAN)
        return e2.bObj != e2.bObj;

    return !e2.sObj.compare(e2.sObj);
}



#endif // OBJECT_H
