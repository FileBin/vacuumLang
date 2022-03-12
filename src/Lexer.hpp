#pragma once

#include "Tokens.hpp"
#include "Keyword.hpp"
#include "Types.hpp"
#include "Operators.hpp"

template<typename _CharT = char_t, int _EOF = -1>
class Lexer {
public:
    typedef _CharT CharType;
    typedef STD basic_istream<_CharT> StreamType;
    typedef typename STD basic_istream<_CharT>::int_type IntType;
    typedef STD basic_string<_CharT> StringType;

    union Char {
        CharType ch;
        char_t wch;
        IntType val;
    };
private:
    StreamType& stream;
    static constexpr size_t BUFFER_SIZE = 0x80;
    char_t buffer[BUFFER_SIZE] = { 0 };
    IntType current = 0;
    Token token;
public:
    Lexer(StreamType& stream) : stream(stream) {}
    // GetNextToken - Return the next token from stream.
    Token GetNextToken() {
        using namespace std;

        // Init
        if (current == 0) moveNext();

        skipSpaces();

        // If all tokens read
        if (!isop(last())) {

            // Identifier
            if (iswalpha(current) || current == '_') {
                token.ty = Token::Identifier;
                String str;
                do {
                    str.push_back((wchar_t)current);
                    moveNext();
                } while (iswalnum(current) || current == '_'); //read token

                Keyword* kw;
                if (Keyword::TryParse(str, kw, true)) {
                    token.ty = Token::Keyword;
                    token.SetData<Keyword>(kw);
                } else token.SetData<String>(new String(str));
                clearBuffer();
                return token;
            }

            // Number: [0-9.]+
            if (iswdigit(current) || current == '.') {
                token.ty = Token::Number;
                token.SetData<String>(new String());
                do {
                    token.GetData<String>()->push_back((char_t)current);
                    moveNext();
                } while (isdigit(current) || current == '.');
                clearBuffer();
                return token;
            }

            //Const String
            if (current == '\"') {
                token.ty = Token::String;
                token.SetData<String>(new String());
                while (current != _EOF) {
                    moveNext();
                    if (current == '\"') break;
                    token.GetData<String>()->push_back((wchar_t)current);
                }
                clearBuffer();
                moveNext();
                return token;
            }


        } else if (isop(current)) {
            while (isop(moveNext())) skipSpaces();
        }

        // Comment
        if (consist(L"//")) {
            skipUntil('\n');

            clearBuffer();
            moveNext();
            if (current != _EOF)
                return GetNextToken();
        }

        if (consist(L";")) {
            pop();
            return token = Token::CmdEnd;
        }

        if (consist(L",")) {
            pop();
            return token = Token::Comma;
        }

        if (consist(L":")) {
            pop();
            return token = Token::Colon;
        }

        //Operators
        Operator* op;
        if (Operator::TryParse(reversedBuffer(), op)) {
            pop(op->GetLen()); //drop operator from the buffer
            token.ty = Token::Operator;
            token.SetData<Operator>(op);
            return token;
        }

        // Check for end of file.  Don't eat the EOF.
        if (current == _EOF)
            return token = Token::End;

        token.SetData<String>(new String(reversedBuffer()));
        clearBuffer();
        moveNext();
        return token;
    }

private:
    bool isop(IntType ch) {
        return !(iswalnum(ch) || iswspace(ch) || ch == '\n' || ch == 0 || ch == _EOF || ch == '\"');
    }
    void skipUntil(IntType ch) {
        while (stream.get() != ch);
    }

    IntType move() {
        IntType t;
        while (iswspace(t = moveNext()));
        return t;
    }

    void skipSpaces() {
        while (iswspace(current)) moveNext();
    }

    IntType moveNext() {
        current = stream.get(); //read char
        if (iswspace(current) || current == _EOF) return current; //don't store any spaces

        for (size_t i = BUFFER_SIZE - 2;i > 0;i--) //shift buffer
            buffer[i] = buffer[i - 1];

        return buffer[0] = (char_t)current; //set first element
    }

    String reversedBuffer() {
        String str = buffer;
        size_t n = str.size();
        for (size_t i = 0;i < n / 2;i++) {
            STD swap(str[i], str[n - i - 1]);
        }
        return str;
    }

    size_t bufSize() {
        return wcslen(buffer);
    }

    char_t last() {
        if (isBufferFree()) return 0;
        return buffer[bufSize() - 1];
    }

    void clearBuffer(size_t offset = 0) {
        while (isop(buffer[offset])) offset++;
        memset(buffer + offset, 0, BUFFER_SIZE - offset);
    }

    bool isBufferFree() {
        return bufSize() == 0;
    }

    void pop(size_t count = 1) {
        size_t last = bufSize() - 1;
        for (size_t i = 0;i < count;i++) {
            buffer[last - i] = 0;
        }
    }

    bool consist(const wchar_t* string) {
        size_t i = bufSize();
        wchar_t ch = string[0];
        for (const wchar_t* p = string; ch = *p; p++) {
            if (i <= 0)
                return false;
            if (ch != buffer[--i])
                return false;
        }
        return true;
    }
};