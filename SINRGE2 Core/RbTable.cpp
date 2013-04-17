#include "RbTable.h"
#include "sin_types.h"

VALUE rb_cTable;

void RbTable::InitLibrary()
{
	/**
	 *	@classname
	 *		Table
	 *
	 *	@desc
	 *		多维数组的类。
	 */
	rb_cTable = rb_define_class_under(rb_mSin, "Table", rb_cObject);

	// special method
	rb_define_alloc_func(rb_cTable, ObjAllocate<RbTable>);
	rb_define_method(rb_cTable, "initialize", (RbFunc)dm_initialize, -1);

	// class method
	rb_define_singleton_method(rb_cTable, "_load",(RbFunc)dm_load,	1);

	// instance method
	rb_define_method(rb_cTable, "resize",	(RbFunc)dm_resize,		-1);
	rb_define_method(rb_cTable, "xsize",	(RbFunc)dm_get_xsize,	0);
	rb_define_method(rb_cTable, "ysize",	(RbFunc)dm_get_ysize,	0);
	rb_define_method(rb_cTable, "zsize",	(RbFunc)dm_get_zsize,	0);

	rb_define_method(rb_cTable, "_dump",	(RbFunc)dm_dump,		1);

	// object attribute
	rb_define_method(rb_cTable, "[]",		(RbFunc)dm_get_element,	-1);
	rb_define_method(rb_cTable, "[]=",		(RbFunc)dm_set_element,	-1);
}

/**
 *	@call
 *		Table.new(xsize)					-> table 对象。
 *		Table.new(xsize, ysize)				-> table 对象。
 *		Table.new(xsize, ysize, zsize)		-> table 对象。
 *
 *	@desc
 *		生成 Table 对象。指定多维数组各维的尺寸。能生成 1 ～ 3 维的数组。生成单元数为 0 的数组也是可能的。
 */
VALUE RbTable::initialize(int argc, VALUE *argv, VALUE obj)
{
	VALUE xsize, ysize, zsize;

	rb_scan_args(argc, argv, "12", &xsize, &ysize, &zsize);

	int i;

	for (i = 0; i < argc; ++i)
	{
		SafeFixnumValue(argv[i]);
	}
	
	m_dims = argc;
	m_dim_size[0] = m_dim_size[1] = m_dim_size[2] = 1;
	m_size = 1;

	for (i = 0; i < argc; ++i)
	{
		m_dim_size[i] = SinMax(FIX2INT(argv[i]), 0);
		m_size *= m_dim_size[i];
	}

	if (m_size != 0)
	{
		m_data = (VALUE*)xmalloc(sizeof(VALUE) * m_size);
	
	//	memset(m_data, RUBY_0, sizeof(VALUE) * m_size);	//	single byte set
		for(i = 0; i < m_size; ++i)
			m_data[i] = RUBY_0;
	}

	return obj;
}

VALUE RbTable::_dump(VALUE depth)
{
	//	|dims|dim_size[3]|size|size * sizeof(VALUE)|
	VALUE base = rb_str_new((const char*)&m_dump_data[0], sizeof(m_dump_data));

	return rb_str_buf_cat(base, (const char*)m_data, sizeof(VALUE) * m_size);
}

VALUE RbTable::dm_load(VALUE klass, VALUE str)
{
	int		dump_data[5];

	VALUE	__argv[3];

	long	len = RSTRING_LEN(str);
	
	if (len < sizeof(dump_data))					//	error
		return Qnil;

	memcpy(dump_data, RSTRING_PTR(str), sizeof(dump_data));

	if (len != sizeof(dump_data) + sizeof(VALUE) * dump_data[4])
		return Qnil;

	for (int i = 0; i < dump_data[0]; ++i)
		__argv[i] = INT2FIX(dump_data[i + 1]);

	VALUE obj = rb_class_new_instance(dump_data[0], __argv, klass);

	RbTable* tbl = GetObjectPtr<RbTable>(obj);

	memcpy(tbl->m_data, RSTRING_PTR(str) + sizeof(dump_data), sizeof(VALUE) * dump_data[4]);

	return obj;
}

