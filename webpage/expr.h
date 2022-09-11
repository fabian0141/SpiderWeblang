#ifndef EXPR_H
#define EXPR_H

#include "token.h"
#include <QList>
#include <QMap>

typedef Object* R;



class Expr {

public:
    class Aray;class Assign;class Binary;class Call;class Get;class GetArray;class GetStyle;class Grouping;class Literal;
    class Logical;class Set;class SetArray;class Super;class This;class Unary;class Variable;

    class Visitor {
    public:
        virtual R visitArrayExpr(Aray* expr){}
        virtual R visitAssignExpr(Assign* expr){}
        virtual R visitBinaryExpr(Binary* expr){}
        virtual R visitCallExpr(Call* expr){}
        virtual R visitGetExpr(Get* expr){}
        virtual R visitGetArrayExpr(GetArray* expr){}
        virtual R visitGetStyleExpr(GetStyle* expr){}
        virtual R visitGroupingExpr(Grouping* expr){}
        virtual R visitLiteralExpr(Literal* expr){}
        virtual R visitLogicalExpr(Logical* expr){}
        virtual R visitSetExpr(Set* expr){}
        virtual R visitSetArrayExpr(SetArray* expr){}
        virtual R visitSuperExpr(Super* expr){}
        virtual R visitThisExpr(This* expr){}
        virtual R visitUnaryExpr(Unary* expr){}
        virtual R visitVariableExpr(Variable* expr){}
    };

    virtual R accept(Visitor* visitor){}


};

inline bool operator<(const Expr &e1, const Expr &e2)
{
    return e1 < e2;
}

class Expr::Aray : public Expr {
public:
    Aray(QHash<Expr*,Expr*>* values) : values(values) {

    }

    R accept(Visitor* visitor) {
        return visitor->visitArrayExpr(this);
    }

    QHash<Expr*,Expr*>* values;
};

class Expr::Assign : public Expr {
public:
    Assign(Token name, Expr* value) : name(name), value(value) {

    }

    R accept(Visitor* visitor) {
        return visitor->visitAssignExpr(this);
    }

    Token name;
    Expr* value;
};

class Expr::Binary : public Expr {
public:
    Binary(Expr* left, Token bOperator, Expr* right) : left(left), bOperator(bOperator), right(right) {

    }

    R accept(Visitor* visitor) {
        return visitor->visitBinaryExpr(this);
    }

    Expr* left;
    Token bOperator;
    Expr* right;
};
class Expr::Call : public Expr {
public:
    Call(Expr* callee, Token paren, QList<Expr*> arguments) : callee(callee), paren(paren), arguments(arguments){

    }

    R accept(Visitor* visitor) {
        return visitor->visitCallExpr(this);
    }

    Expr* callee;
    Token paren;
    QList<Expr*> arguments;
};
class Expr::Get : public Expr {
public:
    Get(Expr* object, Token name) : object(object), name(name){

    }

    R accept(Visitor* visitor) {
        return visitor->visitGetExpr(this);
    }

    Expr* object;
    Token name;
};

class Expr::GetArray : public Expr {
public:
    GetArray(Expr* object, Expr* id) : object(object), id(id){

    }

    R accept(Visitor* visitor) {
        return visitor->visitGetArrayExpr(this);
    }

    Expr* object;
    Expr* id;
};

class Expr::GetStyle : public Expr {
public:
    GetStyle(Expr* object, QList<Token>* names) : object(object), names(names){

    }

    R accept(Visitor* visitor) {
        return visitor->visitGetStyleExpr(this);
    }

    Expr* object;
    QList<Token>* names;
};

class Expr::Grouping : public Expr {
public:
    Grouping(Expr* expression) : expression(expression){

    }

    R accept(Visitor* visitor) {
        return visitor->visitGroupingExpr(this);
    }

    Expr* expression;
};
class Expr::Literal : public Expr {
public:
    Literal(Object* value) : value(value){

    }

    R accept(Visitor* visitor) {
        return visitor->visitLiteralExpr(this);
    }

    Object* value;
};
class Expr::Logical : public Expr {
public:
    Logical(Expr* left, Token lOperator, Expr* right) : left(left), lOperator(lOperator), right(right){

    }

    R accept(Visitor* visitor) {
        return visitor->visitLogicalExpr(this);
    }

    Expr* left;
    Token lOperator;
    Expr* right;
};
class Expr::Set : public Expr {
public:
    Set(Expr* object, Token name, Expr* value) : object(object), name(name), value(value){

    }

    R accept(Visitor* visitor) {
        return visitor->visitSetExpr(this);
    }

    Expr* object;
    Token name;
    Expr* value;
};

class Expr::SetArray : public Expr {
public:
    SetArray(Expr* object, Token name, Expr* value) : object(object), name(name), value(value){

    }

    R accept(Visitor* visitor) {
        return visitor->visitSetArrayExpr(this);
    }

    Expr* object;
    Token name;
    Expr* value;
};

class Expr::Super : public Expr {
public:
    Super(Token keyword, Token method) : keyword(keyword), method(method) {

    }

    R accept(Visitor* visitor) {
        return visitor->visitSuperExpr(this);
    }

    Token keyword;
    Token method;
};
class Expr::This : public Expr {
public:
    This(Token keyword) : keyword(keyword){

    }

    R accept(Visitor* visitor) {
        return visitor->visitThisExpr(this);
    }

    Token keyword;
};
class Expr::Unary : public Expr {
public:
    Unary(Token uOperator, Token* name, Expr* right) : uOperator(uOperator), name(name), right(right){

    }

    R accept(Visitor* visitor) {
        return visitor->visitUnaryExpr(this);
    }

    Token uOperator;
    Token* name;
    Expr* right;
};
class Expr::Variable : public Expr {
public:
    Variable(Token name) : name(name){

    }

    R accept(Visitor* visitor) {
        return visitor->visitVariableExpr(this);
    }

    Token name;
};


#endif // EXPR_H
