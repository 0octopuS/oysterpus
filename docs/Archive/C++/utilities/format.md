# Format 


## 1.format

### format ranges

```C++
expect(R"(("hello", 42))"s == std::format("{}", std::tuple{"hello", 42}));
expect("[0x1, 0x2, 0x3]"s == std::format("{::#x}", std::vector{1, 2, 3}));
expect("[*1*, *2*, *3*]"s == std::format("{::*^3}", std::vector{1, 2, 3}));
expect("Q L"s == std::format("{}", std::string{'Q', ' ', 'L'}));
expect(R"(["a", "bc"])"s == std::format("{::?s}", std::vector{std::vector{'a'}, std::vector{'b','c'}}));
expect("[[97], [98, 99]]"s == std::format("{:::d}", std::vector{std::vector{'a'}, std::vector{'b','c'}}));
```
