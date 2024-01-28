# Partition operations

## is__partitiond

## partition

接收两个迭代器和一个一元断言，满足断言的排在前面，不满足排在后面。返回指向第一个不满足断言的元素的迭代器。
时间复杂度：$N$ 次断言判断，满足LegacyBidirectionalIterator则最多 $N/2$交换，否则 $N$次。

```C++
std::vector<int> v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
auto it = std::partition(v.begin(), v.end(), [](int i){return i % 2 == 0;});
// Partitioned vector: 0 8 2 6 4 * 5 3 7 1 9 
```


## partition_copy

## stable_partition

## partition_point

定位一个被 `partition` 操作过的容器中，第二个分区的头。如果都满足则返回end迭代器。
时间复杂度：$O(\log N)$。

```C++
std::array v = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
auto is_even = [](int i){ return i % 2 == 0; };
std::partition(v.begin(), v.end(), is_even);
//After partitioning, v: 8 2 6 4 5 3 7 1 9
const auto pp = std::partition_point(v.cbegin(), v.cend(), is_even);
// Partition point is at 4; v[4] = 5
const auto i = std::distance(v.cbegin(), pp);
// First partition (all even elements): 8 2 6 4
// Second partition (all odd elements): 5 3 7 1 9
```
