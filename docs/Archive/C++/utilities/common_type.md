# Common Types

## 1. pair

## 2. tuple

## 3. optional

[📖 fluentcpp](https://www.fluentcpp.com/2016/11/24/clearer-interfaces-with-optionalt/)

`optional<T>` 代表可能包含或者不包含一个类型为 `T` 的值，替代了之前使用 `nullptr/0/-1/false` 的情况。

和 `nullptr` 比起来，虽然二者都提供了检查是否为空的能力，但是 `optional` 是**值语义**的，例如复制、比较等操作都是基于值的。

和空字符串 `""` 对比起来， `""` 表示的可能是1.字符串值为空 2. 字符串不存在两种情况。
比如需要构建一个询问的时候，可能无法分辨 `""` 的语义是1. 希望查询该字段确实等于 `""`  2. 希望对该字段不做筛选 这两种情况。
但是 `optional` 就可以使用 `value="" / nullopt` 将其区分开来。

对应 Haskell 中的 `Maybe x = Just x | Nothing`  , Rust 中的 `Option<T> = Some<T> | None`。

主要有以下一些优点：

1. 安全代码：编译器或解释器可以强制要求在使用前检查该值是否无效。这有助于防止运行时的错误，如空指针异常。
2. 表达性：比另外检查是否为 `nullptr` 或0更清晰易懂。
3. 工具性：许多现代集成开发环境和代码编辑器在处理可选类型时可以提供有用的建议和自动补全。
4. 类型安全🌟： 在有类型检查的语言中，可选类型可以帮助确保以类型安全的方式使用该值。

### 3.1 constructor

`optional` 主要有以下几种初始化方式

=== "**🔘 空初始化**"
    其值为 `nullopt`。

    ```C++
        optional<L> op1{};           // case1: 空初始化，op1.has_value: false
        // >>> p1.has_value: false // empty
    ```

=== "**🔘 从右值初始化**"
    将会使用 `move`。
  
    ```C++
        optional<L> op2{L{}};        // case2: prvalue初始化，con + move + de
        //  >>> op2.has_value: true, value: L2  // rvalue
        // | +L0   [default0 constructor]
        // | +L2   [move constructor]
        // | ~L0   [destructor]
    ```

=== "**🔘 从左值初始化**"
    将会使用 `copy`。

    ```C++
        L l1;
        // | +L0   [default0 constructor]
        optional<L> op3{l1};         // case3: lvalue初始化，con + copy 
        //  >>> op3.has_value: true, value: L1  
        // | +L1   [copy constructor]
    ```

=== "**🔘 从 `optional` 初始化**"
    注意就算move之后，原来的 `optional` 值还是没变。

    ```C++
        optional<L> op4_1{op2};        // case4: 从optional初始化，copy
        //  | +L3    [copy constructor]
        // >>> op4_1.has_value: true, value: L
        
        optional<L> op4_2{std::move(op2)}; // case5: move optional ????????????????
        //  | +L4    [move constructor]
        // >>> op4_2.has_value: true, value: L4  // move
        // >>> op2.has_value: true, value: L2  // rvalue
    ```

=== "**🔘 原地构造**"
    直接传所含类型的构造参数（最快）。

    ```C++
        optional<L> op5{std::in_place, 55}; // case6: in_place construct
        //  >>> op5.has_value: true, value: L55
    ```

### 3.2 modifier

**🔘 `optional.emplace`** 是进行原地构造。


=== "直接 `emplace` 构造参数"

    和上面的constructor case5 相同。

      ```C++
      optional<L> op1{};
      // >>> op1.has_value: false // empty
      op1.emplace(7);
      // | +L7    [default constructor]
      //  >>> op1.has_value: true, value: L7 
      ```

== "在原有值的基础上 `emplace`"

    会先析构之前的值；也可以 `emplace` 已存在的內部值，会相应调用move/copy

      ```C++
      op1.emplace(L{11});
      // | +L11   [default constructor]
      // | ~L7    [destructor]
      // | +L13   [move constructor]
      // | ~L11   [destructor]
      L l1{16};
      op1.emplace(l1); 
      //  >>> op1.has_value: true, value: L13 
      // | +L16   [default constructor]
      // | ~L13   [destructor]
      // | +L17   [copy constructor]
      ```

**🔘 `optional.reset`** 就是清空值，变成 `nullopt`。

**🔘 `optional.swap`** 交换两个`optional`。需要所包含的类型相同。

### 3.3 Observers

如何判断 `optional` 是否包含一个值？可以使用 `has_value`。

    ```C++
    auto observe1(auto op) {
      if (op.has_value()) {
        fmt::print("op has value: {}\n", op.value());
      } else {
        fmt::print("op has no value\n");
      }
    }

    optional<int> op{1};
    optional<int> op_empty{};
    observe1(op);       // op has value: 1
    observe1(op_empty); // op has no value
    ```

但是通常判断有无值后都想调用 `value()`，而 `value()` 函数中本来就调用了一次 `has_value()`，在没有值时抛出异常。

或者使用重载的操作符 `bool` 和 `*` 来判断、取值。

    ```C++
    auto observe2(auto op) {
      if (op) {
        fmt::print("op has value: {}\n", *op);
      } else {
        fmt::print("op has no value\n");
      }
    }

        optional<int> op{2};
        optional<int> op_empty{};
        observe2(op);       // op has value: 2
        observe2(op_empty); // op has no value
    ```

也可以使用 `value_or` 来指定默认值

    ```C++
    auto observe3(auto op) {
      fmt::print("op has {}value: {}\n", op.has_value() ? "" : "no ",
                op.value_or(42));
    }
        optional<int> op{3};
        optional<int> op_empty{};
        observe3(op);       // op has value: 3
        observe3(op_empty); // op has no value: 42
    ```

### 3.4 monadic operations

**🔘 and_then**
如果 `optional` 中包含一个值，则调用函数 `F`，否则返回 `nullopt`。
**🔘 or_else**
如果 `optional` 中包含一个值，则返回它本身，否则调用函数 `F`。

!!! Note `optional` 的setter设置
    例如结构体里有一个 `optional<Cert> cert_`，现在要写一个成员函数 `void setCert(???);`，接收一个外部 `Cert` ，如何设定参数？

    - 通常使用 `void setCert(optional<Cert>);`，然后 `move` 进外部 `Cert`。
    - 通过 `void setCert(Cert&&) + void setCert(std::optional<Cert>)` ，在纯右值的时候能省一次move。
    - ❌基本不用 `void setCert(const std::optional<Cert>&)` ，因为调用者不一定有个 `optional<Cert>`， 可能会给一个 `Cert` 临时值。


# expected

# variant

# any
