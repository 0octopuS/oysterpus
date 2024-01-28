# Heap operations


```C++
#include <algorithm>
#include <functional>
#include <iostream>
#include <string_view>
#include <vector>
 
void print(std::string_view text, std::vector<int> const& v = {}) {
    std::cout << text << ": ";
    for (const auto& e : v) std::cout << e << ' ';
    std::cout << '\n';
}
 
int main()
{
    print("Max heap");
 
    std::vector<int> v { 3, 2, 4, 1, 5, 9 };
    print("initially, v", v);
 
    std::make_heap(v.begin(), v.end());
    print("after make_heap, v", v);
 
    std::pop_heap(v.begin(), v.end());
    print("after pop_heap, v", v);
 
    auto top = v.back();
    v.pop_back();
    print("former top element", {top});
    print("after removing the former top element, v", v);
 
    print("\nMin heap");
 
    std::vector<int> v1 { 3, 2, 4, 1, 5, 9 };
    print("initially, v1", v1);
 
    std::make_heap(v1.begin(), v1.end(), std::greater<>{});
    print("after make_heap, v1", v1);
 
    std::pop_heap(v1.begin(), v1.end(), std::greater<>{});
    print("after pop_heap, v1", v1);
 
    auto top1 = v1.back();
    v1.pop_back();
    print("former top element", {top1});
    print("after removing the former top element, v1", v1);
}

Max heap:
initially, v: 3 2 4 1 5 9 
after make_heap, v: 9 5 4 1 2 3 
after pop_heap, v: 5 3 4 1 2 9 
former top element: 9
after removing the former top element, v: 5 3 4 1 2 
 
Min heap:
initially, v1: 3 2 4 1 5 9 
after make_heap, v1: 1 2 4 3 5 9 
after pop_heap, v1: 2 3 4 9 5 1 
former top element: 1
after removing the former top element, v1: 2 3 4 9 5
```

## is_heap

接收两个迭代器和（一个二元操作比较）。检查范围内是否是堆。
时间复杂度：$O(N)$次比较，其中 `N=std::distance(first, last)`。

## is_heap_until

接收两个迭代器和（一个二元操作比较）。检查范围内最大到哪里是堆，返回一个指向这个元素的迭代器。
时间复杂度：$O(N)$次比较，其中 `N=std::distance(first, last)`。

```C++
 std::make_heap(v.begin(), v.end());
 v.push_back(2);
 v.push_back(6);
 auto heap_end = std::is_heap_until(v.begin(), v.end());

// all of v:  9 5 4 1 1 3 2 6 
// only heap: 9 5 4 1 1 3 2
```

## make_heap

接收两个迭代器和（一个二元操作比较）。默认使用 `(<)`，构造最大堆。
时间复杂度： 最多 `3*std::distance(first, last)` 次比较。

```C++
std::make_heap(v.begin(), v.end()); // 最大堆
std::make_heap(v1.begin(), v1.end(), std::greater<>{}); // 最小堆
```

## push_heap

在容器末尾插入1个新元素后，调用 `push_heap` 来插入堆。
时间复杂度：最多 $log(N)$ 次比较，其中 `N=std::distance(first, last)`。

```C++
 std::make_heap(v.begin(), v.end());
 v.push_back(6);
 std::push_heap(v.begin(), v.end());
```

## pop_heap

将堆中的第一个 `swap` 到最后一个，前 $n-1$ 个重新建堆。（需要显式移掉那个元素）
时间复杂度：最多 $2×log(N)$ 次比较，其中 `N=std::distance(first, last)`。

```C++
std::make_heap(v.begin(), v.end());
std::pop_heap(v.begin(), v.end());

int largest = v.back(); // remove
v.pop_back();
```

## sort_heap

如果 `pop_heap` 中不进行移除，那n次后其实就得到有序（堆排序）。

```C++
while (first != last)
        std::pop_heap(first, last--, comp);
```

时间复杂度：最多 $2×N×log(N)$ 次比较，其中 `N=std::distance(first, last)`。

```C++
std::make_heap(v.begin(), v.end());
std::sort_heap(v.begin(), v.end());
```
