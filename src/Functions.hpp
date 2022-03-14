#pragma once
#include "stdafx.hpp"
#include <stdexcept>

/// logError - This is little helper functions for error handling.
void logError(String str) noexcept(false) {
    PRINT(SPREF "Error: " STRPARAM, str);
    throw str.c_str();
}

void logWarning(String str) {
    PRINT(SPREF "Warning: " STRPARAM, str);
}