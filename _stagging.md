# 函数细节
## g_source_attach
函数签名：
```c
guint g_source_attach (GSource *source, GMainContext *context)
```
如果 context=NULL，表示将该 source attach 到 default_main_context 中。