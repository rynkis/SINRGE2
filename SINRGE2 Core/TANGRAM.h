#ifndef __TANGRAM_H__
#define __TANGRAM_H__

class IC7pkgReader
{
public:
	virtual bool			IsFileExist(const wchar_t * filename) const = 0;
	virtual unsigned long	FileLength(const wchar_t * filename) const = 0;
	virtual bool			LoadData(const wchar_t * filename, void * data) const = 0;

protected:
	virtual ~IC7pkgReader(){}
};

class IC7pkgWriter
{
public:
	virtual bool			DumpFile(const wchar_t * filename) const = 0;
	virtual bool			Defrag() const = 0;

protected:
	virtual ~IC7pkgWriter(){}
};

typedef IC7pkgReader * (*func_pr_open)(const wchar_t * filename, const wchar_t * password);
typedef void (*func_pr_close)(IC7pkgReader * package);
typedef IC7pkgWriter * (*func_pw_open)(const wchar_t * filename, const wchar_t * password);
typedef void (*func_pw_close)(IC7pkgWriter * package);

void SealStartup();
void SealCleanup();

#endif // __TANGRAM_H__