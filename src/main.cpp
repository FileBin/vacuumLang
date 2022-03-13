#include <fstream>
#include <locale>
#include <codecvt>
#include "Lexer.hpp"

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//
int main() {
    STD wifstream file("Core.vac");
    auto utf8 = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>);
    file.imbue(utf8);
    setlocale(LC_ALL, "");
    Lexer<> lexer(file);
    Token token;
    while((token = lexer.GetNextToken()).ty != Token::End)
        wprintf(L"Read token %ls\n", token.ToString().c_str());

    return 0;
}