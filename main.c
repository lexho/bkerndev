/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Main.c: C code entry.
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>

void *memcpy(void *dest, const void *src, size_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void *memset(void *dest, char val, size_t count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

size_t strlen(const char *str)
{
    size_t retval;
    for(retval = 0; *str != '\0'; str++) retval++;
    return retval;
}

unsigned char inportb (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

 
#define CMOS_PORT_ADDRESS 0x70
#define CMOS_PORT_DATA    0x71
 
/**
 * Liest ein Byte aus den CMOS
 *  @param offset Offset im CMOS
 *  @return Gelesener Wert
 */
int cmos_read(int offset) {
  int tmp = inportb(CMOS_PORT_ADDRESS);
  outportb(CMOS_PORT_ADDRESS, (tmp & 0x80) | (offset & 0x7F));
	int bcd = inportb(CMOS_PORT_DATA);
 int formatted = ( (bcd & 0xF0) >> 1) + ( (bcd & 0xF0) >> 3) + (bcd & 0xf);
  return formatted;
}

void main()
{
    int i;

	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	init_video();
	timer_install();
	keyboard_install();
	file_install();
	outportb(0x0B, 0b00000111); // config CMOS

    __asm__ __volatile__ ("sti");
	csrTo(3,10);
	setColor(0x4F);
	puts("==========================================================\n");
	csrTo(4,10);
    	puts("         +++         Welcome  to  my         +++          \n");
	csrTo(5,10);
	puts("     ++++++  H O M E B R E W   K E R N E L !  +++++       \n");
	csrTo(6,10);
	puts("==========================================================\n");
	//while(getCsrX() < 70) putch(' ');	
	setColor(0x1F);
	csrTo(8,0);
	csrTo(10,0);
    	puts("\tTime: ");

    for (;;);
}
