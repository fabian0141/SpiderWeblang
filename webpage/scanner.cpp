#include "scanner.h"
#include "webpage.h"
#include <QDebug>

Scanner::Scanner(QString source) : source(source)
{
    keywords.insert("and",    AND);
    keywords.insert("class",  CLASS);
    keywords.insert("else",   ELSE);
    keywords.insert("false",  FALSE);
    keywords.insert("for",    FOR);
    keywords.insert("fun",    FUN);
    keywords.insert("if",     IF);
    keywords.insert("nil",    NIL);
    keywords.insert("or",     OR);
    keywords.insert("print",  PRINT);
    keywords.insert("return", RETURN);
    keywords.insert("style",  STYLE);
    keywords.insert("super",  SUPER);
    keywords.insert("this",   THIS);
    keywords.insert("true",   TRUE);
    keywords.insert("var",    VAR);
    keywords.insert("while",  WHILE);
}

QList<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        // We are at the beginning of the next lexeme.
        start = current;
        scanToken();
    }

    tokens.append(Token(EOT, "", line));
    return tokens;
}

bool Scanner::isAtEnd() {
    return current >= source.length();
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
    case '(': addToken(LEFT_PAREN); break;
    case ')': addToken(RIGHT_PAREN); break;
    case '[': addToken(LEFT_BRACKET); break;
    case ']': addToken(RIGHT_BRACKET); break;
    case '{': addToken(LEFT_BRACE); break;
    case '}': addToken(RIGHT_BRACE); break;
    case ',': addToken(COMMA); break;
    case '.': addToken(DOT); break;
    case '-': addToken(match('-') ? DECREMENT : MINUS);break;
    case '+': addToken(match('+') ? INCREMENT : PLUS);break;
    case ';': addToken(SEMICOLON); break;
    case '*': addToken(STAR); break;
    case '!': addToken(match('=') ? BANG_EQUAL : BANG); break;
    case '=': addToken(match('=') ? EQUAL_EQUAL : EQUAL); break;
    case '<': addToken(match('=') ? LESS_EQUAL : match('-') ? ASSIGN : LESS); break;
    case '>': addToken(match('=') ? GREATER_EQUAL : GREATER); break;
    case '/':
        if (match('/')) {
            // A comment goes until the end of the line.
            while (peek() != '\n' && !isAtEnd()) advance();
        } else {
            addToken(SLASH);
        }
        break;
    case ' ':
    case '\r':
    case '\t':
        // Ignore whitespace.
        break;

    case '\n':
        line++;
        break;
    case '"': string(); break;
    default:
        if (isDigit(c)) {
            number();
        } else if (isAlpha(c)) {
            identifier();
        } else {
            Webpage::error(line, "Unexpected character.");
        }
        break;
    }
}

void Scanner::identifier() {
    while (isAlphaNumeric(peek())) advance();

    QString text = source.mid(start, current - start);

    TokenType type = keywords.value(text);
    if (type == NULL) type = IDENTIFIER;
    addToken(type);
}

void Scanner::number() {
    while (isDigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the "."
        advance();

        while (isDigit(peek())) advance();
    }

    addToken(NUMBER, new Object(source.mid(start, current - start).toDouble()));
}

void Scanner::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }

    // Unterminated string.
    if (isAtEnd()) {
        Webpage::error(line, "Unterminated string.");
        return;
    }

    // The closing ".
    advance();

    // Trim the surrounding quotes.
    QString value = source.mid(start + 1, current - start - 2);
    addToken(STRING, new Object(value));
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source.at(current).toLatin1() != expected) return false;

    current++;
    return true;
}

char Scanner::peek() {
    if (isAtEnd()) return '\0';
    return source.at(current).toLatin1();
}

char Scanner::peekNext() {
    if (current + 1 >= source.length()) return '\0';
    return source.at(current + 1).toLatin1();
}

bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

char Scanner::advance() {
    current++;
    return source.at(current - 1).toLatin1();
}

void Scanner::addToken(TokenType type) {
    addToken(type, NULL);
}

void Scanner::addToken(TokenType type, Object* literal) {
    QString text = source.mid(start, current - start);
    tokens.append(Token(type, text, line, literal));
}
