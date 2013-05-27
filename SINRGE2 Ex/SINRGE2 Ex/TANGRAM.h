#ifndef __TANGRAM_H__
#define __TANGRAM_H__

#ifdef EXFUNC4S_EXPORTS
#define EXFUNC4S_API __declspec(dllexport)
#else
#define EXFUNC4S_API __declspec(dllimport)
#endif

class EXFUNC4S_API IC7pkgReader
{
public:
	virtual bool			IsFileExist(const wchar_t * filename) const = 0;
	virtual unsigned long	FileLength(const wchar_t * filename) const = 0;
	virtual bool			LoadData(const wchar_t * filename, void * data) const = 0;

protected:
	virtual ~IC7pkgReader(){}
};

class EXFUNC4S_API IC7pkgWriter
{
public:
	virtual bool			DumpFile(const wchar_t * filename) const = 0;
	virtual bool			Defrag() const = 0;

protected:
	virtual ~IC7pkgWriter(){}
};

extern "C" EXFUNC4S_API IC7pkgReader* pr_open(const wchar_t * filename, const wchar_t * password);
extern "C" EXFUNC4S_API void pr_close(IC7pkgReader* package);
extern "C" EXFUNC4S_API IC7pkgWriter* pw_open(const wchar_t * filename, const wchar_t * password);
extern "C" EXFUNC4S_API void pw_close(IC7pkgWriter* package);

#endif // __TANGRAM_H__