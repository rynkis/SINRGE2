#ifndef __7PKG_WRITER_H__
#define __7PKG_WRITER_H__
#include "RbClassBase.h"
#include "TANGRAM.h"
#include "sin_app.h"


class Rb7pkgWriter : public RbClassBase
{
public:
	Rb7pkgWriter();													//	allocate
	virtual ~Rb7pkgWriter();

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