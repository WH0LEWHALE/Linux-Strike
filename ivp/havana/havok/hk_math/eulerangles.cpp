#include <hk_math/vecmath.h>
#include <hk_math/eulerangles.h>

/*****************************************************************
 *		Name: create_rot_axis_and_angles
 *		input tref_ws_os:  the reference objects rotation
 *		input tatt_ws_os:  the attached objects rotation
 *		input i0:			the first axis to rotate (yaw)
 *		input i1:			the middle axis to rotage (pitch)
 *		input i2:			the last axis to rotate (roll)
 *		
 *		output taxis_ws_os_out: the constraint axis to be used by the constraint solver
 *		output angles_out:		the eulerangle to be used by the constraint solver
 *		
 *		Note:	expects the transforms to be in column major order
 *		Note:	whole function can be optimized using assembler in PS2
 *****************************************************************/
void hk_Euler_Angles::create_rot_axis_and_angles(
				 const hk_Rotation &tref_ws_os, const hk_Rotation &tatt_ws_os, 
				 int i0, int i1, int i2,
				 // in
				 hk_Matrix3 &taxis_ws_os_out, hk_Vector3 &angles_out)
				 //out 
{
	const hk_Vector3 &a0 = tref_ws_os.get_column( i0 );
	const hk_Vector3 &b0 = tref_ws_os.get_column( i1 );
	const hk_Vector3 &c0 = tref_ws_os.get_column( i2 );

	const hk_Vector3 &a1 = tatt_ws_os.get_column( i0 );
	const hk_Vector3 &b1 = tatt_ws_os.get_column( i1 );
	const hk_Vector3 &c1 = tatt_ws_os.get_column( i2 );

	hk_Vector3 b;
	b.set_cross( c1, a0 );
	hk_real sin_b = a0.dot(c1);
	hk_real cos_b = b.length();
	hk_real inv_cos_b = 1.0f / cos_b;
	b *= inv_cos_b;

	hk_real sin_a = c1.dot(b0) * inv_cos_b;
	hk_real cos_a = c1.dot(c0) * inv_cos_b;

	hk_real sin_g = a0.dot(b1) * inv_cos_b;
	hk_real cos_g = a0.dot(a1) * inv_cos_b;

	angles_out.x = atan2( sin_a, cos_a );
	angles_out.y = atan2( sin_b, cos_b );
	angles_out.z = atan2( sin_g, cos_g );

	taxis_ws_os_out.set_cols( a0, b, c1 );
}
