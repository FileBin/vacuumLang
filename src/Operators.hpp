#pragma once
#include "stdafx.hpp"
namespace Constant {
    CEXPRSTR operators[] = {
        
        L"+", //Add
        L"-",    //Sub,
        L"*",    //Mul,
        L"/",    //Div,
        L"%",    //Mod,

        L"++",    //Incr,
        L"--",    //Decr,

        //bit
        L"<<",    //BitMoveL,
        L">>",    //BitMoveR,
        L"&",    //BitAnd,
        L"|",    //BitOr,
        L"~",    //BitNot,
        L"^",    //BitXor,

        //boolean
        L"&&",    //And,
        L"||",    //Or,
        L"!",    //Not,

        //compare
        L">",    //Greater,
        L"<",    //Less,
        L"==",    //Equal,
        L">=",    //GreaterEqual,
        L"<=",    //LessEqual,
        L"!=",    //NotEqual,

        //assign
        L"=",    //Assign,
        L"+=",    //AssignAdd,
        L"-=",    //AssignSub,
        L"*=",    //AssignMul,
        L"/=",    //AssignDiv,
        L"%=",    //AssignMod,

        L"<<=",    //AssignBitMoveL,
        L">>=",    //AssignBitMoveR,
        L"&=",    //AssignBitAnd,
        L"|=",    //AssignBitOr,
        L"~=",    //AssignBitNot,
        L"^=",    //AssignBitXor,

        //Brackets
        L"(",    //BracketOpen,
        L")",    //BracketClose,
        L"[",    //SqBracketOpen,  //[
        L"]",    //SqBracketClose, //]
        L"{",    //BraceOpen,      //{
        L"}",    //BraceClose,     //}

        //Other
        L".",    //Dot,
        L"?",    //Ternar,
    };
}

class Operator : public IPrintable {
    public:
    enum Enum {
        Unknown = -1,

        //arifmetic
        Add = 0,
        Sub,
        Mul,
        Div,
        Mod,

        Incr,
        Decr,

        //bit
        BitMoveL,
        BitMoveR,
        BitAnd,
        BitOr,
        BitNot,
        BitXor,

        //boolean
        And,
        Or,
        Not,

        //compare
        Greater,
        Less,
        Equal,
        GreaterEqual,
        LessEqual,
        NotEqual,

        //assign
        Assign,
        AssignAdd,
        AssignSub,
        AssignMul,
        AssignDiv,
        AssignMod,

        AssignBitMoveL,
        AssignBitMoveR,
        AssignBitAnd,
        AssignBitOr,
        AssignBitNot,
        AssignBitXor,

        //Brackets
        BracketOpen,    //(
        BracketClose,   //)
        SqBracketOpen,  //[
        SqBracketClose, //]
        BraceOpen,      //{
        BraceClose,     //}

        //Other
        Dot,
        Ternar,
    } ty;

    int priority = 1;
    public:
    Operator(Enum e = Unknown){
        ty = e;
        priority = GetPriority(e);
    }

    static bool hasBinaryForm(Enum op) {
        switch (op) {
        case Add:
        case Sub:
        case Mul:
        case Div:
        case Mod:

        case BitMoveL:
        case BitMoveR:
        case BitAnd:
        case BitOr:
        case BitXor:

        case And:
        case Or:

        case Assign:
        case AssignAdd:
        case AssignSub:
        case AssignMul:
        case AssignDiv:
        case AssignMod:

        case AssignBitMoveL:
        case AssignBitMoveR:
        case AssignBitAnd:
        case AssignBitOr:
        case AssignBitNot:
        case AssignBitXor:

        case Dot:
            return true;
        
        default:
            return false;
        }
    }

    static int GetPriority(Enum op) {
        switch (op)
        {
        case Assign:
        case AssignAdd:
        case AssignSub:
        case AssignMul:
        case AssignMod:
        case AssignBitMoveL:
        case AssignBitMoveR:
        case AssignBitAnd:
        case AssignBitOr:
        case AssignBitNot:
        case AssignBitXor:
            return 1;

        case Ternar:
            return 2;

        case Or:
            return 3;

        case And:
            return 4;

        case BitOr:
            return 5;

        case BitXor:
            return 6;

        case BitAnd:
            return 7;

        case Equal:
        case NotEqual:
            return 8;

        case Greater:
        case GreaterEqual:
        case Less:
        case LessEqual:
            return 9;

        case BitMoveL:
        case BitMoveR:
            return 10;
        
        case Add:
        case Sub:
            return 11;

        case Mul:
        case Div:
        case Mod:
            return 12;

        case Incr:
        case Decr:
        case Not:
        case BitNot:
            return 13;
        case Dot:
            return 14;

        case BraceOpen:
        case BraceClose:
            return 15;

        case SqBracketOpen:
        case SqBracketClose:
            return 16;
                
        case BracketOpen:
        case BracketClose:
            return 17;
        
        default:
            return -(int)op;
            break;
        }
    }
    size_t GetLen() { return LEN(Constant::operators[(size_t)ty]); }

    String toString() override {
        switch (ty)
        {
        case Assign: return L"Assign";
        case AssignAdd: return L"AssignAdd";
        case AssignSub: return L"AssignSub";
        case AssignMul: return L"AssignMul";
        case AssignMod: return L"AssignMod";
        case AssignBitMoveL: return L"AssignBitMoveL";
        case AssignBitMoveR: return L"AssignBitMoveR";
        case AssignBitAnd: return L"AssignBitAnd";
        case AssignBitOr: return L"AssignBitOr";
        case AssignBitNot: return L"AssignBitNot";
        case AssignBitXor: return L"AssignBitXor";
        case Ternar: return L"Ternar";
        case Or: return L"Or";
        case And: return L"And";
        case BitOr: return L"BitOr";
        case BitXor: return L"BitXor";
        case BitAnd: return L"BitAnd";
        case Equal: return L"Equal";
        case NotEqual: return L"NotEqual";
        case Greater: return L"Greater";
        case GreaterEqual: return L"GreaterEqual";
        case Less: return L"Less";
        case LessEqual: return L"LessEqual";
        case BitMoveL: return L"BitMoveL";
        case BitMoveR: return L"BitMoveR";
        case Add: return L"Add";
        case Sub: return L"Sub";
        case Mul: return L"Mul";
        case Div: return L"Div";
        case Mod: return L"Mod";
        case Incr: return L"Incr";
        case Decr: return L"Decr";
        case Not: return L"Not";
        case BitNot: return L"BitNot";
        case Dot: return L"Dot";
        case BracketOpen: return L"BracketOpen";
        case BracketClose: return L"BracketClose";
        case SqBracketOpen: return L"SqBracketOpen";
        case SqBracketClose: return L"SqBracketClose";
        case BraceOpen: return L"BraceOpen";
        case BraceClose: return L"BraceClose";
        
        default: return L"Unknown";
        }
    }

    static bool TryParse(String str, Operator* &op){
        using namespace Constant;

        Enum ty = Unknown;
        size_t len = 0;
        for(size_t i=0; i<ARRSIZE(operators); i++){
            if(len < LEN(operators[i]) && str.substr(0, LEN(operators[i])) == operators[i]){
                len = LEN(operators[i]);
                ty = (Enum)i;
            }
        }
        op = new Operator(ty);
        return ty != Unknown;
    }
};