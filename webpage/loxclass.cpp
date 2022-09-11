#include "loxclass.h"
#include "interpreter.h"

LoxClass::LoxClass(QString name, LoxClass* superclass, QMap<QString, LoxFunction*>* methods) : name(name), superclass(superclass), methods(methods){

}

LoxFunction* LoxClass::findMethod(LoxInstance* instance, QString name) {
    if (methods->contains(name)) {
        LoxFunction* func = methods->value(name)->bind(instance);
        return func;
    }

    if (superclass != NULL) {
        return superclass->findMethod(instance, name);
    }

    return NULL;
}

QString LoxClass::toString() {
    return name;
}
Object* LoxClass::call(Interpreter* interpreter, QList<Object*> arguments) {
    LoxInstance* instance = new LoxInstance(this);

    LoxFunction* initializer = methods->value("init");
    if (initializer != NULL) {
        initializer->bind(instance)->call(interpreter, arguments);
    }

    return new Object(instance);
}

int LoxClass::arity() {
    return 0;
}
