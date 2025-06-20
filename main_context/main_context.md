# 基础：IO 多路复用
IO 多路复用的意义：单线程下可以管理多个文件描述符，不因某个文件描述符对应的事件没有就绪而阻塞其他文件描述符的处理。

Linux 下有三种 IO 多路复用的方式：`select`，`poll` 和 `epoll`。
```c
int select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds, fd_set *restrict errorfds, struct timeval *restrict timeout);

int poll(struct pollfd fds[], nfds_t nfds, int timeout);

int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout); 
```

# glib IO 多路复用核心函数：`g_poll`

glib 作为跨平台库，支持 Linux 和 windows，为不同操作系统提供了相同的 `g_poll` 接口形式。后续只讨论 Linux。

Linux 下 `g_poll` 是对 `poll` 的简单封装。具体代码:
```c
// glib/gpoll.c
# #ifdef HAVE_POLL
// ...
gint
g_poll (GPollFD *fds,
	guint    nfds,
	gint     timeout)
{
  return poll ((struct pollfd *)fds, nfds, timeout);
}
```

