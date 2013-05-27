#ifndef __SHY07_PACKAGE_H__
#define __SHY07_PACKAGE_H__
#include "zpack.h"
#include "TANGRAM.h"

class Encryptor;

class C7pkgReader : public IC7pkgReader
{
public:
	C7pkgReader(const wchar_t * pkgname, const wchar_t * password);
	~C7pkgReader();

public:
	virtual bool			IsFileExist(const wchar_t * filename) const;
	virtual unsigned long	FileLength(const wchar_t * filename) const;
	virtual bool			LoadData(const wchar_t * filename, void * data) const;

	inline bool				IsValid() const { return m_valid; }

private:
	zp::IPackage *	m_package;
	Encryptor *		m_encryptor;

	bool			m_valid;
};

class C7pkgWriter : public IC7pkgWriter
{
public:
	C7pkgWriter(const wchar_t * pkgname, const wchar_t * password);
	~C7pkgWriter();

public:
	virtual bool			DumpFile(const wchar_t * filename) const;
	virtual bool			Defrag() const;

	inline bool				IsValid() const { return m_valid; }

private:
	zp::IPackage *	m_package;
	Encryptor *		m_encryptor;

	bool			m_valid;
};

#endif // __SHY07_PACKAGE_H__