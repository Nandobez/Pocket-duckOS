#include <duckos/kernel.h>
#include <duckos/tty.h>

/*
 * Cores VGA:
 * 0=Preto,  1=Azul,    2=Verde,   3=Ciano
 * 4=Verm,   5=Magenta, 6=Marrom,  7=CinzaClaro
 * 8=CinzaEsc,9=AzulClaro,10=VerdeClaro,11=CianoClaro
 * 12=VermClaro,13=MagentaClaro,14=Amarelo,15=Branco
 */

static int atoi(const char *s)
{
    int n = 0;
    while (*s >= '0' && *s <= '9') {
        n = n * 10 + (*s - '0');
        s++;
    }
    return n;
}

void cmd_color(char *args)
{
    int fg = 6;  /* marrom/laranja default */
    int bg = 0;  /* preto */
    char *p;

    if (!args || !args[0]) {
        printk("\ncolor: usage: color <fg> [bg]\n");
        printk("Cores: 0=preto 1=azul 2=verde 3=ciano\n");
        printk("       4=verm 5=magenta 6=laranja 7=cinza\n");
        printk("       8-15 = tons claros, 14=amarelo 15=branco\n");
        return;
    }

    fg = atoi(args);

    /* procura segundo argumento */
    p = args;
    while (*p && *p != ' ') p++;
    if (*p == ' ') {
        p++;
        bg = atoi(p);
    }

    if (fg > 15) fg = 15;
    if (bg > 7) bg = 7;

    setcolor((unsigned char)fg, (unsigned char)bg);
    clrscr();
    printk("Cor alterada: fg=%d bg=%d\n", fg, bg);
}
