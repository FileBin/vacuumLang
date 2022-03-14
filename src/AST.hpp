#pragma once
#include "stdafx.hpp"
#include "ASTheader.h"
#include "Operators.hpp"
#include "Keyword.hpp"
#include "Types.hpp"
#include "Metadata.hpp"
class Parser;

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

namespace AST {
    using ::Parser;
    interface IASTNode {
        virtual llvm::Value* codeGen(Metadata* pmeta) = 0;
    };
    /// ExprAST - Base class for all expression nodes.
    class ExprAST : public IASTNode {
    public:
        STD shared_ptr<Parser> parser;
        ExprAST(Parser* parser) : parser(parser) {}
        virtual ~ExprAST() = default;
    };

    /// NumberExprAST - Expression class for numeric literals like "1.0".
    class NumberExprAST : public ExprAST
    {
        Type::Enum ty;
        String str;

    public:
        NumberExprAST(Parser* parser, String str) : ExprAST(parser), str(str) {
            ty = Type::Num;

            if (str.find('e') || str.find('.')) {
                ty = Type::Dbl;
            }
        }

        llvm::Value* codeGen(Metadata* pmeta) override;
    };

    /// VariableExprAST - Expression class for referencing a variable, like "a".
    class VariableExpression : public ExprAST {
        Type* type;
        String name;

    public:
        VariableExpression(Parser* parser, String name, Type* type) : ExprAST(parser), name(name), type(type) {}

        llvm::Value* codeGen(Metadata* pmeta) override;
    };

    class ClassExpression : public ExprAST {
        String name;
        STD vector<Keyword> modifiers;
        Type* super;

    public:
        ClassExpression(Parser* parser, String name, Type* super_ty, STD vector<Keyword> modifiers = {})
            : ExprAST(parser), name(name), modifiers(modifiers), super(super_ty) {}

        llvm::Value* codeGen(Metadata* pmeta) override;
    };

    class InterfaceExpression : public ExprAST {
        String name;
        STD vector<Keyword> modifiers;
        Type* super;

    public:
        InterfaceExpression(Parser* parser, String name, Type* super_ty, STD vector<Keyword> modifiers = {})
            : ExprAST(parser), name(name), modifiers(modifiers), super(super_ty) {}

        llvm::Value* codeGen(Metadata* pmeta) override;
    };

    /// BinaryExprAST - Expression class for a binary operator.
    class BinaryExprAST : public ExprAST
    {
        Operator Op;
        STD unique_ptr<ExprAST> LHS, RHS;

    public:
        BinaryExprAST(Parser* parser, Operator Op, STD unique_ptr<ExprAST> LHS,
            STD unique_ptr<ExprAST> RHS)
            : ExprAST(parser), Op(Op), LHS(STD move(LHS)), RHS(STD move(RHS)) {}

        llvm::Value* codeGen(Metadata* pmeta) override;
    };

    /// CallExprAST - Expression class for function calls.
    class CallExprAST : public ExprAST
    {
        String Callee;
        STD vector<STD unique_ptr<ExprAST>> Args;

    public:
        CallExprAST(Parser* parser, const String& Callee,
            STD vector<STD unique_ptr<ExprAST>> Args)
            : ExprAST(parser), Callee(Callee), Args(STD move(Args)) {}
        llvm::Value* codeGen(Metadata* pmeta) override;
    };

    /// PrototypeAST - This class represents the "prototype" for a function,
    /// which captures its name, and its argument names (thus implicitly the number
    /// of arguments the function takes).
    class PrototypeAST : IASTNode
    {
        STD shared_ptr<Parser> parser;
        String name;
        STD vector<::Variable> args;

    public:
        PrototypeAST(Parser* parser, const String& name, STD vector<::Variable> args)
            : parser(parser), name(name), args(STD move(args)) {}

        llvm::Function* codeGen(Metadata* pmeta) override;
        const String& getName() const { return name; }
    };

    /// FunctionAST - This class represents a function definition itself.
    class FunctionAST : IASTNode
    {
        STD shared_ptr<Parser> parser;
        STD unique_ptr<PrototypeAST> Proto;
        STD unique_ptr<ExprAST> Body;

