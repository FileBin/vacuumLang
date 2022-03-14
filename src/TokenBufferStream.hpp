#pragma once
#include "stdafx.hpp"
#include "Tokens.hpp"

class TokenBufferStream {
private:
    typedef std::vector<Token> buffType;
    buffType buffer;
    buffType::iterator currentTok;
public:

    void push(Token& tok) {
        buffer.push_back(tok);
        currentTok = buffer.begin();
    }

    void pop() {
        buffer.pop_back();
        currentTok = buffer.begin();
    }

    Token getCurrToken() {
        return *currentTok;
    }

    Token moveNext() {
        auto next = currentTok++;
        if (next == buffer.end()) {
            return Token::End;
        }
        return *next;
    }
};