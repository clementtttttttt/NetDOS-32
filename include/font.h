#include <stdbool.h>

#include <stddef.h>

#include <stdint.h>
 
#include <multiboot.h>


/* Hardware text mode color constants. */

enum vga_color {

	VGA_COLOR_BLACK = 0,

	VGA_COLOR_BLUE = 1,

	VGA_COLOR_GREEN = 2,

	VGA_COLOR_CYAN = 3,

	VGA_COLOR_RED = 4,

	VGA_COLOR_MAGENTA = 5,

	VGA_COLOR_BROWN = 6,

	VGA_COLOR_LIGHT_GREY = 7,

	VGA_COLOR_DARK_GREY = 8,

	VGA_COLOR_LIGHT_BLUE = 9,

	VGA_COLOR_LIGHT_GREEN = 10,

	VGA_COLOR_LIGHT_CYAN = 11,

	VGA_COLOR_LIGHT_RED = 12,

	VGA_COLOR_LIGHT_MAGENTA = 13,

	VGA_COLOR_LIGHT_BROWN = 14,

	VGA_COLOR_WHITE = 15,

};

 
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
static inline uint16_t vga_entry(unsigned char uc, uint8_t color);
size_t strlen(const char* str);

#include<mutex.h>

void printglyph(unsigned long* vram,int xsize,int x,int y,uint32_t c,unsigned char* font);
void terminal_initialize(void);


void terminal_setcolor(uint8_t color);


 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) ;

void scroll_terminal();

void terminal_putchar(char c);


 

void terminal_write(const char* data, size_t size);



void printstring(const char* data);

void printstring_wrapper(const char* in);

void swap(char* x,char* y);
void reverse(char* str, int length);

// Implementation of itoa() 
char* itoa(unsigned int num, char* str, int base);
