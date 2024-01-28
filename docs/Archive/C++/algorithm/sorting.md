# Sorting operations

## is_sorted

## is_sorted_until

## sort

可以传含 $operator()$ 的struct或者lambda比较。
时间复杂度：$N(\log N)$。

## partial_sort

进行部分排列，接收三个迭代器 `first, middle, last`， 在 `[first, middle)` 的范围间将有序。
时间复杂度：`(last-first)log(middle-first)` 比较。

```C++
std::array<int, 10> s{5, 7, 4, 2, 8, 6, 1, 9, 0, 3};
    std::partial_sort(s.begin(), s.begin() + 3, s.end());
    std::partial_sort(s.rbegin(), s.rbegin() + 4, s.rend());
    std::partial_sort(s.rbegin(), s.rbegin() + 5, s.rend(), std::greater{});
5 7 4 2 8 6 1 9 0 3
 
0 1 2 7 8 6 5 9 4 3
──────^
4 5 6 7 8 9 3 2 1 0
          ^────────
4 3 2 1 0 5 6 7 8 9
        ^──────────
```

## partial_sort_copy

## stable_sort

## nth_element

`nth_element` 将一个数组中的第 $n$ 大的元素放在第 $n$ 个位置。这个新的第n个元素之前的所有元素都 $\le$ 新的第n个元素之后的元素。
时间复杂度：平均 $O(N)$

```C++
 std::vector<int> v{5, 10, 6, 4, 3, 2, 6, 7, 9, 3};
 
 auto m = v.begin() + v.size()/2;
 std::nth_element(v.begin(), m, v.end());
 // The median is 6
 // v= {3, 2, 3, 4, 5, 6, 10, 7, 9, 6, }

 std::nth_element(v.begin(), v.begin()+1, v.end(), std::greater{});
 // The second largest element is 9
 // The largest element is 10
 // v= {10, 9, 6, 7, 6, 3, 5, 4, 3, 2, }
```
