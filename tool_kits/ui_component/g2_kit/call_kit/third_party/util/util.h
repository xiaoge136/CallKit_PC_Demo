#ifndef STRING_UTIL_H_
#define STRING_UTIL_H_

#include <list>
#include <shlobj.h>
#include <shellapi.h>

namespace necall_kit
{

typedef char UTF8Char;
typedef wchar_t UTF16Char;
typedef int32_t UTF32Char;

typedef std::basic_string<UTF8Char> UTF8String;
typedef UTF8String U8String;
typedef std::basic_string<UTF16Char> UTF16String;
typedef UTF16String U16String;
typedef std::basic_string<UTF32Char> UTF32String;
typedef UTF32String U32String;

typedef unsigned int	UTF32;	/* at least 32 bits */
typedef unsigned short	UTF16;	/* at least 16 bits */
typedef unsigned char	UTF8;	/* typically 8 bits */
typedef unsigned char	Boolean; /* 0 or 1 */

/* Some fundamental constants */
#define UNI_REPLACEMENT_CHAR (UTF32)0x0000FFFD
#define UNI_MAX_BMP (UTF32)0x0000FFFF
#define UNI_MAX_UTF16 (UTF32)0x0010FFFF
#define UNI_MAX_UTF32 (UTF32)0x7FFFFFFF
#define UNI_MAX_LEGAL_UTF32 (UTF32)0x0010FFFF

typedef enum {
	conversionOK, 		/* conversion successful */
	sourceExhausted,	/* partial character in source, but hit end */
	targetExhausted,	/* insuff. room in target for conversion */
	sourceIllegal		/* source sequence is illegal/malformed */
} ConversionResult;

typedef enum {
	strictConversion = 0,
	lenientConversion
} ConversionFlags;

std::wstring UTF8ToUTF16(const UTF8Char *utf8, size_t length);
std::string UTF16ToUTF8(const UTF16Char *utf16, size_t length);
std::wstring UTF8ToUTF16(const std::string &utf8);
std::string UTF16ToUTF8(const std::wstring &utf16);
ConversionResult ConvertUTF8toUTF16(
	const UTF8** sourceStart, const UTF8* sourceEnd,
	UTF16** targetStart, UTF16* targetEnd, ConversionFlags flags);
ConversionResult ConvertUTF16toUTF8(
	const UTF16** sourceStart, const UTF16* sourceEnd,
	UTF8** targetStart, UTF8* targetEnd, ConversionFlags flags);
std::wstring GetLocalAppDataDir(HANDLE token = NULL );
}

#endif // STRING_UTIL_H_
