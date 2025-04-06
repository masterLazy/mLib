#pragma once
/*
* [组件头]
* Fastmath: 基于多种后端的并行加速数学库
*/
#include <vector>
#include "result.hpp"
#include "fastmath/device.hpp"
#include "fastmath/vector.hpp"

// C++ AMP 后端 (开发中)
#ifdef FASTMATH_AMP
#define _SILENCE_AMP_DEPRECATION_WARNINGS
#include <amp.h>
#include "fastmath/amp/amp_device.hpp"
#include "fastmath/amp/amp_vector_fp16.hpp"
#endif // FASTMATH_HPP