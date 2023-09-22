#include "panic.h"

#include <cstdio>
#include <cstdlib>

namespace panic {
//--------------------------------------------------------------------------------------------------
// panic_impl
//--------------------------------------------------------------------------------------------------
[[noreturn]] void panic_impl(const char* s) noexcept {
    std::fputs(s, stderr);
    std::abort();
}
}  // namespace panic
