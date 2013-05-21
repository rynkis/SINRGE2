#ifndef __7PKG_WRITER_H__
#define __7PKG_WRITER_H__
#include "CRbClassBase.h"
#include "TANGRAM.h"
#include "sin_app.h"


class CRb7pkgWriter : public CRbClassBase
{
public:
	CRb7pkgWriter();													//	allocate
	virtual ~CRb7pkgWriter();

public:
	static void		InitLibrary();

protected:
	virtual VALUE	initialize(int argc, VALUE * argv, VALUE obj);	//	initialize

protected:
	void			check_raise();

protected:
	IC7pkgWriter *	m_7pkgWriter;

	bool			m_closed;

protected:
	dm_method(is_closed)
	dm_method(close)
	dm_method01(dump_file)
	dm_method(defrag)

};


#endif // __7PKG_WRITER_H__