#ifndef __LIB_DEBUG_H__
#define __LIB_DEBUG_H__
#include <cstddef>

using backtrace_callback_t = void (*)(size_t);
static_assert(sizeof(size_t) == 4, "Why are we not actually compiling 32 bit code?");
void backtrace(size_t* ptr, backtrace_callback_t callback);

#endif
