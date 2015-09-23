/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Timer driver
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>

/* This will keep track of how many ticks that the system
*  has been running for */
int timer_ticks = 0;
double time = 0.0;
int seconds = 0;
char byte;
int sndtime;

/* Handles the timer. In this case, it's very simple: We
*  increment the 'timer_ticks' variable every time the
*  timer fires. By default, the timer fires 18.222 times
*  per second. Why 18.222Hz? Some engineer at IBM must've
*  been smoking something funky */
int sndstat = 0;
void tone_512() {
	if(sndstat == 0) {
		//char byte;
		outportb(0x43, 0xb6); // write to timer mode register 
		outportb(0x42, 0); // write LSB
		outportb(0x42, 6); // write MSB
		byte = inportb(0x61); // get current port setting
		byte |= 3; // turn speaker on
		outportb(0x61, byte);
		sndstat = 1; sndtime = 0; // needed to stop sound
	}
}

void sndOff() {
	outportb(0x61, byte & 0xfc); /* restore original settings */
	sndstat = 0;
}

void timer_handler(struct regs *r)
{
    /* Increment our 'tick count' */
    timer_ticks++;

	csrTo(10,14);
	/* Make time */
	time = timer_ticks / 18.222;
	/*putint((int) (time * 100));
	puts(" ==> ");*/
	//if(round((double) time) > seconds) {sndtime = 0; tone_512();}

	/* Make Beep Sound every minute*/	
	//if(sndtime > 4) sndOff();
	//else if(seconds % 60 == 0 ) { tone_512(); }

	sndtime++;
	seconds = round((double) time);
	//putint(seconds);
	putint(cmos_read(0x04)); putch(':');
	putint(cmos_read(0x02)); putch(':');
	putint(cmos_read(0x0)); puts("\n \t");
	/*putbin(cmos_read(0x04)); putch(':');
	putbin(cmos_read(0x02)); putch(':');
	putbin(cmos_read(0x0)); puts("\n \t");*/
	//putch(cmos_read(0x0));
	//puts(" Seconds");
}

/* This will continuously loop until the given time has
*  been reached */
void timer_wait(int ticks)
{
    unsigned long eticks;

    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks);
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install()
{
    /* Installs 'timer_handler' to IRQ0 */
    irq_install_handler(0, timer_handler);
}
