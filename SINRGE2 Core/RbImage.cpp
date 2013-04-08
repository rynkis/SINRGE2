#include "RbExport.h"
#include "sin_image.h"
#include "Kconv.h"
#include "RbRect.h"

using namespace Sin;

VALUE rb_cImage;

static VALUE image_load(int argc, VALUE filename)
{
	SafeStringValue(filename);

	image_p img = ImageLoad(Kconv::UTF8ToUnicode(RSTRING_PTR(filename)));
	if (img == 0)
		return Qnil;
	else
		return Data_Wrap_Struct(rb_cImage, 0, ImageFree, img);
}

static VALUE image_create(int argc, VALUE width, VALUE height)
{
	SafeFixnumValue(width);
	SafeFixnumValue(height);

	image_p img = ImageCreate(FIX2INT(width), FIX2INT(height));
	if (img == 0)
		return Qnil;
	else
		return Data_Wrap_Struct(rb_cImage, 0, ImageFree, img);
}

static VALUE image_clone(int argc, VALUE img)
{
	SafeImageValue(img);

	image_p pimage, clone;
	Data_Get_Struct(img, image_t, pimage);
	clone = ImageClone(pimage);
	if (clone)
		return Data_Wrap_Struct(rb_cImage, 0, ImageFree, clone);
	else
		return Qnil;
}

static VALUE image_free(int argc, VALUE img)
{
	SafeImageValue(img);

	image_p pimage;
	Data_Get_Struct(img, image_t, pimage);
	ImageFree(pimage);
	return Qnil;
}

static VALUE image_clear(int argc, VALUE img)
{
	SafeImageValue(img);

	image_p pimage;
	Data_Get_Struct(img, image_t, pimage);
	ImageClear(pimage);
	return Qnil;
}

static VALUE image_to_image(int argc, VALUE src, VALUE des, VALUE dx, VALUE dy)
{
	SafeImageValue(src);
	SafeImageValue(des);
	SafeNumericValue(dx);
	SafeNumericValue(dy);

	image_p src_img, des_img;
	Data_Get_Struct(src, image_t, src_img);
	Data_Get_Struct(des, image_t, des_img);
	ImageToImage(src_img, des_img, NUM2INT(dx), NUM2INT(dx));
	return Qnil;
}

static VALUE image_to_image_ex(int argc, VALUE src, VALUE des, VALUE sx, VALUE sy, VALUE sw, VALUE sh, VALUE dx, VALUE dy/*, VALUE flag*/)
{
	SafeImageValue(src);
	SafeImageValue(des);
	SafeNumericValue(sx);
	SafeNumericValue(sy);
	SafeNumericValue(sw);
	SafeNumericValue(sh);
	SafeNumericValue(dx);
	SafeNumericValue(dy);

	image_p src_img, des_img;

	Data_Get_Struct(src, image_t, src_img);
	Data_Get_Struct(des, image_t, des_img);

	ImageToImageEx(src_img, des_img, NUM2INT(sx), NUM2INT(sy),
		NUM2INT(sw), NUM2INT(sh), NUM2INT(dx), NUM2INT(dy)/*, flag == Qtrue*/);

	return Qnil;
}

static VALUE image_fliph(int argc, VALUE img)
{
	SafeImageValue(img);

	image_p pimage;
	Data_Get_Struct(img, image_t, pimage);
	ImageFlipH(pimage);
	return Qnil;
}

static VALUE image_flipv(int argc, VALUE img)
{
	SafeImageValue(img);

	image_p pimage;
	Data_Get_Struct(img, image_t, pimage);
	ImageFlipV(pimage);
	return Qnil;
}

static VALUE image_fillrect(int argc, VALUE img, VALUE x, VALUE y, VALUE width, VALUE height, VALUE color)
{
	SafeImageValue(img);
	SafeNumericValue(x);
	SafeNumericValue(y);
	SafeNumericValue(width);
	SafeNumericValue(height);
	SafeNumericValue(color);

	image_p pimage;
	Data_Get_Struct(img, image_t, pimage);
	ImageFillRect(pimage, NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height), NUM2UINT(color));
	return Qnil;
}

static VALUE image_hue_change(int argc, VALUE img, VALUE hue)
{
	SafeImageValue(img);
	SafeNumericValue(hue);

	image_p pimage;
	Data_Get_Struct(img, image_t, pimage);
	ImageHueChange(pimage, NUM2INT(hue));
	return Qnil;
}

void bind_image()
{
	rb_cImage = rb_define_class_under(rb_mNge, "Image", rb_cObject);
	rb_undef_method(rb_cImage, "new");
	rb_undef_method(rb_cImage, "dup");
	rb_undef_method(rb_cImage, "clone");
	//rb_define_alloc_func(rb_cImage, alloc_img);
	
	rb_define_module_function(rb_mNge, "image_load", RbFunc(image_load), 1);
	rb_define_module_function(rb_mNge, "image_create", RbFunc(image_create), 2);
	rb_define_module_function(rb_mNge, "image_clone", RbFunc(image_clone), 1);
	rb_define_module_function(rb_mNge, "image_free", RbFunc(image_free), 1);
	rb_define_module_function(rb_mNge, "image_clear", RbFunc(image_clear), 1);
	rb_define_module_function(rb_mNge, "image_to_image", RbFunc(image_to_image), 4);
	rb_define_module_function(rb_mNge, "image_to_image_ex", RbFunc(image_to_image_ex), 8);
	rb_define_module_function(rb_mNge, "image_fliph", RbFunc(image_fliph), 1);
	rb_define_module_function(rb_mNge, "image_flipv", RbFunc(image_flipv), 1);
	rb_define_module_function(rb_mNge, "image_fillrect", RbFunc(image_fillrect), 6);
	rb_define_module_function(rb_mNge, "image_hue_change", RbFunc(image_hue_change), 2);
	
}

namespace Sin
{
	void InitRbImage()
	{
		bind_image();
	}
}