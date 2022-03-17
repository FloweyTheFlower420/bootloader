#include <term_formatting.h>

namespace term
{
    bool font::test_pixel(char ch, style s, uint8_t x, uint8_t y) const
    {
        if(fonts[s] == nullptr)
            return this->notfound;
       
        size_t index = (size_t)width() * (size_t)y + x;
        return ((uint8_t*)fonts[s])[bpc() * ch + (index >> 3)] & (1 << (index & 7));
    }
}
