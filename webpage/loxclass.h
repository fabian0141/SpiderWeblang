#ifndef LOXCLASS_H
#define LOXCLASS_H

#include "loxcallable.h"
#include "loxfunction.h"

class Interpreter;

class LoxClass : public LoxCallable
{
public:
    LoxClass(QString name, LoxClass* superclass, QMap<QString, LoxFunction*>* methods);
    QString name;
    LoxClass* superclass;


    LoxFunction* findMethod(LoxInstance* instance, QString name);
    QString toString();
    Object* call(Interpreter* interpreter, QList<Object*> arguments);
    int arity();
private:
    QMap<QString, LoxFunction*>* methods;
};

#endif // LOXCLASS_H
