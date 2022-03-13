#include <fstream>
#include <locale>
#include <codecvt>
#include "MetadataGenerator.hpp"
#include "Lexer.hpp"

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//
int main(int argc, const char* argv[]) {
    FILE_STREAM file("Core.vac");
    auto utf8 = std::locale(std::locale(), new std::codecvt_utf8<char_t>);
    file.imbue(utf8);
    setlocale(LC_ALL, "");
    STD shared_ptr<Lexer<>> lexer = STD make_shared<Lexer<>>(file);

    MetadataGenerator metaGen(lexer);
    metaGen.generateMetadata();
    wprintf(L"metadata dump:\n %ls\n",metaGen.dump().c_str());
    //Token token;
    //while((token = lexer->GetNextToken()).ty != Token::End)
    //    wprintf(L"Read token %ls\n", token.ToString().c_str());

    return 0;
}