# Bit operations


## bit_cast
  
(C++20)

reinterpret the object representation of one type as that of another
(function template)

## byteswap
  
(C++23)

reverses the bytes in the given integer value
(function template)

## has_single_bit

(C++20)
检查一个 integer 是否只有1个比特，也就是 -> 该数字是否是2的幂。
可能有两种实现

```C++
 x != 0 && (x & (x - 1)) == 0; // impl1
 std::popcount(x) == 1;        // impl2
```

用例： 打印二叉树、堆的分层。

```C++
 for (auto t{1U}; auto i : v)
        std::cout << i << (std::has_single_bit(++t) ? " │ " : " ");
// 9 │ 6 9 │ 5 5 9 7 │ 1 1 3 5 8 3 4 2 │
```

## bit_ceil / bit_floor
  
(C++20)

finds the largest integral power of two not greater than the given value
(function template)

## bit_width
  
(C++20)

finds the smallest number of bits needed to represent the given value
(function template)

## rotl / rotr
  
(C++20)

computes the result of bitwise right-rotation
(function template)

## countl_zero / countl_one / countr_zero / countr_one
  
(C++20)

counts the number of consecutive 1 bits, starting from the least significant bit
(function template)

## popcount
  
(C++20)

counts the number of 1 bits in an unsigned integer
(function template)

## endian
  
(C++20)

indicates the endianness of scalar types
(enum)
