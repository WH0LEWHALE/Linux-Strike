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
