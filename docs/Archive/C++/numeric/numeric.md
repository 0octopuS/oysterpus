# Numeric operations

Defined in header <numeric>

## reduce / transfrom_reduce

和`accumulate` 用法一样，多个policy。

`transfrom_reduce` 在 `reduce` 的基础上再加一个函数，对元素本身先进行transform。

```C++
// transform_reduce instead of reduce; e.g., a + b * b != b + a * a
void print_sum_squared(long const num)
{
    std::cout.imbue(std::locale{"en_US.UTF8"});
    std::cout << "num = " << num << '\n';
 
    // create an immutable vector filled with pattern: 1,2,3,4, 1,2,3,4 ...
    const std::vector<long> v { [n = num * 4] {
        std::vector<long> v;
        v.reserve(n);
        std::generate_n(std::back_inserter(v), n,
            [i=0]() mutable { return 1 + i++ % 4; });
        return v;
    }()};
 
    auto squared_sum = [](auto sum, auto val) { return sum + val * val; };
 
    auto sum1 = std::accumulate(v.cbegin(), v.cend(), 0L, squared_sum);
    std::cout << "accumulate(): " << sum1 << '\n';
 
    auto sum2 = std::reduce(PAR v.cbegin(), v.cend(), 0L, squared_sum);
    std::cout << "reduce(): " << sum2 << '\n';
 
    auto sum3 = std::transform_reduce(PAR v.cbegin(), v.cend(), 0L, std::plus{},[](auto val) { return val * val; });
    std::cout << "transform_reduce(): " << sum3 << "\n\n";
}
 
int main()
{
    print_sum_squared(1);
    print_sum_squared(1'000);
    print_sum_squared(1'000'000);
}
```



## inclusive_scan / exclusive_scan

进行（去当前元素）前缀扫描，默认使用 `binary_op (or std::plus<>()` 作为scan操作符。


```C++ 
int main()
{
  std::vector data {3, 1, 4, 1, 5, 9, 2, 6};
 
  std::cout << "exclusive sum: ";
  std::exclusive_scan(data.begin(), data.end(),
        std::ostream_iterator<int>(std::cout, " "), 0); // 去掉当前前缀，，初始值0
  std::cout << "\ninclusive sum: ";
  std::inclusive_scan(data.begin(), data.end(),
        std::ostream_iterator<int>(std::cout, " ")); // 前缀和
 
  std::cout << "\n\nexclusive product: ";  
  std::exclusive_scan(data.begin(), data.end(),
        std::ostream_iterator<int>(std::cout, " "),
        1, std::multiplies<>{});  // 去掉当前前缀乘，初始值1
  std::cout << "\ninclusive product: ";
  std::inclusive_scan(data.begin(), data.end(),
        std::ostream_iterator<int>(std::cout, " "),
        std::multiplies<>{});        // 前缀乘
}
```

输出为

```C++
exclusive sum: 0 3 4 8 9 14 23 25 
inclusive sum: 3 4 8 9 14 23 25 31 
 
exclusive product: 1 3 3 12 12 60 540 1080 
inclusive product: 3 3 12 12 60 540 1080 6480
```

## transform_inclusive_scan / transform_exclusive_scan

在 inclusive_scan / exclusive_scan 的基础上再加一个函数，对元素本身先进行transform。

```C++
int main()
{
  std::vector data {3, 1, 4, 1, 5, 9, 2, 6};
 
  auto times_10 = [](int x) { return x * 10; };
 
  std::cout << "10 times exclusive sum: ";
  std::transform_exclusive_scan(data.begin(), data.end(),
    std::ostream_iterator<int>(std::cout, " "),
    0, std::plus<int>{}, times_10);
  std::cout << "\n10 times inclusive sum: ";
  std::transform_inclusive_scan(data.begin(), data.end(),
    std::ostream_iterator<int>(std::cout, " "),
    std::plus<int>{}, times_10);
}
```

结果为

```C++
10 times exclusive sum: 0 30 40 80 90 140 230 250 
10 times inclusive sum: 30 40 80 90 140 230 250 310
```
