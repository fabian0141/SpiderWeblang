#ifndef SCANNER_H
#define SCANNER_H

#include<QString>
#include<QList>
#include "token.h"

class Scanner
{
public:
    Scanner(QString);
    QList<Token> scanTokens();

private:
    QMap<QString, TokenType> keywords;
    QString source;
    QList<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;
    bool isAtEnd();
    void scanToken();
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, Object* literal);
    bool match(char expected);
    void string();
    char peek();

    bool isDigit(char c);
    void number();
    char peekNext();
    void identifier();
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
};

#endif // SCANNER_H
