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
本质上都是关注文件描述符产生的事件。

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

# e.g.1 一个简单的 main event loop 示例
功能：在 main event loop 里关注一个 timeout 事件源，该事件源每 1 s 触发一次。触发 10 次后主进程退出。
```c
// The main loop manages a timeout source with internal=1 second;
// The main loop quits after firing 10 times.

#include <stdio.h>
#include <glib.h>

static int timeout_count = 0;

gboolean timeout_callbask(gpointer user_data){
    printf("timeout_source fired %d times\n", timeout_count + 1);
    if(timeout_count == 9){
        GMainLoop *l = (GMainLoop*)user_data;
        printf("Quit the main loop\n");
        g_main_loop_quit(l);
    }
    timeout_count++;
}

int main(int argc, char *argv[]){
    GMainContext *context = g_main_context_default();
    GSource *timeout_source = g_timeout_source_new(1000);
    g_source_set_callback(timeout_source, timeout_callbask, &timeout_count, NULL);
    g_source_attach(timeout_source, context);
    
    GMainLoop *l = g_main_loop_new(context, FALSE);
    g_source_set_callback(timeout_source, timeout_callbask, l, NULL);
    g_main_loop_run(l);
    return 0;
}
```

这个示例里包含了 main event loop 的基本使用方式：
1. 创建事件源 `GSource` ，设置回调函数；
2. 创建 `GMainContext` ，将 `Gsource` 关联到 `GMainContext`；
3. 创建 `GMainLoop`，将 `GMainContext` 关联到 `GMainLoop`;
4. 运行 `GMainLoop`；