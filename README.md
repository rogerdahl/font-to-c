### font2c.py

![Text Sample](https://github.com/rogerdahl/font-to-c/blob/master/draw_text_sample.png)

font2c.py is a small Python script that generates C code for TrueType, OpenType and other fonts supported by FreeType. The generated code can be compiled and linked directly into C programs to enable text drawing without any external libraries. Example C code for using the generated code is included. This is mainly intended for use in embedded systems.

There are many of these generators around, but I didn't find one that met my three requirements:

- Run on Linux
- Draw antialiased fonts
- Preserve and use "advance" to allow slanted characters to overlap (it's not complete kerning but helps a lot)

#### Usage

Prepare dependencies

    $ sudo apt-get install python-pip
    $ sudo pip install freetype-py
    
Get the path to a TTF font. If you don't know where they are, try:

    $ locate "*.ttf" | less

In `font2c.py`:

- Set `FONT_FILE_PATH` to the path found earlier.
- Set `FONT_PIXEL_SIZE` to the size of the font, in pixels (seems to be approximate)
- If the complete range of characters from ASCII 32 to 127 is not required, edit FIRST_CHAR and LAST_CHAR.

Run `font2c.py`:

    $ ./font2c.py

Then compile the C program that will use the generated C code:

    $ gcc -std=c99 -o draw_text draw_text.c century_schoolbook_l_bold_30_font.c
    
Run the program:

    $ ./draw_text
    
And view the resulting image with an image viewer that supports `.ppm` files. For instance, `feh`.
    
    $ feh ./draw_text_sample.ppm
    
#### Example output

Two files will be generated. For instance:

    century_schoolbook_l_bold_30_font.h
    century_schoolbook_l_bold_30_font.c

A table of pixel values for each character in the range set by FIRST_CHAR and LAST_CHAR is generated. An
example for the character "*" in 30 pixels for the font Century Schoolbook L Bold:

```c
// * (42)
0x00,0x00,0x00,0x00,0x00,0x85,0xf1,0x52,0x00,0x00,0x00,0x00,0x00, //      +@-     
0x00,0x00,0x00,0x00,0x01,0xf1,0xff,0xb8,0x00,0x00,0x00,0x00,0x00, //      @@#     
0x00,0x11,0x1c,0x00,0x00,0xd3,0xff,0x8e,0x00,0x01,0x27,0x08,0x00, //   .  %@+  .  
0x1b,0xf6,0xff,0x57,0x00,0x7a,0xff,0x3e,0x01,0x96,0xff,0xd9,0x02, // .@@- =@: +@% 
0x4a,0xff,0xff,0xf3,0x27,0x1f,0xea,0x02,0x5a,0xff,0xff,0xfd,0x0f, // :@@@..@ -@@@ 
0x05,0xa2,0xf6,0xff,0xd9,0x28,0xc6,0x47,0xf1,0xfe,0xde,0x71,0x00, //  *@@%.#:@@%= 
0x00,0x00,0x02,0x1a,0x5d,0xcf,0xf3,0xb9,0x41,0x0a,0x00,0x00,0x00, //    .-#@#:    
0x00,0x01,0x13,0x32,0x71,0xce,0xed,0xcc,0x60,0x34,0x1e,0x01,0x00, //    .=#@#-:.  
0x11,0xd4,0xff,0xff,0xbb,0x18,0xd1,0x29,0xdc,0xff,0xff,0xb7,0x01, //  %@@# %.%@@# 
0x45,0xff,0xff,0xde,0x10,0x3e,0xf7,0x10,0x23,0xe9,0xff,0xff,0x11, // :@@% :@ .%@@ 
0x06,0x9a,0xb0,0x22,0x00,0x96,0xff,0x6e,0x00,0x2f,0xa7,0x71,0x00, //  +*. +@= .*= 
0x00,0x00,0x00,0x00,0x01,0xe9,0xff,0xb6,0x00,0x00,0x00,0x00,0x00, //      %@#     
0x00,0x00,0x00,0x00,0x00,0xcc,0xff,0x8c,0x00,0x00,0x00,0x00,0x00, //      #@+     
0x00,0x00,0x00,0x00,0x00,0x1f,0x5e,0x0b,0x00,0x00,0x00,0x00,0x00, //      .-      
```

Another table of offsets into this table and other font metadata is also generated. These tables can then be used to draw text by looking up the characters and copying them to the display.

The image at the top was generated with the following calls in the included `draw_text.c` sample app:

```c
drawText(buf, 0, 0 * TALLEST_CHAR_PIXELS, "The quick brown fox jumps over the lazy dog");
drawCenteredText(buf, 1 * TALLEST_CHAR_PIXELS , "AWA\\\\\\///AAWWVVAAWWVWVWVW");
drawRightAdjustedText(buf, 2 * TALLEST_CHAR_PIXELS, "Ava Awa Wka Via Vja");
drawCenteredText(buf, 3 * TALLEST_CHAR_PIXELS, "1234567890 !@#$%^&*()-=_+~`';:?></.,");
```

#### Todo:

- The font pixel data can be stored using much less room at a small cost in performance and complexity of drawText().
- Unicode.
- Support monochrome fonts stored as bitmaps (would take 1/8 of the space but there would be no antialiasing).

