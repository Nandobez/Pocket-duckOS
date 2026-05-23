#include <string.h>
#include <time.h>
#include <asm/io.h>
#include <asm/system.h>
#include <duckos/tty.h>
#include <duckos/kernel.h>
#include <duckos/features.h>

extern void timer_wait(size_t ticks);

void prompt(void)
{
	printk("\nducky $ ");
}

void shell(void)
{
	fs_init();
	prompt();
}

static void reboot(void)
{
	unsigned char good = 0x02;
	while (good & 0x02)
		good = inb(0x64);
	outb(0x64, 0xFE);
	hlt();
}

static void about(void)
{
	printk("\nDuckyOS %s is an educational operating system for x86 machines \n", VERSION);
	printk("written in C and Assembly and developed by Fernando Bezerra.\n");
	printk("\nLicense : GNU General Public License v3.0\n");
}

static void help(void)
{
	printk("\nDucky shell, version 1.0.0 (ducky-%s)\n", VERSION);
	printk("These commands are defined internally.\n\n");
	printk(" clear      clear the screen\n");
	printk(" date       print system date and time\n");
	printk(" help       print this help list\n");
	printk(" ducky      print about ducky-%s\n", VERSION);
	printk(" reboot     reboot the system\n");
	printk(" quit       shutdown the system\n");
	printk(" ls         list files\n");
	printk(" cd <dir>   change directory\n");
	printk(" mkdir <n>  create directory\n");
	printk(" pwd        print working directory\n");
	printk(" echo <t>   print text\n");
	printk(" touch <n>  create file\n");
	printk(" cat <n>    show file content\n");
	printk(" write <f> <txt>  write to file\n");
	printk(" rm <n>     remove file/dir\n");
	printk(" vim <n>    edit file\n");
	printk(" color <fg> [bg]  change colors\n");
}

static char *get_arg(char *cmd)
{
	char *p = cmd;
	while (*p && *p != ' ') p++;
	if (*p == ' ') {
		*p = '\0';
		p++;
		while (*p == ' ') p++;
		return (*p) ? p : (char *)0;
	}
	return (char *)0;
}

void proccmd(char *cmd)
{
	char *arg = get_arg(cmd);

	if (strcmp(cmd, "clear") == 0) {
		clrscr();
		gotoxy(0, -1);
	} else if (strcmp(cmd, "ducky") == 0)
		about();
	else if (strcmp(cmd, "reboot") == 0) {
		printk("\nrebooting...\n");
		timer_wait(10);
		printk("\n[NOW]");
		timer_wait(5);
		reboot();
	} else if (strcmp(cmd, "help") == 0)
		help();
	else if (strcmp(cmd, "date") == 0) {
		struct time t;
		kernel_mktime(&t);
		printk("\n%02d/%02d\t%02d:%02d:%02d\t%04d\n", t.month, t.day, t.hour, t.min, t.sec, t.year);
	} else if (strcmp(cmd, "quit") == 0)
		cmd_quit();
	else if (strcmp(cmd, "ls") == 0)
		cmd_ls(arg);
	else if (strcmp(cmd, "cd") == 0)
		cmd_cd(arg);
	else if (strcmp(cmd, "mkdir") == 0)
		cmd_mkdir(arg);
	else if (strcmp(cmd, "pwd") == 0)
		cmd_pwd();
	else if (strcmp(cmd, "echo") == 0)
		cmd_echo(arg);
	else if (strcmp(cmd, "touch") == 0)
		cmd_touch(arg);
	else if (strcmp(cmd, "cat") == 0)
		cmd_cat(arg);
	else if (strcmp(cmd, "write") == 0)
		cmd_write(arg);
	else if (strcmp(cmd, "rm") == 0)
		cmd_rm(arg);
	else if (strcmp(cmd, "vim") == 0)
		cmd_vim(arg);
	else if (strcmp(cmd, "color") == 0)
		cmd_color(arg);
	else
		printk("\nDucky shell : %s : command not found\n", cmd);
}
