#include "resolver.h"
#include<QDebug>
Resolver::Resolver(Interpreter* interpreter) : interpreter(interpreter){

}

Object* Resolver::visitBlockStmt(Stmt::Block* stmt) {
    beginScope();
    resolve(stmt->statements);
    endScope();
}

Object* Resolver::visitClassStmt(Stmt::Class* stmt) {
    declare(stmt->name);
    define(stmt->name);

    ClassType enclosingClass = currentClass;
    currentClass = ClassType::CLASS;

    if (stmt->superclass != NULL) {
        currentClass = ClassType::SUBCLASS;
        resolve(stmt->superclass);

        beginScope();
        scopes.top().insert("super", true);
    }

    beginScope();
    scopes.top().insert("this", true);

    for (Stmt::Function * method : stmt->methods) {
        FunctionType declaration = FunctionType::METHOD;

        if (method->name.lexeme.compare("init")) {
            declaration = FunctionType::INITIALIZER;
        }

        resolveFunction(method, declaration);
    }

    endScope();
    if (stmt->superclass != NULL)
        endScope();
    currentClass = enclosingClass;
}

Object* Resolver::visitExpressionStmt(Stmt::Expression* stmt) {
    resolve(stmt->expression);

}

Object* Resolver::visitFunctionStmt(Stmt::Function* stmt) {
    declare(stmt->name);
    define(stmt->name);

    resolveFunction(stmt, FunctionType::FUNCTION);
}

Object* Resolver::visitIfStmt(Stmt::If* stmt) {
    resolve(stmt->condition);
    resolve(stmt->thenBranch);
    if (stmt->elseBranch != NULL)
        resolve(stmt->elseBranch);
}

Object* Resolver::visitPrintStmt(Stmt::Print* stmt) {
    resolve(stmt->expression);
}

Object* Resolver::visitReturnStmt(Stmt::Return* stmt) {
    if (currentFunction == FunctionType::NONE) {
        //Start.error(stmt.keyword, "Cannot return from top-level code.");
    }

    if (stmt->value != NULL) {
        resolve(stmt->value);
    }

    if (currentFunction == FunctionType::INITIALIZER) {
        //Start.error(stmt.keyword, "Cannot return a value from an initializer.");
    }
}

Object *Resolver::visitStyleStmt(Stmt::Style *stmt)
{
    ClassType enclosingClass = currentClass;
    currentClass = ClassType::CLASS;

    beginScope();
    scopes.top().insert("this", true);

    for (Stmt::Function * method : stmt->style->stObj->methods) {
        FunctionType declaration = FunctionType::METHOD;

        if (method->name.lexeme.compare("init")) {
            declaration = FunctionType::INITIALIZER;
        }

        resolveFunction(method, declaration);
    }

    endScope();

    currentClass = enclosingClass;
}

Object* Resolver::visitVarStmt(Stmt::Var* stmt) {
    declare(stmt->name);
    if (stmt->initializer != NULL) {
        resolve(stmt->initializer);
    }
    define(stmt->name);
}

Object* Resolver::visitWhileStmt(Stmt::While* stmt) {
    resolve(stmt->condition);
    resolve(stmt->body);
}

Object *Resolver::visitArrayExpr(Expr::Aray *expr)
{
    return NULL;
}

Object* Resolver::visitAssignExpr(Expr::Assign* expr) {
    resolve(expr->value);
    resolveLocal(expr, &expr->name);
}

Object* Resolver::visitBinaryExpr(Expr::Binary* expr) {
    resolve(expr->left);
    resolve(expr->right);
}

Object* Resolver::visitCallExpr(Expr::Call* expr) {
    resolve(expr->callee);

    for (Expr* argument : expr->arguments) {
        resolve(argument);
    }
}

Object* Resolver::visitGetExpr(Expr::Get* expr) {
    resolve(expr->object);
}

Object* Resolver::visitGetArrayExpr(Expr::GetArray* expr) {
    resolve(expr->object);
}

