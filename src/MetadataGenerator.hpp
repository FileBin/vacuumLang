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
    auto val_ty = ClassTree.root->AddChild(Type::createPrototype(this, "ValueType"));

    auto node_bool = val_ty->AddChild(Type::createPrototype(this, Type::getEnumTypeName(Type::Bool)));
    auto node_sbyte = val_ty->AddChild(Type::createPrototype(this, Type::getEnumTypeName(Type::SByte)));
    auto node_byte = val_ty->AddChild(Type::createPrototype(this, Type::getEnumTypeName(Type::Byte)));
    auto node_short = val_ty->AddChild(Type::createPrototype(this, Type::getEnumTypeName(Type::Short)));
    auto node_ushort = val_ty->AddChild(Type::createPrototype(this, Type::getEnumTypeName(Type::UShort)));
    auto node_int = val_ty->AddChild(Type::createPrototype(this, Type::getEnumTypeName(Type::Int)));
    auto node_uint = val_ty->AddChild(Type::createPrototype(this, Type::getEnumTypeName(Type::UInt)));
    auto node_num = val_ty->AddChild(Type::createPrototype(this, Type::getEnumTypeName(Type::Num)));
    auto node_unum = val_ty->AddChild(Type::createPrototype(this, Type::getEnumTypeName(Type::UNum)));

    auto node_flt = val_ty->AddChild(Type::createPrototype(this, Type::getEnumTypeName(Type::Flt)));
    auto node_dbl = val_ty->AddChild(Type::createPrototype(this, Type::getEnumTypeName(Type::Dbl)));

    auto node_char = val_ty->AddChild(Type::createPrototype(this, Type::getEnumTypeName(Type::Char)));

    ClassTree.root->data->createDefinition(nullptr, Objects::Object::getMembers(this));

    val_ty->data->createDefinition(ClassTree.root->data);

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
                if (token.GetData<Keyword>()->ty == Keyword::Class) {
                    token = lexer->GetNextToken();
                    if (token.ty == Token::Identifier) {
                        String className = *token.GetData<String>();
                        token = lexer->GetNextToken();
                        if (token.ty == Token::Operator && token.GetData<Operator>()->ty == Operator::BraceOpen) {
                            token = lexer->GetNextToken();
                            if (token.ty == Token::Keyword) {
                                if (token.GetData<Keyword>()->isVisiblityModifier()) {

                                } else {
                                    LogError(String("Illegal keyword ") + token.GetData<Keyword>()->ToString());
                                }
                            }
                            metadata.ClassTree.root->AddChild(Type::createType(&metadata, className, currentNamespaces, {}));
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
    }

    String dump() {
        return "ClassTree: " + metadata.ClassTree.ToString() + "\n"; //+ String("\n InterfaceTree: ") + metadata.InterfaceTree.ToString();
    }
};

