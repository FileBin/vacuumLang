#pragma once
#include "stdafx.hpp"
#include "ASTheader.h"
#include "Operators.hpp"
#include "Types.h"

class Parser;

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

namespace AST {
    interface IASTNode {
        virtual llvm::Value* GenCode() = 0;
    };
    /// ExprAST - Base class for all expression nodes.
    class ExprAST : public IASTNode {
    public:
        STD shared_ptr<Parser> parser;
        ExprAST(Parser *parser) : parser(parser) {}
        virtual ~ExprAST() = default;
    };

    /// NumberExprAST - Expression class for numeric literals like "1.0".
    class NumberExprAST : public ExprAST
    {
        Type type;
        String str;

    public:
        NumberExprAST(Parser *parser, String str) : ExprAST(parser), str(str)
        {
            type = Type::Long;

            if (str.find('e') || str.find('.'))
            {
                type = Type::LongDbl;
            }
        }

        llvm::Value *GenCode() override;
    };

    /// VariableExprAST - Expression class for referencing a variable, like "a".
    class VariableExprAST : public ExprAST
    {
        String Name;

    public:
        VariableExprAST(Parser *parser, const String &Name) : ExprAST(parser), Name(Name) {}

        llvm::Value *GenCode() override;
    };

    /// BinaryExprAST - Expression class for a binary operator.
    class BinaryExprAST : public ExprAST
    {
        Operator Op;
        STD unique_ptr<ExprAST> LHS, RHS;

    public:
        BinaryExprAST(Parser *parser, Operator Op, STD unique_ptr<ExprAST> LHS,
                      STD unique_ptr<ExprAST> RHS)
            : ExprAST(parser), Op(Op), LHS(STD move(LHS)), RHS(STD move(RHS)) {}

        llvm::Value *GenCode() override;
    };

    /// CallExprAST - Expression class for function calls.
    class CallExprAST : public ExprAST
    {
        String Callee;
        STD vector<STD unique_ptr<ExprAST>> Args;

    public:
        CallExprAST(Parser *parser, const String &Callee,
                    STD vector<STD unique_ptr<ExprAST>> Args)
            : ExprAST(parser), Callee(Callee), Args(STD move(Args)) {}

        llvm::Value *GenCode() override;
    };

    /// PrototypeAST - This class represents the "prototype" for a function,
    /// which captures its name, and its argument names (thus implicitly the number
    /// of arguments the function takes).
    class PrototypeAST : IASTNode
    {
        STD shared_ptr<Parser> parser;
        String Name;
        STD vector<String> Args;

    public:
        PrototypeAST(Parser *parser, const String &Name, STD vector<String> Args)
            : parser(parser), Name(Name), Args(STD move(Args)) {}

        llvm::Function *GenCode();
        const String &getName() const { return Name; }
    };

    /// FunctionAST - This class represents a function definition itself.
    class FunctionAST : IASTNode
    {
        STD shared_ptr<Parser> parser;
        STD unique_ptr<PrototypeAST> Proto;
        STD unique_ptr<ExprAST> Body;

    public:
        FunctionAST(Parser *parser, STD unique_ptr<PrototypeAST> Proto,
                    STD unique_ptr<ExprAST> Body)
            : parser(parser), Proto(STD move(Proto)), Body(STD move(Body)) {}

        llvm::Function *GenCode();
    };

//===----------------------------------------------------------------------===//
// Code Generation
//===----------------------------------------------------------------------===//

#include "Parser.hpp"

    llvm::Value *NumberExprAST::GenCode()
    {
        using namespace llvm;
        auto s = ToStdString(str);
        switch (type.GetEnum())
        {
        case ::Type::Long:
            return ConstantInt::get(*parser->TheContext, APSInt(s));
        case ::Type::Dbl:
            return llvm::ConstantFP::get(*parser->TheContext, APFloat(APFloat::IEEEdouble(), s));

        default:
            return ::LogErrorV("Unknown number type");
        }
    }

    llvm::Value *VariableExprAST::GenCode()
    {
        // Look this variable up in the function.
        llvm::Value *V = parser->NamedValues[Name];
        if (!V)
            return LogErrorV("Unknown variable name");
        return V;
    }

