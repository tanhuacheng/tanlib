// getpid-byname.c
// 通过名字获取进程 ID

/* 包含头文件 ------------------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

#include "getpid-byname.h"

/* 函数定义 --------------------------------------------------------------------------------------*/
static inline bool start_with (const char* dstp, const char* patp)
{
    for (; *dstp && *patp && (*dstp == *patp); dstp++, patp++) {}
    return !(*patp);
}

static int find_name (FILE* fp, char* namep, size_t size)
{
    char strline[256] = "";
    char strname[256] = "";

    for (;;) {
        if (NULL == fgets(strline, sizeof(strline), fp)) {
            break;
        }
        strline[254] = '\n';
        strline[255] = '\0';
        if (start_with(strline, "Name:")) {
            sscanf(strline, "%*s %s", strname);
            strncpy(namep, strname, size - 1);
            namep[size - 1] = '\0';
            return 0;
        }
    }

    return -1;
}

pid_t getpid_byname (const char* namep, pid_t skip)
{
    if (NULL == namep) {
        return -1;
    }

    DIR* dirp = opendir("/proc");
    if (NULL == dirp) {
        return -1;
    }

    char strbuff[strlen(namep) + 24];

    for (struct dirent* dp = NULL; ;) {
        if (NULL == (dp = readdir(dirp))) {
            break;
        }
        if ((DT_DIR != dp->d_type) || !strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")) {
            continue;
        }

        pid_t pid = atoi(dp->d_name);
        if ((pid <= 0) || (pid == skip)) {
            continue;
        }
        sprintf(strbuff, "/proc/%d/status", pid);

        FILE* fp = fopen(strbuff, "r");
        if (NULL == fp) {
            continue;
        }

        if (find_name(fp, strbuff, sizeof(strbuff)) < 0) {
            fclose(fp);
            continue;
        }
        fclose(fp);

        if (!strcmp(strbuff, namep)) {
            closedir(dirp);
            return pid;
        }
    }

    closedir(dirp);
    return -1;
}

/****************************************** end of file *******************************************/
