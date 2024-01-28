# Dynamic memory management

## Smart pointers

### `unique_ptr`

`unique_ptr` 是管理另一个object的smart pointer，在`unique_ptr`周期结束的时候对管理的object进行处理。
构造函数为

```C++
unique_ptr( pointer p );
// deleter 为 value
unique_ptr(pointer p, const A& d) noexcept; // copy
unique_ptr(pointer p, A&& d) noexcept;      // move
// deleter 为左值引用 A&
unique_ptr(pointer p, A&& d) noexcept;
unique_ptr(pointer p, A&& d) = delete;
// deleter 为左值引用 const A&
unique_ptr(pointer p, const A& d) noexcept;
unique_ptr(pointer p, const A&& d) = delete;
// 从另一个unique_ptr中获取所有权
unique_ptr( unique_ptr&& u ) noexcept;
```

通常直接管理一个物品，然后用默认的清理。在这种情况下最好使用`make_unique`。也可以管理某个数组或者容器等。

```C++
std::unique_ptr<Foo> up(new Foo);
std::unique_ptr<Foo> v1 = std::make_unique<Foo>();
std::unique_ptr<Fo0[]> v2 = std::make_unique<Foo[]>(5);
```

### `shared_ptr`

`shared_ptr`是有着共享所有权的smart pointer，当没有任何`shared_ptr`指向管理object的时候，这个object才会被摧毁。

```C++
std::cout << "2) constructor with object\n";
std::shared_ptr<Foo> sh2(new Foo{10}); 
std::cout << "sh2.use_count(): " << sh2.use_count() << '\n'; //sh2.use_count(): 1
std::shared_ptr<Foo> sh3(sh2); 
std::cout << "sh2.use_count(): " << sh2.use_count() << '\n'; //sh2.use_count(): 2
std::cout << "sh3.use_count(): " << sh3.use_count() << '\n'; //sh3.use_count(): 2
```

`make_shard` 返回一个`shared_ptr`，有以下几种用法

```C++
//提供管理object T的参数，构造一个对象
shared_ptr<T> make_shared( Args&&... args ); 
// 构造多维数据管理
shared_ptr<T> make_shared( std::size_t N );
shared_ptr<T> make_shared(); // 这里T本身是个例如Arr[123]这样的数组
```

### `weak_ptr`

## Allocators

## Memory resources

## Uninitialized storage
