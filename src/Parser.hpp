#pragma once
#include "stdafx.hpp"
#include "Lexer.hpp"
#include "AST.hpp"

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//
class Parser
{
protected:
    Token currentToken;

public:
    std::unique_ptr<llvm::LLVMContext> TheContext;
    std::unique_ptr<llvm::Module> TheModule;
    std::unique_ptr<llvm::IRBuilder<>> Builder;
    std::map<String, llvm::Value *> NamedValues;
    std::unique_ptr<Lexer> lexer;

public:
    Parser()
    {
        init();
    }
    void ParseStream(std::istream &stream)
    {
        lexer = std::make_unique<Lexer>(stream);

        while (true) {
            switch (currentToken.token) {
            case Token::CmdEnd: // ignore top-level semicolons.
                GetNextToken();
                break;
            case Token::Extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
            }
        }
    }

protected:
    void init()
    {
        using namespace llvm;
        // Open a new context and module.
        TheContext = std::make_unique<LLVMContext>();
        TheModule = std::make_unique<Module>("my cool jit", *TheContext);

        // Create a new builder for the module.
        Builder = std::make_unique<IRBuilder<>>(*TheContext);
    }

    Token GetNextToken()
    {
        return currentToken = lexer->GetNextToken();
    }

    std::unique_ptr<ExprAST> ParseExpression();

    /// numberexpr ::= number
    std::unique_ptr<ExprAST> ParseNumberExpr()
    {
        auto Result = std::make_unique<NumberExprAST>(currentToken.str);
        GetNextToken(); // consume the number
        return std::move(Result);
    }

    /// parenexpr ::= '(' expression ')'
    std::unique_ptr<ExprAST> ParseParenExpr()
    {
        GetNextToken(); // eat (.
        auto V = ParseExpression();
        if (!V)
            return nullptr;

        if (currentToken.val != ')')
            return LogError("expected ')'");
        GetNextToken(); // eat ).
        return V;
    }

    /// identifierexpr
    ///   ::= identifier
    ///   ::= identifier '(' expression* ')'
    std::unique_ptr<ExprAST> ParseIdentifierExpr()
    {
        std::wstring IdName = IdentifierStr;

        getNextToken(); // eat identifier.

        if (CurTok.val != '(') // Simple variable ref.
            return std::make_unique<VariableExprAST>(IdName);

        // Call.
        getNextToken(); // eat (
        std::vector<std::unique_ptr<ExprAST>> Args;
        if (CurTok.val != ')')
        {
            while (true)
            {
                if (auto Arg = ParseExpression())
                    Args.push_back(std::move(Arg));
                else
                    return nullptr;

                if (CurTok.val == ')')
                    break;

                if (CurTok.val != ',')
                    return LogError("Expected ')' or ',' in argument list");
                getNextToken();
            }
        }

        // Eat the ')'.
        getNextToken();

        return std::make_unique<CallExprAST>(IdName, std::move(Args));
    }

    /// primary
    ///   ::= identifierexpr
    ///   ::= numberexpr
    ///   ::= parenexpr
    static std::unique_ptr<ExprAST> ParsePrimary()
    {
        switch (CurTok.tok)
        {
        default:
            return LogError("unknown token when expecting an expression");
        case Token::Identifier:
            return ParseIdentifierExpr();
        case Token::Number:
            return ParseNumberExpr();
        case (Token)'(':
            return ParseParenExpr();
        }
    }

    /// binoprhs
    ///   ::= ('+' primary)*
    std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS)
    {
        // If this is a binop, find its precedence.
        while (true)
        {
            int TokPrec = GetTokPrecedence();

            // If this is a binop that binds at least as tightly as the current binop,
            // consume it, otherwise we are done.
            if (TokPrec < ExprPrec)
                return LHS;

            // Okay, we know this is a binop.
            int BinOp = CurTok.val;
            getNextToken(); // eat binop

            // Parse the primary expression after the binary operator.
            auto RHS = ParsePrimary();
            if (!RHS)
                return nullptr;

            // If BinOp binds less tightly with RHS than the operator after RHS, let
            // the pending operator take RHS as its LHS.
            int NextPrec = GetTokPrecedence();
            if (TokPrec < NextPrec)
            {
                RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
                if (!RHS)
                    return nullptr;
            }

            // Merge LHS/RHS.
            LHS =
                std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
        }
    }

    /// expression
    ///   ::= primary binoprhs
    ///
    std::unique_ptr<ExprAST> ParseExpression()
    {
        auto LHS = ParsePrimary();
        if (!LHS)
            return nullptr;

        return ParseBinOpRHS(0, std::move(LHS));
    }

    /// prototype
    ///   ::= id '(' id* ')'
    std::unique_ptr<PrototypeAST> ParsePrototype()
    {
        if (CurTok.tok != Token::Identifier)
            return LogErrorP("Expected function name in prototype");

        std::wstring FnName = IdentifierStr;
        getNextToken();

        if (CurTok.val != '(')
            return LogErrorP("Expected '(' in prototype");

        std::vector<std::wstring> ArgNames;
        while (getNextToken() == Token::Identifier)
            ArgNames.push_back(IdentifierStr);
        if (CurTok.val != ')')
            return LogErrorP("Expected ')' in prototype");

        // success.
        getNextToken(); // eat ')'.

        return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
    }

    /// definition ::= 'def' prototype expression
    std::unique_ptr<FunctionAST> ParseDefinition()
    {
        getNextToken(); // eat def.
        auto Proto = ParsePrototype();
        if (!Proto)
            return nullptr;

        if (auto E = ParseExpression())
            return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
        return nullptr;
    }

    /// toplevelexpr ::= expression
    std::unique_ptr<FunctionAST> ParseTopLevelExpr()
    {
        if (auto E = ParseExpression())
        {
            // Make an anonymous proto.
            auto Proto = std::make_unique<PrototypeAST>("__anon_expr",
                                                        std::vector<std::string>());
            return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
        }
        return nullptr;
    }

    /// external ::= 'extern' prototype
    static std::unique_ptr<PrototypeAST> ParseExtern()
    {
        getNextToken(); // eat extern.
        return ParsePrototype();
    }
};