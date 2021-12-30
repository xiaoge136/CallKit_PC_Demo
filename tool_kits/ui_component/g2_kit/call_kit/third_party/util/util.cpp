/*
 *
 *	Author		wrt(guangguang)
 *	Date		2011-06-08
 *	Copyright	Hangzhou, Netease Inc.
 *	Brief		Utilities for string operation
 *
 */

#include "util.h"
#include <stdio.h>			/* for vsnprintf */
#include <assert.h>			/* for assert */
#include <memory.h>			/* for mem*** */
#include <stdarg.h>			/* va_list, va_start, va_end */
#include <ctype.h>
#include <string.h>

#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Ole32.lib")

namespace necall_kit
{
#ifndef COUNT_OF
#define COUNT_OF(array)			(sizeof(array)/sizeof(array[0]))
#endif

static const int halfShift = 10; /* used for shifting by 10 bits */
static const UTF32 halfBase = 0x0010000UL;
static const UTF32 halfMask = 0x3FFUL;

#define UNI_SUR_HIGH_START  (UTF32)0xD800
#define UNI_SUR_HIGH_END    (UTF32)0xDBFF
#define UNI_SUR_LOW_START   (UTF32)0xDC00
#define UNI_SUR_LOW_END     (UTF32)0xDFFF
#define false	   0
#define true	    1

static Boolean isLegalUTF8(const UTF8 *source, int length) {
	UTF8 a;
	const UTF8 *srcptr = source + length;
	switch (length) {
	default: return false;
		/* Everything else falls through when "true"... */
	case 4: if ((a = (*--srcptr)) < 0x80 || a > 0xBF) return false;
	case 3: if ((a = (*--srcptr)) < 0x80 || a > 0xBF) return false;
	case 2: if ((a = (*--srcptr)) > 0xBF) return false;

		switch (*source) {
			/* no fall-through in this inner switch */
		case 0xE0: if (a < 0xA0) return false; break;
		case 0xED: if (a > 0x9F) return false; break;
		case 0xF0: if (a < 0x90) return false; break;
		case 0xF4: if (a > 0x8F) return false; break;
		default:   if (a < 0x80) return false;
		}

	case 1: if (*source >= 0x80 && *source < 0xC2) return false;
	}
	if (*source > 0xF4) return false;
	return true;
}

static const char trailingBytesForUTF8[256] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

static const UTF32 offsetsFromUTF8[6] = { 0x00000000UL, 0x00003080UL, 0x000E2080UL,
			 0x03C82080UL, 0xFA082080UL, 0x82082080UL };

static const UTF8 firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

std::wstring UTF8ToUTF16(const UTF8Char *utf8, size_t length)
{
	UTF16Char output[4096];
	const UTF8 *src_begin = reinterpret_cast<const UTF8 *>(utf8);
	const UTF8 *src_end = src_begin + length;
	UTF16 *dst_begin = reinterpret_cast<UTF16 *>(output);

	std::wstring  utf16;
	while (src_begin < src_end)
	{
		ConversionResult result = ConvertUTF8toUTF16(&src_begin,
													 src_end,
													 &dst_begin,
													 dst_begin + COUNT_OF(output),
													 lenientConversion);

		utf16.append(100, dst_begin - reinterpret_cast<UTF16 *>(output));
		dst_begin = reinterpret_cast<UTF16 *>(output);
		if (result == sourceIllegal || result == sourceExhausted)
		{
			utf16.clear();
			break;
		}
	}

	return utf16;
}

std::string UTF16ToUTF8(const UTF16Char *utf16, size_t length)
{
	UTF8Char output[8192];
	const UTF16 *src_begin = reinterpret_cast<const UTF16 *>(utf16);
	const UTF16 *src_end = src_begin + length;
	UTF8 *dst_begin = reinterpret_cast<UTF8 *>(output);

	std::string utf8;
	while (src_begin < src_end)
	{
		ConversionResult result = ConvertUTF16toUTF8(&src_begin,
													 src_end,
													 &dst_begin,
													 dst_begin + COUNT_OF(output),
													 lenientConversion);

		utf8.append(output, dst_begin - reinterpret_cast<UTF8 *>(output));
		dst_begin = reinterpret_cast<UTF8 *>(output);
		if (result == sourceIllegal || result == sourceExhausted)
		{
			utf8.clear();
			break;
		}
	}

	return utf8;
}

std::wstring UTF8ToUTF16(const std::string &utf8)
{
	return UTF8ToUTF16(utf8.c_str(), utf8.length());
}

std::string UTF16ToUTF8(const std::wstring &utf16)
{
	return UTF16ToUTF8(utf16.c_str(), utf16.length());
}

ConversionResult ConvertUTF8toUTF16(
	const UTF8** sourceStart, const UTF8* sourceEnd,
	UTF16** targetStart, UTF16* targetEnd, ConversionFlags flags) {
	ConversionResult result = conversionOK;
	const UTF8* source = *sourceStart;
	UTF16* target = *targetStart;
	while (source < sourceEnd) {
		UTF32 ch = 0;
		unsigned short extraBytesToRead = trailingBytesForUTF8[*source];
		if (source + extraBytesToRead >= sourceEnd) {
			result = sourceExhausted; break;
		}
		/* Do this check whether lenient or strict */
		if (!isLegalUTF8(source, extraBytesToRead + 1)) {
			result = sourceIllegal;
			break;
		}
		/*
		 * The cases all fall through. See "Note A" below.
		 */
		switch (extraBytesToRead) {
		case 5: ch += *source++; ch <<= 6; /* remember, illegal UTF-8 */
		case 4: ch += *source++; ch <<= 6; /* remember, illegal UTF-8 */
		case 3: ch += *source++; ch <<= 6;
		case 2: ch += *source++; ch <<= 6;
		case 1: ch += *source++; ch <<= 6;
		case 0: ch += *source++;
		}
		ch -= offsetsFromUTF8[extraBytesToRead];

		if (target >= targetEnd) {
			source -= (extraBytesToRead + 1); /* Back up source pointer! */
			result = targetExhausted; break;
		}
		if (ch <= UNI_MAX_BMP) { /* Target is a character <= 0xFFFF */
			/* UTF-16 surrogate values are illegal in UTF-32 */
			if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
				if (flags == strictConversion) {
					source -= (extraBytesToRead + 1); /* return to the illegal value itself */
					result = sourceIllegal;
					break;
				}
				else {
					*target++ = UNI_REPLACEMENT_CHAR;
				}
			}
			else {
				*target++ = (UTF16)ch; /* normal case */
			}
		}
		else if (ch > UNI_MAX_UTF16) {
			if (flags == strictConversion) {
				result = sourceIllegal;
				source -= (extraBytesToRead + 1); /* return to the start */
				break; /* Bail out; shouldn't continue */
			}
			else {
				*target++ = UNI_REPLACEMENT_CHAR;
			}
		}
		else {
			/* target is a character in range 0xFFFF - 0x10FFFF. */
			if (target + 1 >= targetEnd) {
				source -= (extraBytesToRead + 1); /* Back up source pointer! */
				result = targetExhausted; break;
			}
			ch -= halfBase;
			*target++ = (UTF16)((ch >> halfShift) + UNI_SUR_HIGH_START);
			*target++ = (UTF16)((ch & halfMask) + UNI_SUR_LOW_START);
		}
	}
	*sourceStart = source;
	*targetStart = target;
	return result;
}

