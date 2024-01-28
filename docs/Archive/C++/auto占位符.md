# ``auto``占位符(C++11~C++17)

## 1. 定义及使用

``auto``占位符用于在声明变量时根据初始化表达式自动推断该变量的类型、声明函数时函数返回值的占位符。

```C++
auto i = 5;               // i被推断为int
auto str = "Hello World"; // str被推断为const char*
auto sum(int a1, int a2) -> int{   // 返回类型后置，auto为返回值占位符
    return a1 + a2; 
}
```

在对变量进行初始化的时候，``auto``关键字可以通过给定的赋值来推断变量的类型。
而``auto``作为函数返回值占位符的时候，该方法主要用于函数模板的返回值推导。

!!![note]
    不要滥用``auto``，最好是在以下两种情况使用
    - 一眼就能看出初始化类型的时候(例如容器的``iterator``: ``auto it = map.begin()``)。
    - 对于复杂类型，例如``lambda``表达式、``bind``等(例如``auto l = [](int a1,int a2){ return a1+a2;}``)。

使用``auto``关键字声明变量时时，一定要进行初始化，否则编译器无法推断类型则会出错，例如：

```C++
auto i; // 编译器
i = 5；
```

使用``auto``声明变量时有以下4个注意点：

- 当用**一个``auto``关键字声明多个变量**，编译器从左到右推导，以最左表达式判断``auto``类型。

    ```C++
    int n = 5;
    auto *pn = &n, m = 10; // *pn被推导为int*，auto推导为int
    auto *pn = &n, m = 10.1; // 编译失败，声明类型不统一
    ```

- 当用条件表达式初始化``auto``声明的变量时，编译器总是选择用**表达能力更强**的类型。

    ```C++
    auto i = true ? 5 : 8.0; // i 被推导为double类型 
    ```

- C++11中，``auto``无法对**成员变量初始化**进行类型推断，除非使用``const``限定符。而C++17中，对于静态成员变量，可以没有const。

    ```C++
    struct sometype {
        auto i = 5; // C++11错误，无法编译通过
    }

    struct sometype {
        static const auto i = 5; // C++11编译成功，i常量化
    }

    struct sometype {
        static inline auto i = 5; // C++17编译成功
    }
    ```

- C++20前，无法在函数形参列表中使用``auto``声明形参。

    ```C++
    void echo(auto str){...} // C++20前编译失败， C++20成功
    ```

## 2. 类型推导规则

``auto``类型推导遵循以下规则：

- 如果``auto``声明变量时**按值初始化**，则推导出的类型会忽略``cv``限定符。

    ```C++
    const int i = 5; 
    auto j = i;  // auto推导为int，不是const int(按值初始化)
    auto &m = i; // auto推导为const int, m推导为const int&(按引用初始化)
    auto *k = il // auto推导为const int, k推导为const int*(按指针初始化)
    const auto n = j; // auto推导为int, n的类型为const int(用const结合auto)
    ```

- 如果**目标对象是引用**，那么引用属性将会被忽略。

    ```C++
    int i = 5;
    int &j = i;
    auto m = j; /// auto推导为int，不是int&
    ```

- 使用``auto``和**万能引用**声明变量时（mark:右值引用），对于**左值**会将auto推导为引用类型。

    ```C++
    int i = 5;    // 
    auto&& m = i; // auto推导为int&(i是一个左值)
    auto&& j = 5; // auto推导为int(5是一个右值)
    ```

- 如果目标对象是一个**数组/函数**，则``auto``会被推导为对应的指针类型。

    ```C++
    int i[5];
    auto m = i; // auto推导为int*
    int sum(int a1, int a2){
        return a1 + a2;
    }
    auto j = sum; // auto推导为int(_cdecl *)(int,int)
    ```

- 当``auto``与列表初始化组合时：
  1. 直接使用列表初始化，列表中必须为单元素，否则无法编译。
  2. 用等号加列表初始化，列表中可包含单个、多个元素（类型必须相同），auto被推断为``std::initializer_list<T>``。

    ```C++
    auto x1 = {1,2};   // x1推导为std::initializer_list<T>
    auto x2 = {1,2.0}; // 等号+多元素，类型不同，编译失败
    auto x3{1,2};      // 直接+多元素，编译失败
    auto x4{3};        // 直接+单元素，x4推导为int
    ```

## 3. 返回类型推导

对函数类型返回可以声明为``auto``，如果有多处返回，那么类型值需要相同。

```C++
auto large(int a, int b){
    if (a >= b){
        return a;
    }
    else{
        return b;
    }
```

## 4. ``lambda``表达式中使用``auto``

C++14可以将``auto``写到``lambda``表达式的形参中，这样就得到了一个泛型表达式。

```C++
auto l = [](auto a1, auto a2){ return a1 + a2; }
auto retval = l(5,5.0); //推导为double类型
```

## 5. 非类型模板形参占位符

C++17后可以作为非类型模板形参的占位符，例如

```C++
template <auto N>
void f(){std:: cout << N; }

int main(){
    f<5>();   // N为int类型
    f<'c'>(); // N为Char类型
    f<5.0>(); // 编译失败，模板参数不能为double
}
```
