# 基础：IO 多路复用
IO 多路复用的意义：单线程下可以管理多个文件描述符，不因某个文件描述符对应的事件没有就绪而阻塞其他文件描述符的处理。

glib 多路复用的关键函数是:
```c
// /root/glib/glib/gpoll.c
gint
g_poll (GPollFD *fds,
	guint    nfds,
	gint     timeout)
{
  // ...
}
```
glib 作为跨平台库，支持 Linux 和 windows，为不同操作系统提供了相同的 `g_poll` 接口形式。后续只讨论 Linux。

