#include "RbNgeImage.h"
#include "RbColor.h"
#include "RbRect.h"
#include "RbFont.h"
#include "RbTone.h"

VALUE rb_cImage;

RbNgeImage::RbNgeImage()
	: m_disposed(true)
	, m_modify_count(0)
	, m_filename(Qnil)
	, m_rect_ptr(0)
	, m_font_ptr(0)
{
}

RbNgeImage::~RbNgeImage()
{
	image_free(m_image);
	//m_image.Dispose();
}

void RbNgeImage::InitLibrary()
{
	/**
	 *	@classname
	 *		Image
	 *
	 *	@desc
	 *		位图的类。所谓位图即表示图像其本身。
	 */
	rb_cImage = rb_define_class_under(rb_mSin, "Image", rb_cObject);
	
	// special method
	rb_define_alloc_func(rb_cImage, ObjAllocate<RbNgeImage>);
	rb_define_method(rb_cImage, "initialize",			(RbFunc)dm_initialize,			-1);

	// instance method
	rb_define_method(rb_cImage, "dispose",				(RbFunc)dm_dispose,				0);
	rb_define_method(rb_cImage, "disposed?",			(RbFunc)dm_is_disposed,			0);
	rb_define_method(rb_cImage, "save_to_file",			(RbFunc)dm_save_to_file,		1);	//	snap_to_image

	rb_define_method(rb_cImage, "hue_change",			(RbFunc)dm_hue_change,			1);
	rb_define_method(rb_cImage, "brightness_change",	(RbFunc)dm_brightness_change,	1);
	rb_define_method(rb_cImage, "tone_change",			(RbFunc)dm_tone_change,			-1);

	rb_define_method(rb_cImage, "blt",					(RbFunc)dm_blt,					-1);
	rb_define_method(rb_cImage, "stretch_blt",			(RbFunc)dm_stretch_blt,			-1);
	rb_define_method(rb_cImage, "fill_rect",			(RbFunc)dm_fill_rect,			-1);
	rb_define_method(rb_cImage, "clear",				(RbFunc)dm_clear,				0);
	rb_define_method(rb_cImage, "get_pixel",			(RbFunc)dm_get_pixel,			2);
	rb_define_method(rb_cImage, "set_pixel",			(RbFunc)dm_set_pixel,			3);

	rb_define_method(rb_cImage, "draw_text",			(RbFunc)dm_draw_text,			-1);
	rb_define_method(rb_cImage, "text_size",			(RbFunc)dm_text_size,			1);

	rb_define_method(rb_cImage, "gradient_fill_rect",	(RbFunc)dm_gradient_fill_rect,	-1);
	rb_define_method(rb_cImage, "clear_rect",			(RbFunc)dm_clear_rect,			-1);
	rb_define_method(rb_cImage, "blur",					(RbFunc)dm_blur ,				0);
	rb_define_method(rb_cImage, "radial_blur",			(RbFunc)dm_radial_blur,			2);
		
	// object attribute
	rb_define_method(rb_cImage, "width",				(RbFunc)dm_get_width,			0);
	rb_define_method(rb_cImage, "height",				(RbFunc)dm_get_height,			0);
	rb_define_method(rb_cImage, "rect",					(RbFunc)dm_get_rect,			0);
	rb_define_method(rb_cImage, "filename",				(RbFunc)dm_get_filename,		0);

	rb_define_method(rb_cImage, "font",					(RbFunc)dm_get_font,			0);
	rb_define_method(rb_cImage, "font=",				(RbFunc)dm_set_font,			1);

	// supplement
 	rb_define_method(rb_cImage, "to_s",					(RbFunc)dm_to_string,			0);
}

void RbNgeImage::mark()
{
	if (m_rect_ptr)	m_rect_ptr->MarkObject();
	if (m_font_ptr)	m_font_ptr->MarkObject();

	rb_gc_mark(m_filename);
}

/**
 *	@call
 *		Image.new(filename[, colorkey])		-> image 对象。
 *		Image.new(width, height)			-> image 对象。
 *
 *	@desc
 *		读取 filename 指定的图像文件生成 Image 对象，可以指定透明颜色。或直接生成指定尺寸的位图对象。
 *
 *	@excp
 *		如果指定的文件加载失败会抛出RGEError异常。
 */
