#ifndef ARRAY_H
#define ARRAY_H

#include <QHash>

class Expr;
class Object;

class Array
{
public:
    Array(QHash<Object, Expr*>*);
    QHash<Object, Expr*>* values;
};

#endif // ARRAY_H
