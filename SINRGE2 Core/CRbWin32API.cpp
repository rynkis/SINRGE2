/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Win32API
*/
#include "CRbWin32API.h"

VALUE rb_cWin32API;

namespace
{
	///<	参数类型枚举
	enum EArgsType
	{
		_T_VOID		= 0,
		_T_NUMBER	= 1,
		_T_POINTER	= 2,
		_T_INTEGER	= 3,
	};

	///<	解析输入参数
	static VALUE	SinParseImportArgs(VALUE import)
	{
		int i;
		int len;

		VALUE a_import = rb_ary_new();

		const int obj_type = rb_type(import);

		if (obj_type == T_ARRAY)
		{
			VALUE * ptr = RARRAY_PTR(import);
			for (i = 0, len = RARRAY_LEN(import); i < len; ++i) 
			{
				SafeStringValue(ptr[i]);
				switch (*(char *)RSTRING_PTR(ptr[i]))
				{
				case 'N': case 'n': case 'L': case 'l':
					rb_ary_push(a_import, INT2FIX(_T_NUMBER));
					break;
				case 'P': case 'p':
					rb_ary_push(a_import, INT2FIX(_T_POINTER));
					break;
				case 'I': case 'i':
					rb_ary_push(a_import, INT2FIX(_T_INTEGER));
					break;
				}
			}
		}
		else if (obj_type != T_NIL)
		{
			SafeStringValue(import);
			char * s = RSTRING_PTR(import);
			for (i = 0, len = RSTRING_LEN(import); i < len; ++i)
			{
				switch (*s++) 
				{
				case 'N': case 'n': case 'L': case 'l':
					rb_ary_push(a_import, INT2FIX(_T_NUMBER));
					break;
				case 'P': case 'p':
					rb_ary_push(a_import, INT2FIX(_T_POINTER));
					break;
				case 'I': case 'i':
					rb_ary_push(a_import, INT2FIX(_T_INTEGER));
					break;
				}
			}
		}

		return a_import;
	}

	///<	解析输出参数
	static int	SinParseExportArgs(VALUE _export)
	{
		int ex = _T_VOID;

		if (NIL_P(_export))
		{
			ex = _T_VOID;
		} 
		else 
		{
			SafeStringValue(_export);
			switch (*RSTRING_PTR(_export)) 
			{
			case 'V': case 'v':
				ex = _T_VOID;
				break;
			case 'N': case 'n': case 'L': case 'l':
				ex = _T_NUMBER;
				break;
			case 'P': case 'p':
				ex = _T_POINTER;
				break;
			case 'I': case 'i':
				ex = _T_INTEGER;
				break;
			}
		}

		return ex;
	}
}

CRbWin32API::CRbWin32API()
	: m_hDll(NULL)
{
}

CRbWin32API::~CRbWin32API()
{
	if (m_hDll != NULL)
	{
		FreeLibrary(m_hDll);
		m_hDll = NULL;
	}
}

void CRbWin32API::InitLibrary()
{
	rb_cWin32API = rb_define_class_under(rb_mSin, "Win32API", rb_cObject);

	// special method
	rb_define_alloc_func(rb_cWin32API, ObjAllocate<CRbWin32API>);
	rb_define_method(rb_cWin32API, "initialize",			(RbFunc)dm_initialize,			-1);

	// instance method
	rb_define_method(rb_cWin32API, "call",					(RbFunc)dm_call,				-1);
}

VALUE CRbWin32API::initialize(int argc, VALUE * argv, VALUE obj)
{
	VALUE dllname, proc, _import, _export;
	rb_scan_args(argc, argv, "4", &dllname, &proc, &_import, &_export);

	SafeStringValue(dllname);
	SafeStringValue(proc);
	m_hDll = LoadLibraryA(RSTRING_PTR(dllname));
	if (!m_hDll)
		rb_raise(rb_eSinError, "LoadLibrary: %s\n", RSTRING_PTR(dllname));
	
	HANDLE hproc = (HANDLE)GetProcAddress(m_hDll, RSTRING_PTR(proc));
	if (!hproc) 
	{
		VALUE str = rb_str_new3(proc); str = rb_str_cat(str, "A", 1);
		hproc = (HANDLE)GetProcAddress(m_hDll, RSTRING_PTR(str));
		if (!hproc)
		{
			rb_raise(rb_eSinError, "GetProcAddress: %s or %s\n", RSTRING_PTR(proc), RSTRING_PTR(str));
		}
	}
	rb_iv_set(obj, "__dll__",		rb_uint2inum((u32)m_hDll));
	rb_iv_set(obj, "__dllname__",	dllname);
	rb_iv_set(obj, "__proc__",		rb_uint2inum((u32)hproc));

	VALUE ary_import = SinParseImportArgs(_import);
	if (16 < RARRAY_LEN(ary_import))
	{
		rb_raise(rb_eSinError, "too many parameters: %ld\n", RARRAY_LEN(ary_import));
	}
	rb_iv_set(obj, "__import__", ary_import);

	rb_iv_set(obj, "__export__", INT2FIX(SinParseExportArgs(_export)));

	return obj;
}

VALUE CRbWin32API::call(int argc, VALUE * argv, VALUE obj)
{
	struct 
	{
		u32 params[16];
	} param;

	VALUE obj_proc		= rb_iv_get(obj, "__proc__");
	VALUE obj_import	= rb_iv_get(obj, "__import__");
	VALUE obj_export	= rb_iv_get(obj, "__export__");
	FARPROC ApiFunction	= (FARPROC)rb_num2ulong(obj_proc);

	VALUE args;

	int items	= rb_scan_args(argc, argv, "0*", &args);
	int nimport	= RARRAY_LEN(obj_import);

	if (items != nimport)
		rb_raise(rb_eSinError, "wrong number of parameters: expected %d, got %d", nimport, items);

	for (int i = 0; i < nimport; ++i) 
	{
		u32 lParam = 0;
		switch (FIX2INT(rb_ary_entry(obj_import, i))) 
		{
			VALUE str;
		case _T_NUMBER:
		case _T_INTEGER:
		default:
			lParam = rb_num2ulong(rb_ary_entry(args, i));
			break;
		case _T_POINTER:
			str = rb_ary_entry(args, i);
			if (NIL_P(str)) 
			{
				lParam = 0;
			} 
			else if (FIXNUM_P(str))
			{
				lParam = rb_num2ulong(str);
			} 
			else 
			{
				rb_string_value(&str);
				rb_str_modify(str);
				lParam = (u32)rb_string_value_ptr(&str);
			}
			break;
		}
		param.params[i] = lParam;
	}

	u32 retval;

	__try
	{
		__asm
		{
			mov			ebx, esp
			sub			esp, 40h
			mov			ecx, 10h
			lea			esi, [param]
			mov			edi, esp
			rep movs	dword ptr es:[edi], dword ptr [esi] 
			call        dword ptr [ApiFunction] 
			mov         esp, ebx
			mov         dword ptr [retval], eax 
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		rb_raise(rb_eSinError, "Api Crashed...");
	}

	switch (FIX2INT(obj_export)) 
	{
	case _T_NUMBER:
	case _T_INTEGER:
		return rb_int2inum(retval);
	case _T_POINTER:
		return (retval ? rb_str_new2((char *)retval) : Qnil);
	case _T_VOID:
	default:
		return INT2FIX(0);
	}
}

imp_method_vargs(CRbWin32API, call)