# Query operations

## all_of / any_of / none_of

## mismatch

接收 `first1, last1, first2, (last2), (BinaryPredicate p)`
返回两个range之间第一个不匹配的对（一个 `pair`）。可以自定义 `(==)` 断言。

!!! tip 回文
    如果给一个序列的左右两端，可以做 `mismatch` 回文判断。


```C++
std::string mirror_ends(const std::string& in)
{
    return std::string(in.begin(),
                       std::mismatch(in.begin(), in.end(), in.rbegin()).first);
}
 
int main()
{
    std::cout << mirror_ends("abXYZba") << '\n'
              << mirror_ends("abca") << '\n'
              << mirror_ends("aba") << '\n';
}
```

[mismatch的回文用法](https://leetcode.cn/problems/valid-palindrome)

## equal

接收 `first1, last1, first2, (last2), (BinaryPredicate p)`
返回是否全等的 `Bool` 值。

```C++
constexpr bool is_palindrome(const std::string_view& s){
    return std::equal(s.begin(), s.begin() + s.size() / 2, s.rbegin());}
```
