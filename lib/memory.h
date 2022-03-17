#ifndef __MEMORY_H__
#define __MEMORY_H__
#include <cstddef>
#include <cstdint>
#include <libc.h>
#include <config.h>
#include MALLOC_IMPL_PATH

class mregion
{
protected:
    void* buf;
public:
    inline mregion(void* buf) : buf(buf) {}
    template<typename T>
    inline T* data() { return (T*)buf; }
    inline void copy(mregion& rhs, size_t n) { memcpy(rhs.buf, buf, n); }
    inline void set(uint8_t v, size_t n) { memset(buf, v, n); }
};

class mbuffer final : public mregion
{
    size_t n;
public:
    inline mbuffer(size_t n) : mregion(alloc::malloc(n)), n(n) {}
    mbuffer(const mbuffer& rhs) = delete;
    inline mbuffer(mbuffer&& rhs) : mregion(rhs.buf) { rhs.buf = nullptr; }
    inline ~mbuffer() { alloc::free(buf); }
    inline void copy(mbuffer& rhs) { mregion::copy(rhs, min(n, rhs.n)); }
    inline void set(uint8_t v) { mregion::set(v, n); }
};

#endif
