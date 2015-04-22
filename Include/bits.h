
#pragma once

#if defined(_WIN32_WINNT)

	typedef char SInt8;
	typedef unsigned char UInt8;
	typedef short SInt16;
	typedef unsigned short UInt16;
	typedef long SInt32;
	typedef unsigned long UInt32;
	typedef long long SInt64;
	typedef unsigned long long UInt64;

#else /* defined(_WIN32_WINNT) */

	#include <inttypes.h>

	typedef int8_t SInt8;
	typedef uint8_t UInt8;
	typedef int16_t SInt16;
	typedef uint16_t UInt16;
	typedef int32_t SInt32;
	typedef uint32_t UInt32;
	typedef int64_t SInt64;
	typedef uint64_t UInt64;

#endif /* defined(_WIN32_WINNT) */

#ifndef SINT8_MIN
	#define SINT8_MIN 0x80
#endif /* SINT8_MIN */

#ifndef SINT8_MAX
	#define SINT8_MAX 0x7F
#endif /* SINT8_MAX */

#ifndef UINT8_MIN
	#define UINT8_MIN 0x0U
#endif /* UINT8_MIN */

#ifndef UINT8_MAX
	#define UINT8_MAX 0xFFU
#endif

#ifndef SINT16_MIN
	#define SINT16_MIN 0x8000
#endif /* SINT16_MIN */

#ifndef SINT16_MAX
	#define SINT16_MAX 0x7FFF
#endif /* SINT16_MAX */

#ifndef UINT16_MIN
	#define UINT16_MIN 0x0U
#endif /* UINT16_MIN */

#ifndef UINT16_MAX
	#define UINT16_MAX 0xFFFFU
#endif /* UINT16_MAX */

#ifndef SINT32_MIN
	#define SINT32_MIN 0x80000000L
#endif /* SINT32_MIN */

#ifndef SINT32_MAX
	#define SINT32_MAX 0x7FFFFFFFL
#endif /* SINT32_MAX */

#ifndef UINT32_MIN
	#define UINT32_MIN 0x0UL
#endif /* UINT32_MIN */

#ifndef UINT32_MAX
	#define UINT32_MAX 0xFFFFFFFFUL
#endif /* UINT32_MAX */

#ifndef SINT64_MIN
	#define SINT64_MIN 0x8000000000000000LL
#endif /* SINT64_MIN */

#ifndef SINT64_MAX
	#define SINT64_MAX 0x7FFFFFFFFFFFFFFFLL
#endif /* SINT64_MAX */

#ifndef UINT64_MIN
	#define UINT64_MIN 0x0ULL
#endif /* UINT64_MIN */

#ifndef UINT64_MAX
	#define UINT64_MAX 0xFFFFFFFFFFFFFFFFULL
#endif /* UINT64_MAX */

/* Declare our 'traditional' fixed-width types for code interchangeability. */
typedef UInt8      u8;
typedef UInt16     u16;
typedef UInt32     u32;
typedef UInt64     u64;
typedef SInt8      s8;
typedef SInt16     s16;
typedef SInt32     s32;
typedef SInt64     s64;

#define S8_MIN     SINT8_MIN
#define S8_MAX     SINT8_MAX
#define U8_MIN     UINT8_MIN
#define U8_MAX     UINT8_MAX
#define S16_MIN    SINT16_MIN
#define S16_MAX    SINT16_MAX
#define U16_MIN    UINT16_MIN
#define U16_MAX    UINT16_MAX
#define S32_MIN    SINT32_MIN
#define S32_MAX    SINT32_MAX
#define U32_MIN    UINT32_MIN
#define U32_MAX    UINT32_MAX
#define S64_MIN    SINT64_MIN
#define S64_MAX    SINT64_MAX
#define U64_MIN    UINT64_MIN
#define U64_MAX    UINT64_MAX

/* Pull in definition of size_t MIN/MAX macros from the proper header. */
#if defined(_WIN32_WINNT)
	/* Windows keeps the size_t MIN/MAX macros in limits.h. */
	#include <limits.h>
#else
	/* Environments conforming to the C99 standard should have them in stdint.h. */
	#include <stdint.h>
#endif /* defined(_WIN32_WINNT) */

