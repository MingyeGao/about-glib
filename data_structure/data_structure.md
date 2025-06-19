# GHashTable

GHashTable 可以被用作 dict（key-value 查找）与 set（判断一个元素是否在集合中）。

## 创建 GHashTable
创建 GHashTable 使用：
```c
GHashTable* g_hash_table_new (GHashFunc hash_func, GEqualFunc key_equal_func);
```

其中 hash_func 和 key_equal_func 可以为 NULL。

为 NULL 时，二者的默认值分别为 g_direct_hash 和 g_direct_equal，内部实现就是把 void* 转为 uint。

glib 自带对以下类型数据结构的 hash 和 equal 函数：
* str
* int
* int64
* double

g_hash_table 可以作为 set 或 dict 使用。

## 作为 set 使用
* `g_hash_table_add` 加入元素；
* `g_hash_table_contains` 判断元素是否在 set 中；

## 作为 dict 使用
* g_hash_table_insert 加入元素作为 key，另有一个参数作为 value
* g_hash_table_lookup 查找元素对应的 value

> *BTW*
> 
> **Q:** 如果用 g_hash_table_add() 加入一个元素，那么它可以作为 key/value 对被 lookup 查询吗？查询到的值是什么?
> 
> **A:** 可以被查询到，通过 add 被加入的元素，key 和 value 都是它本身。

