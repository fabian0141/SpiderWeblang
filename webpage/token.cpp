#include "token.h"
#include<QVariant>

Token::Token(TokenType type, QString lexeme, int line, Object* literal): type(type),lexeme(lexeme), line(line), literal(literal)
{
}

QString Token::toString() {
    char* strLiteral = (char*)literal;
    return type + " " + lexeme + " " + strLiteral;
}
