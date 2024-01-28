# Modify operations

## reverse

接收两个迭代器，将元素逆序。
时间复杂度：准确的 $N/2$ 次交换。

```C++
std::vector<int> v{1, 2, 3};
std::reverse(v.begin(), v.end()); // 3 2 1
```

## rotate

接收三个迭代器 `(first, middle, last)`，其中 `middle` 代表将会出现在容器头的元素。
时间复杂度：$O(N)$

```C++
std::rotate(v.begin(), v.begin() + 1, v.end()); // 左移1
std::rotate(v.rbegin(), v.rbegin() + 1, v.rend()); // 右移1
```

## shuffle

接收两个迭代器和一个随机种子，经常使用 `std::rand`。对容器中的元素进行随机排列。
时间复杂度：$O(N)$

```C++
std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
std::random_device rd;
std::mt19937 g(rd());
std::shuffle(v.begin(), v.end(), g);
```

## next_permutation / prev_permutation

将一个range按照字典序逐个输出，每次调用生成下一个。如果存在这样的 "下一个排列组合"，则返回true；否则，将该范围转化为lexicographically的第一个排列组合（就像通过`std::sort(first, last, comp)`）并返回false。

```C++
std::string s = "aba";
std::sort(s.begin(), s.end());
do {
    std::cout << s << '\n';
} while(std::next_permutation(s.begin(), s.end()));

// aab
// aba
// baa
```

