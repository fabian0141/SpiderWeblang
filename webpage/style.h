#ifndef STYLE_H
#define STYLE_H

#include "expr.h"
#include "stmt.h"
#include "token.h"
#include <QList>

class Style
{
public:
    Style(QList<Token>* names, QList<Expr*>* values, QList<Stmt::Function*> methods) : names(names), values(values),methods(methods){}

    QList<Object*> objs;

    void addToStyleGroup(Object* obj){
        objs.append(obj);
    }

    QList<Token>* names;
    QList<Expr*>* values;
    QList<Stmt::Function*> methods;

};

#endif // STYLE_H
