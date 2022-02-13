#include "term.h"

namespace term
{
    static video_buffer* buffer = nullptr;
    static const font* font_ptr = nullptr;
    static term_status current_term_status;

    void set_video_buffer(video_buffer* new_buf)
    {
        buffer = new_buf;
    }

    void set_font(const font& a)
    {
        font_ptr = &a;
    }

    void draw(size_t x, size_t y, char ch, const rgb& color)
    {
        if(font_ptr == nullptr)
            return;

        for(int i = 0; i < font_ptr->width(); i++)
        {
            for(int j = 0; j < font_ptr->height(); i++)
            {
                size_t xindex = x + i;
                size_t yindex = y + j;
                if(font_ptr->test_pixel(ch, current_term_status.text_style, (uint8_t)i, (uint8_t)j))
                    buffer->draw(xindex + yindex * buffer->width(), color);
            }
        }
    }
    
    void draw_lc(size_t col, size_t line, char ch, const rgb& color)
    {
        if(font != nullptr)
            draw(line * font_ptr->height(), col * font_ptr->width(), ch, color);
    }

    
    void scrolldown(size_t n)
    {
        buf->scroll(n);
    }

    void putc(char ch)
    {
        // not implemented yet
    }

    void putstr(const char* c)
    {
        size_t idx = current_term_status.cursor_x + current_term_status.cursor_y * columns();
        while(*c)
        {
            char ch = *c++;
            switch(ch)
            {
                case '\n':
                    idx = ((idx + lines() - 1) / lines()) * lines();
                    break;
                case '\r':
                    idx = ((idx / lines()) * lines());
                    break;
                case '\t':
                    size_t target = (((idx + 3) >> 2) << 2); // what the fuck?
                    
                    if(target >= lines() * columns())
                    {
                        target -= columns();
                        idx -= columns();
                        scrolldown(1);
                    }

                    while(idx < target)
                        buffer->draw_lc(idx++, 0, ch, current_term_status.fg);

                    break;
                default:
                    buffer->draw_lc(idx++, 0, ch, current_term_status.fg);
                
            }
            if(idx >= lines() * columns())
            {
                idx -= columns();
                scrolldown(1);
            }
        }

        // update positional info
        current_term_status.cursor_x = idx % columns();
        current_term_status.cursor_y = idx / columns();
    }

    

    term_status& status()
    {
        return current_term_status;
    }
}