#if defined(_WIN32_WINNT)

	#if defined(_X86_) || defined(_IA64_) || defined(_AMD64_)
		#define __MG_LITTLE_ENDIAN 1
	#else
		#error "Unknown endianness."
	#endif

	#define __bswap16(x) _byteswap_ushort(x)
	#define __bswap32(x) _byteswap_ulong(x)
	#define __bswap64(x) _byteswap_uint64(x)

#elif defined(__APPLE__) || defined(__DARWIN__)

	#include <machine/endian.h>

	#if BYTE_ORDER == BIG_ENDIAN
		#define __MG_BIG_ENDIAN 1
	#elif BYTE_ORDER == LITTLE_ENDIAN
		#define __MG_LITTLE_ENDIAN 1
	#else
		#error "Unknown endianness."
	#endif

	#include <libkern/OSByteOrder.h>

	#define __bswap16(x) OSSwapInt16(x)
	#define __bswap32(x) OSSwapInt32(x)
	#define __bswap64(x) OSSwapInt64(x)

#elif defined(__linux__) || defined(__FreeBSD_kernel__)

	/* Really glibc-specific but cannot test for glibc before any header has been
	 * included so rely on compiler-defines instead. */
	#include <endian.h>
	#include <byteswap.h>

	#if __BYTE_ORDER == __BIG_ENDIAN
		#define __MG_BIG_ENDIAN 1
	#elif __BYTE_ORDER == __LITTLE_ENDIAN
		#define __MG_LITTLE_ENDIAN 1
	#else
		#error "Unknown endianness."
	#endif

	#define __bswap16(x) __bswap_16(x)
	#define __bswap32(x) __bswap_32(x)
	#define __bswap64(x) __bswap_64(x)

#elif defined(__FreeBSD__) || defined(__DragonFly__)

	#include <sys/endian.h>

	#if _BYTE_ORDER == _BIG_ENDIAN
		#define __MG_BIG_ENDIAN 1
	#elif _BYTE_ORDER == _LITTLE_ENDIAN
		#define __MG_LITTLE_ENDIAN 1
	#else
		#error "Unknown endianness."
	#endif

	/* FreeBSD/DragonFly BSD already defines __bswap[16|32|64] in sys/endian.h. */

#elif defined(__NetBSD__)

	#include <sys/endian.h>

	#if _BYTE_ORDER == _BIG_ENDIAN
		#define __MG_BIG_ENDIAN 1
	#elif _BYTE_ORDER == _LITTLE_ENDIAN
		#define __MG_LITTLE_ENDIAN 1
	#else
		#error "Unknown endianness."
	#endif

	#define __bswap16(x) bswap16(x)
	#define __bswap32(x) bswap32(x)
	#define __bswap64(x) bswap64(x)

#elif defined(__OpenBSD__)

	#include <machine/endian.h>
	#include <sys/endian.h>

	#if _BYTE_ORDER == _BIG_ENDIAN
		#define __MG_BIG_ENDIAN 1
	#elif _BYTE_ORDER == _LITTLE_ENDIAN
		#define __MG_LITTLE_ENDIAN 1
	#else
		#error "Unknown endianness."
	#endif

	#define __bswap16(x) __swap16(x)
	#define __bswap32(x) __swap32(x)
	#define __bswap64(x) __swap64(x)

#else
	#error "No endianness functions defined for platform."
#endif

#if !defined(__MG_LITTLE_ENDIAN) && !defined(__MG_BIG_ENDIAN)
	#error Define __MG_LITTLE_ENDIAN or __MG_BIG_ENDIAN
#endif

/* Declare our 'traditional' little-endian types for code interchangeability. */
typedef u16 le16;
typedef u32 le32;
typedef u64 le64;

/* Note that the signed endian-specific types are typedef:d to unsigned versions
 * of the same type, just to make sure no sign-aware magic happens before we
 * have de-endianized them (e.g. arithmetic right-shift). */
typedef u16 sle16;
typedef u32 sle32;
typedef u64 sle64;

/* Constant byte-swapping macros. */
#define __bswap_c16(x) \
	(u16)((((u16)(x) & 0xff00) >> 8) | \
	      (((u16)(x) & 0x00ff) << 8))

#define __bswap_c32(x) \
	(u32)((((u32)(x) & 0xff000000u) >> 24) | \
	      (((u32)(x) & 0x00ff0000u) >>  8) | \
	      (((u32)(x) & 0x0000ff00u) <<  8) | \
	      (((u32)(x) & 0x000000ffu) << 24))

