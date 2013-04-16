/*
** Haaf's Game Engine 1.7
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hgeStringTable helper class header
*/


#ifndef HGESTRINGS_H
#define HGESTRINGS_H


#include "hge.h"


#define MAXSTRNAMELENGTH 64


struct NamedString
{
	wchar_t			name[MAXSTRNAMELENGTH];
	wchar_t			*string;
	NamedString		*next;
};

/*
** HGE String table class
*/
class hgeStringTable
{
public:
	hgeStringTable(const wchar_t *filename);
	~hgeStringTable();

	wchar_t			*GetString(const wchar_t *name);

private:
	hgeStringTable(const hgeStringTable &);
	hgeStringTable&	operator= (const hgeStringTable &);

	NamedString		*strings;

	static HGE		*hge;
};


#endif
