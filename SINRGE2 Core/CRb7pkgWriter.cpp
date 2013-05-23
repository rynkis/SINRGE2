/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** Ruby Class SpkgWriter
*/
#include "CRb7pkgWriter.h"

VALUE rb_c7pkgWriter;

CRb7pkgWriter::CRb7pkgWriter()
	: m_7pkgWriter(0)
	, m_closed(false)
{
}

CRb7pkgWriter::~CRb7pkgWriter()
{
	if (m_7pkgWriter)
	{
		GetAppPtr()->Close7pkgWriter(m_7pkgWriter);
		m_7pkgWriter = NULL;
	}
}

void CRb7pkgWriter::InitLibrary()
{
	rb_c7pkgWriter = rb_define_class_under(rb_mSin, "SpkgWriter", rb_cObject);
	
	// special method
	rb_define_alloc_func(rb_c7pkgWriter, ObjAllocate<CRb7pkgWriter>);
	rb_define_method(rb_c7pkgWriter, "initialize", (RbFunc)dm_initialize,	-1);

	// instance method
	rb_define_method(rb_c7pkgWriter, "close",		(RbFunc)dm_close,			0);
	rb_define_method(rb_c7pkgWriter, "closed?",		(RbFunc)dm_is_closed,		0);
	rb_define_method(rb_c7pkgWriter, "dump_file",	(RbFunc)dm_dump_file,		1);
	rb_define_method(rb_c7pkgWriter, "defrag",		(RbFunc)dm_defrag,			0);

	// supplement
 	rb_define_method(rb_c7pkgWriter, "to_s",		(RbFunc)dm_to_string,	0);
}

VALUE CRb7pkgWriter::initialize(int argc, VALUE * argv, VALUE obj)
{
	SafeStringValue(argv[0]);
	SafeStringValue(argv[1]);

	wchar_t szFilename[MAX_PATH], szPassword[MAX_PATH];
	wcscpy_s(szFilename, Kconv::UTF8ToUnicode(RSTRING_PTR(argv[0])));
	wcscpy_s(szPassword, Kconv::UTF8ToUnicode(RSTRING_PTR(argv[1])));

	m_7pkgWriter = GetAppPtr()->Open7pkgWriter(szFilename, szPassword);
	if (!m_7pkgWriter)
		rb_raise(rb_eSinError, "Failed to open or create 7package.");
	
	return obj;
}

void CRb7pkgWriter::check_raise()
{
	if (m_closed)
		rb_raise(rb_eSinError, "closed 7pkgWriter");
}

VALUE CRb7pkgWriter::is_closed()
{
	return C2RbBool(m_closed);
}

VALUE CRb7pkgWriter::close()
{
	if (m_closed)
		return Qnil;

	if (m_7pkgWriter)
	{
		GetAppPtr()->Close7pkgWriter(m_7pkgWriter);
		m_7pkgWriter = NULL;
	}

	m_closed = true;

	return Qnil;
}

VALUE CRb7pkgWriter::dump_file(VALUE filename)
{
	check_raise();

	SafeStringValue(filename);
	wchar_t * filenameW = Kconv::UTF8ToUnicode(RSTRING_PTR(filename));

	if (m_7pkgWriter)
	{
		if (!GetAppPtr()->IsFileExist(filenameW))
			rb_raise(rb_eSinError, "File is not exist:`%s'.", RSTRING_PTR(filename));

		return C2RbBool(m_7pkgWriter->DumpFile(filenameW));
	}
	return Qfalse;
}

VALUE CRb7pkgWriter::defrag()
{
	check_raise();
	if (m_7pkgWriter)
	{
		if (m_7pkgWriter->Defrag())
			return Qtrue;
	}
	return Qfalse;
}

imp_method(CRb7pkgWriter, is_closed)
imp_method(CRb7pkgWriter, close)
imp_method01(CRb7pkgWriter, dump_file)
imp_method(CRb7pkgWriter, defrag)
