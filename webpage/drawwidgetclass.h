#ifndef DRAWWIDGETCLASS_H
#define DRAWWIDGETCLASS_H

#include "environment.h"
#include "interpreter.h"
#include "stmt.h"
#include "object.h"
#include "widget.h"

class DrawWidgetClass : public LoxCallable
{
public:
    DrawWidgetClass(Environment *environment);
    int arity();
    Object *call(Interpreter *interpreter, QList<Object *> arguments);

    Environment* environment;

private:
    QColor* convHexToCol(QString);

};

#endif // DRAWWIDGETCLASS_H
