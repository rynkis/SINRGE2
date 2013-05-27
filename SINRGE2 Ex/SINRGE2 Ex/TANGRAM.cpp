#include "TANGRAM.h"
#include "7pkg.h"

IC7pkgReader* pr_open(const wchar_t *filename, const wchar_t * password)
{
	C7pkgReader *pr = new C7pkgReader(filename, password);
	if (!pr->IsValid())
	{
		delete pr;
		pr = NULL;
	}
	return pr;
}

void pr_close(IC7pkgReader* package)
{
	delete static_cast<C7pkgReader*>(package);
}

IC7pkgWriter* pw_open(const wchar_t *filename, const wchar_t * password)
{
	C7pkgWriter *pw = new C7pkgWriter(filename, password);
	if (!pw->IsValid())
	{
		delete pw;
		pw = NULL;
	}
	return pw;
}

void pw_close(IC7pkgWriter* package)
{
	delete static_cast<C7pkgWriter*>(package);
}