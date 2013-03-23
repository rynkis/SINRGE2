#ifndef __K_CONV_H__
#define __K_CONV_H__

class CAutoBuffer;

namespace Kconv
{
	wchar_t*	AnsiToUnicode(const char* str, CAutoBuffer* pAutoBuffer = 0);
	wchar_t*	UTF8ToUnicode(const char* str, CAutoBuffer* pAutoBuffer = 0);
	char*		UnicodeToAnsi(const wchar_t* str, CAutoBuffer* pAutoBuffer = 0);
	char*		UnicodeToUTF8(const wchar_t* str, CAutoBuffer* pAutoBuffer = 0);
	char*		UTF8ToAnsi(const char* str, CAutoBuffer* pAutoBuffer = 0);
	char*		AnsiToUTF8(const char* str, CAutoBuffer* pAutoBuffer = 0);
};

#endif	//	__K_CONV_H__

