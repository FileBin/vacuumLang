#include <fstream>
#include <locale>
#include <codecvt>
#include "CompilerModule.hpp"
#include "ConsoleApplication.hpp"

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//
void programme(ConsoleApplication& app) {
    String path = app.getParameterValues("i")[0];
    FILE_STREAM file(ToStdString(path));
    if(!file.is_open()) logError("File" + path + " not founded");
    auto utf8 = std::locale(std::locale(), new std::codecvt_utf8<char_t>);
    file.imbue(utf8);
    setlocale(LC_ALL, "");
    STD shared_ptr<Lexer<>> lexer = STD make_shared<Lexer<>>(file);

    CompilerModule metaGen(lexer);
    metaGen.generateMetadata();

    app.Log(L"metadata dump:\n" + metaGen.dump() + "\n");
    Token token;
    while((token = lexer->GetNextToken()).ty != Token::End)
        wprintf(L"Read token %ls\n", token.toString().c_str());
}

int main(int argc, char const* argv[]) {
    try {
        ConsoleApplication app(argc, argv, programme);
    } catch(int i) {
        return i;
    }
    return 0;
}