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
    typedef STD vector<Keyword> Mods;

protected:
    Metadata metadata;
    Token token;
    pLexer lexer;
    std::vector<String> currentNamespaces;

public:

    MetadataGenerator(pLexer lexer) : lexer(lexer) {

    }

    void generateMetadata() {
        STD vector<Keyword> modifiers;
        while (token.ty != Token::End) {
            moveNext();
            if (token.ty == Token::Keyword) {
                Keyword kw = *token.getData<Keyword>();
                if (kw.isModifier()) {
                    addModifier(kw, modifiers);
                }
                switch (kw.ty) {
                case Keyword::Class:
                    parseClass(modifiers);
                    break;
                case Keyword::Namespace:
                    parseNamespace();
                    break;
                }
                if (token.getData<Keyword>()->ty == Keyword::Namespace) {
                    // token = lexer->GetNextToken();
                    // if (token.ty == Token::Identifier) {
                    //     currentNamespaces.push_back(*token.getData<String>());
                    //     token = lexer->GetNextToken();
                    //     if (!(token.ty == Token::Operator && token.getData<Operator>()->ty == Operator::BraceOpen)) {
                    //         logError("Missing namespace opening bracket");
                    //     }
                    // } else {
                    //     logError("Namespace definition error");
                    // }
                }
            }
        }
    }

    String dump() {
        return "ClassTree: " + metadata.ClassTree.toString() + "\n"; //+ String("\n InterfaceTree: ") + metadata.InterfaceTree.ToString();
    }

private:

    Token moveNext() {
        return token = lexer->GetNextToken();
    }

    void addModifier(Keyword mod, Mods& mods) {
        if (!mod.isModifier()) logError(mod.toString() + "is not a modifier");
        for (Keyword& m : mods) {
            if (m.ty == mod.ty) {
                logWarning("Dublicate modifier " + mod.toString() + "!");
                continue;
            }
            if (m.isVisiblityModifier() && mod.isVisiblityModifier()) {
                logError("Multi visiblity modifiers are not allowed!");
            }
        }
    }

    //current ::= Class
    void parseClass(Mods modifiers) {
        //Keyword currentVisibilityModifier = Keyword::Private;
        if (token.ty != Token::Keyword || token.getData<Keyword>()->ty != Keyword::Class)
            logError("'class' expected!");

        if (token.ty == Token::Identifier) {
            String className = *token.getData<String>();
            moveNext();
            if (token.ty == Token::Operator && token.getData<Operator>()->ty == Operator::BraceOpen) {
                metadata.ClassTree.root->addChild(Type::createPrototype(&metadata, className, currentNamespaces));
                while (1) {
                    if (token.ty == Token::Operator && token.getData<Operator>()->ty == Operator::BraceClose) break;
                    parseMember();
                }
            } else {
                logError("Missing class opening bracket");
            }
        } else {
            logError("Class definition error");
        }
    }
    void parseNamespace() {

    }
    void parseMember() {
        Mods modifiers = {};
        if (token.ty == Token::Keyword) {
            Keyword kw = *token.getData<Keyword>();
            if (kw.isModifier()) {
                addModifier(kw, modifiers);
            } else {
                logError("Unknown keyword " + kw.toString() + "!");
            }
            moveNext();
            return;
        }
        if (token.ty == Token::Identifier) {
            Type::createPrototype(&metadata, *token.getData<String>(), currentNamespaces);
            moveNext();
            String name;
            if (token.ty == Token::Identifier) {
                name = *token.getData<String>();
                moveNext();
                //Function
                if (token.ty == Token::Operator) {
                    //Function
                    if (token.getData<Operator>()->ty == Operator::BracketOpen) {
                        parseFunction(modifiers, name);
                        return;
                    }
                    if (token.getData<Operator>()->ty == Operator::BraceOpen) {
                        parseProperty();
                        return;
                    }
                }
                if (token.ty == Token::CmdEnd) {
                    parseField();
                    return;
                }

                logError("Unknown operator");
            }
        }
    }

    void parseField() {

    }
    //current token '('
    void parseFunction(Mods mods, String name) {

    }
    //current token '{'
    void parseFunctionBody(Mods mods, String name) {

    }

    void parseProperty() {
        moveNext();
        Keyword kw, kw_vis;
        Mods modifiers = {};
        if (token.ty == Token::Keyword && (kw = *token.getData<Keyword>()).ty == Keyword::Get 
        || kw.ty == Keyword::Get
        || kw.ty == Keyword::Set) {
            moveNext();
            if (token.ty == Token::CmdEnd){
                if (kw.ty == Keyword::Set){
                    // TODO add empty funtion parsing
                } else {
                    
                }
            } else if (token.ty == Token::Operator && token.getData<Operator>()->ty == Operator::BraceOpen) {
                parseFunctionBody(modifiers, kw.toString());
            } else if (token.ty == Token::Keyword && (kw_vis = *token.getData<Keyword>()).isVisiblityModifier()){
                addModifier(kw_vis, modifiers);
            } else {
                logError("Invalid token: " + token.toString());
            }
        }
    }
};

