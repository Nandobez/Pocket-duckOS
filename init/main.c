#define __DUCKOS_LIBC 1
#include <stddef.h>
#include <stdarg.h>
#include <string.h>

/* kernel includes */
#include <time.h>
#include <asm/io.h>
#include <asm/system.h>
#include <duckos/tty.h>
#include <duckos/head.h>
#include <duckos/sched.h>
#include <duckos/kernel.h>
#include <duckos/features.h>

static char buf[1024];

extern int vsprintf();
static void init();

/* This is the kernel entry point */
void main()
{	
	/*
	 * Interrupts are still disabled. Do necessary setups, then
	 * enable them.
	 */
	tty_init();
	trap_init();
	sched_init();
	sti();

	init();

	for (;;);
}

static size_t printf(const char *fmt, ...)
{
	va_list args;
        size_t i;

        va_start(args, fmt);
        i = vsprintf(buf, fmt, args);
        va_end(args);
        puts(buf);
        return i;
}

static void init()
{
	printf("Initializing ...\n");
	fs_init();
	timer_wait(50);
	printf("\n[OK]");
	timer_wait(10);
	clrscr();

	show_boot_art();

	printf("Welcome to DuckyOS %s\n", VERSION);
	printf("\nDuckyOS %s is an educational operating system for x86 machines \n", VERSION);
	printf("written in C and Assembly and developed by Fernando Bezerra.\n");
	shell();
}
