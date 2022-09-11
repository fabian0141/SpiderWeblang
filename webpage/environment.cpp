#include "environment.h"

Environment::Environment(Environment* enclosing) : enclosing(enclosing){

}

void Environment::define(QString name, Object* value) {
    values.insert(name, value);
}

Environment* Environment::ancestor(int distance) {
    Environment* environment = this;
    for (int i = 0; i < distance; i++) {
        environment = environment->enclosing;
    }

    return environment;
}

Object* Environment::getAt(int distance, QString name) {
    return ancestor(distance)->values.value(name);
}

void Environment::assignAt(int distance, Token* name, Object* value) {
    ancestor(distance)->values[name->lexeme] = value;
}

Object* Environment::get(Token name) {
    if (values.contains(name.lexeme)) {
        return values.value(name.lexeme);
    }

    if (enclosing != NULL)
        return enclosing->get(name);

    //throw new RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

void Environment::assign(Token* name, Object* value) {
    if (values.contains(name->lexeme)) {
        values[name->lexeme] = value;
        return;
    }

    if (enclosing != NULL) {
        enclosing->assign(name, value);
        return;
    }

    //throw new RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

