#include "util.h"

#include <atlconv.h> 


std::wstring s2w(const std::string& source) {
    USES_CONVERSION;

    return std::wstring(A2W(source.c_str()));
}