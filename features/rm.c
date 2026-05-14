#include <duckos/kernel.h>
#include <duckos/features.h>
#include <string.h>

void cmd_rm(char *name)
{
    char path[MAX_PATH];
    int idx;

    if (!name || !name[0]) {
        printk("\nrm: missing name\n");
        return;
    }

    fs_make_path(path, name);
    idx = fs_find(path);

    if (idx < 0) {
        printk("\nrm: '%s' not found\n", name);
        return;
    }

    fs[idx].used = 0;
    fs[idx].name[0] = '\0';
    fs[idx].path[0] = '\0';
    fs[idx].content[0] = '\0';
}
