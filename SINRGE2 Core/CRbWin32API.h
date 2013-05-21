/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Win32API
*/
#ifndef __C_RB_WIN32API_H__
#define __C_RB_WIN32API_H__

#include "CRbClassBase.h"

class CRbWin32API : public CRbClassBase
{
public:
	CRbWin32API();
	virtual ~CRbWin32API();

public:
	static void				InitLibrary();

protected:
	virtual VALUE			initialize(int argc, VALUE * argv, VALUE obj);

private:
	HMODULE					m_hDll;

protected:
	dm_method_vargs(call)
};

#endif	//	__C_RB_WIN32API_H__
