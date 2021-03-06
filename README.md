# tanlib
收集一些常用的代码

## 使用如下命令在linux下编译测试代码
> gcc -Wall --std gnu99 -I include/ src/memfill.c test/tmemfill.c  

## 1. memfill, 使用内存块填充内存区域.
可用于使用一个数据初始化一个数组, 特别是结构体数组
> void* memfill (void* restrict dstp, size_t dstsize, const void* restrict patp, size_t patsize);  
> dstp: 填充区域  
> dstsize: 填充区域字节数  
> patp: 内存块  
> patsize: 内存块字节数  

## 2. divmod, 除法和取余.
有些平台(列如ARM), 不支持除法指令, 取余和除法运算比较慢(有时可能需要几百条指令才能完成)
> static inline uint32_t incmod (uint32_t inc, const uint32_t mod);  
> inc加1后对mod取余, 但须保证inc小于mod  
>  
> static inline uint32_t divmod (uint32_t x, uint32_t y, uint32_t* z);  
> 计算x/y: z在原值上加商, 返余数. 注意, 仅在x/y较小时用  

## 3. queue-array, 列队(数组实现).
依赖memfill模块. 该实现不是线程安全的, 但是允许一个线程read另一个线程write或fill, 而无需保护, 但是不能在没有保护的情况下多个线程同时读(写); 使用flush时必须由应用保证直到其返回前不能进行其它操作.
> int32_t queue_array_init (queue_array_t* q, void* buff, const size_t size, const uint16_t mlen);  
> buff: 列队使用的缓冲区  
> size: 一个数据单元的字节数  
> mlen: 缓冲取中最多可存放的数据单元数(列队满时实际包含mlen-1个数据单元)  
>  
> int32_t queue_array_read (queue_array_t* q, void* buff, uint16_t length);  
> buff的大小至少需要有 length * size 字节  
>  
> int32_t queue_array_fill (queue_array_t* q, const void* buff, uint16_t length);  
> buff只包含一个字节数为size的数据单元, 使用该数据连续入队length次  
>  
> int32_t queue_array_write (queue_array_t* q, const void* buff, uint16_t length);  
> buff至少须包含 length * size 字节, 依序写入队列中  
>  
> int32_t queue_array_flush (queue_array_t* q);  
> 清空列队  
>  
> int32_t queue_array_length (const queue_array_t* q);  
> 获取列队中包含的数据单元数  
>  

## 4. jitter, 抖动缓冲区.
创建jitter需要提供数据单元的字节数和每次读取的数据单元数. 缓冲区大小等在jitter.h中定义
> jitter_t* jitter_create (const size_t size, const uint16_t unit);  
> size: 数据单元的字节数; unit: 调用 jitter_read 时返回的数据单元数.  
>  
> int32_t jitter_read (jitter_t* jitter, void* buffer);  
> buffer中至少须包含 unit*size 字节, 返回<0表示出错, 否则代表由于缓冲区内数据较少而插入的全0数据单元数  
>  
> int32_t jitter_write (jitter_t* jitter, const void* buffer, uint16_t length);  
> 返回<0表示出错, 否则代表由于缓冲区数据太多而丢弃的数据单元数  
>  
> int32_t jitter_flush (jitter_t* jitter);  
> 清空缓冲区  
>  
> int32_t jitter_destroy (jitter_t* jitter);  
>  释放 jitter 占用资源  
>  

## 5. queue-list, 列队(链表实现).
依赖 pthread.h 和 sys/time.h, 读列队时可选择立即返回, 超时等待, 一直等待等方式. 
> queue_list_t* queue_list_create (void);  
> 创建一个列队  
>  
> int32_t queue_list_read (queue_list_t* ql, void* buffer, uint16_t limit, int32_t timeout);  
> 读取一个buffer, timeout: <0 = 立即返回, 0 = 一直等待, >0 = 超时时间(ms)  
>  
> int32_t queue_list_write (queue_list_t* ql, const void* buffer, uint16_t size);  
>  写入一个buffer, 如果读列队操作正在等待, 将激活之  
>  
> int32_t queue_list_flush (queue_list_t* ql);  
> 清空列队, 将激活所有正在等待的读列队操作  
>  
> int32_t queue_list_destroy (queue_list_t* ql);  
> 销毁列队, 须保证销毁ql之时和之后没有任何其它线程使用它  
>  

## 6. queue-link, 列队(链表实现, 固定数据长度).
和 queue-list 类似， 但是每次读写时不用提供数据长度参数， 而是在创建时指定, 之后的读写都使用这个长度. 其中使用了一个空闲数据栈, 避免了每次读写时的释放和申请内存的花销
> queue_link_t* queue_link_create (const int32_t size);  
> int32_t queue_link_read (queue_link_t* ql, void* buff, const int32_t timeout);  
> int32_t queue_link_write (queue_link_t* ql, const void* buff);  
> int32_t queue_link_flush (queue_link_t* ql);  
> int32_t queue_link_destroy (queue_link_t* ql);  
>  

## 7. 二叉搜索树

## 8. foreach

## 9. sort(insert-sort, merge-sort)

## 10. max-subarray, 查找最大子数组

## 11. zero-cross, 计算信号的短时过零数

## 12. log, 记录日志消息

## 13. getpid-byname, 通过名字获取进程 ID