VALUE RbNgeImage::initialize(int argc, VALUE *argv, VALUE obj)
{
	VALUE arg01, arg02;

	int dwColorValue = 0;

	if (rb_scan_args(argc, argv, "11", &arg01, &arg02) == 1)
	{
		SafeStringValue(arg01);
		goto __image_load;
	}
	else
	{
		if (TYPE(arg01) == T_STRING)
		{
			SafeColorValue(arg02);
			{
				RbColor* ccol;
				ccol = GetObjectPtr<RbColor>(arg02);
				dwColorValue = ccol->GetColor();
			}
			goto __image_load;
		}
		else goto __image_create;
	}

__image_load:
	{
		m_image = image_load_colorkey(Kconv::UTF8ToAnsi(RSTRING_PTR(arg01)), DISPLAY_PIXEL_FORMAT_8888, dwColorValue, 1);
	}
	goto __finish;

__image_create:
	{
		SafeFixnumValue(arg01);
		SafeFixnumValue(arg02);

		u32 w = FIX2INT(arg01);
		u32 h = FIX2INT(arg02);

		m_image = image_create(w, h, DISPLAY_PIXEL_FORMAT_8888);
		if (m_image == 0)
			rb_raise(rb_eSINBaseError, "Failed to create image with `%d x %d' !", w, h);
	}

__finish:
	{
		//	create rect
		VALUE __argv[] = {RUBY_0, RUBY_0, INT2FIX(m_image->texw), INT2FIX(m_image->texh)};

		VALUE rect = rb_class_new_instance(4, __argv, rb_cRect);
		m_rect_ptr = GetObjectPtr<RbRect>(rect);

		// create a font object for the image-obj
		VALUE font = rb_class_new_instance(0, 0, rb_cFont);
		m_font_ptr = GetObjectPtr<RbFont>(font);

		m_disposed = false;
	}
	return obj;
}

void RbNgeImage::check_raise()
{
	if (m_disposed)
		rb_raise(rb_eSINDisposedObjectError, "disposed image");
}

VALUE RbNgeImage::dispose()
{
	if (m_disposed)
		return Qnil;
	
	image_free(m_image);

	m_disposed = true;

	return Qnil;
}

VALUE RbNgeImage::is_disposed()
{
	return C2RbBool(m_disposed);
}

