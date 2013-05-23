#include "sin_kconv.h"
#include "sin_auto_buffer.h"

#include <Windows.h>
//#include <assert.h>

namespace
{
	static CAutoBuffer s_cAutoBuffer01;				///<	临时缓冲区01
	static CAutoBuffer s_cAutoBuffer02;				///<	临时缓冲区02
}

/**
 *	Anis转Unicode
 */
wchar_t * Kconv::AnsiToUnicode(const char * str, CAutoBuffer * pAutoBuffer)
{
	if (!str) 
		return 0;

	if (!pAutoBuffer) pAutoBuffer = &s_cAutoBuffer01;

	// Ansi 转 Unicode
	int wide_str_len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);

	pAutoBuffer->ensure_size(sizeof(wchar_t) * wide_str_len);	///<	确保缓冲区大小

	wchar_t * wb_string = (wchar_t *)pAutoBuffer->get_buffer_ptr();
	//assert(wb_string);

	MultiByteToWideChar(CP_ACP, 0, str, -1, wb_string, wide_str_len);

	return wb_string;
}

/**
 *	UTF8转Unicode
 */
wchar_t * Kconv::UTF8ToUnicode(const char * str, CAutoBuffer * pAutoBuffer)
{
	if (!str) 
		return 0;

	if (!pAutoBuffer) pAutoBuffer = &s_cAutoBuffer01;

	// UTF8 转 Unicode
	int wide_str_len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);

	pAutoBuffer->ensure_size(sizeof(wchar_t) * wide_str_len);	///<	确保缓冲区大小

	wchar_t * wb_string = (wchar_t *)pAutoBuffer->get_buffer_ptr();
	//assert(wb_string);

	MultiByteToWideChar(CP_UTF8, 0, str, -1, wb_string, wide_str_len);

	return wb_string;
}

/**
 *	Unicode转Ansi
 */
char * Kconv::UnicodeToAnsi(const wchar_t * str, CAutoBuffer * pAutoBuffer)
{
	if (!str) 
		return 0;

	if (!pAutoBuffer) pAutoBuffer = &s_cAutoBuffer01;

	//	Unicode 转 Ansi
	int ansi_len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);

	pAutoBuffer->ensure_size(ansi_len);	///<	确保缓冲区大小

	char * mb_ansi_string = (char *)pAutoBuffer->get_buffer_ptr();
	//assert(mb_ansi_string);

	WideCharToMultiByte(CP_ACP, 0, str, -1, (LPSTR)mb_ansi_string, ansi_len, NULL, NULL);

	return mb_ansi_string;
}

/**
 *	Unicode转UTF8
 */
char* Kconv::UnicodeToUTF8(const wchar_t * str, CAutoBuffer * pAutoBuffer)
{
	if (!str) 
		return 0;

	if (!pAutoBuffer) pAutoBuffer = &s_cAutoBuffer01;

	//	Unicode 转 UTF8
	int utf8_len = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);

	pAutoBuffer->ensure_size(utf8_len);	///<	确保缓冲区大小

	char * mb_utf8_string = (char *)pAutoBuffer->get_buffer_ptr();
	//assert(mb_utf8_string);

	WideCharToMultiByte(CP_UTF8, 0, str, -1, (LPSTR)mb_utf8_string, utf8_len, NULL, NULL);

	return mb_utf8_string;
}

/**
 *	UTF8转Ansi
 */
char * Kconv::UTF8ToAnsi(const char * str, CAutoBuffer * pAutoBuffer)
{
	if (!str) 
		return 0;

	//	UTF8 转 Unicode
	int wide_str_len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);

	s_cAutoBuffer02.ensure_size(sizeof(wchar_t) * wide_str_len);	///<	确保缓冲区大小

	wchar_t * wide_str_buf = (wchar_t *)s_cAutoBuffer02.get_buffer_ptr();
	//assert(wide_str_buf);

	MultiByteToWideChar(CP_UTF8, 0, str, -1, wide_str_buf, wide_str_len);

	//	Unicode 转 Ansi
	return UnicodeToAnsi(wide_str_buf, pAutoBuffer);
}

/**
 *	Ansi转UTF8
 */
char * Kconv::AnsiToUTF8(const char * str, CAutoBuffer * pAutoBuffer)
{
	if (!str) 
		return 0;

	// Ansi 转 Unicode
	int wide_str_len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);

	s_cAutoBuffer02.ensure_size(sizeof(wchar_t) * wide_str_len);	///<	确保缓冲区大小

	wchar_t * wide_str_buf = (wchar_t *)s_cAutoBuffer02.get_buffer_ptr();
	//assert(wide_str_buf);

	MultiByteToWideChar(CP_ACP, 0, str, -1, wide_str_buf, wide_str_len);

	//	Unicode 转 UTF8
	return UnicodeToUTF8(wide_str_buf, pAutoBuffer);
}