#ifndef LOXINSTANCE_H
#define LOXINSTANCE_H

#include<QMap>
#include "token.h"
#include "object.h"

class LoxClass;
class LoxFunction;

class LoxInstance
{
public:
    LoxInstance(LoxClass* klass);
    void set(Token name, Object* value);
    void setMethod(Token name, LoxFunction* method);
    Object* get(Token name);
    QString toString();
private:
    LoxClass* klass;
    QMap<QString, Object*>* fields;
    QMap<QString, LoxFunction*>* methods;
};

#endif // LOXINSTANCE_H
