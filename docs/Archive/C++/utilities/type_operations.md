# Type operations

## forward

[reference](https://en.cppreference.com/w/cpp/utility/forward)

`forward` 接受一个左值或右值。

```C++
// 将左值转发为左值或右值，取决于T
template< class T >
constexpr T&& forward( std::remove_reference_t<T>& t ) noexcept; // (1)

// 将右值转发为右值，不可能左值
template< class T >
constexpr T&& forward( std::remove_reference_t<T>&& t ) noexcept; // (1)
```

**🔘 (1)接收左值作为参数**
当 `t` 是一个 [forwarding reference](../language/6.declarations.md#41-reference-declaration)，`forward` 使得参数到另一个函数，类型转为该函数所声明的。

```C++
template<class T>
void wrapper(T&& arg)
{
    // arg is always lvalue
    foo(std::forward<T>(arg)); // Forward as lvalue or as rvalue, depending on T
}

// pass rvalue string -> T = std::String
// pass const lvalue string -> T = const std::String &
// pass non-const lvalue string -> T = std::String&
```

**🔘 (2)接收右值作为参数**
这个重载使得转发一个表达式（比如函数调用）的结果成为可能，这个表达式可以是rvalue或lvalue，作为转发引用参数的原始值类别。

```C++
// transforming wrapper
// 一个包装器不只是转发其参数，而是在该参数上调用一个成员函数，并转发其结果。
template<class T>
void wrapper(T&& arg)
{
    foo(forward<decltype(forward<T>(arg).get())>(forward<T>(arg).get()));
}
```



## forward_like

## move

!!! note move or forward
    在一个函数体里应该用move还是forward？
    | Signature                           | Action                              | Note                                     |
    | ----------------------------------- | ----------------------------------- | ---------------------------------------- |
    | `void Fun(Object p)`                | `x = std::move(p);`                 | p是函数拥有的一个对象                    |
    | `template<class T> void Fun(T &&p)` | `x = std::forward<T>(p);`           | forward 转发，判断传入的是左值还是右值。 |
    | `Object o{/* */}`                   | `x = std::move(p);`                 | 自己的对象。                             |
    | `auto&& o{/* */}`                   | `x = std::forward<decltype(o)>(o);` | auto是type推断，也是forward转发          |

## move_if_noexcept

`move_if_noexcept` 将获得其参数的**右值引用**，如果它的移动构造函数是 `noexcept`，或者没有 copy 构造函数（move-only 类型），则，否则获得其参数的左值引用。它通常被用来结合移动语义和强异常保证。

输入 `x`，根据异常保证返回`std::move(x)` 或 `x`。

!!! note 关于vector resize 和 noexcept
    例如，这被std::vector::resize所使用，它可能需要分配新的存储空间，然后将元素从旧的存储空间移动或复制到新的存储空间。如果在这个操作中发生了异常，std::vector::resize就会撤销它到此为止所做的一切，这只有在std::move_if_noexcept被用来决定是使用移动构造还是复制构造时才可能


```C++
#include <iostream>
#include <utility>
 
struct Bad
{
    Bad() {}
    Bad(Bad&&) { std::cout << "Throwing move constructor called\n"; }
    Bad(const Bad&) { std::cout << "Throwing copy constructor called\n";}
};
 
struct Good
{
    Good() {}
    Good(Good&&) noexcept {std::cout << "Non-throwing move constructor called\n";}
    Good(const Good&) noexcept { std::cout << "Non-throwing copy constructor called\n";}
};
 
int main()
{
    Good g;
    Bad b;
    Good g2 = std::move_if_noexcept(g); // Non-throwing move constructor called
    Bad b2 = std::move_if_noexcept(b); // Throwing copy constructor called
}
```

## to_underlying
  

