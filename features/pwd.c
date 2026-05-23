#include <duckos/kernel.h>
#include <duckos/features.h>

void cmd_pwd(void)
{
    printk("\n%s\n", cwd);
}
