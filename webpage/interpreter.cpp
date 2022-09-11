#include "interpreter.h"
#include "QTime"
#include <QDebug>
#include "loxclass.h"
#include "drawwidgetclass.h"
#include "array.h"

Interpreter::Interpreter(QMap<QString,Widget*>* widgets) : widgets(widgets) {
    class ClockClass : public LoxCallable {
    public:
        int arity() {
            return 0;
        }

        Object* call(Interpreter* interpreter, QList<Object*> arguments) {
            return new Object(QTime::currentTime().toString());
        }
    };
    globals->define("clock", new Object(new ClockClass));

    globals->define("drawWidget", new Object( new DrawWidgetClass(environment)));
}

void Interpreter::interpret(QList<Stmt*> statements) {
    //try {
    for (Stmt* statement : statements) {
        execute(statement);
    }
    //} catch (RuntimeError error) {
    //Start.runtimeError(error);
    //}
}

Object* Interpreter::visitPrintStmt(Stmt::Print* stmt) {
    Object* value = evaluate(stmt->expression);
    qDebug() << stringify(value);
}

Object* Interpreter::visitReturnStmt(Stmt::Return* stmt) {
    Object* value = NULL;
    if (stmt->value != NULL)
        value = evaluate(stmt->value);


    throw value;
}

Object *Interpreter::visitStyleStmt(Stmt::Style *stmt)
{
    environment->define(stmt->name.lexeme, stmt->style);
}

Object* Interpreter::visitVarStmt(Stmt::Var* stmt) {
    Object* value = NULL;
    if (stmt->initializer != NULL) {
        value = evaluate(stmt->initializer);
    }
    environment->define(stmt->name.lexeme, value);
}

Object* Interpreter::visitWhileStmt(Stmt::While* stmt) {
    while (isTruthy(evaluate(stmt->condition))) {
        execute(stmt->body);
    }
}

Object *Interpreter::visitArrayExpr(Expr::Aray *expr)
{
    QHash<Object, Expr*>* values = new QHash<Object,Expr*>();

    for(Expr* exp : expr->values->keys()){
        values->insert(*evaluate(exp),expr->values->value(exp));
    }
    return new Object(new Array(values));
}

Object* Interpreter::visitAssignExpr(Expr::Assign* expr) {
    Object* value = evaluate(expr->value);

    int distance = locals.value(expr);
    if (distance != NULL) {
        environment->assignAt(distance, &expr->name, value);
    } else {
        globals->assign(&expr->name, value);
    }
}

Object* Interpreter::visitExpressionStmt(Stmt::Expression* stmt) {
    evaluate(stmt->expression);
}

Object* Interpreter::visitFunctionStmt(Stmt::Function* stmt) {
    LoxFunction* function = new LoxFunction(stmt, environment, false);
    environment->define(stmt->name.lexeme, new Object(function));
}

Object* Interpreter::visitIfStmt(Stmt::If* stmt) {
    if (isTruthy(evaluate(stmt->condition))) {
        execute(stmt->thenBranch);
    } else if (stmt->elseBranch != NULL) {
        execute(stmt->elseBranch);
    }
}

void Interpreter::interpret(Expr* expression) {
    //try {
    Object* value = evaluate(expression);
    qDebug() << stringify(value);
    /*} catch (RuntimeError error) {
        Start.runtimeError(error);
    }*/
}

Object* Interpreter::visitLiteralExpr(Expr::Literal* expr) {
    return expr->value;
}

Object* Interpreter::visitLogicalExpr(Expr::Logical* expr) {
    Object* left = evaluate(expr->left);

    if (expr->lOperator.type == OR) {
        if (isTruthy(left))
            return left;
    } else {
        if (!isTruthy(left))
            return left;
    }

    return evaluate(expr->right);
}

Object* Interpreter::visitSetExpr(Expr::Set* expr) {
    Object* object = evaluate(expr->object);

    /*if (!(object instanceof LoxInstance)) {
        throw new RuntimeError(expr.name, "Only instances have fields.");
    }*/



    Object* value = evaluate(expr->value);

    if(object->lt == Object::STYLE){
        return updateStyle(object->stObj,value, expr->name);
    }

    object->liObj->set(expr->name, value);

    return value;
}

Object *Interpreter::updateStyle(Style* style, Object* value, Token name)
{
    for (Object* obj : style->objs) {
        obj->liObj->set(name, value);
    }
    return value;
}

