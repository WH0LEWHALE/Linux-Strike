inline hk_real hk_Math::clamp( hk_real r, hk_real mn, hk_real mx)
{
	IVP_ASSERT(mn<=mx);
	return ((r<mn)
				? mn
				: ((r>mx) ? mx : r));
}

inline hk_real hk_Math::fast_rsqrt( hk_real num )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5f;

	x2 = num * 0.5F;
	y = num;
	i = *( long * ) &y; // evil floating point bit level hacking
	i = 0x5f3759df - ( i >> 1 ); // what the fuck?
	y = *( float * ) &i;
	y = y * ( threehalfs - ( x2 * y * y ) );

	return y;
}

inline hk_real hk_Math::rand01()
{
	// BSD rand function
	const unsigned a = 1103515245;
	const unsigned c = 12345;
	const unsigned m = unsigned(-1) >> 1;
	hk_random_seed = (a * hk_random_seed + c ) & m;
	return hk_real(hk_random_seed) / m;
}

inline hk_real hk_Math::fast_approx_atan2_normized( hk_real y, hk_real x)
{
	//const hk_real delta_at_pi_4 = HK_PI * 0.25f - hk_Math::sin(HK_PI * 0.25f);
	//const hk_real f = delta_at_pi_4 / hk_Math::sin(HK_PI * 0.25f) / hk_Math::sin(HK_PI * 0.25f)/ hk_Math::sin(HK_PI * 0.25f);
	const hk_real f = 0.2214414775f;
	hk_real r;
	if ( x > y ){
		if ( x > - y){
			r = y;
			r += y * y * y * f;
		}else{
			r = x - HK_PI_2;
			r += x * x * x * f;
		}
	}else{
		if ( x > - y){
			r = HK_PI_2 - x;
			r -= x * x * x * f;
		}else{
			if ( y > 0 ){
				r = HK_PI - y;
			}else{
				r = -y - HK_PI;
			}
			r -= y * y * y * f;
		}
	}
	return r;
}
inline hk_real hk_Math::fast_approx_atan2( hk_real y, hk_real x)
{
	//const hk_real delta_at_pi_4 = HK_PI * 0.25f - hk_Math::sin(HK_PI * 0.25f);
	//const hk_real f = delta_at_pi_4 / hk_Math::sin(HK_PI * 0.25f) / hk_Math::sin(HK_PI * 0.25f)/ hk_Math::sin(HK_PI * 0.25f);
	const hk_real f = 0.2214414775f;
	hk_real r;
	hk_real q = hk_Math::fast_rsqrt( x * x + y * y );
	if ( x > y ){
		if ( x > - y){
			y *= q;
			r = y;
			r += y * y * y * f;
		}else{
			x *= q;
			r = x - HK_PI_2;
			r += x * x * x * f;
		}
	}else{
		if ( x > - y){
			x *= q;
			r = HK_PI_2 - x;
			r -= x * x * x * f;
		}else{
			y *= q;
			if ( y > 0 ){
				r = HK_PI - y;
			}else{
				r = -y - HK_PI;
			}
			r -= y * y * y * f;
		}
	}
	return r;
}