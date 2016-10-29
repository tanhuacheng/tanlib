// log.h
// 记录日志消息
// 
// API
//     LOG_NEWLINE(), LOGE(fmt, args...), LOGW(fmt, args...), LOGI(fmt, args...), 分别向日志文
// 件写入 "空行消息", "错误消息", "警告消息" 以及 "提示消息". 成功时返回写入日志文件的字符数,
// 否则返回一个小于 0 的值.
// 
// 配置
//     LOG_LEVEL 可设置为 LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_WARN, LOG_LEVEL_INFO 之一.
// 级别低于 LOG_LEVEL 的消息不被记录; "提示消息" 级别最低; LOG_LEVEL 设置为 LOG_LEVEL_NONE 时,
// 不会生成任何代码, 更不会记录任何消息; "空行" 级别最高.
//     创建和删除的文件名格式为 "LOG_FILE_DIR[/] + LOG_FILE_PREFIX + yyyy[x]-mm[x]-dd[x].log".
// 如果 LOG_FILE_DIR 不以字符 '/' 结束, 则自动追加字符 '/'; yyyy, mm, dd 分别表示调用 API 时的
// 年, 月, 日; [x] 表示任意字符串.
//     日志文件会保留 LOG_FILE_DAYS 天, 第一次或第二天第一次调用 API 时将自动删除更早期的文件.

#ifndef LOG_H_VT48WFXN
#define LOG_H_VT48WFXN

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_LEVEL_NONE  (0) // 关闭日志 - none
#define LOG_LEVEL_ERROR (1) // 错误消息 - LOGE
#define LOG_LEVEL_WARN  (2) // 警告消息 - LOGW
#define LOG_LEVEL_INFO  (3) // 提示消息 - LOGI

// 配置项
#define LOG_LEVEL       LOG_LEVEL_INFO
#define LOG_FILE_DIR    "./log"
#define LOG_FILE_PREFIX "test-"
#define LOG_FILE_DAYS   (15)

#if ((LOG_LEVEL < LOG_LEVEL_NONE) || (LOG_LEVEL > LOG_LEVEL_INFO))
#error LOG_LEVEL must be within LOG_LEVEL_NONE and LOG_LEVEL_INFO
#endif
#if ((LOG_LEVEL != LOG_LEVEL_NONE) && (LOG_FILE_DAYS <= 0))
#error LOG_FILE_DAYS must be great than 0
#endif

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
