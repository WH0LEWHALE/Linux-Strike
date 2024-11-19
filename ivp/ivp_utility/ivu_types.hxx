// Copyright (C) Ipion Software GmbH 1999-2000. All rights reserved.

//IVP_EXPORT_PUBLIC

#ifndef _IVP_U_TYPES_INCLUDED
#define _IVP_U_TYPES_INCLUDED

#ifdef _LINUX
	#include <signal.h>
#else
	#include <stdio.h>
#endif

#ifdef WIN32
//#define IVP_PIII			/* set for P3 specific code */
//#define IVP_WILLAMETTE	/* set for Willamette specific code */
//#define IVP_WMT_ALIGN		/* set to compile with MS but Willamette compatible */
#endif
#define IVP_VECTOR_UNIT_FLOAT  /* set if extra data is inserted to support vector units */
#define IVP_VECTOR_UNIT_DOUBLE /* set if extra data should be insersted to utilize double vector units */

#ifdef IVP_WILLAMETTE
#define IVP_IF_WILLAMETTE_OPT(cond) if(cond)
#else
#define IVP_IF_WILLAMETTE_OPT(cond) if(0)
#endif

#if  defined(IVP_WILLAMETTE) || defined(IVP_PIII) || defined(IVP_WMT_ALIGN)
#define IVP_WINDOWS_ALIGN16
#endif

#if !defined(IVP_VECTOR_UNIT_DOUBLE)
#	define IVP_VECTOR_UNIT_FLOAT
#	define IVP_VECTOR_UNIT_DOUBLE
#endif

// typedefs for our special types

#if defined(NDEBUG) + defined(DEBUG) != 1
       Exactly DEBUG or NDEBUG has to be defined, check Makefile
#endif

#define CORE *(int *)0 = 0  /* send fatal signal */

#ifdef	NDEBUG
#	define IVP_ASSERT(cond)	
#	define IVP_USE(a) 
#	define IVP_IF(flag)	if (0==1)
#else
#	if defined(GEKKO)
#		define IVP_ASSERT(cond) \
		{ \
			if(!(cond)) \
			{ \
				::fprintf(stderr, "\nASSERTION FAILURE: %s\nFILE: %s\nLINE: %d\n\n", cond, __FILE__, __LINE__); \
				CORE; \
			} \
		}
#	elif defined(__MWERKS__) && defined(__POWERPC__)
#		include <MacTypes.h>
#		define IVP_ASSERT(cond) \
		{ \
			if(!(cond)) \
			{ \
						char error[128]; \
						sprintf(error, (char*)"\pASSERT FAILURE: \nFILE: %s\nLINE: %d\n\n", __FILE__, __LINE__);  \
		 			DebugStr((unsigned char *)error); \
			} \
		}
