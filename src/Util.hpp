#pragma once
#include <string>
#include "stdafx.hpp"
#include <codecvt>
#include <locale>
#ifdef WIN32
#include <Windows.h>
#endif

struct String;

typedef STD basic_string<char_t> std_string;

interface IPrintable {
    virtual String ToString() = 0;
};

struct String : public std_string, public IPrintable {
    String() : std_string() {};
    String(const wchar_t* wstr) : std_string(wstr) {}
    String(const std_string& wstr) : std_string(wstr) {}
    String ToString() override {
        return *this;
    }
};

template<typename T>
class Tree {
public:
    struct Node;
    typedef  STD shared_ptr<Node> PNode;
    struct Node {
    private:
        PNode parent;
        STD vector<PNode> children;
    public:

        T data;
        void AddChild(T d) {
            children.push_back(STD make_shared<Node>({ this, {}, d }));
        }

        bool RemoveChild(T d) {
            for (auto it = children.begin(); it != children.end(); it++) {
                if ((*it)->data == d) {
                    children.erase(it);
                    return true;
                }
            }
            return false;
        }
    };

    PNode root;

    Tree(T rootData = {}) {
        root = STD make_shared<Node>();
        root->data = rootData;
    }

    PNode find(T data) {
        STD vector<PNode> l = { root }, buf;
        do {
            buf.clear();
            for (const PNode& node : l) {
                if (node->data == data) {
                    return node;
                }
                buf.insert(node->children.begin(), node->children.end());
            }
            l = buf;
        } while (!buf.empty());
        return nullptr;
    }
};

String Utf8ToWstring(const std::string& str) {
#ifdef __linux__
    std::wstring_convert<std::codecvt_utf8<char_t>> utf8_conv;
    return utf8_conv.from_bytes(str);
#endif
#ifdef WIN32
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
#endif
}

std::string ToStdString(const String& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
    return utf8_conv.to_bytes(str);
}