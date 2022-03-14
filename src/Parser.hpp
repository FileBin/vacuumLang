#pragma once
#include "stdafx.hpp"
#include "Functions.hpp"
#include "Lexer.hpp"
#include "AST.hpp"
#include "Metadata.hpp"

//===----------------------------------------------------------------------===//
// Parser
// parses only inner function code
//===----------------------------------------------------------------------===//
class Parser
{
protected:

#define LOG_IDENTIFIER_EXPECTED logError("Identifier expected!")

    typedef STD unique_ptr<AST::IASTNode> node_t;
    typedef STD unique_ptr<AST::ExprAST> expr_t;

    Token currentToken;
    std::stack<String> path;
    STD vector<Keyword> modifiers;

    enum class Context {
        Class, Function, Global
    } context;
public:
    std::unique_ptr<llvm::LLVMContext> TheContext;
    std::unique_ptr<llvm::Module> TheModule;
    std::unique_ptr<llvm::IRBuilder<>> Builder;
    std::map<String, llvm::Value*> NamedValues;
    TokenBufferStream* stream;

public:
    Parser() {
        init();
    }

    node_t parse(TokenBufferStream& stream) {
        //TODO make function parsing
    }

protected:
    void init() {
        using namespace llvm;
        // Open a new context and module.
        TheContext = STD make_unique<LLVMContext>();
        TheModule = STD make_unique<Module>("main", *TheContext);

        // Create a new builder for the module.
        Builder = STD make_unique<IRBuilder<>>(*TheContext);
    }

    Token GetNextToken() {
        return currentToken = stream->moveNext();
    }

    /// numberexpr ::= number
    expr_t ParseNumberExpr() {
        auto Result = std::make_unique<AST::NumberExprAST>(this, *currentToken.getData<String>());
        GetNextToken(); // consume the number
        return std::move(Result);
    }

    /// parenexpr ::= '(' expression ')'
    expr_t ParseParenExpr() {
        if (currentToken.ty != Token::Operator || currentToken.getData<Operator>()->ty != Operator::BracketOpen)
            logError("'(' expected!");
        GetNextToken();
        auto V = ParseExpression();
        if (!V)
            return nullptr;

        if (currentToken.ty != Token::Operator || currentToken.getData<Operator>()->ty != Operator::BracketClose)
            logError("')' expected!");
        GetNextToken(); // eat ).
        return V;
    }

    /// binoprhs
    ///   ::= ('+' primary)*
    expr_t ParseBinOpRHS(int priority, expr_t LHS) {
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
            auto RHS = ParsePrimary();


            // If BinOp binds less tightly with RHS than the operator after RHS, let
            // the pending operator take RHS as its LHS.

            int next_priority = 0;

            if (currentToken.ty == Token::Operator)
                next_priority = Operator::GetPriority(currentToken.getData<Operator>()->ty);

            if (op_priority < next_priority) {
                RHS = ParseBinOpRHS(priority + 1, std::move(RHS));
            }

            // Merge LHS/RHS.
            LHS = std::make_unique<AST::BinaryExprAST>(this, ::Operator(BinOp), STD move(LHS), STD move(RHS));
        }
    }

    expr_t ParseReadExpr() {
        //TODO: insert check code for variable, property, function
    }

    expr_t ParsePrimary() {
        switch (currentToken.ty) {
        default: logError("Unknown token when expecting an expression");
        case Token::Identifier: return ParseReadExpr();
        case Token::Number: return ParseNumberExpr();
        case Token::Operator: return ParseParenExpr();
        }
    }

    /// expression
    ///   ::= primary binoprhs
    ///
    expr_t ParseExpression() {
        auto LHS = ParsePrimary();
        return ParseBinOpRHS(0, std::move(LHS));
    }
};