#define __bswap_c64(x) \
	(u64)((((u64)(x) & 0xff00000000000000ull) >> 56) | \
	      (((u64)(x) & 0x00ff000000000000ull) >> 40) | \
	      (((u64)(x) & 0x0000ff0000000000ull) >> 24) | \
	      (((u64)(x) & 0x000000ff00000000ull) >>  8) | \
	      (((u64)(x) & 0x00000000ff000000ull) <<  8) | \
	      (((u64)(x) & 0x0000000000ff0000ull) << 24) | \
	      (((u64)(x) & 0x000000000000ff00ull) << 40) | \
	      (((u64)(x) & 0x00000000000000ffull) << 56))

#if defined(__MG_LITTLE_ENDIAN)

	#define const_le16_to_cpu(x)     ((u16) (x))
	#define const_le32_to_cpu(x)     ((u32) (x))
	#define const_le64_to_cpu(x)     ((u64) (x))
	#define const_sle16_to_cpu(x)    ((s16) (x))
	#define const_sle32_to_cpu(x)    ((s32) (x))
	#define const_sle64_to_cpu(x)    ((s64) (x))

	#define le16_to_cpu(x)           ((u16) (x))
	#define le32_to_cpu(x)           ((u32) (x))
	#define le64_to_cpu(x)           ((u64) (x))
	#define sle16_to_cpu(x)          ((s16) (x))
	#define sle32_to_cpu(x)          ((s32) (x))
	#define sle64_to_cpu(x)          ((s64) (x))

	#define const_be16_to_cpu(x)     ((u16) __bswap_c16(x))
	#define const_be32_to_cpu(x)     ((u32) __bswap_c32(x))
	#define const_be64_to_cpu(x)     ((u64) __bswap_c64(x))
	#define const_sbe16_to_cpu(x)    ((s16) __bswap_c16(x))
	#define const_sbe32_to_cpu(x)    ((s32) __bswap_c32(x))
	#define const_sbe64_to_cpu(x)    ((s64) __bswap_c64(x))

	#define be16_to_cpu(x)           ((u16) __bswap16(x))
	#define be32_to_cpu(x)           ((u32) __bswap32(x))
	#define be64_to_cpu(x)           ((u64) __bswap64(x))
	#define sbe16_to_cpu(x)          ((s16) __bswap16(x))
	#define sbe32_to_cpu(x)          ((s32) __bswap32(x))
	#define sbe64_to_cpu(x)          ((s64) __bswap64(x))

#endif /* defined(__THFS_LITTLE_ENDIAN) */

/* Declare our 'traditional' big-endian types for code interchangeability. */
typedef u16 be16;
typedef u32 be32;
typedef u64 be64;

/* Note that the signed endian-specific types are typedef:d to unsigned versions
 * of the same type, just to make sure no sign-aware magic happens before we
 * have de-endianized them (e.g. arithmetic right-shift). */
typedef u16 sbe16;
typedef u32 sbe32;
typedef u64 sbe64;

#if defined(__MG_BIG_ENDIAN)

	#define const_le16_to_cpu(x)     ((u16) __bswap_c16(x))
	#define const_le32_to_cpu(x)     ((u32) __bswap_c32(x))
	#define const_le64_to_cpu(x)     ((u64) __bswap_c64(x))
	#define const_sle16_to_cpu(x)    ((s16) __bswap_c16(x))
	#define const_sle32_to_cpu(x)    ((s32) __bswap_c32(x))
	#define const_sle64_to_cpu(x)    ((s64) __bswap_c64(x))

	#define le16_to_cpu(x)           ((u16) __bswap16(x))
	#define le32_to_cpu(x)           ((u32) __bswap32(x))
	#define le64_to_cpu(x)           ((u64) __bswap64(x))
	#define sle16_to_cpu(x)          ((s16) __bswap16(x))
	#define sle32_to_cpu(x)          ((s32) __bswap32(x))
	#define sle64_to_cpu(x)          ((s64) __bswap64(x))

	#define const_be16_to_cpu(x)     ((u16) (x))
	#define const_be32_to_cpu(x)     ((u32) (x))
	#define const_be64_to_cpu(x)     ((u64) (x))
	#define const_sbe16_to_cpu(x)    ((s16) (x))
	#define const_sbe32_to_cpu(x)    ((s32) (x))
	#define const_sbe64_to_cpu(x)    ((s64) (x))

	#define be16_to_cpu(x)           ((u16) (x))
	#define be32_to_cpu(x)           ((u32) (x))
	#define be64_to_cpu(x)           ((u64) (x))
	#define sbe16_to_cpu(x)          ((s16) (x))
	#define sbe32_to_cpu(x)          ((s32) (x))
	#define sbe64_to_cpu(x)          ((s64) (x))

