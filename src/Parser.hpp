#pragma once
#include "stdafx.hpp"
#include "Functions.hpp"
#include "Lexer.hpp"
#include "AST.hpp"
#include "Metadata.hpp"
#include "CompilerModule.hpp"

//===----------------------------------------------------------------------===//
// Parser
// parses only inner function code
//===----------------------------------------------------------------------===//
class CodeParser
{
protected:

#define LOG_IDENTIFIER_EXPECTED logError("Identifier expected!")

    typedef STD unique_ptr<AST::IASTNode> node_t;
    typedef STD unique_ptr<AST::ExprAST> expr_t;
    typedef STD unique_ptr<AST::ScopeAST> scope_t;

    std::stack<String> path;
    STD vector<Keyword> modifiers;

    enum class Context {
        Class, Function, Global
    } context;
public:
    TokenBufferStream* stream;
    CompilerModule* cmodule;
    STD shared_ptr<Enviroment> enviroment;

public:
    CodeParser(CompilerModule* mod) {
        cmodule = mod;
    }

    node_t parse(Function* func, STD shared_ptr<Enviroment> env) {
        //TODO: make function parsing
        enviroment = env;
        stream = &func->getBody();
        return parsePrimary();
    }

protected:

    node_t parsePrimary() {
        switch(stream->getCurrToken().ty) {
            case Token::BraceOpen:
                return parseScope();
            case Token::Keyword:
            case Token::Identifier:
                return parseCommand();
            default:
                logError("Unexpected Token!");
        }
    }

    scope_t parseScope() {
        while(stream->getCurrToken().ty != Token::BraceClose) {
            parsePrimary();
        }
    }

    node_t parseCommand() {
        STD vector<Keyword> modifiers;
        while(stream->getCurrToken().ty == Token::Keyword) {
            modifiers.push_back(*stream->getCurrToken().getData<Keyword>());
            stream->moveNext();
        }
        Type* ty;
        if(Type::tryGetInstance(ty, &cmodule->getMeta(), *stream->getCurrToken().getData<String>(), *enviroment)) {
            //variable definition
            if(stream->moveNext().ty != Token::Identifier)
                logError("Identifier expected!");

            String name = *stream->getCurrToken().getData<String>();
             var = 
            enviroment->addLocal(name, );
            
            if(!stream->moveNext().isOperator(Operator::Assign))
                logError("'=' expected!");

            stream->moveNext();

        }
    }
    /// numberexpr ::= number
    expr_t ParseNumberExpr(scope_t& scope) {
        Type* ty = Type::getInstance(&cmodule->getMeta(), Type::Num);
        auto Result = std::make_unique<AST::NumberExprAST>(cmodule, scope, ty, *stream->getCurrToken().getData<String>());
        stream->moveNext(); // consume the number
        return std::move(Result);
    }

    /// parenexpr ::= '(' expression ')'
    expr_t ParseParenExpr(scope_t& scope) {
        if (currentToken.ty != Token::Operator || currentToken.getData<Operator>()->ty != Operator::BracketOpen)
            logError("'(' expected!");
        GetNextToken();
        auto V = ParseExpression(scope);
        if (!V)
            return nullptr;

        if (currentToken.ty != Token::Operator || currentToken.getData<Operator>()->ty != Operator::BracketClose)
            logError("')' expected!");
        GetNextToken(); // eat ).
        return V;
    }

    /// binoprhs
    ///   ::= ('+' primary)*
    expr_t ParseBinOpRHS(int priority, expr_t LHS, scope_t& scope) {
        if (currentToken.ty != Token::Operator) logError("Operator expected!");
        Operator& op = *currentToken.getData<Operator>();
        if (!Operator::hasBinaryForm(op.ty)) logError("Binary operator expected!");
        // If this is a binop, find its precedence.
        while (true) {
            int op_priority = Operator::GetPriority(op.ty);

            // If this is a binop that binds at least as tightly as the current binop,
            // consume it, otherwise we are done.
            if (op_priority < priority)
                return LHS;

            // Okay, we know this is a binop.
            auto BinOp = op.ty;
            GetNextToken(); // eat binop

            // Parse the primary expression after the binary operator.
            auto RHS = ParsePrimary(scope);


            // If BinOp binds less tightly with RHS than the operator after RHS, let
            // the pending operator take RHS as its LHS.

            int next_priority = 0;

            if (currentToken.ty == Token::Operator)
                next_priority = Operator::GetPriority(currentToken.getData<Operator>()->ty);

            if (op_priority < next_priority) {
                RHS = ParseBinOpRHS(priority + 1, std::move(RHS), scope);
            }

            Type* ty;
            //TODO: make some type casting
            ty = LHS->type;

            // Merge LHS/RHS.
            LHS = std::make_unique<AST::BinaryExprAST>(cmodule, scope, ty, ::Operator(BinOp), STD move(LHS), STD move(RHS));
        }
    }

    expr_t ParseReadExpr(scope_t& scope) {
        //TODO: insert check code for variable, property, function
    }

    expr_t ParsePrimary(scope_t& scope) {
        switch (currentToken.ty) {
        default: logError("Unknown token when expecting an expression");
        case Token::Identifier: return ParseReadExpr(scope);
        case Token::Number: return ParseNumberExpr(scope);
        case Token::Operator: return ParseParenExpr(scope);
        }
    }

    /// expression
    ///   ::= primary binoprhs
    ///
    expr_t ParseExpression(scope_t& scope) {
        auto LHS = ParsePrimary(scope);
        return ParseBinOpRHS(0, std::move(LHS), scope);
    }
};