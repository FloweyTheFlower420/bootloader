#ifndef __LIB_TERM_H__
#define __LIB_TERM_H__
#include <cstddef>
#include <cstdint>
#include <term_formatting.h>

namespace term
{
    // video mode operations
    class video_buffer
    {
    public:
        virtual void draw(const rgb&, uint32_t);
        virtual void height() const;
        virtual void width() const;
        virtual void scroll(size_t) const;
    };

    void set_video_buffer(video_buffer*);
    void set_font(void* fontbuffer, uint8_t width, uint8_t height, style s);

    uint8_t font_height();
    uint8_t font_width();

    uint8_t screen_height();
    uint8_t screen_width();

    inline uint8_t lines() { return screen_height() / font_height(); }
    inline uint8_t columns() { return screen_width() / screen_height(); }

    // draws text, at char position x y (pixels)
    void draw(size_t x, size_t y, char ch, const rgb& color);
    // draws text, at char position line col
    void draw_lc(size_t col, size_t line, char ch, const rgb& color);
    void draw(size_t x, size_t y, const rgb& rgb);
    
    void scrolldown(size_t n);

    // cool functions i guess
    void putc(char ch);

    inline void putstr(const char* ch) 
    {
        while(*ch)
            putc(*ch++);
    }

    // get terminal color
    term_status& status();
}

#endif
