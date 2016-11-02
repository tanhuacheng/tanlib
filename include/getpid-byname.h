// getpid-byname.h
// 通过名字获取进程 ID

#ifndef GETPID_BYNAME_H_HW9RSVIT
#define GETPID_BYNAME_H_HW9RSVIT

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

pid_t getpid_byname (const char* namep, pid_t skip);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: GETPID_BYNAME_H_HW9RSVIT */
