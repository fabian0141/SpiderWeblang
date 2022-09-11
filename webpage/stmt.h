#ifndef STMT_H
#define STMT_H

#include "expr.h"
#include "token.h"
#include <QList>

typedef Object* R;

class Stmt {
public:
    class Block;class Class;class Expression;class Function;class If;
    class Print;class Return;class Style;class Var;class While;

    class Visitor {
    public:
        virtual R visitBlockStmt(Block* stmt){}
        virtual R visitClassStmt(Class* stmt){}
        virtual R visitExpressionStmt(Expression* stmt){}
        virtual R visitFunctionStmt(Function* stmt){}
        virtual R visitIfStmt(If* stmt){}
        virtual R visitPrintStmt(Print* stmt){}
        virtual R visitReturnStmt(Return* stmt){}
        virtual R visitStyleStmt(Style* stmt){}
        virtual R visitVarStmt(Var* stmt){}
        virtual R visitWhileStmt(While* stmt){}
    };

    virtual  R accept(Visitor* visitor){}
};

class Stmt::Block : public Stmt {
public:
    Block(QList<Stmt*>* statements) : statements(statements) {

    }

    R accept(Visitor* visitor) {
        return visitor->visitBlockStmt(this);
    }

    QList<Stmt*>* statements;
};
class Stmt::Class : public Stmt {
public:
    Class(Token name, Expr* superclass, QList<Stmt::Function*> methods) : name(name), superclass(superclass), methods(methods){

    }

    R accept(Visitor* visitor) {
        return visitor->visitClassStmt(this);
    }

    Token name;
    Expr* superclass;
    QList<Stmt::Function*> methods;
};
class Stmt::Expression : public Stmt {
public:
    Expression(Expr*expression) : expression(expression){

    }

    R accept(Visitor* visitor) {
        return visitor->visitExpressionStmt(this);
    }

    Expr*expression;
};
class Stmt::Function : public Stmt {
public:
    Function(Token name, QList<Token> parameters, QList<Stmt*>* body) : name(name), parameters(parameters), body(body){

    }

    R accept(Visitor* visitor) {
        return visitor->visitFunctionStmt(this);
    }

    Token name;
    QList<Token> parameters;
    QList<Stmt*>* body;
};

class Stmt::If : public Stmt {
public:
    If(Expr* condition, Stmt* thenBranch, Stmt* elseBranch) : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch){

    }

    R accept(Visitor* visitor) {
        return visitor->visitIfStmt(this);
    }

    Expr* condition;
    Stmt* thenBranch;
    Stmt* elseBranch;
};
class Stmt::Print : public Stmt {
public:
    Print(Expr*expression) : expression(expression){

    }

    R accept(Visitor* visitor) {
        return visitor->visitPrintStmt(this);
    }

    Expr*expression;
};
class Stmt::Return : public Stmt {
public:
    Return(Token keyword, Expr*value) : keyword(keyword), value(value) {

    }

    R accept(Visitor* visitor) {
        return visitor->visitReturnStmt(this);
    }

    Token keyword;
    Expr*value;
};

class Stmt::Style : public Stmt {
public:
    Style(Token name, Object* style) : name(name), style(style){

    }

    R accept(Visitor* visitor) {
        return visitor->visitStyleStmt(this);
    }

    Token name;
    Object* style;
};

class Stmt::Var : public Stmt {
public:
    Var(Token name, Expr*initializer) : name(name), initializer(initializer){

    }

    R accept(Visitor* visitor) {
        return visitor->visitVarStmt(this);
    }

    Token name;
    Expr*initializer;
};
class Stmt::While : public Stmt {
public:
    While(Expr*condition, Stmt* body) : condition(condition), body(body){

    }

    R accept(Visitor* visitor) {
        return visitor->visitWhileStmt(this);
    }

    Expr*condition;
    Stmt* body;
};




#endif // STMT_H
