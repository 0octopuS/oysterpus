# Ranges adaptors

## join / join_view

将一个 `view` 中的内容摊平，获得一串序列。

```C++
const auto bits = { "https:"sv, "//"sv, "cppreference"sv, "."sv, "com"sv };
    for (char const c : bits | std::views::join) std::cout << c;
//https://cppreference.com
 const std::vector<std::vector<int>> v{ {1,2}, {3,4,5}, {6}, {7,8,9} };
    auto jv = std::ranges::join_view(v);
//1 2 3 4 5 6 7 8 9
```
