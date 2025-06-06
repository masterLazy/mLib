# ✨ mLib

### 🎂 本仓库创建时间：2023.2.19。

这是一个标头库，集合了很多杂乱的功能。

**“m”** 取自 masterLazy 的首字母，**“Lib”** 即 Library。

## 使用方法

mLib 是一个仅标头库，将 `include/` 添加到你的包含目录下，然后就可以使用 `mlib/` 下的头文件了。

直接 `#include` 你需要的子组件就行。

<hr/>

主要分支：

## 🏷️ neo (main)

作为主要分支开发中。

各组件的总头文件放在根目录下，主体放在不同子目录内。

统一使用命名空间  `mlib`，有的组件下设各子命名空间。目前计划开发下列组件：

| 组件     | 子命名空间       | 内容                                                         |
| -------- | ---------------- | ------------------------------------------------------------ |
| fastmath | `mlib::fastmath` | 基于多种后端的并行加速数学库，计划开发 C++ AMP、OpenCL 两种后端。C++ AMP 后端的开发已停滞，OpenCL 后端的开发尚未开始。 |
| console  | `mlib::console`  | 用于控制台的实用工具。                                       |
| result   | `mlib::result`   |                                                              |
| logger   | `mlib::logger`   | 简单的日志类。                                               |
| process  | `mlib::process`  | 进程包装器，可以对进程进行标准 I/O 操作。                    |

## 🏷️ classic

这是最早的分支。但是功能很多且缺乏秩序，现已停止维护。但这其中的一些代码片段仍被复用。

我放弃 classic 分支后，创建了 [LazyWeb](https://github.com/masterLazy/LazyWeb)、[LazyMath](https://github.com/masterLazy/LazyMath)、LazyGUI（未上传 github）等库，继承了原 classic 分支的一些子部件。

