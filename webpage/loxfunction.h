#ifndef LOXFUNCTION_H
#define LOXFUNCTION_H

#include "environment.h"
#include "stmt.h"
#include "loxinstance.h"
#include<QList>
#include "token.h"
#include "loxcallable.h"
class Interpreter;

class LoxFunction : public LoxCallable
{
public:
    LoxFunction(Stmt::Function* declaration, Environment* closure, bool isInitializer);

    QString toString();
    int arity();
    Object* call(Interpreter* interpreter, QList<Object*> arguments);
    LoxFunction* bind(LoxInstance* instance);

private:
    Stmt::Function* declaration;
    Environment* closure;
    bool isInitializer;
};

#endif // LOXFUNCTION_H
