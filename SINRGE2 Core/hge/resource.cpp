/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core functions implementation: resources management
*/


#include "sin_app.h"
#include "hge_impl.h"

#define NOCRYPT

void * CALL HGE_Impl::Resource_Load(const wchar_t * filename, DWORD * size)
{
	void * data = 0;
	HANDLE	handle = NULL;
	DWORD	file_size;

	if (GetAppPtr()->Get7pkgReader())
	{
		file_size = GetAppPtr()->Get7pkgReader()->FileLength(filename);
		if (!(data = malloc(file_size)))
			goto failed_return;

		if (!GetAppPtr()->Get7pkgReader()->LoadData(filename, data))
			goto failed_return;
	}
	else
	{
		handle = CreateFile(Resource_MakePath(filename), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);
		if (handle == INVALID_HANDLE_VALUE)
			return 0;
	
		file_size = GetFileSize(handle, NULL);

		if (!(data = malloc(file_size)))
			goto failed_return;

		if (ReadFile(handle, data, file_size, &file_size, NULL) == 0)
			goto failed_return;
	
		CloseHandle(handle);
	}

	if (size)	*size = file_size;

	return data;

failed_return:
	if (handle != NULL)
	{
		CloseHandle(handle);
		handle = NULL;
	}
	if (data)
	{
		free(data);
		data = 0;
	}
	return 0;
}

void * CALL HGE_Impl::Resource_Load_Without_Suffix(const wchar_t * filename, DWORD * size, wchar_t * suffixs[], int suffixs_size, int * suffix_idx)
{
	void * data;

	int	i;

	if (!filename)	return 0;

	wchar_t szTmpFilename[MAX_PATH];

	for (i = 0; i < suffixs_size; ++i)
	{
		wcscpy_s(szTmpFilename, filename);
		wcscat_s(szTmpFilename, suffixs[i]);

		data = Resource_Load(szTmpFilename, size);

		if (data)
		{
			if (suffix_idx)	*suffix_idx = i;
			return data;
		}
	}
	return 0;
}

void CALL HGE_Impl::Resource_Free(void * res)
{
	if(res) free(res);
}

wchar_t * CALL HGE_Impl::Resource_MakePath(const wchar_t * filename)
{
	int i;

	if(!filename)
		wcscpy_s(szTmpFilename, szAppPath);
	else if(filename[0]=='\\' || filename[0]=='/' || filename[1]==':')
		wcscpy_s(szTmpFilename, filename);
	else
	{
		wcscpy_s(szTmpFilename, szAppPath);
		if(filename) wcscat_s(szTmpFilename, filename);
	}

	for(i=0; szTmpFilename[i]; i++) { if(szTmpFilename[i]=='/') szTmpFilename[i]='\\'; }
	return szTmpFilename;
}
