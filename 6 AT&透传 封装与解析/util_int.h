#ifndef UTIL_H
#define UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#define MCU_BIG_ENDIAN		//大端模式
//#define MCU_LITTLE_ENDIAN	//小端模式 add@sail@1117

/* 7.18.1.1 Exact-width integer types */

    typedef    char   int8_t ;
    typedef unsigned char  uint8_t;
    typedef          short  int16_t;
    typedef unsigned short uint16_t;
    typedef          int    int32_t;
    typedef unsigned int   uint32_t;
    typedef          long long  int64_t;
    typedef unsigned long long uint64_t;
    
    /* 7.18.1.2 Minimum-width integer types */

    typedef  int8_t   int_least8_t;
    typedef uint8_t  uint_least8_t;

    typedef  int16_t  int_least16_t;
    typedef uint16_t uint_least16_t;
    typedef  int32_t  int_least32_t;
    typedef uint32_t uint_least32_t;


    typedef  int64_t  int_least64_t;
    typedef uint64_t uint_least64_t;

/* 7.18.1.3 Fastest minimum-width integer types */

    typedef  int32_t  int_fast8_t;
    typedef uint32_t uint_fast8_t;
    typedef  int32_t  int_fast16_t;
    typedef uint32_t uint_fast16_t;

    typedef  int32_t  int_fast32_t;
    typedef uint32_t uint_fast32_t;


    typedef  int64_t  int_fast64_t;
    typedef uint64_t uint_fast64_t;
    
    /*布尔类型定义*/
#ifndef bool
        typedef uint8_t bool;
#endif
        
#ifndef false
    #define false  (0)
#endif

#ifndef true
    #define true   (!false)
#endif
    /*END*布尔类型定义*/
        
#ifndef NULL   
	#define NULL 0L  
#endif
    
#ifndef   __WEAK
  	#define __WEAK                                 __attribute__((weak))
#endif
        
#ifdef MCU_BIG_ENDIAN
    //大小端模式转换
    #define Swap16(A)   ((((uint16_t)(A) & 0xff00) >> 8)|(( (uint16_t)(A) & 0x00ff) << 8))  
     
    #define Swap32(A)   ((( (uint32_t)(A) & 0xff000000) >> 24) |  \
    				     (( (uint32_t)(A) & 0x00ff0000) >> 8)  |  \
    				     (( (uint32_t)(A) & 0x0000ff00) << 8)  |  \
    				     (( (uint32_t)(A) & 0x000000ff) << 24))  
    //强制类型转换
	#define SWAP_16T8(A)   ((uint8_t)(((uint16_t)(A) & 0xff00) >> 8))  
	 
	#define SWAP_32T8(A)   ((uint8_t)(((uint32_t)(A) & 0xff000000) >> 24))
    
	#define SWAP_32T16(A)   (((uint16_t)((( (uint32_t)(A) & 0xff000000) >> 24) |  \
	     	 	 	 	 				 (( (uint32_t)(A) & 0x00ff0000) >> 8)))
		
	#define SWAP_16T32(A)   (((uint16_t)((( (uint32_t)(A) & 0x0000ff00) << 8)  |  \
	     	 	 	 	 	 	 	 	 (( (uint32_t)(A) & 0x000000ff) << 24))) 
    
#endif
    

	
#ifdef __cplusplus
}
#endif

#endif /* UTIL_H */
