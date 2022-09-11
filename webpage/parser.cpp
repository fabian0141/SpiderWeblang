#include "parser.h"
#include "array.h"
#include "style.h"
#include <QDebug>

Parser::Parser(QList<Token> tokens) : tokens(tokens){

}

QList<Stmt*> Parser::parse() {
    QList<Stmt*> statements;
    while (!isAtEnd()) {
        statements.append(declaration());
    }

    return statements;
}

Expr *Parser::expression() {
    return assignment();
}

Stmt* Parser::declaration() {
    //try {
    if (match(new TokenType[1]{CLASS},1))
        return classDeclaration();
    if (match(new TokenType[1]{FUN},1))
        return function("function");
    if (match(new TokenType[1]{STYLE},1))
        return style();
    if (match(new TokenType[1]{VAR},1))
        return varDeclaration();

    return statement();
    /*} catch (ParseError error) {
        synchronize();
        return NULL;
    }*/
}

Stmt* Parser::classDeclaration() {
    Token name = consume(IDENTIFIER, "Expect class name.");

    Expr* superclass = NULL;
    if (match(new TokenType[1]{LESS},1)) {
        consume(IDENTIFIER, "Expect superclass name.");
        superclass = new Expr::Variable(previous());
    }

    consume(LEFT_BRACE, "Expect '{' before class body.");

    QList<Stmt::Function*> methods;
    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        methods.append(function("method"));
    }

    consume(RIGHT_BRACE, "Expect '}' after class body.");

    return new Stmt::Class(name, superclass, methods);
}

Stmt::Function* Parser::function(QString kind) {
    Token name = consume(IDENTIFIER, "Expect " + kind + " name.");
    consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
    QList<Token> parameters;
    if (!check(RIGHT_PAREN)) {
        do {
            if (parameters.size() >= 32) {
                //error(peek(), "Cannot have more than 8 parameters.");
            }

            parameters.append(consume(IDENTIFIER, "Expect parameter name."));
        } while (match(new TokenType[1]{COMMA},1));
    }
    consume(RIGHT_PAREN, "Expect ')' after parameters.");
    consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
    QList<Stmt*>* body = block();
    return new Stmt::Function(name, parameters, body);
}

Stmt::Style *Parser::style()
{
    Token name = consume(IDENTIFIER, "Expect variable name.");
    consume(LEFT_BRACE, "Expect '{' after style name.");

    QList<Token>* names = new QList<Token>();
    QList<Expr*>* values = new QList<Expr*>();
    QList<Stmt::Function*> methods;

    do {
        if(match(new TokenType[1]{FUN},1)){
            methods.append(function("method"));
        }else{
            names->append(consume(IDENTIFIER,"Expect identfier"));
            values->append(expression());
        }
    } while (!match(new TokenType[1]{RIGHT_BRACE},1));

    return new Stmt::Style(name, new Object(new Style(names,values,methods)));
}

Stmt::Var* Parser::varDeclaration() {
    Token name = consume(IDENTIFIER, "Expect variable name.");
    Expr* initializer = NULL;
    if (match(new TokenType[1]{EQUAL},1)) {
        initializer = expression();
    }
    consume(SEMICOLON, "Expect ';' after variable declaration.");

    return new Stmt::Var(name, initializer);
}

Stmt* Parser::whileStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'while'.");
    Expr* condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after condition.");
    Stmt* body = statement();

    return new Stmt::While(condition, body);
}

Stmt* Parser::statement() {
    if (match(new TokenType[1]{FOR},1))
        return forStatement();

    if (match(new TokenType[1]{IF},1))
        return ifStatement();

    if (match(new TokenType[1]{PRINT},1))
        return printStatement();

    if (match(new TokenType[1]{RETURN},1))
        return returnStatement();

    if (match(new TokenType[1]{WHILE},1))
        return whileStatement();

    if (match(new TokenType[1]{LEFT_BRACE},1))
        return new Stmt::Block(block());

    return expressionStatement();
}

Stmt* Parser::returnStatement() {
    Token keyword = previous();
    Expr* value = NULL;
    if (!check(SEMICOLON)) {
        value = expression();
    }

    consume(SEMICOLON, "Expect ';' after return value.");
    return new Stmt::Return(keyword, value);
}

