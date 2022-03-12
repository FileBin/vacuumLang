#pragma once
#include "stdafx.hpp"
#include "Functions.hpp"
#include "Lexer.hpp"
#include "AST.hpp"

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//
class Parser
{
protected:

#define LOG_IDENTIFIER_EXPECTED LogError("Identifier expected!")

    typedef STD unique_ptr<AST::IASTNode> node_t;

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
    std::unique_ptr<Lexer<>> lexer;

public:
    Parser() {
        init();
    }

    void ParseStream(std::istream& stream) {
        lexer = STD make_unique<Lexer<>>(stream);
        while (true) {
            GetNextToken();
            switch (currentToken.ty) {
                break;
            case Token::Keyword:
                ParseKeyword();
                break;
            default:
                LogError("Amogusus expected!\n");
                break;
            }
        }
    }

protected:
    void init() {
        using namespace llvm;
        // Open a new context and module.
        TheContext = std::make_unique<LLVMContext>();
        TheModule = std::make_unique<Module>("main", *TheContext);

        // Create a new builder for the module.
        Builder = std::make_unique<IRBuilder<>>(*TheContext);
    }

    Token GetNextToken() {
        return currentToken = lexer->GetNextToken();
    }

    node_t ParseKeyword() {
        if (currentToken.ty != Token::Keyword) LogError("Fuck you leatherman!");
        auto& kw = *currentToken.GetData<Keyword>();
        if (context == Context::Global) {
            if (kw.isDefinition())
                return ParseDefinitionKw(kw);
        }
    }

#pragma region Keywords
    node_t ParseDefinitionKw(Keyword kw) {
        using namespace AST;
        GetNextToken();
        if (currentToken.ty != Token::Identifier) LOG_IDENTIFIER_EXPECTED;
        String name = *currentToken.GetData<String>();
        switch (kw.ty) {
        case Keyword::Class: {
            Type super_type = Type::Object;
            ParseType4Class(super_type);
            return node_t(new ClassExpression(this, name, modifiers, super_type));
        }

        case Keyword::Interface: {
            Type super_type = Type::Object;
            ParseType4Interface(super_type);
            return node_t(new ClassExpression(this, name, modifiers, super_type));
        }

        case Keyword::Var: {
            Type type;
            ParseType4Var(type);
            return node_t(new VariableExpression(this, name, type));
        }

        default:
            LogError("Show me the boss of this gym!");
            break;
        }
    }

    void ParseType4Class(Type& superType) {
        superType = Type::Object;
        if (GetNextToken().ty == Token::Colon) {
            if (GetNextToken().ty != Token::Identifier) LOG_IDENTIFIER_EXPECTED;
                //superType = *currentToken.GetData<Type>();
                //TODO: insert metadata lookup for type check
        }
    }

    void ParseType4Interface(Type& superType) {
        superType = Type::Object;
        if (GetNextToken().ty == Token::Colon) {
            if (GetNextToken().ty != Token::Identifier) LOG_IDENTIFIER_EXPECTED;
            //TODO: insert metadata lookup for type check
        }
    }


    void ParseType4Var(Type& superType) {
        superType = Type::Object;
        if (GetNextToken().ty == Token::Operator && currentToken.GetData<Operator>()->type == Operator::Assign) {
            //TODO: insert type check
        } else LogError("= expected!");
    }
#pragma endregion

    /// numberexpr ::= number
    node_t ParseNumberExpr() {
        auto Result = std::make_unique<AST::NumberExprAST>(currentToken.GetData<String>());
        GetNextToken(); // consume the number
        return std::move(Result);
    }

    /// parenexpr ::= '(' expression ')'
    node_t ParseParenExpr() {
        if (currentToken.ty != Token::Operator || currentToken.GetData<Operator>()->type != Operator::BracketOpen)
            LogError("'(' expected!");
        GetNextToken();
        auto V = ParseExpression();
        if (!V)
            return nullptr;

        if (currentToken.ty != Token::Operator || currentToken.GetData<Operator>()->type != Operator::BracketClose)
            LogError("')' expected!");
        GetNextToken(); // eat ).
        return V;
    }

    /// binoprhs
    ///   ::= ('+' primary)*
    node_t ParseBinOpRHS(int priority, node_t LHS) {
        if (currentToken.ty != Token::Operator) LogError("Operator expected!");
        Operator& op = *currentToken.GetData<Operator>();
        if (!Operator::hasBinaryForm(op.type)) LogError("Binary operator expected!");
        // If this is a binop, find its precedence.
        while (true) {
            int op_priority = Operator::GetPriority(op.type);

            // If this is a binop that binds at least as tightly as the current binop,
            // consume it, otherwise we are done.
            if (op_priority < priority)
                return LHS;

            // Okay, we know this is a binop.
            auto BinOp = op.type;
            GetNextToken(); // eat binop

            // Parse the primary expression after the binary operator.
            auto RHS = ParsePrimary();


            // If BinOp binds less tightly with RHS than the operator after RHS, let
            // the pending operator take RHS as its LHS.

            int next_priority = 0;

            if (currentToken.ty == Token::Operator)
                next_priority = Operator::GetPriority(currentToken.GetData<Operator>()->type);

            if (op_priority < next_priority) {
                RHS = ParseBinOpRHS(priority + 1, std::move(RHS));
            }

            // Merge LHS/RHS.
            LHS = std::make_unique<AST::BinaryExprAST>(BinOp, STD move(LHS), STD move(RHS));
        }
    }

    node_t ParseReadExpr() {
        //TODO: insert check code for variable, property, function
    }

    node_t ParsePrimary() {
        switch (currentToken.ty) {
        default: LogError("Unknown token when expecting an expression");
        case Token::Identifier: return ParseReadExpr();
        case Token::Number: return ParseNumberExpr();
        case Token::Operator: return ParseParenExpr();
        }
    }

    /// expression
    ///   ::= primary binoprhs
    ///
    node_t ParseExpression() {
        auto LHS = ParsePrimary();
        return ParseBinOpRHS(0, std::move(LHS));
    }
};