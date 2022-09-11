#ifndef TOKEN_H
#define TOKEN_H

#include<QString>
#include "tokentype.h"
#include "object.h"

class Token
{
public:
    Token(TokenType type, QString lexeme, int line, Object* literal = NULL);
    QString toString();

    TokenType type;
    QString lexeme;
    Object* literal;
    int line;

};

#endif // TOKEN_H