Object *Resolver::visitGetStyleExpr(Expr::GetStyle *expr)
{
    resolve(expr->object);
}

Object* Resolver::visitGroupingExpr(Expr::Grouping* expr) {
    resolve(expr->expression);
}

Object* Resolver::visitLiteralExpr(Expr::Literal* expr) {
    return NULL;
}

Object* Resolver::visitVariableExpr(Expr::Variable* expr) {
    if (!scopes.isEmpty() && !scopes.top().isEmpty()) {
        if(scopes.top().contains(expr->name.lexeme)){
            if(scopes.top().value(expr->name.lexeme) == false){
                //Start.error(expr.name, "Cannot read local variable in its own initializer.");
                qDebug() << "Cannot read local variable in its own initializer.";
                return NULL;
            }
        }
    }

    resolveLocal(expr, &expr->name);
}

Object* Resolver::visitLogicalExpr(Expr::Logical* expr) {
    resolve(expr->left);
    resolve(expr->right);
}

Object* Resolver::visitSetExpr(Expr::Set* expr) {
    resolve(expr->value);
    resolve(expr->object);
}

Object *Resolver::visitSetArrayExpr(Expr::SetArray *expr)
{
    resolve(expr->value);
    resolveLocal(expr, &expr->name);

}

Object* Resolver::visitSuperExpr(Expr::Super* expr) {
    if (currentClass == ClassType::NONEC) {
        //Start.error(expr.keyword, "Cannot use 'super' outside of a class.");
    } else if (currentClass != ClassType::SUBCLASS) {
        //Start.error(expr.keyword, "Cannot use 'super' in a class with no superclass.");
    }
    resolveLocal(expr, &expr->keyword);
}

Object* Resolver::visitThisExpr(Expr::This* expr) {
    if (currentClass == ClassType::NONEC) {
        //Start.error(expr.keyword, "Cannot use 'this' outside of a class.");
        return NULL;
    }

    resolveLocal(expr, &expr->keyword);
}

Object* Resolver::visitUnaryExpr(Expr::Unary* expr) {
    resolve(expr->right);
    resolveLocal(expr, expr->name);
}

void Resolver::resolve(QList<Stmt*>* statements) {
    for (Stmt* statement : *statements) {
        resolve(statement);
    }
}

void Resolver::resolve(Stmt* stmt) {
    stmt->accept(this);
}

void Resolver::resolve(Expr* expr) {
    expr->accept(this);
}

void Resolver::resolveFunction(Stmt::Function* function, FunctionType type) {
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;
    beginScope();
    for (Token param : function->parameters) {
        declare(param);
        define(param);
    }
    resolve(function->body);
    endScope();

    currentFunction = enclosingFunction;
}

void Resolver::resolveVariable(Stmt::Var* variable) {
    declare(variable->name);
    if (variable->initializer != NULL) {
        resolve(variable->initializer);
    }
    define(variable->name);
}

void Resolver::beginScope() {
    scopes.push(QMap<QString, bool>());
}

void Resolver::endScope() {
    scopes.pop();
}

void Resolver::declare(Token name) {
    if (scopes.isEmpty())
        return;

    QMap<QString, bool>* scope = &scopes.top();
    if (scope->contains(name.lexeme)) {
        //Start.error(name, "Variable with this name already declared in this scope.");
        qDebug() << "Variable with this name already declared in this scope.";
    }
    scope->insert(name.lexeme, false);
}

void Resolver::define(Token name) {
    if (scopes.isEmpty())
        return;
    scopes.top().insert(name.lexeme, true);
}

void Resolver::resolveLocal(Expr *expr, Token* name) {
    for (int i = scopes.size() - 1; i >= 0; i--) {
        if (scopes.at(i).contains(name->lexeme)) {
            interpreter->resolve(expr, scopes.size() - 1 - i);
            return;
        }
    }

    // Not found. Assume it is global.
}