Stmt* Parser::forStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'for'.");

    Stmt* initializer;
    if (match(new TokenType[1]{SEMICOLON},1)) {
        initializer = NULL;
    } else if (match(new TokenType[1]{VAR},1)) {
        initializer = varDeclaration();
    } else {
        initializer = expressionStatement();
    }
    /*Expr* d = ((Stmt::Var*)initializer)->initializer;
    void* e = ((Expr::Literal*)d)->value;
    qDebug() << (double)*((double*)e);*/
    Expr* condition = NULL;
    if (!check(SEMICOLON)) {
        condition = expression();
    }

    /*d = ((Expr::Binary*)condition)->right;
    e = ((Expr::Literal*)d)->value;
    qDebug() << (double)*((double*)e);*/

    consume(SEMICOLON, "Expect ';' after loop condition.");

    Expr* increment = NULL;
    if (!check(RIGHT_PAREN)) {
        increment = expression();
    }

    consume(RIGHT_PAREN, "Expect ')' after for clauses.");

    Stmt* body = statement();

    if (increment != NULL) {
        QList<Stmt*>* bList = new QList<Stmt*>();
        bList->append(body);
        bList->append(new Stmt::Expression(increment));
        body = new Stmt::Block(bList);
    }

    if (condition == NULL)
        condition = new Expr::Literal(new Object(true));
    body = new Stmt::While(condition, body);

    if (initializer != NULL) {
        QList<Stmt*>* bList = new QList<Stmt*>();
        bList->append(initializer);
        bList->append(body);
        body = new Stmt::Block(bList);
    }

    return body;
}

Stmt* Parser::ifStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'if'.");
    Expr* condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after if condition.");

    Stmt* thenBranch = statement();
    Stmt* elseBranch = NULL;
    if (match(new TokenType[1]{ELSE},1)) {
        elseBranch = statement();
    }

    return new Stmt::If(condition, thenBranch, elseBranch);
}

Stmt* Parser::printStatement() {
    Expr* value = expression();
    consume(SEMICOLON, "Expect ';' after value.");
    return new Stmt::Print(value);
}

Stmt* Parser::expressionStatement() {
    Expr* expr = expression();
    consume(SEMICOLON, "Expect ';' after expression.");
    return new Stmt::Expression(expr);
}

