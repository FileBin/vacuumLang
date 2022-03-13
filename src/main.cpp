#include <fstream>
#include <locale>
#include <codecvt>
#include "MetadataGenerator.hpp"
#include "Lexer.hpp"
#include "ConsoleApplication.hpp"

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//
void programme(ConsoleApplication& app) {
    FILE_STREAM file(ToStdString(app.getParameterValues("i")[0]));
    auto utf8 = std::locale(std::locale(), new std::codecvt_utf8<char_t>);
    file.imbue(utf8);
    setlocale(LC_ALL, "");
    STD shared_ptr<Lexer<>> lexer = STD make_shared<Lexer<>>(file);

    MetadataGenerator metaGen(lexer);
    //metaGen.generateMetadata();
    wprintf(L"metadata dump:\n %ls\n", metaGen.dump().c_str());
    //Token token;
    //while((token = lexer->GetNextToken()).ty != Token::End)
    //    wprintf(L"Read token %ls\n", token.ToString().c_str());
}

int main(int argc, char const* argv[]) {
    ConsoleApplication app(argc, argv, programme);
    return 0;
}