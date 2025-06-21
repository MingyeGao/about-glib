验证 glib 代码细节时，需要修改代码，构建自定义 glib 库，然后再在编译时使用该 glib 库。

本文记录从编译 glib 库到构建 glib 应用的完整过程。

# 获取 glib 代码

glib 代码托管在 https://gitlab.gnome.org/GNOME/glib

```shell
git clone git@ssh.gitlab.gnome.org:GNOME/glib.git
```

# 编译 glib 库

## 使用指定分支
```shell
git checkout 2.74.3
```

## 解决依赖
glib 使用 meson 进行构建管理，需要使用 meson 版本 >= 1.4.0。

我验证时使用 ubuntu 发行版，通过 apt 获取的 meson 达不到这个版本，所以需要使用 pip 安装指定版本。

需要注意 pip 安装时需要以 sudo 安装。
否则 meson setup 和 meson compile 使用非 root python 路径，meson install 强制切换为 root python 路径，路径不一致导致 include 文件不匹配，会导致 install 报错而失败。

## 构建与编译
```shell
cd glib
meson setup --prefix /usr/local/my_glib _build # 指定安装 glib 到 /usr/local/my_glib 下，不与系统包管理工具安装的 glib 冲突
meson compile -C _build
meson install -C _build
```


# 构建 glib 应用

## 编译时指定自定义 glib 目录
```c
 gcc -g \
    -I/usr/local/my_glib/include/glib-2.0/ \
    -I/usr/local/my_glib/lib/x86_64-linux-gnu/glib-2.0/include/ \
    -Wl,-rpath,/usr/local/my_glib/lib/x86_64-linux-gnu/ \
    -L/usr/local/my_glib/lib/x86_64-linux-gnu/  \
    -o main main.c -lglib-2.0
```
参数说明：
* `-I` 指定 include 路径为自定义 glib 的路径。
* `-Wl,-rpath` 表示向 linker 传递的参数，linker 运行时检查的路径要包括自定义 glib 路径。否则编译不会报错，但应用程序也不会真实使用自定义的 glib 动态链接库。
* `-L` 表示编译器搜索动态链接库的路径。

## 验证应用使用了自行构建的 glib
使用 ldd 查看应用使用的动态链接库：
```shell
$ ldd main
        linux-vdso.so.1 (0x00007fff21f5c000)
        libglib-2.0.so.0 => /usr/local/my_glib/lib/x86_64-linux-gnu/libglib-2.0.so.0 (0x00007f0eb471a000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f0eb44e8000)
        libpcre2-8.so.0 => /lib/x86_64-linux-gnu/libpcre2-8.so.0 (0x00007f0eb4451000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f0eb4877000)
```
可以看到 libglib 的目录在 `/usr/loca/my_glib` 下，说明成功使用了自行编译的 glib。