#	elif defined (_LINUX)
#		define IVP_ASSERT(cond) \
		{ \
			if(!(cond)) \
			{ \
				::fprintf(stderr, "\nASSERTION FAILURE: %s\nFILE: %s\nLINE: %d\n\n", ""#cond, __FILE__, __LINE__); \
				raise(SIGINT); \
			} \
		}
#	else
#		define IVP_ASSERT(cond) \
		{ \
			if(!(cond)) \
			{ \
				fprintf(stderr, "\nASSERTION FAILURE: %s\nFILE: %s\nLINE: %d\n\n", ""#cond, __FILE__, __LINE__); \
				CORE; \
			} \
		}
//#		define IVP_ASSERT(cond)	if (!(cond)) CORE
#	endif // Mac, etc.  debug assert
#	define IVP_USE(a) a=a
#	define IVP_IF(flag)	if (flag)
#endif


typedef float IVP_FLOAT;
typedef int IVP_INT32;
typedef unsigned int IVP_UINT32;

// ************************
typedef double IVP_DOUBLE;
class IVP_Time {
    double seconds;
public:
    void operator+=(double val){
	seconds += val;
    }
    double get_seconds() const { return seconds; };
    double get_time() const { return seconds; }; // for debugging
    double operator-(const IVP_Time &b) const { return float(this->seconds - b.seconds); }
    void operator-=(const IVP_Time b) { this->seconds -= b.seconds; }
    IVP_Time operator+(double val) const { IVP_Time result; result.seconds = this->seconds + val; return result;}

    IVP_Time() = default;
    IVP_Time(double time){ seconds = time; };
};
 
typedef IVP_FLOAT IVP_HTIME;

typedef unsigned char	uchar; // feel free to remove these three typedefs
typedef unsigned short	ushort;
typedef unsigned int	uint;

#ifdef PLATFORM_64BITS
typedef long long			intp;
typedef unsigned long long	uintp;
#else
typedef int					intp;
typedef unsigned int		uintp;
#endif

typedef const char *IVP_ERROR_STRING;
#define IVP_NO_ERROR 0

#if defined(LINUX) || defined(GEKKO)
//#   define IVP_ALIGN_16  __attribute__ ((aligned(16)))
#endif

#if !defined(IVP_ALIGN_16)
#   define IVP_ALIGN_16 
#endif


/********************************************************************************
 *		    			Simple Base Types
 ********************************************************************************/
  
typedef int IVP_Time_CODE;	// Internal enumeration for time handles.
typedef int ivp_u_bool;		// must be int!!

// boolean enumeration, compatible to C++ bool, ivp_u_bool
enum IVP_BOOL {
    IVP_FALSE = 0,
    IVP_TRUE = 1
};


enum IVP_RETURN_TYPE {
    IVP_FAULT = 0,
    IVP_OK = 1
};

#define IVP_CDECL       /* set this to whatever you need to satisfy your linker */

#if !defined(__MWERKS__) || !defined(__POWERPC__)
#   ifdef OSX
#       include <malloc/malloc.h>
#   else
#       include <malloc.h>
#   endif
#endif

#include <string.h>


	char * IVP_CDECL p_calloc(int nelem,int size);
	void * IVP_CDECL p_realloc(void* memblock, int size);
	void * IVP_CDECL p_malloc(unsigned int size);
	void   IVP_CDECL p_free(void *data);
	
	void * IVP_CDECL ivp_malloc_aligned(int size, int alignment);
	void * IVP_CDECL ivp_calloc_aligned(int size, int alignment);
	
	void  IVP_CDECL ivp_free_aligned(void *data);

	//void  IVP_CDECL ivp_byte_double(double& eightbytes);
	void  IVP_CDECL ivp_byte_swap4(uint& fourbytes);
	void  IVP_CDECL ivp_byte_swap2(ushort& twobytes);

	char *p_strdup(const char *s);

extern void ivp_memory_check(void *a);

#define P_DELETE(a)  { /*ivp_memory_check((void*)a);*/ delete(a); a=NULL; };
#define P_DELETE_THIS(a)  { /*ivp_memory_check((void*)a);*/ delete(a); };
#define P_DELETE_ARRAY(a)  { /*ivp_memory_check((void*)a);*/ delete[](a); (a)= 0; };

#define P_FREE_ALIGNED(a) { if (a) { /*ivp_memory_check((void*)a);*/ ivp_free_aligned( (void *)a ); a = NULL; } };
#define P_FREE(a) { if (a) { /*ivp_memory_check((void*)a);*/ p_free( (char *) a); a = NULL; } };

#define P_MEM_CLEAR(a) memset((char*)(a), 0, sizeof(*a));
#define P_MEM_CLEAR_M4(a) memset((char*)(a)+sizeof(void*), 0, sizeof(*a)-sizeof(void *));
#define P_MEM_CLEAR_ARRAY(clss,elems) memset((char*)(clss), 0, sizeof(*clss)*elems);

#define P_FLOAT_EPS 1e-10f	// used for division checking
#define P_FLOAT_RES 1e-6f	// float resolution for numbers < 1.0
#define P_FLOAT_MAX 1e16f

#define IVP_PI        3.14159265358979323846	/* pi */
#define IVP_PI_2      1.57079632679489661923	/* pi/2 */
#define P_DOUBLE_MAX 1e20
#define P_DOUBLE_RES 1E-12	// double resolution for numbers < 1.0
#define IVP_3D_SOLVER_NULLSTELLE_EPS 1e-8
#define P_DOUBLE_EPS 1e-10	// used for division checking
#define P_MAX_WORLD_DOUBLE 10000 // max world koords

#define P_MAX_OBJECT_SIZE 1000.0f 
#define P_MIN_EDGE_LEN 0.01f	// 10 mm min edge len of polygon objects
#define P_RES_EPS (P_MAX_WORLD_DOUBLE * P_DOUBLE_RES) // effective IVP_DOUBLE resolution for world coords

void ivp_srand(int seed);
IVP_FLOAT ivp_rand();		// returns [0 .. 1]

#if defined(WIN32)
#   if defined(IVP_PIII) || defined(IVP_WILLAMETTE)
#   	if defined(IVP_PIII)
#		define IVP_PREFETCH_CLINE_SIZE 0x20
#	else
#		define IVP_PREFETCH_CLINE_SIZE 0x40
#	endif
#	define IVP_IF_PREFETCH_ENABLED(x) if(x)
#	include <xmmintrin.h>
#	define IVP_PREFETCH( pntr, offset) _mm_prefetch( int(offset) + (char *)pntr, _MM_HINT_T1);
#   endif
#endif


#if !defined(IVP_PREFETCH)
#	define IVP_PREFETCH_CLINE_SIZE 0x100
#	define IVP_PREFETCH(pntr, offset)
#	ifdef DEBUG
#		define IVP_IF_PREFETCH_ENABLED(x) if(1)
#	else
#		define IVP_IF_PREFETCH_ENABLED(x) if(0)
#	endif
#	define IVP_PREFETCH_BLOCK(a,b)  /* IVP_ASSERT(b < 128);*/
#else

#   define IVP_PREFETCH_BLOCK(pntr, size) {	\
    IVP_PREFETCH(pntr,0);	\
    if ( size > IVP_PREFETCH_CLINE_SIZE)   IVP_PREFETCH( pntr, IVP_PREFETCH_CLINE_SIZE);   \
    if ( size > 2*IVP_PREFETCH_CLINE_SIZE) IVP_PREFETCH( pntr, 2*IVP_PREFETCH_CLINE_SIZE); \
    if ( size > 3*IVP_PREFETCH_CLINE_SIZE) IVP_PREFETCH( pntr, 3*IVP_PREFETCH_CLINE_SIZE); \
    /*if ( size > 4*IVP_PREFETCH_CLINE_SIZE) IVP_PREFETCH( pntr, 4*IVP_PREFETCH_CLINE_SIZE);*/ \
    /*IVP_PREFETCH( pntr, (size - sizeof(void *))) */\
}

#endif

#ifndef IVP_FAST_WHEELS_ENABLED
#define IVP_FAST_WHEELS_ENABLED
#endif // IVP_FAST_WHEELS_ENABLED

// define this if you have the havana constraints module
#ifndef HAVANA_CONSTRAINTS
//#define HAVANA_CONSTRAINTS
#endif // HAVANA_CONSTRAINTS

// define this if you have the havok MOPP module
#ifndef HAVOK_MOPP
//#define HAVOK_MOPP
#endif // HAVOK_MOPP
			   
#endif


