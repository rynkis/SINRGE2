/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Win32API
*/
#ifndef __RB_WIN32API_H__
#define __RB_WIN32API_H__

#include "RbClassBase.h"

class RbWin32API : public RbClassBase
{
public:
	RbWin32API();
	virtual ~RbWin32API();

public:
	static void				InitLibrary();

protected:
	virtual VALUE			initialize(int argc, VALUE *argv, VALUE obj);

private:
	HMODULE					m_hDll;

protected:
	dm_method_vargs(call)
};

#endif	//	__RB_WIN32API_H__