#endif /* defined(__THFS_BIG_ENDIAN) */

#define const_cpu_to_le16(x)         ((le16) le16_to_cpu(x))
#define const_cpu_to_le32(x)         ((le32) le32_to_cpu(x))
#define const_cpu_to_le64(x)         ((le64) le64_to_cpu(x))
#define const_cpu_to_sle16(x)        ((sle16) sle16_to_cpu(x))
#define const_cpu_to_sle32(x)        ((sle32) sle32_to_cpu(x))
#define const_cpu_to_sle64(x)        ((sle64) sle64_to_cpu(x))

#define cpu_to_le16(x)               ((le16) le16_to_cpu(x))
#define cpu_to_le32(x)               ((le32) le32_to_cpu(x))
#define cpu_to_le64(x)               ((le64) le64_to_cpu(x))
#define cpu_to_sle16(x)              ((sle16) sle16_to_cpu(x))
#define cpu_to_sle32(x)              ((sle32) sle32_to_cpu(x))
#define cpu_to_sle64(x)              ((sle64) sle64_to_cpu(x))

#define const_cpu_to_be16(x)         ((be16) be16_to_cpu(x))
#define const_cpu_to_be32(x)         ((be32) be32_to_cpu(x))
#define const_cpu_to_be64(x)         ((be64) be64_to_cpu(x))
#define const_cpu_to_sbe16(x)        ((sbe16) be16_to_cpu(x))
#define const_cpu_to_sbe32(x)        ((sbe32) be32_to_cpu(x))
#define const_cpu_to_sbe64(x)        ((sbe64) be64_to_cpu(x))

#define cpu_to_be16(x)               ((be16) be16_to_cpu(x))
#define cpu_to_be32(x)               ((be32) be32_to_cpu(x))
#define cpu_to_be64(x)               ((be64) be64_to_cpu(x))
#define cpu_to_sbe16(x)              ((sbe16) be16_to_cpu(x))
#define cpu_to_sbe32(x)              ((sbe32) be32_to_cpu(x))
#define cpu_to_sbe64(x)              ((sbe64) be64_to_cpu(x))


/*
 * printf format string placeholders and argument cast macros, for format string
 * portability.
 */

#if defined(_WIN32_WINNT)

	#define __printf_fmt_char         _T("%")
	#define __printf_fmt_short        _T("%")
	#define __printf_fmt_int          _T("%")
	#define __printf_fmt_long         _T("%l")
	#define __printf_fmt_long_long    _T("%ll")
	#define __printf_fmt_size_t       _T("%ll")
	#define __printf_fmt_wchar_char   _T("%C")
	#define __printf_type_char        int
	#define __printf_type_short       int
	#define __printf_type_int         int
	#define __printf_type_long        long
	#define __printf_type_long_long   long long
	#define __printf_type_ssize_t     long long
	#define __printf_type_size_t      unsigned long long
	#define __printf_type_wchar_char  wchar_t

#else

	#define __printf_fmt_char         _T("%hh")
	#define __printf_fmt_short        _T("%h")
	#define __printf_fmt_int          _T("%")
	#define __printf_fmt_long         _T("%l")
	#define __printf_fmt_long_long    _T("%ll")
	#define __printf_fmt_size_t       _T("%z")
	#define __printf_fmt_wchar_char   _T("%lc")
	#define __printf_type_char        char
	#define __printf_type_short       short
	#define __printf_type_int         int
	#define __printf_type_long        long
	#define __printf_type_long_long   long long
	#define __printf_type_ssize_t     ssize_t
	#define __printf_type_size_t      size_t
	#define __printf_type_wchar_char  wchar_t

#endif /* defined(_WIN32_WINNT) */

/* char printf format placeholders. */
#define FMThhd __printf_fmt_char _T("d")
#define FMThhu __printf_fmt_char _T("u")
#define FMThho __printf_fmt_char _T("o")
#define FMThhx __printf_fmt_char _T("x")
#define FMThhX __printf_fmt_char _T("X")

