#ifndef RESOLVER_H
#define RESOLVER_H

#include "token.h"
#include "stmt.h"
#include "expr.h"
#include "interpreter.h"
#include<QStack>

class Resolver : Expr::Visitor, Stmt::Visitor
{
public:
    Resolver(Interpreter* interpreter);
    Object* visitBlockStmt(Stmt::Block* stmt);
    Object* visitClassStmt(Stmt::Class* stmt);
    Object* visitExpressionStmt(Stmt::Expression* stmt);
    Object* visitFunctionStmt(Stmt::Function* stmt);
    Object* visitIfStmt(Stmt::If* stmt);
    Object* visitPrintStmt(Stmt::Print* stmt);
    Object* visitReturnStmt(Stmt::Return* stmt);
    Object* visitStyleStmt(Stmt::Style* stmt);
    Object* visitVarStmt(Stmt::Var* stmt);
    Object* visitWhileStmt(Stmt::While* stmt);
    Object* visitArrayExpr(Expr::Aray* expr);
    Object* visitAssignExpr(Expr::Assign* expr);
    Object* visitBinaryExpr(Expr::Binary* expr);
    Object* visitCallExpr(Expr::Call* expr);
    Object* visitGetExpr(Expr::Get* expr);
    Object *visitGetArrayExpr(Expr::GetArray *expr);
    Object *visitGetStyleExpr(Expr::GetStyle *expr);
    Object* visitGroupingExpr(Expr::Grouping* expr);
    Object* visitLiteralExpr(Expr::Literal* expr);
    Object* visitVariableExpr(Expr::Variable* expr);
    Object* visitLogicalExpr(Expr::Logical* expr);
    Object* visitSetExpr(Expr::Set* expr);
    Object* visitSetArrayExpr(Expr::SetArray* expr);
    Object* visitSuperExpr(Expr::Super* expr);
    Object* visitThisExpr(Expr::This* expr);
    Object* visitUnaryExpr(Expr::Unary* expr);
    void resolve(QList<Stmt*>* statements);
private:
    Interpreter* interpreter;
    QStack<QMap<QString, bool>> scopes;


    enum FunctionType {
        NONE, FUNCTION, INITIALIZER, METHOD
    };

    FunctionType currentFunction = FunctionType::NONE;

    enum ClassType {
        NONEC, CLASS, SUBCLASS
    };

    ClassType currentClass = ClassType::NONEC;

    void resolve(Stmt* stmt);
    void resolve(Expr* expr);
    void resolveFunction(Stmt::Function* function, FunctionType type);
    void beginScope();
    void endScope();
    void declare(Token name);
    void define(Token name);
    void resolveLocal(Expr* expr, Token* name);
    void resolveVariable(Stmt::Var *variable);
};

#endif // RESOLVER_H
