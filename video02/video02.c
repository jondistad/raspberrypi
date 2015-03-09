
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


unsigned int MailboxWrite ( unsigned int fbinfo_addr, unsigned int channel )
{
    unsigned int mailbox;

    mailbox=0x3F00B880;
    while(1)
    {
        if((GET32(mailbox+0x18)&0x80000000)==0) break;
    }
    PUT32(mailbox+0x20,fbinfo_addr+channel);
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
            ra=GET32(mailbox+0x18);
            if((ra&0x40000000)==0) break;
        }
        //hexstrings(ra);
        ra=GET32(mailbox+0x00);
        //hexstring(ra);
        if((ra&0xF)==channel) break;
    }
    return(ra);
}


//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra,rb;
    unsigned int ry,rx;

    uart_init();
    hexstring(0x12345678);
    hexstring(GETPC());

    /* Print L2 Cache info */
    unsigned int l2c;
    asm ("MRC p15, 1, %0, c0, c0, 1"
         : "=rm" (l2c));
    hexstring(l2c);

    timer_init();

    PUT32(0x40000, 640); /* #0 Physical Width */
    PUT32(0x40004, 480); /* #4 Physical Height */
    PUT32(0x40008, 640); /* #8 Virtual Width */
    PUT32(0x4000C, 480); /* #12 Virtual Height */
    PUT32(0x40010, 0); /* #16 GPU - Pitch */
    PUT32(0x40014, 24); /* #20 Bit Depth */
    PUT32(0x40018, 0); /* #24 X */
    PUT32(0x4001C, 0); /* #28 Y */
    PUT32(0x40020, 0); /* #32 GPU - Pointer */
    PUT32(0x40024, 0); /* #36 GPU - Size */


    unsigned char attempt = 0;
    while (1) {
        MailboxWrite(0xC0040000,1);
        if (MailboxRead(1) == 1) { // returns channel on success
            rb=0x40000;
            if (!attempt++)
                for(ra=0;ra<10;ra++)
                {
                    hexstrings(rb); hexstring(GET32(rb));
                    rb+=4;
                }
            if (GET32(0x40020)) break;
        }
    }
    rb=0x40000;
    for(ra=0;ra<10;ra++)
    {
        hexstrings(rb); hexstring(GET32(rb));
        rb+=4;
    }

    unsigned int cvals[3] = { 0, 0, 0 };
    unsigned int chan = 0;
    // unsigned int timer;
    while(1) {
        rb=GET32(0x40020) - 0xC0000000;
        for(ry=0;ry<480;ry++) {
            for(rx=0;rx<640;rx+=4) {
                asm volatile (
                    "str %1,[%0]\n\t"
                    "str %2,[%0, #4]\n\t"
                    "str %3,[%0, #8]\n\t"
                    :
                    : "r" (rb),
                      "r" (cvals[0]),
                      "r" (cvals[1]),
                      "r" (cvals[2])
                    : "memory");
                rb+=12;
            }
        }
        switch (chan) {
        case 0:
            cvals[0] += 8;
            cvals[0] &= 0xFF;
            cvals[0] |= (cvals[0] << 24);
            cvals[1] += 8 << 16;
            cvals[1] &= 0xFF0000;
            cvals[2] += 8 << 8;
            cvals[2] &= 0xFF00;
            break;
        case 1:
            cvals[1] += 8;
            cvals[1] &= 0xFF;
            cvals[1] |= (cvals[1] << 24);
            cvals[2] += 8 << 16;
            cvals[2] &= 0xFF0000;
            cvals[0] += 8 << 8;
            cvals[0] &= 0xFF00;
            break;
        case 2:
            cvals[2] += 8;
            cvals[2] &= 0xFF;
            cvals[2] |= (cvals[2] << 24);
            cvals[0] += 8 << 16;
            cvals[0] &= 0xFF0000;
            cvals[1] += 8 << 8;
            cvals[1] &= 0xFF00;
            break;
        }
        if (0 == cvals[0]) {if (++chan > 2) chan = 0;}
    }
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

