#pragma once
#include "stdafx.hpp"
#include <stdexcept>

/// LogError* - These are little helper functions for error handling.
void LogError(String Str) {
    throw Str;
}