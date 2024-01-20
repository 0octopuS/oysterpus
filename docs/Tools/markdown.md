# Markdown / Mkdocs

## Markdown Code block

``` py title="bubble_sort.py" linenums="1"  hl_lines="2 3"
def bubble_sort(items):
    for i in range(len(items)):
        for j in range(len(items) - 1 - i):
            if items[j] > items[j + 1]:
                items[j], items[j + 1] = items[j + 1], items[j]
```


## Markdown Link and URL

!!! note 基本用法
    1. 链接文本放在中括号内
    2. 链接地址放在后面的括号中
    3. 链接title可选（当鼠标悬停在链接上时会出现的文字）
    - 尖括号简单处理URL/email
    - 引用类型链接

超链接Markdown语法代码： [超链接显示名](超链接地址 "超链接title")
`[超链接显示名](超链接地址 "超链接title")`

对应的HTML代码：<a href="超链接地址" title="超链接title">超链接显示名</a>
`<a href="超链接地址" title="超链接title">超链接显示名</a>`

使用尖括号可以很方便地把URL或者email地址变成可点击的链接。
`<abc@cde.a>`  <abc@cde.a>

引用类型链接分为两部分：

1. 与文本保持内联的部分
   第一组方括号包围应显示为链接的文本。第二组括号显示了一个标签，该标签用于指向您存储在文档其他位置的链接。 [链接1][1]   [链接2][2]

    ```markdown
    [链接1][1]   [链接2][2]
    ```

2. 存储在文件中其他位置的部分，以使文本易于阅读。

   [1]: https://www.baidu.com
   [2]: linux.md

    ```markdown
    [1]: https://www.baidu.com
    [2]: linux.md
    ```

## Markdown table

| Syntax    | Description |
| --------- | ----------- |
| Header    | Title       |
| Paragraph | Text        |

## mkdocs 插件

### amonition

通过 `!!!` 语法来给定一些告警


```markdown
!!! note Name

```

可以使用以下风格，网页预览和vscode不太一样

!!! warning
!!! danger
!!! bug
!!! note
!!! abstract
!!! info
!!! example
!!! hint
!!! tip
!!! success
!!! question
!!! quote



If use `???`, then the block can be folded( only visible in browser, not in vscode preview).

??? note 

    Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla et euismod
    nulla. Curabitur feugiat, tortor non consequat finibus, justo purus auctor
    massa, nec semper lorem quam in massa.


!!! info inline end "Lorem ipsum"

If use `inline end` ,then the **next** line will be inlined to **right** side.

The **next next** line will not be inlined, for example this line.

!!! info inline "Lorem ipsum"

If use `inline` ,then the **next** line will be inlined to **left** side.


使用 `===` 能够进标签切换，冒号前的内容指定[图标](https://icon-sets.iconify.design/octicon/)
=== ":octicons-mark-github-16: Tab A"
    Content of part A
=== ":octicons-mark-github-16: Tab B"
    Content of part B