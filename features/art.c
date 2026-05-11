#include <duckos/kernel.h>

void show_duckyos_art(void)
{
    printk("\n");
    printk("  ____             _          ___  ____  \n");
    printk(" |  _ \\ _   _  ___| | ___   _/ _ \\/ ___| \n");
    printk(" | | | | | | |/ __| |/ / | | | | | \\___ \\ \n");
    printk(" | |_| | |_| | (__|   <| |_| | |_| |___) |\n");
    printk(" |____/ \\__,_|\\___|_|\\_\\\\__, |\\___/|____/ \n");
    printk("                       |___/              \n");
    printk("\n");
}

void show_duck_art(void)
{
    printk("\n");
    printk("      __\n");
    printk("   __( o)>\n");
    printk("   \\ <_. )\n");
    printk("    `---' \n");
    printk("\n");
}

void show_boot_art(void)
{
    show_duckyos_art();
    show_duck_art();
}
