/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core functions implementation: ini file
*/


#include "hge_impl.h"


void CALL HGE_Impl::Ini_SetInt(const wchar_t *section, const wchar_t *name, int value)
{
	wchar_t buf[256];

	if(szIniFile[0]) {
		wsprintf(buf,L"%d",value);
		WritePrivateProfileString(section, name, buf, szIniFile);
	}
}


int CALL HGE_Impl::Ini_GetInt(const wchar_t *section, const wchar_t *name, int def_val)
{
	wchar_t buf[256];

	if(szIniFile[0]) {
		if(GetPrivateProfileString(section, name, L"", buf, sizeof(buf), szIniFile))
		{ return _wtoi(buf); }
		else { return def_val; }
	}
	return def_val;
}


void CALL HGE_Impl::Ini_SetFloat(const wchar_t *section, const wchar_t *name, float value)
{
	wchar_t buf[256];

	if(szIniFile[0]) {
		wsprintf(buf,L"%f",value);
		WritePrivateProfileString(section, name, buf, szIniFile);
	}
}


float CALL HGE_Impl::Ini_GetFloat(const wchar_t *section, const wchar_t *name, float def_val)
{
	wchar_t buf[256];

	if(szIniFile[0]) {
		if(GetPrivateProfileString(section, name, L"", buf, sizeof(buf), szIniFile))
		{ return (float)_wtof(buf); }
		else { return def_val; }
	}
	return def_val;
}


void CALL HGE_Impl::Ini_SetString(const wchar_t *section, const wchar_t *name, const wchar_t *value)
{
	if(szIniFile[0]) WritePrivateProfileString(section, name, value, szIniFile);
}


wchar_t* CALL HGE_Impl::Ini_GetString(const wchar_t *section, const wchar_t *name, const wchar_t *def_val)
{
	if(szIniFile[0]) GetPrivateProfileString(section, name, def_val, szIniString, sizeof(szIniString), szIniFile);
	else wcscpy_s(szIniString, def_val);
	return szIniString;
}

