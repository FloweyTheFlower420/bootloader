#ifndef __LIB_TERM_FORMATTING_H__
#define __LIB_TERM_FORMATTING_H__
#include <cstdint>
#include <cstddef>

namespace term
{
    struct rgb
    {
        uint8_t r, g, b;
    };

    enum style
    {
        NORMAL,
        BOLD,
        ITALIC,
        BOLD_ITALIC // BOLD | ITALIC == BOLD_ITALIC
    };

    struct term_status
    {
        style text_style;
        size_t cursor_x;
        size_t cursor_y;
        rgb fg;
        rgb bg;
    };

    // class that represents a font
    // the api to set a font is pretty straightforward
    class font
    {
        void* fonts[4];
        const void* notfound;
        const uint8_t font_width;
        const uint8_t font_height;
    public:
        constexpr font(void* notfound, uint8_t w, uint8_t h)
            : notfound(notfound),
              font_width(w),
              font_height(h),
              fonts{nullptr, nullptr, nullptr, nullptr} {}
        constexpr font(void* notfound, uint8_t w, uint8_t h, void* n, void* b, void* i, void* bi)
            : notfound(notfound),
              font_width(w),
              font_height(h),
              fonts{n, b, i, bi} {}
        constexpr font(const font& f)
            : fonts{f.fonts[0], f.fonts[1], f.fonts[2], f.fonts[3]},
              notfound(f.notfound),
              font_width(f.font_width),
              font_height(f.font_height) {}
        constexpr font(font&& f)
            : fonts{f.fonts[0], f.fonts[1], f.fonts[2], f.fonts[3]},
              notfound(f.notfound),
              font_width(f.font_width),
              font_height(f.font_height) {}

        constexpr uint8_t width() const { return font_width; }
        constexpr uint8_t height() const { return font_height; }
        constexpr size_t bpc() const { return font_width * font_height; }
        constexpr void set_font(void* font, style s) { fonts[s] = font; }
        bool test_pixel(char ch, style s, uint8_t x, uint8_t y) const; // should print the notfound symbol
    };
}

#endif
