#ifndef TOKENTYPE_H
#define TOKENTYPE_H


enum TokenType {LEFT_PAREN, RIGHT_PAREN, LEFT_BRACKET, RIGHT_BRACKET, LEFT_BRACE, RIGHT_BRACE,
                COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

                // One or two character tokens.
                ASSIGN,
                BANG, BANG_EQUAL,
                EQUAL, EQUAL_EQUAL,
                GREATER, GREATER_EQUAL,
                LESS, LESS_EQUAL, INCREMENT, DECREMENT,

                // Literals.
                IDENTIFIER, STRING, NUMBER,

                // Keywords.
                AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
                PRINT, RETURN, STYLE, SUPER, THIS, TRUE, VAR, WHILE, EOT


               };


#endif // TOKENTYPE_H
