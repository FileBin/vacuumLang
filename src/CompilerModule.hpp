#pragma once
#include "stdafx.hpp"
#include "TokenBufferStream.hpp"
#include "Metadata.hpp"
#include "Lexer.hpp"
#include "ASTheader.h"

class CompilerModule {
public:
    typedef STD shared_ptr<Lexer<char_t>> pLexer;
    typedef STD vector<Keyword> Mods;

protected:
    Metadata metadata;
    STD vector<Function*> functions;
    Token token;
    pLexer lexer;
    Location currentLocation;
    STD vector<Location> imported = {};

    std::unique_ptr<llvm::LLVMContext> TheContext;
    std::unique_ptr<llvm::Module> TheModule;
    std::unique_ptr<llvm::IRBuilder<>> Builder;
public:

    CompilerModule(pLexer lexer) : lexer(lexer) {

    }

    void generateMetadata() {
        moveNext(); //init
        while (token.ty != Token::End) parsePrimary();
    }

    void codeGen();

    String dump() {
        return "ClassTree: " + metadata.classTree.toString() + "\n"; //+ String("\n InterfaceTree: ") + metadata.InterfaceTree.ToString();
    }

    llvm::IRBuilder<>& getBuilder() {
        return *Builder.get();
    }

    llvm::Module& getModule() {
        return *TheModule.get();
    }

    llvm::LLVMContext& getContext() {
        return *TheContext.get();
    }

private:

    void parsePrimary() {
        Keyword kw;
        auto modifiers = parseModifiers(); // get all modifiers
        if (token.ty != Token::Keyword)
            logError("Keyword expected!");
        switch (token.getData<Keyword>()->ty) {
        case Keyword::Class:
            parseClass(modifiers);
            break;
        case Keyword::Namespace:
            parseNamespace();
            break;
        case Keyword::Entrypoint:
            if (!moveNext().isOperator(Operator::BraceOpen)) logError("'{' expected!");
            parseFunctionBody({}, {}); //parse entrypoint of program
            break;
        case Keyword::Import:
            while (moveNext().ty != Token::CmdEnd);
            moveNext();
            break;
        default:
            logError("Unexpeced token!");
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

                auto node = metadata.classTree.find(className);
                if (node != nullptr) { // if has prototype
                    if (node->data->hasDefinition())
                        logError("Redefenition of class " + node->toString());
                } else { // create prototype of class
                    node = metadata.classTree.find<Type&>(*super_ty)
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
        moveNext(); //skip 'namespace'
        if (token.ty == Token::Identifier) { //get name
            currentLocation += *token.getData<String>(); //place name to the location variable
            moveNext(); // skip name
            if (!token.isOperator(Operator::BraceOpen)) {
                logError("Missing namespace opening bracket");
            }
            moveNext(); // skip '{'
            while (token.isOperator(Operator::BraceClose))
                parsePrimary(); //parse like a normal way until the namespace ends
            --currentLocation; //pop the namespace from the location
            moveNext(); //skip '}'
            return;     // and leave fucntion
        }
        logError("Identifier expected!");

    }
    //current token ::=
    Member* parseMember(Type* class_ty) {
        Mods modifiers = {};
        while (token.ty == Token::Keyword) {
            Keyword kw = *token.getData<Keyword>();
            if (kw.isModifier()) {
                addModifier(kw, modifiers);
            } else {
                logError("Unknown keyword " + kw.toString() + "!");
            }
            moveNext();
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
                        return parseProperty(modifiers, name, class_ty);
                    }
                }

                if (token.ty == Token::CmdEnd) {
                    return new Field(name, class_ty, ty);
                }

                logError("Unknown operator");
            }
        }
        logError("Identifier expected");
        return 0;
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
        return 0;
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
        TokenBufferStream stream;
    }

    Property* parseProperty(Mods mods, String name, Type* class_ty) {
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
        name = *token.getData<String>();
        Type* t;
        moveNext();
        if (t = metadata.classTree.find((currentLocation + name).toString())->data)
            return t;

        for (auto& loc : imported) {
            if (t = metadata.classTree.find((currentLocation + name).toString())->data)
                return t;
        }
        return Type::createPrototype(&metadata, name, currentLocation);
    }

    Mods parseModifiers() {
        Mods mods;
        while (1) { // parse modifiers
            if (token.ty != Token::Keyword) return mods;
            auto kw = *token.getData<Keyword>();
            if (!kw.isModifier()) return mods;
            addModifier(kw, mods);
            moveNext();
        }
    }
};

#include "AST.hpp"

void CompilerModule::codeGen() {
    //TODO: llvm codegeneration! OwO
}

