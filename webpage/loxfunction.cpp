#include "loxfunction.h"
#include "interpreter.h"

LoxFunction::LoxFunction(Stmt::Function *declaration, Environment* closure, bool isInitializer) : declaration(declaration), closure(closure), isInitializer(isInitializer)
{

}

LoxFunction* LoxFunction::bind(LoxInstance* instance) {
    Environment* environment = new Environment(closure);
    environment->define("this", new Object(instance));
    //Stmt::Function* ptrDeclaration = declaration;
    return new LoxFunction(declaration, environment, isInitializer);
}

Object* LoxFunction::call(Interpreter* interpreter, QList<Object*> arguments) {
    Environment* environment = new Environment(closure);
    QList<Token> params(declaration->parameters);
    for (int i = 0; i < params.size(); i++) {
        environment->define(params.at(i).lexeme, arguments.at(i));
    }

    try {
        interpreter->executeBlock(declaration->body, environment);
    } catch (Object* returnValue) {
        return returnValue;
    }
    if (isInitializer)
        return closure->getAt(0, "this");

    return NULL;
}

int LoxFunction::arity() {
    return declaration->parameters.size();
}

QString LoxFunction::toString() {
    return "<fn " + declaration->name.lexeme + ">";
}
