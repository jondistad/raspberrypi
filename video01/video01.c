
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

// 2  outer corner
// 4
// 6
// 8  TX out
// 10 RX in

// The raspberry pi firmware at the time this was written defaults
// loading at address 0x8000.  Although this bootloader could easily
// load at 0x0000, it loads at 0x8000 so that the same binaries built
// for the SD card work with this bootloader.  Change the ARMBASE
// below to use a different location.

// #include "image_data.h"

extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern unsigned int GETPC ( void );
extern void BRANCHTO ( unsigned int );
extern void dummy ( unsigned int );

extern void uart_init ( void );
extern unsigned int uart_lcr ( void );
extern void uart_flush ( void );
extern void uart_send ( unsigned int );
extern unsigned int uart_recv ( void );
extern unsigned int uart_check ( void );
extern void hexstring ( unsigned int );
extern void hexstrings ( unsigned int );
extern void timer_init ( void );
extern unsigned int timer_tick ( void );

extern void timer_init ( void );
extern unsigned int timer_tick ( void );

#define DMB asm ("dmb")

unsigned int MailboxWrite ( unsigned int fbinfo_addr, unsigned int channel )
{
    unsigned int mailbox;

    mailbox=0x3F00B880;
    while(1)
    {
        DMB;
        if((GET32(mailbox+0x18)&0x80000000)==0) break;
    }
    DMB;
    PUT32(mailbox+0x20, fbinfo_addr | channel);
    return(0);
}

unsigned int MailboxRead ( unsigned int channel )
{
    unsigned int ra;
    unsigned int mailbox;

    mailbox=0x3F00B880;
    while(1)
    {
        while(1)
        {
            DMB;
            ra=GET32(mailbox+0x18);
            if((ra&0x40000000)==0) break;
        }
        //hexstrings(ra);
        DMB;
        ra=GET32(mailbox+0x00);
        //hexstring(ra);
        if((ra&0xF)==channel) break;
    }
    return(ra>>4);
}

struct fbinfo {
    unsigned int width;
    unsigned int height;
    unsigned int virtual_width;
    unsigned int virtual_height;
    unsigned int pitch;
    unsigned int depth;
    unsigned int x_offset;
    unsigned int y_offset;
    volatile unsigned int *pointer;
    unsigned int size;
};

//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra,rb;
    // unsigned int ry,rx;
    unsigned char attempt;

    volatile struct fbinfo *fb;

    uart_init();
    hexstring(0x12345678);
    hexstring(GETPC());
    timer_init();

    fb = (struct fbinfo*)0x400000;
    // for (int i = 0; i < sizeof(struct fbinfo)/sizeof(unsigned int); i++)
    //     PUT32(((unsigned int*)&fb)+i, 0);

    attempt = 0;
    while (1) {
        fb->width = 640;
        fb->height = 480;
        fb->virtual_width = 640;
        fb->virtual_height = 480;
        fb->pitch = 1920;
        fb->depth = 24;
        fb->x_offset = 0;
        fb->y_offset = 0;
        fb->pointer = 0;
        fb->size = 0;
        // PUT32(0x400000, 640); /* #0 Physical Width */
        // PUT32(0x400004, 480); /* #4 Physical Height */
        // PUT32(0x400008, 640); /* #8 Virtual Width */
        // PUT32(0x40000C, 480); /* #12 Virtual Height */
        // PUT32(0x400010, 0); /* #16 GPU - Pitch */
        // PUT32(0x400014, 32); /* #20 Bit Depth */
        // PUT32(0x400018, 0); /* #24 X */
        // PUT32(0x40001C, 0); /* #28 Y */
        // PUT32(0x400020, 0); /* #32 GPU - Pointer */
        // PUT32(0x400024, 0); /* #36 GPU - Size */


        MailboxWrite((unsigned)fb,1);
        rb = MailboxRead(1);
        
        // hexstring(rb);
        if (rb) continue;

        
        if (!attempt++) {
            rb=0x400000;
            for(ra=0;ra<10;ra++) 
            {
                hexstrings(rb); hexstring(GET32(rb));
                rb+=4;
            }
        }

        // rb=fb->pointer;
        // hexstring(rb);
        if (fb->pointer) break;
    }
    volatile unsigned int* ip = (unsigned int*)fb;
    for(; ip < (unsigned int*)fb+sizeof(struct fbinfo) / sizeof(unsigned int); ip++) {
        hexstrings((unsigned int)ip); hexstring((unsigned int)*ip);
    }

    hexstring(0x42000000);

    volatile unsigned int *rgb = (unsigned int*)fb->pointer;
    while( rgb < fb->pointer + fb->size )
    {
        *rgb++ = 0xFF0000FF;
        *rgb++ = 0x00FF0000;
        *rgb++ = 0x0000FF00;
    }
    // for(ra=0;ra<10000;ra++)
    // {
    //     PUT32(rb,~((ra&0xFF)<<0));
    //     rb+=4;
    // }
    // for(ra=0;ra<10000;ra++)
    // {
    //     PUT32(rb,~((ra&0xFF)<<8));
    //     rb+=4;
    // }
    // for(ra=0;ra<10000;ra++)
    // {
    //     PUT32(rb,~((ra&0xFF)<<16));
    //     rb+=4;
    // }
    // for(ra=0;ra<10000;ra++)
    // {
    //     PUT32(rb,~((ra&0xFF)<<24));
    //     rb+=4;
    // }
    // rb=fb->pointer;
    // hexstring(rb);
    
    // hexstring(off);
    // ra=0;
    // for(ry=0;ry<480;ry++)
    // {
    //     for(rx=0;rx<480;rx++)
    //     {
    //         PUT32(rb,image_data[ra++]);
    //         rb+=4;
    //     }
    //     // for(;rx<640;rx++)
    //     // {
    //     //     PUT32(rb,0);
    //     //     rb+=4;
    //     // }
    // }
    hexstring(0xDEADBEEF);
    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
//
// Copyright (c) 2012 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------

