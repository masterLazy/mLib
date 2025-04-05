#pragma once
/*
* [���ͷ]
* Fastmath: ���ڶ��ֺ�˵Ĳ��м�����ѧ��
*/
#include <vector>
#include "../result.hpp"
#include "device.hpp"
#include "vector.hpp"

// C++ AMP ��� (������)
#ifdef FASTMATH_AMP
#define _SILENCE_AMP_DEPRECATION_WARNINGS
#include <amp.h>
#include "amp/amp_device.hpp"
#include "amp/amp_vector_fp16.hpp"
#endif // FASTMATH_HPP