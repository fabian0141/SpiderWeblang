#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "token.h"
#include<QMap>

class Environment
{
public:
    Environment(Environment* enclosing = NULL);
    Environment* enclosing;


    void define(QString name, Object* value);
    Environment* ancestor(int distance);
    Object* getAt(int distance, QString name);
    void assignAt(int distance, Token* name, Object* value);

    Object* get(Token name);
    void assign(Token* name, Object* value);
private:
    QMap<QString, Object*> values;

};

#endif // ENVIRONMENT_H
