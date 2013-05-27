#include "7pkg.h"
#include "TANGRAM.h"
#include <Windows.h>

C7pkgWriter::C7pkgWriter(const wchar_t * pkgname, const wchar_t * password)
{
	m_valid = true;
	if (GetFileAttributesW(pkgname) != INVALID_FILE_ATTRIBUTES)
		m_package = zp::open(pkgname, password, 0);
	else
		m_package = zp::create(pkgname, password);

	if (!m_package)
	{
		m_valid = false;
		return;
	}
}

C7pkgWriter::~C7pkgWriter()
{
	zp::close(m_package);
}

bool C7pkgWriter::DumpFile(const wchar_t * filename) const
{
	if (m_package->hasFile(filename))
	{
		if (!m_package->removeFile(filename))
			return false;
	}

	WIN32_FIND_DATA fd;
	HANDLE findFile = ::FindFirstFile(filename, &fd);
	if (findFile != INVALID_HANDLE_VALUE && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
	{
		//it's a file
		if (fd.nFileSizeHigh != 0)
		{
			return false;
		}
		zp::u32 compressSize = 0;
		zp::u32 flag = 0;
		bool ret = m_package->addFile(filename, filename, fd.nFileSizeLow, zp::FILE_COMPRESS, &compressSize, &flag);
		if (ret)
		{
			m_package->flush();
		}
		::FindClose(findFile);
		return ret;
	}
	return false;
}

bool C7pkgWriter::Defrag() const
{
	return m_package->defrag(NULL, NULL);
}
