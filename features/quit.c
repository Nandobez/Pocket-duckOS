#include <duckos/kernel.h>
#include <asm/io.h>
#include <asm/system.h>

void cmd_quit(void)
{
    printk("\nShutting down DuckyOS...\n");
    /* ACPI shutdown para QEMU */
    outw(0x604, 0x2000);
    /* fallback: bochs/older qemu */
    outw(0xB004, 0x2000);
    /* se nada funcionar, halt */
    cli();
    hlt();
}
