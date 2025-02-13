#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

static Token makeToken(TokenType tokenType);
static Token errorToken(char* message);
static Token string();
static Token number();
static void skipWhiteSpace();
static char advance();
static char peek();
static char peekNext();
static bool isDigit(char c);
static bool match(char c);
static bool isAtEnd();

typedef struct {
    const char* start;
    const char* current;
    int line;
} Scanner;

Scanner scanner;

void initScanner(const char* source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

Token scanToken() {
    skipWhiteSpace();
    scanner.start = scanner.current;

    if (isAtEnd()) {
        return makeToken(TOKEN_EOF);
    }

    char c = advance();
    if (isDigit(c)) {
        return number();
    }

    switch (c) {
    case '(': return makeToken(TOKEN_LEFT_PAREN);
    case ')': return makeToken(TOKEN_RIGHT_PAREN);
    case '{': return makeToken(TOKEN_LEFT_BRACE);
    case '}': return makeToken(TOKEN_RIGHT_BRACE);
    case ';': return makeToken(TOKEN_SEMICOLON);
    case ',': return makeToken(TOKEN_COMMA);
    case '.': return makeToken(TOKEN_DOT);
    case '-': return makeToken(TOKEN_MINUS);
    case '+': return makeToken(TOKEN_PLUS);
    case '/': return makeToken(TOKEN_SLASH);
    case '*': return makeToken(TOKEN_STAR);
    case '!':
      return makeToken(
          match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=':
      return makeToken(
          match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '<':
      return makeToken(
          match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>':
      return makeToken(
          match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '"':
      return string();
  }

    return errorToken("Unexpected character.");
}

static Token makeToken(TokenType tokenType) {
    Token token;
    token.type = tokenType;
    token.start = scanner.start;
    token.lenght = (int) (scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}
static Token errorToken(char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.lenght = (int) strlen(message);
    token.line = scanner.line;
    return token;
}

static Token string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            scanner.line++;
        }
        advance();
    }
    if (isAtEnd()) {
        return errorToken("Unterminated string.");
    }
    advance();
    return makeToken(TOKEN_STRING);
}

static Token number() {
    while (isDigit(peek())) {
        advance();
    }
    if (peek() == '.' && isDigit(peekNext())) {
        advance();
    }
    while (isDigit(peek())) {
        advance();
    }
    return makeToken(TOKEN_NUMBER);
}

static void skipWhiteSpace() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            case '/':
                if (peekNext() == '/') {
                    while (peek() != '\n' && !isAtEnd()) {
                        advance();
                    }
                } else {
                    return;
                }
            default:
                return;
        }
    }
}


static char advance() {
    scanner.current++;
    return scanner.current[-1];
}

static char peek() {
    return *scanner.current;
}

static char peekNext() {
    if (isAtEnd()) {
        return '\0';
    }
    return scanner.current[1];
}

static bool match(char c) {
    if (isAtEnd()) {
        return false;
    }
    if (*scanner.current != c) {
        return false;
    }
    scanner.current++;
    return true;
}

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static bool isAtEnd() {
    return *scanner.current == '\0';
}
