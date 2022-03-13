#pragma once
#include "stdafx.hpp"
#include "Functions.hpp"

class Type;

struct Metadata {
    Tree<Type> ClassTree, InterfaceTree;

    Metadata();
};

#include "Lexer.hpp"
#include "Types.hpp"

Metadata::Metadata() : ClassTree(Type::createPrototype(this, "Object")) {
    auto val_ty = ClassTree.root->addChild(Type::createPrototype(this, "ValueType"));

    auto node_bool = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::Bool)));
    auto node_sbyte = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::SByte)));
    auto node_byte = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::Byte)));
    auto node_short = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::Short)));
    auto node_ushort = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::UShort)));
    auto node_int = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::Int)));
    auto node_uint = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::UInt)));
    auto node_num = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::Num)));
    auto node_unum = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::UNum)));

    auto node_flt = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::Flt)));
    auto node_dbl = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::Dbl)));

    auto node_char = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::Char)));

    node_bool->data->type = Type::Bool;
    node_sbyte->data->type = Type::SByte;
    node_byte->data->type = Type::Byte;
    node_short->data->type = Type::Short;
    node_ushort->data->type = Type::UShort;
    node_int->data->type = Type::Int;
    node_uint->data->type = Type::UInt;
    node_num->data->type = Type::Num;
    node_unum->data->type = Type::UNum;
    node_flt->data->type = Type::Flt;
    node_dbl->data->type = Type::Dbl;
    node_char->data->type = Type::Char;

    node_bool->data->createDefinition(val_ty->data);
    node_sbyte->data->createDefinition(val_ty->data);
    node_byte->data->createDefinition(val_ty->data);
    node_short->data->createDefinition(val_ty->data);
    node_ushort->data->createDefinition(val_ty->data);
    node_int->data->createDefinition(val_ty->data);
    node_uint->data->createDefinition(val_ty->data);
    node_num->data->createDefinition(val_ty->data);
    node_unum->data->createDefinition(val_ty->data);
    node_flt->data->createDefinition(val_ty->data);
    node_dbl->data->createDefinition(val_ty->data);
    node_char->data->createDefinition(val_ty->data);
}

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
        Keyword currentVisibilityModifier = Keyword::Private;
        while (token.ty != Token::End) {
            token = lexer->GetNextToken();
            if (token.ty == Token::Keyword) {
                if (token.getData<Keyword>()->ty == Keyword::Class) {
                    token = lexer->GetNextToken();
                    if (token.ty == Token::Identifier) {
                        String className = *token.getData<String>();
                        token = lexer->GetNextToken();
                        if (token.ty == Token::Operator && token.getData<Operator>()->ty == Operator::BraceOpen) {
                            token = lexer->GetNextToken();
                            if (token.ty == Token::Keyword) {
                                if (token.getData<Keyword>()->isVisiblityModifier()) {
                                    
                                } else {
                                    logError(String("Illegal keyword ") + token.getData<Keyword>()->toString());
                                }
                            }
                            metadata.ClassTree.root->addChild(Type::createPrototype(&metadata, className, currentNamespaces));
                        } else {
                            logError("Missing class opening bracket");
                        }
                    } else {
                        logError("Class definition error");
                    }
                } else if (token.getData<Keyword>()->ty == Keyword::Namespace) {
                    token = lexer->GetNextToken();
                    if (token.ty == Token::Identifier) {
                        currentNamespaces.push_back(*token.getData<String>());
                        token = lexer->GetNextToken();
                        if (!(token.ty == Token::Operator && token.getData<Operator>()->ty == Operator::BraceOpen)) {
                            logError("Missing namespace opening bracket");
                        }
                    } else {
                        logError("Namespace definition error");
                    }
                }
            }
        }
    }

    String dump() {
        return "ClassTree: " + metadata.ClassTree.toString() + "\n"; //+ String("\n InterfaceTree: ") + metadata.InterfaceTree.ToString();
    }
};

