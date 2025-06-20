glib 引入 event loop，本质上是引入一套事件驱动的处理框架。该框架与 Linux 原生的系统调用相比，可以处理更多场景。

以 Linux 原生系统调用的视角，线程关注 fd 是否产生事件。

引入 glib event loop 后，线程的关注点产生变化，关注更高维度。

# GMainLoop, GMainContext 与 GSource

## 基本概念 

### GSource
GSource 指一个事件源。GSource 中可以包含多个 Linux FD ，但 GSource 中包含的内容不止 Linux FD。

Linux 原生 IO 多路复用只能通过一个 FD 是否产生事件表示该 FD 代表的事件源是否可以被处理。

引入 GSource 后，表示 “一个事件源可以被处理“ 的方式更加多样了，例如：
* 任意数量的指定 FD 有 poll 的结果；
* 当前时间戳超过了 GSource 中的时间戳；
* 等等


## 数量关系
GMainLoop : GMainContext = 1 : 1

GMainContext : GSource = 1 : n

GSource : Linux Fd = 1 : n

# glib 内置的 GSource 

glib 内置了多种预定义的 GSource:
* timeout_source
* ......

这些 GSource 都定义在 `glib/gmain.c` 中

## timeout_source 实现分析

timeout_source 没有通过 Linux Timer 实现，即 GSource 中是不含 fd 的。

GMainLoop 框架中有一部分通用逻辑，会比较 GSource 中是否有 ready_time。如果有 ready_time，那么比较当前时刻是否大于 GSource->ready_time，是则触发 timeout_source 的 dispatch。

具体代码是：
```c
// glib/gmain.c
if (result == FALSE && source->priv->ready_time != -1)
            {
              if (!context->time_is_fresh)
                {
                  context->time = g_get_monotonic_time ();
                  context->time_is_fresh = TRUE;
                }

              if (source->priv->ready_time <= context->time)
                { // 如果当前时刻超过 GSource 的就绪时刻（ready_time）, 则认为 GSource 可以被处理了
                  source_timeout_usec = 0;
                  result = TRUE;
                }
              else if (source_timeout_usec < 0 ||
                       (source->priv->ready_time < context->time + source_timeout_usec))
                {
                  source_timeout_usec = MAX (0, source->priv->ready_time - context->time);
                }
            }
```
