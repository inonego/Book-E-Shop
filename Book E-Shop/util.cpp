#include "util.h"

#include <atlconv.h> 

using namespace ATL; 

std::wstring s2w(const std::string& source) {
    USES_CONVERSION;

    return std::wstring(A2W(source.c_str()));
}