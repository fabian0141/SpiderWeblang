#include "loxinstance.h"
#include "loxclass.h"


LoxInstance::LoxInstance(LoxClass* klass) : klass(klass){
    fields = new  QMap<QString, Object*>();
    methods = new  QMap<QString, LoxFunction*>();
}

void LoxInstance::set(Token name, Object* value) {
    fields->insert(name.lexeme, value);
}

void LoxInstance::setMethod(Token name, LoxFunction* method) {
    methods->insert(name.lexeme, method);
}

Object* LoxInstance::get(Token name) {
    if (fields->contains(name.lexeme)) {
        return fields->value(name.lexeme);
    }

    LoxFunction* method = klass->findMethod(this, name.lexeme);
    if (method != NULL)
        return new Object(method);

    if (methods->contains(name.lexeme)) {
        return new Object(methods->value(name.lexeme)->bind(this));
    }

    //throw new RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}

QString LoxInstance::toString() {
    return klass->name + " instance";
}
