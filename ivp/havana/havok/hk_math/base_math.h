#ifndef HK_MATH_MATH_H
#define HK_MATH_MATH_H

#include <hk_base/base.h>

#include <math.h>

#define HK_REAL_MAX 1e16f
#define HK_REAL_EPS 1e-16f		/* the minumum resolution of real */
#define HK_REAL_RES 1e-7f		/* resolution of hk_real of  relative to 1.0f */
class hk_Math
{
	public:
		static inline hk_real clamp( hk_real v, hk_real min, hk_real max );
		static inline hk_real fast_rsqrt( hk_real num );

		static void srand01( unsigned seedVal );
		static inline hk_real rand01();
		static unsigned int hk_random_seed;
};

#include <hk_math/math.inl>

#endif /* HK_MATH_MATH_H */
