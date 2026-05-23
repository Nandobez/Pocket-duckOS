#ifndef _FEATURES_H
#define _FEATURES_H

#define MAX_FILES 64
#define MAX_NAME 64
#define MAX_CONTENT 512
#define MAX_PATH 256

struct file_entry {
    char name[MAX_NAME];
    char path[MAX_PATH];
    char content[MAX_CONTENT];
    int is_dir;
    int used;
};

extern struct file_entry fs[MAX_FILES];
extern char cwd[MAX_PATH];

/* fs.c */
void fs_init(void);
int fs_find(const char *path);
int fs_find_free(void);
void fs_make_path(char *out, const char *name);

/* comandos */
void cmd_quit(void);
void cmd_ls(char *arg);
void cmd_cd(char *path);
void cmd_mkdir(char *name);
void cmd_pwd(void);
void cmd_echo(char *text);
void cmd_touch(char *name);
void cmd_cat(char *name);
void cmd_write(char *args);
void cmd_rm(char *name);
void cmd_vim(char *name);
void cmd_color(char *args);

/* art */
void show_boot_art(void);

#endif
