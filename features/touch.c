#include <duckos/kernel.h>
#include <duckos/features.h>

static void mycopy(char *dst, const char *src)
{
    int i = 0;
    while (src[i]) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

void cmd_touch(char *name)
{
    char path[MAX_PATH];
    int idx;

    if (!name || !name[0]) {
        printk("\ntouch: missing name\n");
        return;
    }

    fs_make_path(path, name);

    if (fs_find(path) >= 0) {
        return;
    }

    idx = fs_find_free();
    if (idx < 0) {
        printk("\ntouch: no space\n");
        return;
    }

    mycopy(fs[idx].name, name);
    mycopy(fs[idx].path, path);
    fs[idx].is_dir = 0;
    fs[idx].used = 1;
    fs[idx].content[0] = '\0';
}
