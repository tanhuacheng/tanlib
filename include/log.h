// log.h
// 记录日志消息
// 
// API(s)
//     LOG_NEWLINE(), LOGE(fmt, args...), LOGW(fmt, args...), LOGI(fmt, args...)
//     成功时返回写入的字符数, 否则返回一个小于 0 的值
// 
// 配置
//     LOG_LEVEL 可设置为 LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_WARN, LOG_LEVEL_INFO 之一.
// 级别低于 LOG_LEVEL 的消息不会记录(LOG_LEVEL_INFO 最低); LOG_LEVEL 设置为 LOG_LEVEL_NONE 时,
// 不会生成任何代码, 更不会记录任何消息.
// 

#ifndef LOG_H_VT48WFXN
#define LOG_H_VT48WFXN

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_LEVEL_NONE  0 // 关闭消息
#define LOG_LEVEL_ERROR 1 // 出错消息
#define LOG_LEVEL_WARN  2 // 警告消息
#define LOG_LEVEL_INFO  3 // 常规消息

#define LOG_LEVEL       LOG_LEVEL_INFO
#define LOG_FILE_DIR    "./log"
#define LOG_FILE_PREFIX "test-"

#if (LOG_LEVEL == LOG_LEVEL_NONE)
#define LOG_NEWLINE() ({(int)0;})
#else
int log_newline (void); // 不要直接使用
int log_write (const char* file, int line, const char* hi, const char* fmt, ...); // 不要直接使用
#define LOG_(hi, fmt, args...) log_write(__FILE__, __LINE__, hi, fmt, ##args) // 不要直接使用
#define LOG_NEWLINE() log_newline()
#endif

#define HI_NOP "\033[0m"
#define HI_RED "\033[1;31m"
#define HI_MAG "\033[1;35m"

#if (LOG_LEVEL < LOG_LEVEL_ERROR)
#define LOGE(fmt, args...) ({(int)0;})
#else
#define LOGE(fmt, args...) LOG_(HI_RED "error: " HI_NOP, fmt, ##args)
#endif

#if (LOG_LEVEL < LOG_LEVEL_WARN)
#define LOGW(fmt, args...) ({(int)0;})
#else
#define LOGW(fmt, args...) LOG_(HI_MAG "warning: " HI_NOP, fmt, ##args)
#endif

#if (LOG_LEVEL < LOG_LEVEL_INFO)
#define LOGI(fmt, args...) ({(int)0;})
#else
#define LOGI(fmt, args...) LOG_("", fmt, ##args)
#endif

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: LOG_H_VT48WFXN */
