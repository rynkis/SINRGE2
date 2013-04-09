#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__
#include <windows.h>
#include "sin_types.h"
#include "Kconv.h"
#include "auxSingleton.h"

#ifdef UNICODE
typedef	wchar_t	rgechar;

#define	rgestrlen	wcslen
#define	rgestrcpy	wcscpy_s
#define rgestrcat	wcscat_s
#define rgestr(str)	L##str

#else
typedef	char	rgechar;

#define	rgestrlen	strlen
#define	rgestrcpy	strcpy
#define	rgestrcat	strcat
#define rgestr(str)	str

#endif	// !UNICODE

class ResManager : public AuxSingleton<ResManager>
{
public:
	void	Init();

	void*	Resource_Load(const rgechar *filename, DWORD *size);
	void*	Resource_Load_Without_Suffix(char *utf8_filename, DWORD *size, char *suffixs[], u32 suffixs_size, int *suffix_idx);
	void	Resource_Free(void *data);

	rgechar*	Resource_MakePath(const rgechar *filename);

private:
	rgechar	szTmpFilename[MAX_PATH];
	rgechar	szAppPath[MAX_PATH];
};

inline ResManager*		GetResManager() { return ResManager::Instance(); }

#endif