Object *Interpreter::visitSetArrayExpr(Expr::SetArray *expr)
{
    Object* object = evaluate(expr->object);
    Object* value = evaluate(expr->value);

    *object = *value;
}

Object* Interpreter::visitSuperExpr(Expr::Super* expr) {
    int distance = locals.value(expr);
    LoxClass* superclass = (LoxClass*) environment->getAt(distance, "super")->lcObj;
    LoxInstance* object = environment->getAt(distance - 1, "this")->liObj;
    LoxFunction* method = superclass->findMethod(object, expr->method.lexeme);

    if (method == NULL) {
        //throw new RuntimeError(expr.method, "Undefined property '" + expr.method.lexeme + "'.");
    }

    return new Object(method);
}

Object* Interpreter::visitThisExpr(Expr::This* expr) {
    return lookUpVariable(expr->keyword, expr);
}

Object* Interpreter::visitBinaryExpr(Expr::Binary* expr) {
    Object* left = evaluate(expr->left);
    Object* right = evaluate(expr->right);

    switch (expr->bOperator.type) {
    case GREATER:
        checkNumberOperands(expr->bOperator, left, right);
        return new Object(*left > *right);
    case GREATER_EQUAL:
        checkNumberOperands(expr->bOperator, left, right);
        return new Object(*left >= *right);
    case LESS:
        checkNumberOperands(expr->bOperator, left, right);
        return new Object(*left < *right);
    case LESS_EQUAL:
        checkNumberOperands(expr->bOperator, left, right);
        return  new Object(*left <= *right);
    case BANG_EQUAL:
        return new Object(*left != *right);
    case EQUAL_EQUAL:
        return new Object(*left == *right);
    case MINUS:
        checkNumberOperands(expr->bOperator, left, right);
        return new Object(*left - *right);
    case PLUS:
        if (&left != NULL && &right != NULL) {
            return *left + *right;
        }

        //throw new RuntimeError(expr.operator, "Operands must be two numbers or two strings.");
    case SLASH:
        checkNumberOperands(expr->bOperator, left, right);
        return new Object(*left / *right);
    case STAR:
        checkNumberOperands(expr->bOperator, left, right);
        return new Object(*left * *right);
    }

    // Unreachable.
    return NULL;
}

Object* Interpreter::visitCallExpr(Expr::Call* expr) {
    Object* callee = evaluate(expr->callee);

    QList<Object*> arguments;
    for (Expr* argument : expr->arguments) {
        arguments.append(evaluate(argument));
    }

    if (!(typeid((LoxCallable)*(LoxCallable*)callee) == typeid(LoxCallable))) {
        //throw new RuntimeError(expr.paren, "Can only call functions and classes.");
        qDebug() << "Can only call functions and classes.";
    }

    LoxCallable* function = callee->lcObj;

    if (arguments.size() != function->arity()) {
        //throw new RuntimeError(expr.paren, "Expected " + function.arity() + " arguments but got " + arguments.size() + ".");
        qDebug() << "Expected " + QString::number(function->arity()) + " arguments but got " + QString::number(arguments.size()) + ".";
    }

    return function->call(this,arguments);
}

Object* Interpreter::visitGetExpr(Expr::Get* expr) {
    Object* object = evaluate(expr->object);
    if (object != NULL) {
        return object->liObj->get(expr->name);
    }

    //throw new RuntimeError(expr.name, "Only instances have properties.");
}

Object *Interpreter::visitGetArrayExpr(Expr::GetArray *expr)
{
    Object* object = evaluate(expr->object);

    return evaluate(object->arObj->values->value(*evaluate(expr->id)));
}

Object *Interpreter::visitGetStyleExpr(Expr::GetStyle *expr)
{
    Object* object = evaluate(expr->object);
    Style* style;
    Object* value;

    for (Token t : *expr->names) {
        style = lookUpVariable(t,NULL)->stObj;
        style->addToStyleGroup(object);

        for (int i = 0; i < style->names->size(); ++i) {
            value = evaluate(style->values->at(i));
            object->liObj->set(style->names->at(i), value);
        }
        for (Stmt::Function* method : style->methods) {
            LoxFunction* function = new LoxFunction(method, environment, method->name.lexeme.compare("init"));
            object->liObj->setMethod(method->name, function);
        }
    }

    return object;
}

Object* Interpreter::visitGroupingExpr(Expr::Grouping* expr) {
    // TODO Auto-generated method stub
    return evaluate(expr->expression);
}