/* char printf argument cast macros. */
#define ARGhhd(x) ((__printf_type_char) (x))
#define ARGhhu(x) ((unsigned __printf_type_char) (x))
#define ARGhho(x) ((unsigned __printf_type_char) (x))
#define ARGhhx(x) ((unsigned __printf_type_char) (x))
#define ARGhhX(x) ((unsigned __printf_type_char) (x))

/* short printf format placeholders. */
#define FMThd __printf_fmt_short _T("d")
#define FMThu __printf_fmt_short _T("u")
#define FMTho __printf_fmt_short _T("o")
#define FMThx __printf_fmt_short _T("x")
#define FMThX __printf_fmt_short _T("X")

/* short printf argument cast macros. */
#define ARGhd(x) ((__printf_type_short) (x))
#define ARGhu(x) ((unsigned __printf_type_short) (x))
#define ARGho(x) ((unsigned __printf_type_short) (x))
#define ARGhx(x) ((unsigned __printf_type_short) (x))
#define ARGhX(x) ((unsigned __printf_type_short) (x))

/* int printf format placeholders. */
#define FMTd __printf_fmt_int _T("d")
#define FMTu __printf_fmt_int _T("u")
#define FMTo __printf_fmt_int _T("o")
#define FMTx __printf_fmt_int _T("x")
#define FMTX __printf_fmt_int _T("X")

/* int printf argument cast macros. */
#define ARGd(x) ((__printf_type_int) (x))
#define ARGu(x) ((unsigned __printf_type_int) (x))
#define ARGo(x) ((unsigned __printf_type_int) (x))
#define ARGx(x) ((unsigned __printf_type_int) (x))
#define ARGX(x) ((unsigned __printf_type_int) (x))

/* long printf format placeholders. */
#define FMTld __printf_fmt_long _T("d")
#define FMTlu __printf_fmt_long _T("u")
#define FMTlo __printf_fmt_long _T("o")
#define FMTlx __printf_fmt_long _T("x")
#define FMTlX __printf_fmt_long _T("X")

/* long printf argument cast macros. */
#define ARGld(x) ((__printf_type_long) (x))
#define ARGlu(x) ((unsigned __printf_type_long) (x))
#define ARGlo(x) ((unsigned __printf_type_long) (x))
#define ARGlx(x) ((unsigned __printf_type_long) (x))
#define ARGlX(x) ((unsigned __printf_type_long) (x))

/* long long printf format placeholders. */
#define FMTlld __printf_fmt_long_long _T("d")
#define FMTllu __printf_fmt_long_long _T("u")
#define FMTllo __printf_fmt_long_long _T("o")
#define FMTllx __printf_fmt_long_long _T("x")
#define FMTllX __printf_fmt_long_long _T("X")

/* long long printf argument cast macros. */
#define ARGlld(x) ((__printf_type_long_long) (x))
#define ARGllu(x) ((unsigned __printf_type_long_long) (x))
#define ARGllo(x) ((unsigned __printf_type_long_long) (x))
#define ARGllx(x) ((unsigned __printf_type_long_long) (x))
#define ARGllX(x) ((unsigned __printf_type_long_long) (x))

/* [s]size_t printf format placeholders. */
#define FMTzd __printf_fmt_size_t _T("d")
#define FMTzu __printf_fmt_size_t _T("u")
#define FMTzo __printf_fmt_size_t _T("o")
#define FMTzx __printf_fmt_size_t _T("x")
#define FMTzX __printf_fmt_size_t _T("X")

/* [s]size_t printf argument cast macros. */
#define ARGzd(x) ((__printf_type_ssize_t) (x))
#define ARGzu(x) ((__printf_type_size_t) (x))
#define ARGzo(x) ((__printf_type_size_t) (x))
#define ARGzx(x) ((__printf_type_size_t) (x))
#define ARGzX(x) ((__printf_type_size_t) (x))

/* wchar_t printf format placeholders. */
#define FMTlc __printf_fmt_wchar_char
#define ARGlc(x) ((__printf_type_wchar_char) (x))

/* Precision format placeholders. */
/** Pad with spaces printf format placeholder. */
#define FMTPAD(n) #n
/** Pad with zeroes printf format placeholder. */
#define FMT0PAD(n) "0" #n
/** Left alignment printf format placeholder. */
#define FMTLALIGN "-"
