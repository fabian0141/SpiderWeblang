#ifndef LOXCALLABLE_H
#define LOXCALLABLE_H

#include<QList>
#include "object.h"

class Interpreter;

class LoxCallable {
public:
    virtual Object* call(Interpreter* interpreter, QList<Object*> arguments) {}
    virtual int arity() {}
};

#endif // LOXCALLABLE_H
