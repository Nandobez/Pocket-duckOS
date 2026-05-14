#include <duckos/kernel.h>

void cmd_echo(char *text)
{
    printk("\n%s\n", text ? text : "");
}
