#ifndef HK_MATH_EULERANGLES_H
#define HK_MATH_EULERANGLES_H

// <hk_math/vector3/vector3.h>

//: Rotation represented by three rotation angles about the xyz axes.
class hk_Euler_Angles : public hk_Vector3
{
	public:
		static void create_rot_axis_and_angles(
			 const hk_Rotation &tref_ws_os, const hk_Rotation &tatt_ws_os, 
			 int i0, int i1, int i2,
			 // in
			 hk_Matrix3 &taxis_ws_os_out, hk_Vector3 &angles_out);
			 //out 
};

#endif /* HK_MATH_EULERANGLES_H */
