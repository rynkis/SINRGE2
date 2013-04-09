/*
*
*
*
*/
extern "C"
{
#include "ruby.h"
//#include "ruby/encoding.h"
}
#include "rge_resource.h"

void ResManager::Init()
{
	int i;

	GetModuleFileName(GetModuleHandle(NULL), szAppPath, sizeof(szAppPath));

	for(i = rgestrlen(szAppPath) - 1; i > 0; --i) if (szAppPath[i] == rgestr('\\')) break;

	szAppPath[i + 1] = 0;
}

void* ResManager::Resource_Load(const rgechar *filename, DWORD *size)
{
	void	*data = 0;
	HANDLE	handle = NULL;
	DWORD	file_size;

	handle = CreateFile(Resource_MakePath(filename), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);
	if (handle == INVALID_HANDLE_VALUE)
		return 0;
	
	file_size = GetFileSize(handle, NULL);

	if (!(data = xmalloc(file_size)))
		goto failed_return;

	if (ReadFile(handle, data, file_size, &file_size, NULL) == 0)
		goto failed_return;
	
	CloseHandle(handle);

	if(size)	*size = file_size;

	return data;

failed_return:
	if (handle != NULL)
	{
		CloseHandle(handle);
		handle = NULL;
	}
	if (data)
	{
		xfree(data);
		data = 0;
	}
	return 0;
}

void ResManager::Resource_Free(void *data)
{
	if (data)	xfree(data);
}

/*
*	Resource_Load_Without_Suffix(...)
*
*		从硬盘读取文件，允许省略后缀名。
*
*	@param
*		utf8_filename	-	准备读取的文件名
*		size			-	保存读取成功后的文件字节数，可以为0。
*		suffixs			-	备选的后缀名数组
*		suffixs_size	-	后缀名数组元素个数
*		suffix_idx		-	保存读取成功后的后缀索引号，方便取回实际名字。当没有添加后缀名时，索引号为 -1。
*
*	@return
*		成功返回数据指针，失败返回0。返回的指针不用之后需要Resource_Free掉。
*/
void* ResManager::Resource_Load_Without_Suffix(char *utf8_filename, DWORD *size, char *suffixs[], u32 suffixs_size, int *suffix_idx)
{
	void *data;

	u32	i;

	if (!utf8_filename)	return 0;

	char szTmpFilename[MAX_PATH];

	data = Resource_Load(Kconv::UTF8ToUnicode(utf8_filename), size);		//	UTF8 to Unicode

	if (data)
	{
		if (suffix_idx)	*suffix_idx = -1;
		return data;
	}

	for (i = 0; i < suffixs_size; ++i)
	{
		strcpy_s(szTmpFilename, utf8_filename);
		strcat_s(szTmpFilename, suffixs[i]);

		data = Resource_Load(Kconv::UTF8ToUnicode(szTmpFilename), size);	//	UTF8 to Unicode

		if (data)
		{
			if (suffix_idx)	*suffix_idx = i;
			return data;
		}
	}

	return 0;
}

rgechar* ResManager::Resource_MakePath(const rgechar *filename)
{
	int i;

	if (!filename)
		rgestrcpy(szTmpFilename, szAppPath);
	else if (filename[0] == '\\' || filename[0] == '/' || filename[1] == ':')
		rgestrcpy(szTmpFilename, filename);
	else
	{
		rgestrcpy(szTmpFilename, szAppPath);
		if (filename) rgestrcat(szTmpFilename, filename);
	}

	for (i = 0; szTmpFilename[i]; ++i) { if (szTmpFilename[i] == '/') szTmpFilename[i] = '\\'; }

	return szTmpFilename;
}
