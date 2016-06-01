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
> static inline uint32_t imod (uint32_t x, uint32_t y, uint32_t* z);  
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