    public:
        FunctionAST(Parser* parser, STD unique_ptr<PrototypeAST> Proto,
            TokenBufferStream body)
            : parser(parser), Proto(STD move(Proto)), Body(STD move(Body)) {}

        llvm::Function* codeGen(Metadata* pmeta) override;
    };
}

#include "Parser.hpp"


//===----------------------------------------------------------------------===//
// Code Generation
//===----------------------------------------------------------------------===//
namespace AST {
    llvm::Value* NumberExprAST::codeGen(Metadata* pmeta) {
        using namespace llvm;
        auto s = ToStdString(str);
        switch (ty)
        {
        case ::Type::Num:
            return ConstantInt::get(*parser->TheContext,
                APInt(8 * ::Type::getInstance(pmeta, ::Type::Num)->getByteSize(), s, 10));
        case ::Type::Dbl:
            return llvm::ConstantFP::get(*parser->TheContext, APFloat(APFloat::IEEEdouble(), s));

        default:
            logError("Unknown number type");
            return 0;
        }
    }

    llvm::Value* VariableExpression::codeGen(Metadata* pmeta)
    {
        // Look this variable up in the function.
        llvm::Value* V = parser->NamedValues[name];
        if (!V)
            logError("Unknown variable name");
        return V;
    }

    llvm::Value* BinaryExprAST::codeGen(Metadata* pmeta)
    {
        llvm::Value* L = LHS->codeGen(pmeta);
        llvm::Value* R = RHS->codeGen(pmeta);
        if (!L || !R)
            return nullptr;

        switch (Op.ty)
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
            logError("invalid binary operator");
            return 0;
        }
    }

    llvm::Value* CallExprAST::codeGen(Metadata* pmeta)
    {
        // Look up the name in the global module table.
        llvm::Function* CalleeF = parser->TheModule->getFunction(ToStdString(Callee));
        if (!CalleeF)
            logError("Unknown function referenced");

        // If argument mismatch error.
        if (CalleeF->arg_size() != Args.size())
            logError("Incorrect # arguments passed");

        std::vector<llvm::Value*> ArgsV;
        for (unsigned i = 0, e = Args.size(); i != e; ++i)
        {
            ArgsV.push_back(Args[i]->codeGen(pmeta));
            if (!ArgsV.back())
                return nullptr;
        }

        return parser->Builder->CreateCall(CalleeF, ArgsV, "calltmp");
    }

    llvm::Function* PrototypeAST::codeGen(Metadata* pmeta)
    {
        using namespace llvm;
        using llvm::Type;
        using llvm::Function;
        // Make the function type:  double(double,double) etc.
        std::vector<Type*> Doubles(args.size(), Type::getDoubleTy(*parser->TheContext));
        FunctionType* FT =
            FunctionType::get(Type::getDoubleTy(*parser->TheContext), Doubles, false);

        Function* F =
            Function::Create(FT, Function::ExternalLinkage, ToStdString(name), parser->TheModule.get());

        // Set names for all arguments.
        unsigned idx = 0;
        for (auto& arg : F->args()) {
            arg.setName(ToStdString(args[idx++].getName()));
        }
        return F;
    }

    llvm::Function* FunctionAST::codeGen(Metadata* pmeta) {
        using namespace llvm;
        // First, check for an existing function from a previous 'extern' declaration.
        llvm::Function* TheFunction = parser->TheModule->getFunction(ToStdString(Proto->getName()));

        if (!TheFunction)
            TheFunction = Proto->codeGen(pmeta);

        if (!TheFunction)
            return nullptr;

        // Create a new basic block to start insertion into.
        BasicBlock* BB = BasicBlock::Create(*parser->TheContext, "entry", TheFunction);
        parser->Builder->SetInsertPoint(BB);

        // Record the function arguments in the NamedValues map.
        parser->NamedValues.clear();
        for (auto& Arg : TheFunction->args())
            parser->NamedValues[Utf8ToWstring(std::string(Arg.getName()))] = &Arg;

        if (llvm::Value* RetVal = Body->codeGen(pmeta))
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