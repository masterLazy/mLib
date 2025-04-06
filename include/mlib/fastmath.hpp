#pragma once
/*
* [���ͷ]
* Fastmath: ���ڶ��ֺ�˵Ĳ��м�����ѧ��
*/
#include <vector>
#include "result.hpp"
#include "fastmath/device.hpp"
#include "fastmath/vector.hpp"

// C++ AMP ��� (������)
#ifdef FASTMATH_AMP
#define _SILENCE_AMP_DEPRECATION_WARNINGS
#include <amp.h>
#include "fastmath/amp/amp_device.hpp"
#include "fastmath/amp/amp_vector_fp16.hpp"
#endif // FASTMATH_HPP