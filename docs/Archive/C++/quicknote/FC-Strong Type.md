# Strong Type

在写（构造）函数的时候，经常会遇到参数右相同类型的变量，但是含义不同，例如：

```C++
struct Rectangle{
    double _w;
    double _h;
    Rectangle(double w, double h) :_w{w}, _h{h} {}
};
Rectangle r1(10,12); // 宽10长12
Rectangle r2(12,10); // 宽12长10
```

## 1. 使用强类型来代表原来类型相同的参数

这个代码本身没错。但是注意到两个参数类型都是 `double`，可能存在传反了的现象。
即使不传反，也需要用户去看接口的说明（即使依赖IDE会让这件事情变得简单）。

强类型的目标就是为了解决这样的问题。一个简单的解决方案是把这两个 `double` 都变成类。
这样就不可能出现误传的情况。

```C++
class Width{
private:
    double _w;
public:
    Width(double w): _w{w} {}
    const double get(){return _w;}
};
class Height{ ... };
struct Rectangle{
    double _w;
    double _h;
    Rectangle(Width w, Height h) :_w_{w.get()}, _+_h{h.get()} {}
};
Rectangle r1(Widht{10},Height{12}); // 宽10长12
```

## 2. NameType

因为不想每个其他的参数都设置一个类，所以最好能有一个东西把他们统一起来。
以下是一个抽象出来的 `NameType` 类型，接收某个值，并且给出返回方法。

```C++
template <typename T, typename Parameter>
class NamedType
{
public:
    explicit NamedType(T const& value) : value_(value) {}
    explicit NamedType(T&& value) : value_(std::move(value)) {}
    T& get() { return value_; }
    T const& get() const {return value_; }
private:
    T value_;
};
```

那么现在就可以这么使用它们：


```c++
using Width = NamedType<double, struct WidthParameter>;
using Height = NamedType<double, struct HeightParameter>;
Rectangle r(Width(10), Height(12));
```

这里的 `struct WidthParameter` 起到了一个tag的作用，区分不同的 `NamedType`。