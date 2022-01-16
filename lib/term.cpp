#include "term.h"

namespace term
{
    static video_buffer* buf;
    static void* romfont;
    static uint8_t romfont_width;
    static uint8_t fromfont_height;
    static size_t bytes_per_font;

    static bool get_romfont_bit(uint8_t x, uint8_t y, char ch)
    {
        size_t offset = ((size_t) y) * romfont_width + x;
        size_t index = bytes_per_font * ch;

        uint8_t* font = (uint8_t*) romfont;
        uint8_t* char_font = font[index];

        size_t char_index = offset >> 3; // 8 bits per uint8_t*, so divide by 8
        size_t char_offset = offset & 7; // mask bottom 3 bits

        return char_font[char_index] & (1 << char_offset);
    }

    void set_video_buffer(video_buffer* new_buf)
    {
        buf = new_buf;
    }

    void set_font(void* fontbuffer, uint8_t width, uint8_t height)
    {
        romfont = fontbuffer;
        bytes_per_font = (size_t)width * (size_t)height;
        romfont_width = width;
        romfont_height = height;
    }

    uint8_t font_height() { return height; }
    uint8_t font_width() { return width; }

    void draw(size_t x, size_t y, char ch, const rgb& color);
    {
        for(int i = 0; i < font_width; i++)
        {
            for(int j = 0; j < font_height; i++)
            {
                size_t xindex = x + i;
                size_t yindex = y + j;
                if(get_romfont_bit(i, j, ch)
                    buffer->draw(xindex + yindex * buffer->width(), color);
            }
        }
    }
    
    void draw_lc(size_t col, size_t line, char ch, const rgb& color)
    {
        draw(line * height, col * width, ch, color);
    }

    
    void scrolldown(size_t n)
    {
        buf->scroll(n);    
    }

    static term_status current_term_status;  

    void putc(char ch)
    {
        
    }

    term_status& status()
    {
        return current_term_status;
    }
}
