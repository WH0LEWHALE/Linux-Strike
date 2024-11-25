#include <hk_math/vecmath.h>

// BSD rand function
unsigned int hk_Math::hk_random_seed = 'h'+'a'+'v'+'o'+'k';

void hk_Math::srand01( unsigned seedVal )
{
	hk_random_seed = seedVal;
}
