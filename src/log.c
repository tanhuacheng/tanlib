// log.h
// 记录日志消息

/* 包含头文件 ------------------------------------------------------------------------------------*/
#include "log.h"

#if (LOG_LEVEL > LOG_LEVEL_NONE)

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <dirent.h>
#include <pthread.h>

/* 私有全局变量定义 ------------------------------------------------------------------------------*/
static FILE* log_file = NULL;
static struct tm last_tm;
static struct timeval last_tv;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static const char file_dir[] = LOG_FILE_DIR;
static const char file_prefix[] = LOG_FILE_PREFIX;

/* 函数定义 --------------------------------------------------------------------------------------*/
static inline bool isdiffday (const struct tm* tm_prep, const struct tm* tm_nowp)
{
    return !((tm_prep->tm_yday == tm_nowp->tm_yday) && 
             (tm_prep->tm_year == tm_nowp->tm_year));
}

// 移除当前时间(包含) 7 天前以外的文件(时间从文件名解析)
static inline bool needremove (const struct tm* tm_filp, const struct tm* tm_nowp)
{
    if (tm_filp->tm_year == tm_nowp->tm_year) {
        return (tm_filp->tm_yday < (tm_nowp->tm_yday - 6)) || (tm_filp->tm_yday > tm_nowp->tm_yday);
    } else if (tm_filp->tm_year == (tm_nowp->tm_year - 1)) {
        struct tm tm = *tm_filp;
        tm.tm_sec = 29; // avoid the impact of leap second
        tm.tm_mday += 6;
        mktime(&tm);
        return (tm.tm_year != tm_nowp->tm_year) || (tm.tm_yday < tm_nowp->tm_yday);
    }

    return true;
}

static inline bool start_with (const char* dstp, const char* patp)
{
    for (; *dstp && *patp && (*dstp == *patp); dstp++, patp++) {}
    return !(*patp);
}

// 检查文件名格式 yyyy[x]-mm[x]-dd[x].log([]可选), 并解析其中的创建时间信息
static int tm_filename (const char* filenamep, struct tm* tmp)
{
    memset(tmp, 0, sizeof(struct tm));

    if ((filenamep[0] < '0') || (filenamep[0] > '9')) {
        return -1;
    }
    tmp->tm_year = atoi(filenamep) - 1900;

    const char* chrp = NULL;
#   define MOVE_CHRP_TO_CH(strp, ch) \
    do { \
        if (NULL == (chrp = memchr(strp, ch, strlen(strp)))) { \
            return -1; \
        } \
    } while (0)

    MOVE_CHRP_TO_CH(filenamep, '-');
    filenamep = chrp + 1;
    tmp->tm_mon = atoi(filenamep) - 1;
    if ((tmp->tm_mon < 0) || (tmp->tm_mon > 11)) {
        return -1;
    }

    MOVE_CHRP_TO_CH(filenamep, '-');
    filenamep = chrp + 1;
    tmp->tm_mday = atoi(filenamep);
    if ((tmp->tm_mday < 1) || (tmp->tm_mday > 31)) {
        return -1;
    }

    MOVE_CHRP_TO_CH(filenamep, '.');
    if (strcmp(chrp + 1, "log")) {
        return -1;
    }

    mktime(tmp);

    return 0;
}

static int time_maybe_init (void)
{
    struct tm tm;
    gettimeofday(&last_tv, NULL);
    localtime_r(&last_tv.tv_sec, &tm);

    if ((NULL == log_file) || isdiffday(&last_tm, &tm)) {
        if (log_file) {
            fclose(log_file);
            log_file = NULL;
        }

        errno = 0;
        if ((mkdir(file_dir, 0775) < 0) && (EEXIST != errno)) {
            return -1;
        }
        DIR* dirp = opendir(file_dir);
        if (NULL == dirp) {
            return -1;
        }

#       define STRLEN_C(strp) (sizeof(strp) - 1)
        static char path[STRLEN_C(file_dir) + STRLEN_C(file_prefix) + 64] = LOG_FILE_DIR;
        static int dirlen = -1;
        if (dirlen < 0) {
            dirlen = STRLEN_C(file_dir);
            if (path[dirlen - 1] != '/') {
                path[dirlen++] = '/';
            }
        }

        for (struct dirent* dp = NULL; ;) {
            errno = 0;
            if (NULL == (dp = readdir(dirp))) {
                break;
            }
            if ((DT_REG == dp->d_type) && start_with(dp->d_name, file_prefix)) {
                struct tm tm_dp;
                if (tm_filename(dp->d_name + STRLEN_C(file_prefix), &tm_dp) < 0) {
                    continue;
                }
                if (needremove(&tm_dp, &tm)) {
                    strcpy(path + dirlen, dp->d_name);
                    remove(path);
                }
            }
        }

        if (errno) {
            closedir(dirp);
            return -1;
        }
        closedir(dirp);

        sprintf(path + dirlen, "%s%d-%d-%d.log", file_prefix,
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
        if (NULL == (log_file = fopen(path, "a"))) {
            return -1;
        }
    }
    last_tm = tm;

    return 0;
}

#define TIME_MAYBE_INIT_LOCK() \
do { \
    pthread_mutex_lock(&mutex); \
    if (time_maybe_init() < 0) { \
        pthread_mutex_unlock(&mutex); \
        return -1; \
    } \
} while (0)

#define RETURN_ON_ERROR_UNLOCK(r) \
do { \
    typeof(r) _r = r; \
    if (_r < 0) { \
        pthread_mutex_unlock(&mutex); \
        return _r; \
    } \
} while (0);

#define RETURN_UNLOCK(r) \
do { \
    typeof(r) _r = r; \
    pthread_mutex_unlock(&mutex); \
    return _r; \
} while (0)

int log_newline (void)
{
    TIME_MAYBE_INIT_LOCK();
    RETURN_UNLOCK(fprintf(log_file, "\n"));
}

int log_write (const char* file, int line, const char* hi, const char* fmt, ...)
{
    TIME_MAYBE_INIT_LOCK();

    int n = 0;

    int result = fprintf(log_file, "%dh:%dm:%ds-%ldus: ",
        last_tm.tm_hour, last_tm.tm_min, last_tm.tm_sec, last_tv.tv_usec);
    RETURN_ON_ERROR_UNLOCK(result);
    n += result;

    result = fprintf(log_file, "%s:%d: %s", file, line, hi);
    RETURN_ON_ERROR_UNLOCK(result);
    n += result;

    va_list ap;
    va_start(ap, fmt);
    result = vfprintf(log_file, fmt, ap);
    va_end(ap);
    RETURN_ON_ERROR_UNLOCK(result);
    n += result;

    int fmtlen = strlen(fmt);
    if ((0 == fmtlen) || ('\n' != fmt[fmtlen - 1])) {
        result = fprintf(log_file, "\n");
        RETURN_ON_ERROR_UNLOCK(result);
        n += result;
    }

    RETURN_UNLOCK(n);
}

#endif

/****************************************** end of file *******************************************/
