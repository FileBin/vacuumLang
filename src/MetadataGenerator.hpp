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
#include "TokenBufferStream.hpp"

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
    Location currentLocation;
    STD vector<Location> imported = {};
public:

    MetadataGenerator(pLexer lexer) : lexer(lexer) {

    }

    void generateMetadata() {
        moveNext(); //init
        while (token.ty != Token::End) parsePrimary();
    }

    String dump() {
        return "ClassTree: " + metadata.ClassTree.toString() + "\n"; //+ String("\n InterfaceTree: ") + metadata.InterfaceTree.ToString();
    }

private:

    void parsePrimary() {
        STD vector<Keyword> modifiers;

        Keyword kw;
        while (1) { // parse modifiers
            if (token.ty != Token::Keyword) break;
            kw = *token.getData<Keyword>();
            if (!kw.isModifier()) break;
            addModifier(kw, modifiers);
            moveNext();
        }
        if (token.ty != Token::Keyword) logError("Keyword expected!");
        switch (kw.ty) {
        case Keyword::Class:
            parseClass(modifiers);
            break;
        case Keyword::Namespace:
            parseNamespace();
            break;
        case Keyword::Entrypoint:
            if (!moveNext().isOperator(Operator::BraceOpen)) logError("'{' expected!");
            parseFunctionBody({}, {});
            break;

        }
    }

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

    //current token ::= keyword::Class
    void parseClass(Mods modifiers) {
        Type* super_ty = Type::getInstance(&metadata, Type::Object);

        if (token.ty == Token::Identifier) {
            String className = *token.getData<String>();

            //if it is definition of object
            if (className == Type::getName(Type::Object))
                super_ty = nullptr;

            moveNext();
            if (token.isOperator(Operator::BraceOpen)) { //begin of the body

                auto node = metadata.ClassTree.find(className);
                if (node != nullptr) { // if has prototype
                    if (node->data->hasDefinition())
                        logError("Redefenition of class " + node->toString());
                } else { // create prototype of class
                    node = metadata.ClassTree.find(super_ty)
                        ->addChild(Type::createPrototype(&metadata, className, currentLocation));
                }
                STD vector<Member*> members;
                while (1) {
                    if (token.isOperator(Operator::BraceClose)) break; //end of the body
                    members.push_back(parseMember(node->data));
                }
                node->data->createDefinition(super_ty, members);
                moveNext();
                return;
            }
            logError("Missing class opening bracket");
        }
        logError("Class definition error");
    }

    //current token ::= namespace
    void parseNamespace() {
        moveNext();
        if (token.ty == Token::Identifier) {
            currentLocation += *token.getData<String>();
            moveNext();
            if (!token.isOperator(Operator::BraceOpen)) {
                logError("Missing namespace opening bracket");
            }
            moveNext();
            while (token.isOperator(Operator::BraceClose)) parsePrimary();
            moveNext();
            return;
        } else {
            logError("Identifier expected!");
        }
    }

    Member* parseMember(Type* class_ty) {
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
            Type* ty = parseType();
            String name;
            if (token.ty == Token::Identifier) {
                name = *token.getData<String>();
                moveNext();
                //Function
                if (token.ty == Token::Operator) {
                    //Function
                    if (token.getData<Operator>()->ty == Operator::BracketOpen) {
                        return parseFunction(modifiers, name, class_ty);
                    }
                    if (token.getData<Operator>()->ty == Operator::BraceOpen) {
                        return parseProperty(class_ty);
                    }
                }

                if (token.ty == Token::CmdEnd) {
                    return new Field(name, class_ty, ty);
                }

                logError("Unknown operator");
            }
            logError("Identifier expected");
        }
    }

    //current token ::= all (if can't parse writes error)
    Variable* parseParam() {
        if (token.ty == Token::Identifier) {
            Type* t = parseType();
            moveNext();
            if (token.ty == Token::Identifier) {
                return new Variable(t, *token.getData<String>());
            }
        }
        logError("Unexpected token!");
    }

    //current token ::= '('
    Function* parseFunction(Mods mods, String name, Type* class_ty) {
        moveNext(); //skip brace
        Type* t;
        STD vector<Variable*> args = {};
        while (1) {
            if (token.isOperator(Operator::BracketClose))
                break;
            args.push_back(parseParam());
        }
        moveNext();
        parseFunctionBody(mods, currentLocation + class_ty);
        return new Function(name, class_ty, t, args);
    }

    //current token ::= '{'
    void parseFunctionBody(Mods mods, Location context) {
        // TODO: place all tokens to the buffer and send it to metadata
    }

    Property* parseProperty(Type* class_ty) {
        moveNext();
        Keyword kw, kw_vis;
        Mods modifiers = {};
        if (token.ty == Token::Keyword && (kw = *token.getData<Keyword>()).ty == Keyword::Get
            || kw.ty == Keyword::Get
            || kw.ty == Keyword::Set) {
            moveNext();
            if (token.ty == Token::CmdEnd) {
                if (kw.ty == Keyword::Set) {
                    // TODO add empty funtion parsing
                } else {

                }
            } else if (token.ty == Token::Operator && token.isOperator(Operator::BraceOpen)) {
                parseFunctionBody(modifiers, currentLocation + class_ty);
            } else if (token.ty == Token::Keyword && (kw_vis = *token.getData<Keyword>()).isVisiblityModifier()) {
                addModifier(kw_vis, modifiers);
            } else {
                logError("Invalid token: " + token.toString());
            }
        }
    }

    //token ::= type Identifier
    Type* parseType() {
        String name;
        Type::createPrototype(&metadata, name, currentLocation);
        moveNext();
    }
};

