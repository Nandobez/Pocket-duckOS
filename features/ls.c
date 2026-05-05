#include <duckos/kernel.h>
#include <duckos/features.h>
#include <string.h>

static int starts_with(const char *str, const char *prefix)
{
    while (*prefix) {
        if (*str != *prefix) return 0;
        str++;
        prefix++;
    }
    return 1;
}

static int is_direct_child(const char *path, const char *parent)
{
    int plen = strlen(parent);
    int flen = strlen(path);
    int i, slashes = 0;

    if (flen <= plen) return 0;
    if (!starts_with(path, parent)) return 0;

    /* para root "/" */
    if (plen == 1) {
        for (i = 1; i < flen; i++) {
            if (path[i] == '/') return 0;
        }
        return 1;
    }

    /* para outros dirs */
    if (path[plen] != '/') return 0;

    for (i = plen + 1; i < flen; i++) {
        if (path[i] == '/') slashes++;
    }

    return (slashes == 0);
}

void cmd_ls(char *arg)
{
    int i, found = 0;
    (void)arg;

    printk("\n");
    for (i = 0; i < MAX_FILES; i++) {
        if (fs[i].used && is_direct_child(fs[i].path, cwd)) {
            if (fs[i].is_dir)
                printk("  [DIR] %s\n", fs[i].name);
            else
                printk("        %s\n", fs[i].name);
            found = 1;
        }
    }
    if (!found)
        printk("  (empty)\n");
}
