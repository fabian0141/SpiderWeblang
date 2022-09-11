#include "drawwidgetclass.h"

DrawWidgetClass::DrawWidgetClass(Environment* environment) : environment(environment){

}

int DrawWidgetClass::arity() {
    return 22;
}

Object* DrawWidgetClass::call(Interpreter* interpreter, QList<Object*> arguments) {
    QString id = arguments[0]->sObj;
    Widget * widget;
    if(interpreter->widgets->contains(id)){
        widget = interpreter->widgets->value(id);
    }else{
        widget = new Widget(arguments, interpreter);
        interpreter->widgets->insert(id,widget);
    }

    if(arguments[1] != NULL) widget->x = arguments[1]->dObj;
    if(arguments[2] != NULL) widget->y = arguments[2]->dObj;
    if(arguments[3] != NULL) widget->width = arguments[3]->dObj;
    if(arguments[4] != NULL) widget->height = arguments[4]->dObj;
    if(arguments[5] != NULL) widget->backgroundColor = convHexToCol(arguments[5]->sObj);
    if(arguments[6] != NULL) widget->backgroundImage = arguments[6]->sObj;
    if(arguments[7] != NULL) widget->onClick = arguments[7]->lcObj;
    if(arguments[8] != NULL) widget->onHover = arguments[8]->lcObj;
    if(arguments[9] != NULL) widget->onLeave = arguments[9]->lcObj;
    if(arguments[10] != NULL) widget->text = arguments[10]->dObj;
    if(arguments[11] != NULL) widget->textColor = convHexToCol(arguments[11]->sObj);
    if(arguments[12] != NULL) widget->textX = arguments[12]->dObj;
    if(arguments[13] != NULL) widget->textY = arguments[13]->dObj;
    if(arguments[14] != NULL) widget->borderWidth = arguments[14]->dObj;
    if(arguments[15] != NULL) widget->borderColor = convHexToCol(arguments[15]->sObj);
    if(arguments[16] != NULL) widget->borderRadius = arguments[16]->dObj;
    if(arguments[17] != NULL) widget->shadowWidth = arguments[17]->dObj;
    if(arguments[18] != NULL) widget->shadowColor = convHexToCol(arguments[18]->sObj);
    if(arguments[19] != NULL) widget->shadowX = arguments[19]->dObj;
    if(arguments[20] != NULL) widget->shadowY = arguments[20]->dObj;
    if(arguments[21] != NULL) widget->z = arguments[21]->dObj;

    return NULL;
}

QColor* DrawWidgetClass::convHexToCol(QString hexStr)
{
    return new QColor(hexStr);
}
