#include <duckos/kernel.h>
#include <duckos/features.h>
#include <duckos/tty.h>
#include <asm/io.h>

#define MODE_NORMAL 0
#define MODE_INSERT 1
#define MODE_COMMAND 2

static char buffer[MAX_CONTENT];
static char cmdbuf[64];
static int buflen;
static int cmdlen;
static int mode;
static int file_idx;
static char filename[MAX_NAME];

extern char toascii(unsigned char sc);

static void mycopy(char *dst, const char *src)
{
    int i = 0;
    while (src[i]) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

static int mylen(const char *s)
{
    int i = 0;
    while (s[i]) i++;
    return i;
}

static void vim_clear(void)
{
    clrscr();
}

static void vim_status(void)
{
    gotoxy(0, 24);
    printk("                                                                              ");
    gotoxy(0, 24);
    if (mode == MODE_NORMAL)
        printk("-- NORMAL --");
    else if (mode == MODE_INSERT)
        printk("-- INSERT --");
    else if (mode == MODE_COMMAND)
        printk(":%s", cmdbuf);
}

static void vim_redraw(void)
{
    int i, x = 0, y = 0;

    vim_clear();

    for (i = 0; i < buflen && y < 23; i++) {
        if (buffer[i] == '\n') {
            x = 0;
            y++;
            gotoxy(x, y);
        } else {
            printk("%c", buffer[i]);
            x++;
            if (x >= 80) {
                x = 0;
                y++;
                gotoxy(x, y);
            }
        }
    }

    vim_status();
}

static char vim_getchar(void)
{
    unsigned char sc;
    char ch;

    while (1) {
        if (inb(0x64) & 1) {
            sc = inb(0x60);
            ch = toascii(sc);
            if (ch != 0) return ch;
            if (sc == 1) return 27; /* ESC */
        }
    }
}

static void vim_insert_char(char c)
{
    if (buflen < MAX_CONTENT - 1) {
        buffer[buflen++] = c;
        buffer[buflen] = '\0';
    }
}

static void vim_backspace(void)
{
    if (buflen > 0) {
        buflen--;
        buffer[buflen] = '\0';
    }
}

static void vim_save(void)
{
    if (file_idx >= 0) {
        mycopy(fs[file_idx].content, buffer);
        gotoxy(0, 24);
        printk("                                                                              ");
        gotoxy(0, 24);
        printk("\"%s\" written", filename);
    }
}

void cmd_vim(char *name)
{
    char path[MAX_PATH];
    char ch;
    int running = 1;
    int idx;

    if (!name || !name[0]) {
        printk("\nvim: missing filename\n");
        return;
    }

    mycopy(filename, name);
    fs_make_path(path, name);

    idx = fs_find(path);
    if (idx < 0) {
        idx = fs_find_free();
        if (idx < 0) {
            printk("\nvim: no space\n");
            return;
        }
        mycopy(fs[idx].name, name);
        mycopy(fs[idx].path, path);
        fs[idx].is_dir = 0;
        fs[idx].used = 1;
        fs[idx].content[0] = '\0';
    }

    if (fs[idx].is_dir) {
        printk("\nvim: '%s' is directory\n", name);
        return;
    }

    file_idx = idx;

    mycopy(buffer, fs[idx].content);
    buflen = mylen(buffer);

    mode = MODE_NORMAL;
    cmdlen = 0;
    cmdbuf[0] = '\0';

    vim_redraw();

    while (running) {
        ch = vim_getchar();

        if (mode == MODE_NORMAL) {
            if (ch == 'i' || ch == 'a') {
                mode = MODE_INSERT;
                vim_status();
            } else if (ch == ':') {
                mode = MODE_COMMAND;
                cmdlen = 0;
                cmdbuf[0] = '\0';
                vim_status();
            }
        } else if (mode == MODE_INSERT) {
            if (ch == 27) { /* ESC */
                mode = MODE_NORMAL;
                vim_status();
            } else if (ch == '\b') {
                vim_backspace();
                vim_redraw();
            } else if (ch == '\n') {
                vim_insert_char('\n');
                vim_redraw();
            } else if (ch >= 32 && ch < 127) {
                vim_insert_char(ch);
                vim_redraw();
            }
        } else if (mode == MODE_COMMAND) {
            if (ch == 27) { /* ESC */
                mode = MODE_NORMAL;
                cmdlen = 0;
                cmdbuf[0] = '\0';
                vim_status();
            } else if (ch == '\n') {
                if (cmdbuf[0] == 'q') {
                    running = 0;
                } else if (cmdbuf[0] == 'w') {
                    vim_save();
                    if (cmdlen > 1 && cmdbuf[1] == 'q') {
                        running = 0;
                    }
                }
                mode = MODE_NORMAL;
                cmdlen = 0;
                cmdbuf[0] = '\0';
                if (running) vim_status();
            } else if (ch == '\b') {
                if (cmdlen > 0) {
                    cmdlen--;
                    cmdbuf[cmdlen] = '\0';
                    vim_status();
                }
            } else if (ch >= 32 && ch < 127 && cmdlen < 62) {
                cmdbuf[cmdlen++] = ch;
                cmdbuf[cmdlen] = '\0';
                vim_status();
            }
        }
    }

    vim_clear();
}
