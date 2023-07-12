# mLib
汇集了我编写的各种函数和类，功能覆盖多个方面和领域，正在不断完善中。我的许多个人程序和项目都会运用它们，欢迎大家使用！mLib 遵循 MIT 开源协议（MIT License）。

要想使用 mLib，您需要安装 Visual C++，因为部分代码使用了这些技术。如果您使用 Visual Studio 进行开发，则不需要额外安装任何项。除 mFunction、mGraphics 和 mNetwork 外，代码只需要标准 C++ 即可运行和使用。

目前它包括：
## 1. mFunction.h
本意是函数集，但是也有少部分的类等。总之集合了各种功能。
## 2. mGraphics.h，mGraphics.cpp
基于Direct2D的绘图。
1. mMath/image.h，图片操作（相对独立）。
## 3. mMath.h
数学。
1. mMath/geometry.h，几何。
2. mMath/algebra.h，代数。
3. mMath/num.h，仿 Numpy 的多维数组。
4. mMath/value.h，求值链，主要用来自动求导。
5. mMath/numEx.h，高精度数字。
## 4. mWeb.h
网络。
## 5. mCode.h
编码（Base64，UTF-8，URI）。
## 6. mDnn.h，mDnn.cpp
神经网络（很 NB）。
1. mNetwork/layers.h，神经网络层。
2. mNetwork/AOLR.h，激活函数、优化器、损失函数、正则化方法。
