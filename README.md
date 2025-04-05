# ✨ mLib

### 🎂 本仓库创建时间：2023.2.19。

这是一个标头库，集合了很多杂乱的功能。

**“m”** 取自 masterLazy 的首字母，**“Lib”** 即 Library。

现在有两大分支：

## 🏷️ classic

这是最早的分支。但是功能很多且缺乏秩序，现已停止维护。但这其中的一些代码片段仍被复用。

我放弃 classic 分支后，创建了 [LazyWeb](https://github.com/masterLazy/LazyWeb)、[LazyMath](https://github.com/masterLazy/LazyMath)、LazyGUI（未上传 github）等库，继承了原 classic 分支的一些子部件。

## 🏷️ main

去除了一些已经分出去的部分。也许不会再维护了。

## 🏷️ Neo

开发中。

根目录下的源文件由各组件共享，各组件的主体放在不同文件夹内。

统一使用命名空间  `mlib`，有的组件下设各子命名空间。目前计划开发下列组件：

| 名称     | 子命名空间       | 内容                                                         |
| -------- | ---------------- | ------------------------------------------------------------ |
| FastMath | `mlib::fastmath` | 基于多种后端的并行加速数学库，计划开发 C++ AMP、OpenCL 两种后端。C++ AMP 后端的开发已停滞，OpenCL 后端的开发尚未开始。 |
| Console  | `mlib::console`  | 用于控制台的实用工具。                                       |