ConversionResult ConvertUTF16toUTF8(
	const UTF16** sourceStart, const UTF16* sourceEnd,
	UTF8** targetStart, UTF8* targetEnd, ConversionFlags flags) {
	ConversionResult result = conversionOK;
	const UTF16* source = *sourceStart;
	UTF8* target = *targetStart;
	while (source < sourceEnd) {
		UTF32 ch;
		unsigned short bytesToWrite = 0;
		const UTF32 byteMask = 0xBF;
		const UTF32 byteMark = 0x80;
		const UTF16* oldSource = source; /* In case we have to back up because of target overflow. */
		ch = *source++;
		/* If we have a surrogate pair, convert to UTF32 first. */
		if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END) {
			/* If the 16 bits following the high surrogate are in the source buffer... */
			if (source < sourceEnd) {
				UTF32 ch2 = *source;
				/* If it's a low surrogate, convert to UTF32. */
				if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END) {
					ch = ((ch - UNI_SUR_HIGH_START) << halfShift)
						+ (ch2 - UNI_SUR_LOW_START) + halfBase;
					++source;
				}
				else if (flags == strictConversion) { /* it's an unpaired high surrogate */
					--source; /* return to the illegal value itself */
					result = sourceIllegal;
					break;
				}
			}
			else { /* We don't have the 16 bits following the high surrogate. */
				--source; /* return to the high surrogate */
				result = sourceExhausted;
				break;
			}
		}
		else if (flags == strictConversion) {
			/* UTF-16 surrogate values are illegal in UTF-32 */
			if (ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END) {
				--source; /* return to the illegal value itself */
				result = sourceIllegal;
				break;
			}
		}
		/* Figure out how many bytes the result will require */
		if (ch < (UTF32)0x80) {
			bytesToWrite = 1;
		}
		else if (ch < (UTF32)0x800) {
			bytesToWrite = 2;
		}
		else if (ch < (UTF32)0x10000) {
			bytesToWrite = 3;
		}
		else if (ch < (UTF32)0x110000) {
			bytesToWrite = 4;
		}
		else {
			bytesToWrite = 3;
			ch = UNI_REPLACEMENT_CHAR;
		}

		target += bytesToWrite;
		if (target > targetEnd) {
			source = oldSource; /* Back up source pointer! */
			target -= bytesToWrite; result = targetExhausted; break;
		}
		switch (bytesToWrite) { /* note: everything falls through. */
		case 4: *--target = (UTF8)((ch | byteMark) & byteMask); ch >>= 6;
		case 3: *--target = (UTF8)((ch | byteMark) & byteMask); ch >>= 6;
		case 2: *--target = (UTF8)((ch | byteMark) & byteMask); ch >>= 6;
		case 1: *--target = (UTF8)(ch | firstByteMark[bytesToWrite]);
		}
		target += bytesToWrite;
	}
	*sourceStart = source;
	*targetStart = target;
	return result;
}

