#ifndef SCREEN_H
#define SCREEN_H

void clear_screen();
void kprint(char *message, unsigned char color);
void kprintln(char *message, unsigned char color);
void set_cursor_offset(int offset);
int get_cursor_offset();
void refresh_screen_from_buffer(int current_scroll_row);

#endif