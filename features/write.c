#include <duckos/kernel.h>
#include <duckos/features.h>
#include <string.h>

void cmd_write(char *args)
{
    char fullpath[MAX_PATH];
    char *filename;
    char *content;
    int idx;

    if (!args || strlen(args) == 0) {
        printk("\nwrite: usage: write <file> <content>\n");
        return;
    }

    /* separa filename e content */
    filename = args;
    content = args;
    while (*content && *content != ' ') content++;
    if (*content == ' ') {
        *content = '\0';
        content++;
    } else {
        printk("\nwrite: usage: write <file> <content>\n");
        return;
    }

    fs_make_path(fullpath, filename);
    idx = fs_find(fullpath);

    if (idx < 0) {
        printk("\nwrite: %s: not found\n", filename);
        return;
    }

    if (fs[idx].is_dir) {
        printk("\nwrite: %s: is a directory\n", filename);
        return;
    }

    strcpy(fs[idx].content, content);
    printk("\nwrite: wrote to %s\n", filename);
}