VALUE RbTable::resize(int argc, VALUE *argv, VALUE obj)
{
	/* Check arguments nums */
	VALUE xsize, ysize, zsize;

	rb_scan_args(argc, argv, "12", &xsize, &ysize, &zsize);

	int i, origin_size;

	/* Check arguments type */
	for (i = 0; i < argc; ++i)
	{
		SafeFixnumValue(argv[i]);
	}

	origin_size = m_size;

	m_dims = argc;
	m_dim_size[0] = m_dim_size[1] = m_dim_size[2] = 1;
	m_size = 1;

	for (i = 0; i < argc; ++i)
	{
		m_dim_size[i] = SinMax(FIX2INT(argv[i]), 0);
		m_size *= m_dim_size[i];
	}

	if (m_size == 0)
	{
		if (m_data)
		{
			xfree(m_data);
			m_data = 0;
		}
	}
	else
	{
		VALUE* tmp = (VALUE*)xmalloc(sizeof(VALUE) * m_size);

		if (origin_size > m_size)
			memcpy(tmp, m_data, sizeof(VALUE) * m_size);
		else
		{
			memcpy(tmp, m_data, sizeof(VALUE) * origin_size);
			for(i = 0; i < m_size - origin_size; ++i)
				tmp[origin_size + i] = RUBY_0;
		}

		xfree(m_data);
		m_data = tmp;
	}

	return Qnil;
}

VALUE RbTable::get_element(int argc, VALUE *argv, VALUE obj)
{
	/* variables declare */
	int i, pos, x, y, z;

	/* Check arguments nums */
	if(m_dims != argc)
		rb_raise(rb_eArgError, "wrong # of arguments (%d for %d)", argc, m_dims);

	/* return Qnil if size is 0 */
	if(m_size == 0)
		return Qnil;

	/* Check arguments validity */
	for(i = 0; i < argc; ++i)
	{
		SafeFixnumValue(argv[i]);
		if(FIX2INT(argv[i]) >= m_dim_size[i])
			return Qnil;
	}

	/* calculate the element pos */
	x = y = z = 0;
	switch(argc)
	{
		case 3:	z = FIX2INT(argv[2]);
		case 2:	y = FIX2INT(argv[1]);
		case 1:	x = FIX2INT(argv[0]);
		default:break;
	}
	pos	= z * m_dim_size[0] * m_dim_size[1] + y * m_dim_size[0] + x;

	return m_data[pos];
}

VALUE RbTable::set_element(int argc, VALUE *argv, VALUE obj)
{
	/* variables declare */
	int i, pos, x, y, z;

	/* Check arguments nums */
	if(m_dims != argc - 1)
		rb_raise(rb_eArgError, "wrong # of arguments (%d for %d)", argc, m_dims + 1);

	/* return Qnil if size is 0 */
	if(m_size == 0)
		return Qnil;

	/* Check arguments validity */
	for(i = 0; i < argc - 1; ++i)
	{
		SafeFixnumValue(argv[i]);
		if(FIX2INT(argv[i]) >= m_dim_size[i])
			return argv[argc - 1];
	}

	/* calculate the element pos */
	x = y = z = 0;
	switch(argc - 1)
	{
		case 3:	z = FIX2INT(argv[2]);
		case 2:	y = FIX2INT(argv[1]);
		case 1:	x = FIX2INT(argv[0]);
		default:break;
	}

	pos	= z * m_dim_size[0] * m_dim_size[1] + y * m_dim_size[0] + x;

	m_data[pos] = argv[argc - 1];
	return m_data[pos];
}

VALUE RbTable::get_xsize()
{
	return INT2FIX(m_dim_size[0]);
}

VALUE RbTable::get_ysize()
{
	return INT2FIX(m_dim_size[1]);
}

VALUE RbTable::get_zsize()
{
	return INT2FIX(m_dim_size[2]);
}

/*
 *	以下定义ruby方法
 */
imp_method_vargs(RbTable, resize)

imp_attr_reader(RbTable, xsize)
imp_attr_reader(RbTable, ysize)
imp_attr_reader(RbTable, zsize)

imp_method_vargs(RbTable, get_element)
imp_method_vargs(RbTable, set_element)