QList<Stmt*>* Parser::block() {
    QList<Stmt*>* statements = new QList<Stmt*>();

    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        statements->append(declaration());
    }

    consume(RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

Expr* Parser::assignment() {
    Expr* expr = orManipulation();

    if (match(new TokenType[1]{EQUAL},1)) {
        Token equals = previous();
        Expr* value = assignment();

        if (dynamic_cast<Expr::Variable*>(expr) != NULL) {
            Token name = ((Expr::Variable*) expr)->name;
            return new Expr::Assign(name, value);
        } else if (dynamic_cast<Expr::Get*>(expr) != NULL) {
            Expr::Get* get = (Expr::Get*) expr;
            return new Expr::Set(get->object, get->name, value);
        } else if (dynamic_cast<Expr::GetArray*>(expr) != NULL) {
            Expr::GetArray* exp = (Expr::GetArray*)expr;
            Expr::Variable* ex = (Expr::Variable*) exp->object;
            return new Expr::SetArray(exp,ex->name,value);
        }

        error(equals, "Invalid assignment target.");
    }

    return expr;
}

Expr* Parser::orManipulation() {
    Expr* expr = andManipulation();

    while (match(new TokenType[1]{OR},1)) {
        Token tOperator = previous();
        Expr* right = andManipulation();
        expr = new Expr::Logical(expr, tOperator, right);
    }

    return expr;
}

Expr* Parser::andManipulation() {
    Expr* expr = equality();

    while (match(new TokenType[1]{AND},1)) {
        Token tOperator = previous();
        Expr* right = equality();
        expr = new Expr::Logical(expr, tOperator, right);
    }

    return expr;
}

Expr* Parser::equality() {
    Expr* expr = comparison();

    while (match(new TokenType[2]{BANG_EQUAL, EQUAL_EQUAL},2)) {
        Token tOperator = previous();
        Expr* right = comparison();
        expr = new Expr::Binary(expr, tOperator, right);
    }

    return expr;
}

bool Parser::match(TokenType types[],int i) {
    for(; i > 0;i--) {     // ob das geeeeeeeeeeeeeeht????????
        if (check(types[i - 1])) {
            advance();
            return true;
        }
    }

    return false;
}

bool Parser::check(TokenType tokenType) {
    if (isAtEnd())
        return false;
    return peek().type == tokenType;
}

Token Parser::advance() {
    if (!isAtEnd())
        current++;
    return previous();
}

bool Parser::isAtEnd() {
    return peek().type == EOT;
}

Token Parser::peek() {
    return tokens.at(current);
}

Token Parser::previous() {
    return tokens.at(current - 1);
}

Expr* Parser::comparison() {
    Expr* expr = addition();

    while (match(new TokenType[4]{GREATER, GREATER_EQUAL, LESS, LESS_EQUAL},4)) {
        Token tOperator = previous();
        Expr* right = addition();
        expr = new Expr::Binary(expr, tOperator, right);
    }

    return expr;
}

Expr* Parser::addition() {
    Expr* expr = multiplication();

    while (match(new TokenType[2]{MINUS, PLUS},2)) {
        Token tOperator = previous();
        Expr* right = multiplication();
        expr = new Expr::Binary(expr, tOperator, right);
    }

    return expr;
}

Expr* Parser::multiplication() {
    Expr* expr = unary();

    while (match(new TokenType[2]{SLASH, STAR},2)) {
        Token tOperator = previous();
        Expr* right = unary();
        expr = new Expr::Binary(expr, tOperator, right);
    }

    return expr;
}

Expr* Parser::unary() {
    if (match(new TokenType[2]{BANG, MINUS},2)) {
        Token tOperator = previous();
        Expr* right = unary();
        return new Expr::Unary(tOperator,NULL, right);
    }
    Expr* expr = call();
    if (match(new TokenType[2]{INCREMENT, DECREMENT},2)) {
        Token tOperator = previous();
        Token* name = new Token(tokens.at(current - 2));
        return new Expr::Unary(tOperator,name, expr);
    }

    return expr;
}

Expr* Parser::finishCall(Expr* callee) {
    QList<Expr*> arguments;
    if (!check(RIGHT_PAREN)) {
        do {
            if (arguments.size() >= 32) {
                error(peek(), "Cannot have more than 32 arguments.");
            }

            arguments.append(expression());
        } while (match(new TokenType[1]{COMMA},1));
    }

    Token paren = consume(RIGHT_PAREN, "Expect ')' after arguments.");

    return new Expr::Call(callee, paren, arguments);
}

Expr *Parser::finishArray()
{
    QHash<Expr*,Expr*>* values = new QHash<Expr*,Expr*>();
    double i = 0;

    do {
        Expr* id;
        Expr* expr = expression();
        if(match(new TokenType[1]{ASSIGN},1)){
            id = expr;
            expr = expression();
        }else{
            id = new Expr::Literal(new Object(i++));
        }
        values->insert(id, expr);
    } while (match(new TokenType[1]{COMMA},1));

    consume(RIGHT_BRACKET, "Expect ']' after arguments.");

    return new Expr::Aray(values);
}

Expr* Parser::getArray(Expr* expr)
{
    return new Expr::GetArray(expr, expression());
}

Expr *Parser::getStyles(Expr * expr)
{
    QList<Token>* names = new QList<Token>();

    do {
        names->append(consume(IDENTIFIER,"Expect identfier"));
    } while (!match(new TokenType[1]{RIGHT_BRACE},1));

    new Expr::GetStyle(expr,names);
}

Expr* Parser::call() {
    Expr* expr = primary();

    while (true) {
        if (match(new TokenType[1]{LEFT_PAREN},1)) {
            expr = finishCall(expr);
            if (match(new TokenType[1]{LEFT_BRACE},1)) {
                expr = getStyles(expr);
            }
        } else if (match(new TokenType[1]{DOT},1)) {
            Token name = consume(IDENTIFIER, "Expect property name after '.'.");
            expr = new Expr::Get(expr, name);
        } else if (match(new TokenType[1]{LEFT_BRACKET},1)) {
            expr = getArray(expr);
            consume(RIGHT_BRACKET, "Expect ']' after arguments.");
        } else {
            break;
        }
    }

    return expr;
}

Expr* Parser::primary() {
    if (match(new TokenType[1]{FALSE},1))
        return new Expr::Literal(new Object(false));
    if (match(new TokenType[1]{TRUE},1))
        return new Expr::Literal(new Object(true));
    if (match(new TokenType[1]{NIL},1))
        return new Expr::Literal(NULL);
    if (match(new TokenType[2]{NUMBER, STRING},2))
        return new Expr::Literal(previous().literal);


    if (match(new TokenType[1]{SUPER},1)) {
        Token keyword = previous();
        consume(DOT, "Expect '.' after 'super'.");
        Token method = consume(IDENTIFIER, "Expect superclass method name.");
        return new Expr::Super(keyword, method);
    }

    if (match(new TokenType[1]{THIS},1))
        return new Expr::This(previous());

    if (match(new TokenType[1]{IDENTIFIER},1)) {
        return new Expr::Variable(previous());
    }

    if (match(new TokenType[1]{LEFT_BRACKET},1)) {
        return finishArray();
    }

    if (match(new TokenType[1]{LEFT_PAREN},1)) {
        Expr* expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return new Expr::Grouping(expr);
    }

    error(peek(), "Expect expression.");
}

Token Parser::consume(TokenType type, QString message) {
    if (check(type))
        return advance();

    error(peek(), message);
}

void Parser::error(Token token, QString message) {
    if (token.type == EOT) {
        qDebug() << QString::number(token.line) + " at end" + message;
    } else {
        qDebug() << QString::number(token.line) + " at '" + token.lexeme + "'" + message;
    }
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == SEMICOLON)
            return;

        switch (peek().type) {
        case CLASS:
        case FUN:
        case VAR:
        case FOR:
        case IF:
        case WHILE:
        case PRINT:
        case RETURN:
            return;
        }

        advance();
    }
}

//private static class ParseError extends RuntimeException {
//}
