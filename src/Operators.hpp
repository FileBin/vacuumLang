#pragma once
#include "stdafx.hpp"
namespace Constant {
    CEXPRCSTR operators[] = {

        "+",    //Add
        "-",    //Sub,
        "*",    //Mul,
        "/",    //Div,
        "%",    //Mod,

        "++",    //Incr,
        "--",    //Decr,

        //bit
        "<<",    //BitMoveL,
        ">>",    //BitMoveR,
        "&",    //BitAnd,
        "|",    //BitOr,
        "~",    //BitNot,
        "^",    //BitXor,

        //boolean
        "&&",    //And,
        "||",    //Or,
        "!",    //Not,

        //compare
        ">",    //Greater,
        "<",    //Less,
        "==",    //Equal,
        ">=",    //GreaterEqual,
        "<=",    //LessEqual,
        "!=",    //NotEqual,

        //assign
        "=",    //Assign,
        "+=",    //AssignAdd,
        "-=",    //AssignSub,
        "*=",    //AssignMul,
        "/=",    //AssignDiv,
        "%=",    //AssignMod,

        "<<=",    //AssignBitMoveL,
        ">>=",    //AssignBitMoveR,
        "&=",    //AssignBitAnd,
        "|=",    //AssignBitOr,
        "~=",    //AssignBitNot,
        "^=",    //AssignBitXor,

        //Brackets
        "(",    //BracketOpen,
        ")",    //BracketClose,
        "[",    //SqBracketOpen,  //[
        "]",    //SqBracketClose, //]

        //Other
        ".",    //Dot,
        "?",    //Ternar,
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

        //Other
        Dot,
        Ternar,
    } ty;

    int priority = 1;
public:
    Operator(Enum e = Unknown) {
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

        case SqBracketOpen:
        case SqBracketClose:
            return 15;

        case BracketOpen:
        case BracketClose:
            return 16;

        default:
            return -(int)op;
            break;
        }
    }
    size_t GetLen() { return strlen(Constant::operators[(size_t)ty]); }

    String toString() override {
        switch (ty)
        {
        case Assign: return "Assign";
        case AssignAdd: return "AssignAdd";
        case AssignSub: return "AssignSub";
        case AssignMul: return "AssignMul";
        case AssignMod: return "AssignMod";
        case AssignBitMoveL: return "AssignBitMoveL";
        case AssignBitMoveR: return "AssignBitMoveR";
        case AssignBitAnd: return "AssignBitAnd";
        case AssignBitOr: return "AssignBitOr";
        case AssignBitNot: return "AssignBitNot";
        case AssignBitXor: return "AssignBitXor";
        case Ternar: return "Ternar";
        case Or: return "Or";
        case And: return "And";
        case BitOr: return "BitOr";
        case BitXor: return "BitXor";
        case BitAnd: return "BitAnd";
        case Equal: return "Equal";
        case NotEqual: return "NotEqual";
        case Greater: return "Greater";
        case GreaterEqual: return "GreaterEqual";
        case Less: return "Less";
        case LessEqual: return "LessEqual";
        case BitMoveL: return "BitMoveL";
        case BitMoveR: return "BitMoveR";
        case Add: return "Add";
        case Sub: return "Sub";
        case Mul: return "Mul";
        case Div: return "Div";
        case Mod: return "Mod";
        case Incr: return "Incr";
        case Decr: return "Decr";
        case Not: return "Not";
        case BitNot: return "BitNot";
        case Dot: return "Dot";
        case BracketOpen: return "BracketOpen";
        case BracketClose: return "BracketClose";
        case SqBracketOpen: return "SqBracketOpen";
        case SqBracketClose: return "SqBracketClose";

        default: return "Unknown";
        }
    }

    static bool TryParse(String str, Operator*& op) {
        using namespace Constant;

        Enum ty = Unknown;
        size_t len = 0;
        for (size_t i = 0; i < ARRSIZE(operators); i++) {
            auto l = strlen(operators[i]);
            if (len < l && str.substr(0, l) == String(operators[i])) {
                len = l;
                ty = (Enum)i;
            }
        }
        op = new Operator(ty);
        return ty != Unknown;
    }
};