Object* Interpreter::visitUnaryExpr(Expr::Unary* expr) {
    Object* right = evaluate(expr->right);

    switch (expr->uOperator.type) {
    case BANG:
        return new Object(!isTruthy(right));
    case MINUS:
        checkNumberOperand(expr->uOperator, right);
        return new Object(-right->dObj);
    case INCREMENT:
    {
        checkNumberOperand(expr->uOperator, right);
        int distance = locals.value(expr);
        if (&distance != NULL) {
            environment->assignAt(distance, expr->name, new Object(right + 1));
        } else {
            globals->assign(expr->name, new Object(right + 1));
        }
        return right;
    }
    case DECREMENT:
        checkNumberOperand(expr->uOperator, right);
        int distanceDe = locals.value(expr);
        if (distanceDe != NULL) {
            environment->assignAt(distanceDe, expr->name, new Object(right - 1));
        } else {
            globals->assign(expr->name, new Object(right - 1));
        }
        return right;
    }

    // Unreachable.
    return NULL;
}

Object* Interpreter::visitVariableExpr(Expr::Variable* expr) {
    return lookUpVariable(expr->name, expr);
}

Object* Interpreter::lookUpVariable(Token name, Expr* expr) {
    if (locals.contains(expr)) {
        int distance = locals.value(expr);
        return environment->getAt(distance, name.lexeme);
    } else {
        return globals->get(name);
    }
}

Object* Interpreter::evaluate(Expr* expr) {
    return expr->accept(this);
}

void Interpreter::execute(Stmt* stmt) {
    stmt->accept(this);
}

void Interpreter::resolve(Expr* expr, int depth) {
    locals.insert(expr, depth);
}

void Interpreter::executeBlock(QList<Stmt*>* statements, Environment* environment) {
    Environment* previous = this->environment;
    try {
        this->environment = environment;

        for (Stmt* statement : *statements) {
            execute(statement);
        }
    }catch(Object* value){
        this->environment = previous;
        throw value;
    }

    this->environment = previous;


}

Object* Interpreter::visitBlockStmt(Stmt::Block* stmt) {
    executeBlock(stmt->statements, new Environment(environment));
}

Object* Interpreter::visitClassStmt(Stmt::Class* stmt) {
    environment->define(stmt->name.lexeme, NULL);

    Object* superclass = NULL;
    if (stmt->superclass != NULL) {
        superclass = evaluate(stmt->superclass);
        /*if (!(superclass instanceof LoxClass)) {
            throw new RuntimeError(stmt.name, "Superclass must be a class.");
        }*/

        environment = new Environment(environment);
        environment->define("super", superclass);
    }

    QMap<QString, LoxFunction*>* methods = new QMap<QString,LoxFunction*>;
    for (Stmt::Function* method : stmt->methods) {
        LoxFunction* function = new LoxFunction(method, environment, method->name.lexeme.compare("init"));
        methods->insert(method->name.lexeme, function);
    }

    LoxClass* klass;
    if (superclass != NULL) {
        klass = new LoxClass(stmt->name.lexeme, (LoxClass*) superclass->lcObj, methods);
        environment = environment->enclosing;
    }else{
        klass = new LoxClass(stmt->name.lexeme, NULL, methods);
    }
    environment->assign(&stmt->name, new Object(klass));
}

bool Interpreter::isTruthy(Object* object) {
    if (object == NULL)
        return false;
    if (object != NULL)
        return object->bObj;
    return true;
}

void Interpreter::checkNumberOperand(Token tOperator, Object* operand) {
    if (&tOperator != NULL)
        return;
    //throw new RuntimeError(operator, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token tOperator, Object* left, Object* right) {
    if (left != NULL && right != NULL)
        return;

    //throw new RuntimeError(operator, "Operands must be numbers.");
    qDebug() << "Operands must be numbers.";
}

bool Interpreter::isEqual(Object* a, Object* b) {
    // nil is only equal to nil.
    if (a == NULL && b == NULL)
        return true;
    if (a == NULL)
        return false;

    return new QString(((QString) *(QString*)a).compare(((QString) *(QString*)a))); // einen typlosen pointer, der zu einem pointer mit typen gecastet wird, der zu einem object wird, mit einem typlosen pointer, der zu einem pointer mit typen gecastet wird, der zu einem object wird, vergleichen um einem typenlosen pointer zurückzugeben
}

QString Interpreter::stringify(Object* object) {
    if (object == NULL)
        return "nil";

    // Hack. Work around Java adding ".0" to integer-valued doubles.

    if(object->lt == Object::DOUBLE)
        return QString::number(object->dObj);

    return object->sObj;
}

