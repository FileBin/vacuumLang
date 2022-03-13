#pragma once
#include "stdafx.hpp"

class ConsoleApplication {
public:
    typedef void* mainFunc(ConsoleApplication&);
protected:
    String programName;
    STD map<String, STD vector<String>> parameters;
public:

    ConsoleApplication(int argc, const char* argv[], mainFunc func) {
        programName = argv[0];
        if (argc > 1) {
            String param_name = argv[1];
            if (param_name[0] != '-') {
                Log("Parameter expected!");
            }
            for (size_t i = 0;i < argc;i++) {
                const char* arg = argv[i];
                if (arg[0] == '-') {
                    param_name = arg[0];
                    parameters[param_name] = {};
                } else {
                    parameters[param_name].push_back(arg);
                }
            }
        }
        func();
    }

    void Log(IPrintable o) {
        PRINT(L"[clock:%d]"STRPARAM"\n", clock(), o.ToString().c_str());
    }
}