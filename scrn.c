/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Screen output functions for Console I/O
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>

/* These define our textpointer, our background and foreground
*  colors (attributes), and x and y cursor coordinates */
unsigned short *textmemptr;
//int attrib = 0x0F; // black background and white text
int attrib = 0x1F; // blue background and white text
int csr_x = 0, csr_y = 0;
int shift = 0;

/* Scrolls the screen */
void scroll(void)
{
    unsigned blank, temp;

    /* A blank is defined as a space... we need to give it
    *  backcolor too */
    blank = 0x20 | (attrib << 8);

    /* Row 25 is the end, this means we need to scroll up */
    if(csr_y >= 25)
    {
        /* Move the current text chunk that makes up the screen
        *  back in the buffer by a line */
        temp = csr_y - 25 + 1;
        memcpy (textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
        memsetw (textmemptr + (25 - temp) * 80, blank, 80);
        csr_y = 25 - 1;
    }
}

/* Updates the hardware cursor: the little blinking line
*  on the screen under the last character pressed! */
void move_csr(void)
{
    unsigned temp;

    /* The equation for finding the index in a linear
    *  chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    temp = csr_y * 80 + csr_x;

    /* This sends a command to indicies 14 and 15 in the
    *  CRT Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where the hardware cursor is to be 'blinking'. To
    *  learn more, you should look up some VGA specific
    *  programming documents. A great start to graphics:
    *  http://www.brackeen.com/home/vga */
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

/* Move Cursor */
void csrToLeft() {
	csr_x--;
}
void csrToRight() {
	csr_x++;
}
void csrUp() {
	csr_y--;
}
void csrDown() {
	csr_y++;
}
void csrTo(int y, int x) {
	csr_x = x; csr_y = y;
	move_csr();
}
int getCsrX() {
	return csr_x;
}

int getCsrY() {
	return csr_y;
}

/* Set Text and Background Color */
void setColor(const unsigned c) {
	attrib = c;
}

/* Clears the screen */
void cls()
{
    unsigned blank;
    int i;

    /* Again, we need the 'short' that will be used to
    *  represent a space with color */
    blank = 0x20 | (attrib << 8);

    /* Sets the entire screen to spaces in our current
    *  color */
    for(i = 0; i < 25; i++)
        memsetw (textmemptr + i * 80, blank, 80);

    /* Update out virtual cursor, and then move the
    *  hardware cursor */
    csr_x = 0;
    csr_y = 0;
    move_csr();
}

/* Shift and unshift characters */
void toggleShift() {
	if(shift) shift = 0; else shift = 1;
}

/* Will characters be shifted up? */
int getShiftState() {
	return shift;
}

/* Puts a single character on the screen */
void putch(unsigned char c)
{
    unsigned short *where;
    unsigned att = attrib << 8;

    /* Handle a backspace, by moving the cursor back one space 
     * and delete the character there */
    if(c == 0x08)
    {
        if(csr_x != 0) csr_x--;
        where = textmemptr + (csr_y * 80 + csr_x);
        *where = ' ' | att;	/* Character AND attributes: color */
    }
    /* Handles a tab by incrementing the cursor's x, but only
    *  to a point that will make it divisible by 8 */
    else if(c == 0x09)
    {
        csr_x = (csr_x + 8) & ~(8 - 1);
    }
    /* Handles a 'Carriage Return', which simply brings the
    *  cursor back to the margin */
    else if(c == '\r')
    {
        csr_x = 0;
    }
    /* We handle our newlines the way DOS and the BIOS do: we
    *  treat it as if a 'CR' was also there, so we bring the
    *  cursor to the margin and we increment the 'y' value */
    else if(c == '\n')
    {
        csr_x = 0;
        csr_y++;
    }
    /* Any character greater than and including a space, is a

    *  printable character. The equation for finding the index
    *  in a linear chunk of memory can be represented by:

    *  Index = [(y * width) + x] */
    else if(c >= ' ')
    {
        where = textmemptr + (csr_y * 80 + csr_x);
	if(shift) *where = (c - 32) | att;	/* Character AND attributes: color */
	else *where = c | att;	/* Character AND attributes: color */
        csr_x++;
    }

    /* If the cursor has reached the edge of the screen's width, we
    *  insert a new line in there */
    if(csr_x >= 80)
    {
        csr_x = 0;
        csr_y++;
    }

    /* Scroll the screen if needed, and finally move the cursor */
    scroll();
    move_csr();
}

/* Uses the above routine to output a string... */
void puts(unsigned char *text)
{
    int i;

    for (i = 0; i < strlen(text); i++)
    {
        putch(text[i]);
    }
}

/* Uses the above routine to output an integer number */
void putint(int number) {
	int p = 1, c = 0, s = 0, i = 0;
	char string[10];
	if(number < 10) { putch('0'); putch((char) (number + 48));
	} else {
		while(number >= p) {
			c = number % (p*10) - s;
			s += c;
			c /= p;
			string[i] = (char) (c + 48);
			//putch((char) (c + 48)); /* TODO: digits are inverted
			p *= 10;
			i++;
		}
		for(i--; i >= 0; i--) putch((char) string[i]);
	}
}

void putbin(int number) {
	int p = 1, c = 0, s = 0, i = 0;
	char string[16];
	if(number < 2) putch( (char) (number + 48));
	else {
		while(number >= p) {
			c = number % (p*2) - s;
			s += c;
			c /= p;
			string[i] = (char) (c + 48);
			//putch((char) (c + 48)); /* TODO: digits are inverted
			p *= 2;
			i++;
		}
		for(i--; i >= 0; i--) putch (string[i]);
	}
}

/* Sets the forecolor and backcolor that we will use */
void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

/* Sets our text-mode VGA pointer, then clears the screen for us */
void init_video(void)
{
    textmemptr = (unsigned short *)0xB8000;
    cls();
}
