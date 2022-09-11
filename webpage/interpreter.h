#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "environment.h"
#include "token.h"
#include "stmt.h"
#include "expr.h"
#include "loxcallable.h"
#include "loxfunction.h"
#include "loxinstance.h"
#include "style.h"

class Widget;

class Interpreter : public Expr::Visitor, Stmt::Visitor
{
public:
    Interpreter(QMap<QString,Widget*>*);
    Environment* globals = new Environment();
    QMap<QString,Widget*>* widgets;

    void interpret(QList<Stmt*> statements);
    void interpret(Expr* expression);
    Object* visitPrintStmt(Stmt::Print* stmt);
    Object* visitReturnStmt(Stmt::Return* stmt);
    Object* visitStyleStmt(Stmt::Style* stmt);
    Object* visitVarStmt(Stmt::Var* stmt);
    Object* visitWhileStmt(Stmt::While* stmt);
    Object* visitArrayExpr(Expr::Aray* expr);
    Object* visitAssignExpr(Expr::Assign* expr);
    Object* visitExpressionStmt(Stmt::Expression* stmt);
    Object* visitFunctionStmt(Stmt::Function* stmt);
    Object* visitIfStmt(Stmt::If* stmt);
    Object* visitLiteralExpr(Expr::Literal* expr);
    Object* visitLogicalExpr(Expr::Logical* expr);
    Object* visitSetExpr(Expr::Set* expr);
    Object* updateStyle(Style*,Object*,Token);
    Object* visitSetArrayExpr(Expr::SetArray* expr);
    Object* visitSuperExpr(Expr::Super* expr);
    Object* visitThisExpr(Expr::This* expr);
    Object* visitBinaryExpr(Expr::Binary* expr);
    Object* visitCallExpr(Expr::Call* expr);
    Object* visitGetExpr(Expr::Get* expr);
    Object* visitGetArrayExpr(Expr::GetArray* expr);
    Object* visitGetStyleExpr(Expr::GetStyle* expr);
    Object* visitGroupingExpr(Expr::Grouping* expr);
    Object* visitUnaryExpr(Expr::Unary* expr);
    Object* visitVariableExpr(Expr::Variable* expr);
    void resolve(Expr* expr, int depth);
    Object* visitBlockStmt(Stmt::Block* stmt);
    Object* visitClassStmt(Stmt::Class* stmt);
    void executeBlock(QList<Stmt*>* statements, Environment* environment);
    Environment* environment = globals;
    void execute(Stmt* stmt);
private:
    QMap<Expr*, int> locals;
    Object* lookUpVariable(Token name, Expr* expr);
    Object* evaluate(Expr* expr);

    bool isTruthy(Object* object);
    void checkNumberOperand(Token tOperator, Object* operand);
    void checkNumberOperands(Token tOperator, Object* left, Object* right);
    bool isEqual(Object* a, Object* b);
    QString stringify(Object*);
};

#endif // INTERPRETER_H
