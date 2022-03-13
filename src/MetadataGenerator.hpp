#pragma once
#include "stdafx.hpp"
#include "Types.hpp"
#include "Functions.hpp"

struct Metadata {
    Tree<Type> ClassTree, InterfaceTree;

    Metadata() : ClassTree(Type::Object) {
        auto val_ty = ClassTree.root->AddChild(Objects::ValueType());
        val_ty->AddChild(Type::SByte);
        val_ty->AddChild(Type::Byte);
        val_ty->AddChild(Type::Short);
        val_ty->AddChild(Type::UShort);
        val_ty->AddChild(Type::Int);
        val_ty->AddChild(Type::UInt);
        val_ty->AddChild(Type::Num);
        val_ty->AddChild(Type::UNum);

        val_ty->AddChild(Type::Flt);
        val_ty->AddChild(Type::Dbl);
        val_ty->AddChild(Type::BigDbl);

        val_ty->AddChild(Type::Char);
    }
};

#include "Lexer.hpp"

class MetadataGenerator {
public:
    typedef STD shared_ptr<Lexer<char_t>> pLexer;

protected:
    Metadata metadata;
    pLexer lexer;

public:
    MetadataGenerator(pLexer lexer) : lexer(lexer) {

    }
    void generateMetadata() {
        Token token;

        std::vector<String> currentNamespaces;
        String currentVisibilityModifier = "";
        token = lexer->GetNextToken();
        if (token.ty == Token::Keyword) {
            if (token.GetData<Keyword>()->ty == Keyword::Class) {
                token = lexer->GetNextToken();
                if (token.ty == Token::Identifier) {
                    String* className = token.GetData<String>();
                    token = lexer->GetNextToken();
                    if (token.ty == Token::Operator && token.GetData<Operator>()->ty == Operator::BraceOpen) {
                        token = lexer->GetNextToken();
                        if (token.ty == Token::Keyword) {
                            if (token.GetData<Keyword>()->isVisiblityModifier()){
                                
                            } else {
                                LogError(String("Illegal keyword ") + token.GetData<Keyword>()->ToString());
                            }
                        }
                        metadata.ClassTree.root->AddChild(Type::createType(*className, currentNamespaces, {}));
                    } else {
                        LogError("Missing class opening bracket");
                    }
                } else {
                    LogError("Class definition error");
                }
            } else if (token.GetData<Keyword>()->ty == Keyword::Namespace) {
                token = lexer->GetNextToken();
                if (token.ty == Token::Identifier) {
                    currentNamespaces.push_back(*token.GetData<String>());
                    token = lexer->GetNextToken();
                    if (!(token.ty == Token::Operator && token.GetData<Operator>()->ty == Operator::BraceOpen)) {
                        LogError("Missing namespace opening bracket");
                    }
                } else {
                    LogError("Namespace definition error");
                }
            }
        }
    }
};