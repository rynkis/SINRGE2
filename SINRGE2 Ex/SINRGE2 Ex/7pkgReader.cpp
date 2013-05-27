#include "7pkg.h"

C7pkgReader::C7pkgReader(const wchar_t * pkgname, const wchar_t * password)
{
	m_valid = true;
	m_package = zp::open(pkgname, password);
	if (!m_package)
	{
		m_valid = false;
		return;
	}
}

C7pkgReader::~C7pkgReader()
{
	zp::close(m_package);
}

bool C7pkgReader::IsFileExist(const wchar_t * filename) const
{
	return m_package->hasFile(filename);
}

unsigned long C7pkgReader::FileLength(const wchar_t * filename) const
{
	unsigned long len = 0;
	if (m_package->hasFile(filename))
	{
		zp::IReadFile * file = m_package->openFile(filename);
		len = file->size();
		m_package->closeFile(file);
		return len;
	}
	return len;
}

bool C7pkgReader::LoadData(const wchar_t * filename, void * data) const
{
	if (m_package->hasFile(filename))
	{
		zp::IReadFile * file = m_package->openFile(filename);
		file->read((unsigned char *)data, file->size());
		m_package->closeFile(file);
		return true;
	}
	return false;
}
