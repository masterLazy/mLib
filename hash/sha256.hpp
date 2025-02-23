/*
* SHA-256 algorithm
*/

/*									MIT License
*
*							  Copyright (c) 2025 mLazy
*
*	Permission is hereby granted, free of charge, to any person obtaining a copy
*	of this software and associated documentation files (the "Software"), to deal
*	in the Software without restriction, including without limitation the rights
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*	copies of the Software, and to permit persons to whom the Software is
*	furnished to do so, subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included in all
*	copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*	SOFTWARE.
*/

#ifndef _SHA256_HPP_
#define _SHA256_HPP_
#define self (*this)
#include "../Hash.h"
namespace lazy {
	namespace hasher {
		class Sha256 : public Hasher {
			uint32_t h[8];
			static constexpr uint32_t K256[64] = {
				0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL,
				0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
				0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
				0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
				0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
				0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
				0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL,
				0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
				0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
				0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
				0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL,
				0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
				0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL,
				0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
				0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
				0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
			};
		public:
			Sha256() {
				self.hashLength = 256 / 4;
				h[0] = 0x6a09e667UL;
				h[1] = 0xbb67ae85UL;
				h[2] = 0x3c6ef372UL;
				h[3] = 0xa54ff53aUL;
				h[4] = 0x510e527fUL;
				h[5] = 0x9b05688cUL;
				h[6] = 0x1f83d9abUL;
				h[7] = 0x5be0cd19UL;
			}
			// update digest with appended data
			Hasher& append(void* data, size_t len) {

			}
			// get digest
			std::string get() {

			}
		};
	} // namespace hasher
} // namespace lazy
#undef self
#endif // _SHA256_HPP_