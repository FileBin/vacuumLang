#pragma once
#include "stdafx.hpp"
#include <stdexcept>

/// LogError - This is little helper functions for error handling.
void LogError(String Str) {
    throw Str;
}