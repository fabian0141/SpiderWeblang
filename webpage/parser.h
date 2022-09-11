#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "stmt.h"
#include "expr.h"

class Parser
{
public:
    Parser(QList<Token> tokens);
    QList<Stmt*> parse();
private:
    QList<Token> tokens;
    int current = 0;
    Expr* expression();
    Stmt* declaration();
    Stmt* classDeclaration();
    Stmt::Function* function(QString kind);
    Stmt::Style* style();
    Stmt::Var* varDeclaration();
    Stmt* whileStatement();
    Stmt* statement();
    Stmt* returnStatement();
    Stmt* forStatement();
    Stmt* ifStatement();
    Stmt* printStatement();
    Stmt* expressionStatement();
    QList<Stmt*>* block();
    Expr* assignment();
    Expr* orManipulation();
    Expr* andManipulation();
    Expr* equality();
    bool match(TokenType types[], int i);
    bool check(TokenType tokenType);
    Token advance();
    bool isAtEnd();
    Token peek();
    Expr* comparison();
    Expr* addition();
    Expr* multiplication();
    Expr* unary();
    Expr* finishCall(Expr* callee);
    Expr* finishArray();
    Expr* getArray(Expr*);
    Expr* getStyles(Expr*);
    Expr* call();
    Expr* primary();
    Token consume(TokenType type, QString message);
    void synchronize();
    Token previous();
    void error(Token token, QString message);
};

#endif // PARSER_H
