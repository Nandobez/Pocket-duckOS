#include <duckos/features.h>
#include <duckos/kernel.h>
#include <string.h>

struct file_entry fs[MAX_FILES];
char cwd[MAX_PATH];

void fs_init(void)
{
    int i;
    for (i = 0; i < MAX_FILES; i++) {
        fs[i].used = 0;
        fs[i].is_dir = 0;
        fs[i].name[0] = '\0';
        fs[i].path[0] = '\0';
        fs[i].content[0] = '\0';
    }
    cwd[0] = '/';
    cwd[1] = '\0';
}

static int streq(const char *a, const char *b)
{
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return (a[i] == b[i]);
}

int fs_find(const char *path)
{
    int i;
    if (!path || !path[0]) return -1;
    for (i = 0; i < MAX_FILES; i++) {
        if (fs[i].used && streq(fs[i].path, path))
            return i;
    }
    return -1;
}

int fs_find_free(void)
{
    int i;
    for (i = 0; i < MAX_FILES; i++) {
        if (!fs[i].used)
            return i;
    }
    return -1;
}

void fs_make_path(char *out, const char *name)
{
    int i = 0, j = 0;

    /* copia cwd */
    while (cwd[i]) {
        out[j++] = cwd[i++];
    }

    /* adiciona / se necessário */
    if (j > 1) {
        out[j++] = '/';
    }

    /* copia nome */
    i = 0;
    while (name[i]) {
        out[j++] = name[i++];
    }
    out[j] = '\0';
}
