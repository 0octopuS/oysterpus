# 错误信息

本章中对一些基本的错误类型进行注解和解决.

## 语法错误

```Haskell
Prelude> True +

<interactive>:10:7: error:
    parse error (possibly incorrect indentation or mismatched brackets)
```

遇到语法树无法解析的表达式.

```Haskell
Indent.hs:2:1: error: parse error on input ‘where’
```

缩进错误。


## 类型错误

### Example 1

```Haskell
Prelude> "string" ++ True

<interactive>:1:13: error:  -- 行定位信息
    • Couldn't match expected type ‘[Char]’ with actual type ‘Bool’      -- 错误信息
    • In the second argument of ‘(++)’, namely ‘True’
      In the expression: "string" ++ True
      In an equation for ‘it’: it = "string" ++ True
      -- 具体位置定位
```


expected type`[char]`类型是编译器期望的类型,这是由`(++)`运算符指定的.
actual type `Bool`是编译器找到的类型,也就是输入的`True`.二者不匹配,因此出现了类型错误.

!!! Note
    expected type并不一定总是对的,特别是当用户定义的函数或运算符没有显式给出类型签名的时候,此时编译器会进行自动推断,可能出现错误结果.(因此显式给出函数签名总是好的.)

错误信息的第二部分是逐层给出具体定位.最后一句`In an equation for ‘it’: it = "string" ++ True`表示再定义变量`it`的时候表达式出现错误,`it`是GHCi中对独立表达的默认变量名.

### Example 2

```Haskell
Prelude> True + 1

ghci> :t (+)
(+) :: Num a => a -> a -> a

<interactive>:6:1: error:
    • No instance for (Num Bool) arising from a use of ‘+’
    • In the expression: True + 1
      In an equation for ‘it’: it = True + 1
```

当试图在不合适的变量上使用`+`这样指明了type classes的函数时，就会出现这样的错误。`(Num Bool)`中,前面的符号指明了参数的type classes,后面的`Bool`指明了实际的类型.

```Haskell
halve :: Int -> Int
halve x = x / 2

ghci> :t (/)
(/) :: Fractional a => a -> a -> a

error:
    • No instance for (Fractional Int) arising from a use of ‘/’
    • In the expression: x / 2
      In an equation for ‘halve’: halve x = x / 2
```

这个错误也一样，`(/)`指明了`a`的type classes必须是`Fractional`，但是函数`halve`却使用了一个`Int`进行操作。