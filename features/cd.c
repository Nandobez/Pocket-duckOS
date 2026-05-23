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

static int mylen(const char *s)
{
    int i = 0;
    while (s[i]) i++;
    return i;
}

void cmd_cd(char *path)
{
    char fullpath[MAX_PATH];
    int idx, i, len;

    if (!path || !path[0] || (path[0] == '/' && !path[1])) {
        cwd[0] = '/';
        cwd[1] = '\0';
        return;
    }

    if (path[0] == '.' && path[1] == '.' && !path[2]) {
        len = mylen(cwd);
        if (len <= 1) return;

        for (i = len - 1; i >= 0; i--) {
            if (cwd[i] == '/' && i > 0) {
                cwd[i] = '\0';
                return;
            }
        }
        cwd[0] = '/';
        cwd[1] = '\0';
        return;
    }

    if (path[0] == '/') {
        mycopy(fullpath, path);
    } else {
        fs_make_path(fullpath, path);
    }

    idx = fs_find(fullpath);
    if (idx >= 0 && fs[idx].is_dir) {
        mycopy(cwd, fullpath);
    } else {
        printk("\ncd: '%s' not found\n", path);
    }
}