    llvm::Value *BinaryExprAST::GenCode()
    {
        llvm::Value *L = LHS->GenCode();
        llvm::Value *R = RHS->GenCode();
        if (!L || !R)
            return nullptr;

        switch (Op.type)
        {
        case Operator::Add:
            return parser->Builder->CreateFAdd(L, R, "addtmp");
        case Operator::Sub:
            return parser->Builder->CreateFSub(L, R, "subtmp");
        case Operator::Mul:
            return parser->Builder->CreateFMul(L, R, "multmp");
        case Operator::Less:
            L = parser->Builder->CreateFCmpULT(L, R, "cmptmp");
            // Convert bool 0/1 to double 0.0 or 1.0
            return parser->Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*parser->TheContext), "booltmp");
        default:
            return LogErrorV("invalid binary operator");
        }
    }

    llvm::Value *CallExprAST::GenCode()
    {
        // Look up the name in the global module table.
        llvm::Function *CalleeF = parser->TheModule->getFunction(ToStdString(Callee));
        if (!CalleeF)
            return LogErrorV("Unknown function referenced");

        // If argument mismatch error.
        if (CalleeF->arg_size() != Args.size())
            return LogErrorV("Incorrect # arguments passed");

        std::vector<llvm::Value *> ArgsV;
        for (unsigned i = 0, e = Args.size(); i != e; ++i)
        {
            ArgsV.push_back(Args[i]->GenCode());
            if (!ArgsV.back())
                return nullptr;
        }

        return parser->Builder->CreateCall(CalleeF, ArgsV, "calltmp");
    }

    llvm::Function *PrototypeAST::GenCode()
    {
        using namespace llvm;
        using llvm::Type;
        // Make the function type:  double(double,double) etc.
        std::vector<Type *> Doubles(Args.size(), Type::getDoubleTy(*parser->TheContext));
        FunctionType *FT =
            FunctionType::get(Type::getDoubleTy(*parser->TheContext), Doubles, false);

        Function *F =
            Function::Create(FT, Function::ExternalLinkage, ToStdString(Name), parser->TheModule.get());

        // Set names for all arguments.
        unsigned Idx = 0;
        for (auto &Arg : F->args())
            Arg.setName(ToStdString(Args[Idx++]));

        return F;
    }

    llvm::Function *FunctionAST::GenCode() {
        using namespace llvm;
        // First, check for an existing function from a previous 'extern' declaration.
        llvm::Function *TheFunction = parser->TheModule->getFunction(ToStdString(Proto->getName()));

        if (!TheFunction)
            TheFunction = Proto->GenCode();

        if (!TheFunction)
            return nullptr;

        // Create a new basic block to start insertion into.
        BasicBlock *BB = BasicBlock::Create(*parser->TheContext, "entry", TheFunction);
        parser->Builder->SetInsertPoint(BB);

        // Record the function arguments in the NamedValues map.
        parser->NamedValues.clear();
        for (auto &Arg : TheFunction->args())
            parser->NamedValues[Utf8ToWstring(std::string(Arg.getName()))] = &Arg;

        if (llvm::Value *RetVal = Body->GenCode())
        {
            // Finish off the function.
            parser->Builder->CreateRet(RetVal);

            // Validate the generated code, checking for consistency.
            verifyFunction(*TheFunction);

            return TheFunction;
        }

        // Error reading body, remove function.
        TheFunction->eraseFromParent();
        return nullptr;
    }
}
// static void HandleDefinition() {
//     if (auto FnAST = ParseDefinition())
//     {
//         if (auto *FnIR = FnAST->codegen())
//         {
//             fprintf(stderr, "Read function definition:");
//             FnIR->print(errs());
//             fprintf(stderr, "\n");
//         }
//     } else {
//         // Skip token for error recovery.
//         getNextToken();
//     }
// }

// static void HandleExtern() {
//     if (auto ProtoAST = ParseExtern()) {
//         if (auto *FnIR = ProtoAST->codegen()) {
//             fprintf(stderr, "Read extern: ");
//             FnIR->print(errs());
//             fprintf(stderr, "\n");
//         }
//     } else {
//         // Skip token for error recovery.
//         getNextToken();
//     }
// }

// static void HandleTopLevelExpression() {
//     // Evaluate a top-level expression into an anonymous function.
//     if (auto FnAST = ParseTopLevelExpr()) {
//         if (auto *FnIR = FnAST->codegen()) {
//             fprintf(stderr, "Read top-level expression:");
//             FnIR->print(errs());
//             fprintf(stderr, "\n");

//             // Remove the anonymous expression.
//             FnIR->eraseFromParent();
//         }
//     } else {
//         // Skip token for error recovery.
//         getNextToken();
//     }
// }