VALUE RbNgeImage::hue_change(VALUE hue)
{
#pragma message("		RbNgeImage Unfinished Class Function: " __FUNCTION__)

	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

VALUE RbNgeImage::brightness_change(VALUE brightness)
{
#pragma message("		RbNgeImage Unfinished Class Function: " __FUNCTION__)

	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

/**
 *	@call
 *		tone_change([red[, green[, blue[, gray]]]])					-> nil
 *		tone_change(tone)											-> nil
 *
 *	@desc
 *		更改位图的色调。red|green|blue|gray 省略后的默认值都为255。
 *
 *	@excp
 *		如果对已经释放的对象进行调用会抛出DisposedObjectError异常。
 */
VALUE RbNgeImage::tone_change(int argc, VALUE *argv, VALUE obj)
{
#pragma message("		RbNgeImage Unfinished Class Function: " __FUNCTION__)

	return Qnil;
}

VALUE RbNgeImage::blt(int argc, VALUE *argv, VALUE obj)
{
#pragma message("		RbNgeImage Unfinished Class Function: " __FUNCTION__)

	return Qnil;
}

VALUE RbNgeImage::stretch_blt(int argc, VALUE *argv, VALUE obj)
{
#pragma message("		RbNgeImage Unfinished Class Function: " __FUNCTION__)

	return Qnil;
}

VALUE RbNgeImage::fill_rect(int argc, VALUE *argv, VALUE obj)
{
#pragma message("		RbNgeImage Unfinished Class Function: " __FUNCTION__)

	return Qnil;
}

VALUE RbNgeImage::clear()
{
#pragma message("		RbNgeImage Unfinished Class Function: " __FUNCTION__)

	return Qnil;
}

VALUE RbNgeImage::get_pixel(VALUE x, VALUE y)
{
#pragma message("		RbNgeImage Unfinished Class Function: " __FUNCTION__)

	return Qnil;
}

VALUE RbNgeImage::set_pixel(VALUE x, VALUE y, VALUE color)
{
#pragma message("		RbNgeImage Unfinished Class Function: " __FUNCTION__)

	return Qnil;
}

VALUE RbNgeImage::draw_text(int argc, VALUE *argv, VALUE obj)
{
	check_raise();

	VALUE rect;
	VALUE vx, vy, vw, vh, vstr, algin;
	image_p tempImg;

	int dx, dy, width, height;
	char* txt;
	int	len;
	
	if (argc > 3)
	{
		rb_scan_args(argc, argv, "51", &vx, &vy, &vw, &vh, &vstr, &algin);
		
		SafeFixnumValue(vx);
		SafeFixnumValue(vy);
		SafeFixnumValue(vw);
		SafeFixnumValue(vh);
		SafeStringValue(vstr);
	}
	else
	{
		rb_scan_args(argc, argv, "21", &rect, &vstr, &algin);

		SafeRectValue(rect);
		SafeStringValue(vstr);
		
		dx = GetObjectPtr<RbRect>(rect)->x;
		dy = GetObjectPtr<RbRect>(rect)->y;
		width = GetObjectPtr<RbRect>(rect)->width;
		height = GetObjectPtr<RbRect>(rect)->height;
	}
	
	txt = RSTRING_PTR(vstr);
	len = RSTRING_LEN(vstr);
	int tw, th, tb = 0;
	font_textsize(m_font_ptr->m_hFont, txt, len, &tw, &th, &tb);

	if (TYPE(algin) == T_FIXNUM)
	{
		SafeFixnumValue(algin);
		switch (NUM2INT(algin))
		{
		case 1:
			tw = (width - tw) / 2;
			break;
		case 2:
			tw = width - tw;
			break;
		}
	}

	tempImg = image_create(width, height, DISPLAY_PIXEL_FORMAT_8888);
	MessageBoxW(NULL, L"ok1", L"ok", 0);
	font_drawtext(m_font_ptr->m_hFont, txt, len, tempImg, tw, 0, m_font_ptr->m_shadow == Qtrue ? 1 : 0);
	MessageBoxW(NULL, L"ok2", L"ok", 0);
	BeginTarget(m_image, 0);
	MessageBoxW(NULL, L"ok3", L"ok", 0);
	ImageToScreen(tempImg, (float)dx, (float)dy);
	MessageBoxW(NULL, L"ok4", L"ok", 0);
	EndTarget();
	MessageBoxW(NULL, L"ok5", L"ok", 0);
	image_free(tempImg);
	MessageBoxW(NULL, L"ok6", L"ok", 0);

	return Qnil;
}

VALUE RbNgeImage::text_size(VALUE str)
{
	int width, height, base;

	VALUE vstr = NIL_P(str) ? rb_str_new2("nil") : rb_obj_as_string(str);
	char* cstr = RSTRING_PTR(vstr);
	font_textsize(m_font_ptr->m_hFont, cstr, strlen(cstr), &width, &height, &base);

	VALUE __argv[] = {RUBY_0, RUBY_0, INT2FIX(width), INT2FIX(height)};
	VALUE rect = rb_class_new_instance(4, __argv, rb_cRect);

	return rect;
}

VALUE RbNgeImage::gradient_fill_rect(int argc, VALUE *argv, VALUE obj)
{
#pragma message("		RbNgeImage Unfinished Class Function: " __FUNCTION__)

	return Qnil;
}

VALUE RbNgeImage::clear_rect(int argc, VALUE *argv, VALUE obj)
{
#pragma message("		RbNgeImage Unfinished Class Function: " __FUNCTION__)

	return Qnil;
}

VALUE RbNgeImage::blur()
{
#pragma message("		RbNgeImage Unfinished Class Function: " __FUNCTION__)

	return Qnil;
}

VALUE RbNgeImage::radial_blur(VALUE angle, VALUE division)
{
#pragma message("		RbNgeImage Unfinished Class Function: " __FUNCTION__)

	return Qnil;
}

VALUE RbNgeImage::get_rect()
{
	check_raise();

	return ReturnObject(m_rect_ptr);
}

VALUE RbNgeImage::get_width()
{
	check_raise();

	return INT2FIX(GetImgWidth());
}

VALUE RbNgeImage::get_height()
{
	check_raise();

	return INT2FIX(GetImgHeight());
}

VALUE RbNgeImage::get_filename()
{
	check_raise();

	return m_filename;
}

VALUE RbNgeImage::get_font()
{
	check_raise();

	return ReturnObject(m_font_ptr);
}

VALUE RbNgeImage::set_font(VALUE font)
{
	check_raise();

	SafeFontValue(font);
	m_font_ptr = GetObjectPtr<RbFont>(font);
	return Qnil;
}

VALUE RbNgeImage::save_to_file(VALUE str)
{
	check_raise();

	SafeStringValue(str);

	image_save(m_image, Kconv::UTF8ToAnsi(RSTRING_PTR(str)), 1, 1);

	return Qnil;
}

imp_method(RbNgeImage, dispose)
imp_method(RbNgeImage, is_disposed)
imp_method01(RbNgeImage, save_to_file)
imp_method01(RbNgeImage, hue_change)
imp_method01(RbNgeImage, brightness_change)
imp_method_vargs(RbNgeImage, tone_change)

imp_method_vargs(RbNgeImage, blt)
imp_method_vargs(RbNgeImage, stretch_blt)
imp_method_vargs(RbNgeImage, fill_rect)
imp_method(RbNgeImage, clear)
imp_method02(RbNgeImage, get_pixel)
imp_method03(RbNgeImage, set_pixel)
imp_method_vargs(RbNgeImage, draw_text)
imp_method01(RbNgeImage, text_size)

imp_method_vargs(RbNgeImage, gradient_fill_rect)
imp_method_vargs(RbNgeImage, clear_rect)
imp_method(RbNgeImage, blur)
imp_method02(RbNgeImage, radial_blur)

imp_attr_reader(RbNgeImage, rect)
imp_attr_reader(RbNgeImage, width)
imp_attr_reader(RbNgeImage, height)
imp_attr_reader(RbNgeImage, filename)
imp_attr_accessor(RbNgeImage, font)