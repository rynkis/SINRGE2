#include "zpack.h"
#include "zpPackage.h"
#include "zpFile.h"
#include <fstream>

using namespace std;

namespace zp
{

///////////////////////////////////////////////////////////////////////////////////////////////////
IPackage* open(const Char* filename, const Char * password, u32 flag)
{
	Package* package = new Package(filename, password,
									(flag & OPEN_READONLY) != 0,
									(flag & OPEN_NO_FILENAME) == 0);
	if (!package->valid())
	{
		delete package;
		package = NULL;
	}
	return package;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void close(IPackage* package)
{
	delete static_cast<Package*>(package);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
IPackage* create(const Char* filename, const Char * password, u32 chunkSize, u32 fileUserDataSize)
{
	fstream stream;
	locale loc = locale::global(locale(""));
	stream.open(filename, ios_base::out | ios_base::trunc | ios_base::binary);
	locale::global(loc);
	if (!stream.is_open())
	{
		return NULL;
	}
	PackageHeader * header = (PackageHeader *)malloc(sizeof(PackageHeader));
	header->sign = PACKAGE_FILE_SIGN;
	header->version = CURRENT_VERSION;
	header->headerSize = sizeof(PackageHeader);
	header->fileCount = 0;
	header->fileEntryOffset = sizeof(PackageHeader);
	header->filenameOffset = sizeof(PackageHeader);
	header->allFileEntrySize = 0;
	header->allFilenameSize = 0;
	header->originFilenamesSize = 0;
	header->chunkSize = chunkSize;
#ifdef ZP_USE_WCHAR
	header->flag = PACK_UNICODE;
#else
	header->flag = 0;
#endif
	header->fileEntrySize = sizeof(FileEntry) + fileUserDataSize;
	memset(header->reserved, 0, sizeof(header->reserved));
	memset(header->tempInfo, 0, sizeof(header->tempInfo));
	memset(header->tempInfo, 0, sizeof(header->keySha1));

	stream.write((char *)header, sizeof(PackageHeader));
	stream.close();

	free(header);
	return open(filename, password, 0);
}

}

