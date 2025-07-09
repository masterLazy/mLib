# ✨ mLib

### 🎂 本仓库创建时间：2023.2.19。

这是一个标头库，集合了很多杂乱的功能。

**“m”** 取自 masterLazy 的首字母，**“Lib”** 即 Library。

## 使用方法

mLib 是一个仅标头（head-only）库，将 `include/` 添加到你的包含目录下，然后就可以使用 `mlib/` 下的头文件了。

直接 `#include` 你需要的子组件就行。

<hr/>

主要分支：

## 🏷️ main (neo)

作为主要分支开发中。

各组件的总头文件放在根目录下，主体放在不同子目录内。所有组件都在 `mlib` 命名空间下，文件名总是和类名（子命名空间名）相同。每个组件中的所有内容都会包含在一个类（子命名空间）中。

| 文件           | 类名                           | 内容                                    |
| -------------- | ------------------------------ | --------------------------------------- |
| Console.hpp    | `mlib::Console`（命名空间）    | 用于控制台的实用工具                    |
| Function.hpp   | `mlib::Function`（命名空间）   | 实用函数                                |
| Logger.hpp     | `mlib::Logger`                 | 日志器                                  |
| Process.hpp    | `mlib::Process`                | 包装一个进程，可以对其进行 Std I/O 操作 |
| Result.hpp     | `mlib::Result`                 | 操作结果状态包装器                      |
| ThreadPool.hpp | `mlib::ThreadPool`（命名空间） | 高性能线程池                            |

实例：

```cpp
#include "mlib/Logger.hpp"
int main() {
    mlib::Logger my_logger(Logger::debug);
    return 0;
}
```

## 🏷️ classic

这是最早的分支。但是功能很多且缺乏秩序，现已停止维护。但这其中的一些代码片段仍被复用。

我放弃 classic 分支后，创建了 [LazyWeb](https://github.com/masterLazy/LazyWeb)、[LazyMath](https://github.com/masterLazy/LazyMath)、LazyGUI（未上传 github）等库，继承了原 classic 分支的一些子部件。

