#include <duckos/kernel.h>
#include <duckos/features.h>
#include <string.h>

void cmd_cat(char *name)
{
    char path[MAX_PATH];
    int idx;

    if (!name || !name[0]) {
        printk("\ncat: missing name\n");
        return;
    }

    fs_make_path(path, name);
    idx = fs_find(path);

    if (idx < 0) {
        printk("\ncat: '%s' not found\n", name);
        return;
    }

    if (fs[idx].is_dir) {
        printk("\ncat: '%s' is directory\n", name);
        return;
    }

    printk("\n%s", fs[idx].content);
    if (fs[idx].content[0])
        printk("\n");
}