std::wstring GetLocalAppDataDir(HANDLE token /* = NULL */)
{
#if (NTDDI_VERSION < NTDDI_VISTA)
#ifndef KF_FLAG_CREATE
#define KF_FLAG_CREATE 0x00008000
#endif
#endif

	std::wstring temp_path;

	OSVERSIONINFO os_version = { 0 };
	os_version.dwOSVersionInfoSize = sizeof(os_version);
	GetVersionEx(&os_version);

	if (os_version.dwMajorVersion >= 6)
	{
		typedef HRESULT(WINAPI *__SHGetKnownFolderPath)(REFKNOWNFOLDERID, DWORD, HANDLE, PWSTR*);
		HMODULE moudle_handle = ::LoadLibraryW(L"shell32.dll");
		if (moudle_handle != NULL)
		{
			__SHGetKnownFolderPath _SHGetKnownFolderPath =
				reinterpret_cast<__SHGetKnownFolderPath>(::GetProcAddress(moudle_handle, "SHGetKnownFolderPath"));
			if (_SHGetKnownFolderPath != NULL)
			{
				PWSTR result = NULL;
				if (S_OK == _SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_CREATE, token, &result))
				{
					temp_path = result;
					::CoTaskMemFree(result);
				}
			}
			::FreeLibrary(moudle_handle);
		}
	}
	else
	{
		// On Windows XP, CSIDL_LOCAL_APPDATA represents "{user}\Local Settings\Application Data"
		// while CSIDL_APPDATA represents "{user}\Application Data"
		wchar_t buffer[MAX_PATH];
		if (S_OK == ::SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE, token, SHGFP_TYPE_CURRENT, buffer))
			temp_path = buffer;
	}
	if (!temp_path.empty())
		if (temp_path.back() != L'\\')
			temp_path.push_back(L'\\');
	return temp_path;
}

} // namespace nbase
