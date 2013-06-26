/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** Ruby Moudle Audio
*/
#include "MRbAudio.h"
#include "sin_app.h"

#ifndef SIN_USE_SEAL
namespace
{
	static	wchar_t			cmdStr[MAX_PATH];

	int SendString()
	{
		if (mciSendStringW(cmdStr, 0, 0, 0) != 0)
			return 1;
		else return 0;
	}

	int SetAudio(wchar_t * device, wchar_t * flags)
	{
		wsprintfW(cmdStr, L"setaudio %s %s", device, flags);
		return SendString();
	}

	int Cue(wchar_t * device)
	{
		wsprintfW(cmdStr, L"cue %s", device);
		return SendString();
	}

	int Set(wchar_t * device, wchar_t * flags)
	{
		wsprintfW(cmdStr, L"set %s %s", device, flags);
		return SendString();
	}

	wchar_t * Capability(wchar_t * device, wchar_t *flags)
	{
		wchar_t * rt = new wchar_t[MAX_PATH];
		wsprintfW(cmdStr, L"capability %s %s", device, flags);
		mciSendStringW(cmdStr, rt, MAX_PATH, 0);
		return rt;
	}

	int Open(wchar_t * device, wchar_t * filename)
	{
		wsprintfW(cmdStr, L"open \"%s\" type mpegvideo alias %s wait", filename, device);
		return SendString();
	}

	int Play(wchar_t * device, wchar_t * flags)
	{
		if (!wcscmp(Capability(device, L"can play"), L"false"))
			return 1;

		wsprintfW(cmdStr, L"play %s %s", device, flags);
		return SendString();
	}

	int Stop(wchar_t * device)
	{
		wsprintfW(cmdStr, L"stop %s", device);
		return SendString();
	}

	int SetVolume(wchar_t * device, int volume)
	{
		volume = SinBound(volume, 0, 100);
		volume *= 10;
		wchar_t flags[MAX_PATH];
		wsprintfW(flags, L"volume to %d", volume);
		return SetAudio(device, flags);
	}

	int Close(wchar_t * device)
	{
		wsprintfW(cmdStr, L"close %s", device);
		return SendString();
	}
}

VALUE mAudio;

static wchar_t lastBGM[MAX_PATH];
static wchar_t lastBGS[MAX_PATH];
static wchar_t lastME[MAX_PATH];
static wchar_t lastSE[MAX_PATH];

VALUE MRbAudio::bgm_play(int argc, VALUE * argv)
{
	SafeStringValue(argv[0]);
	wchar_t * filename = Kconv::UTF8ToUnicode(RSTRING_PTR(argv[0]));
	if (wcscmp(lastBGM, filename))
	{
		if (lastBGM[0] != 0)
		{
			 if (Close(L"BGM"))
				rb_raise(rb_eSinError, "Failed to close device.");
		}
		wcscpy_s(lastBGM, filename);
		GetAppPtr()->SystemUpdate();
		if (Open(L"BGM", lastBGM))
			rb_raise(rb_eSinError, "Failed to open audio file: `%s'.", Kconv::UnicodeToAnsi(lastBGM));
		GetAppPtr()->SystemUpdate();
		if (Play(L"BGM", L"repeat"))
			rb_raise(rb_eSinError, "Failed to play audio file: `%s'.", Kconv::UnicodeToAnsi(lastBGM));
		GetAppPtr()->SystemUpdate();
	}
	int volume = 100;
	if (!NIL_P(argv[1]))
	{
		SafeFixnumValue(argv[1]);
		volume = FIX2INT(argv[1]);
	}
	if (SetVolume(L"BGM", volume))
		rb_raise(rb_eSinError, "Failed to set volume.");

	return Qnil;
}

VALUE MRbAudio::bgm_stop()
{
	if (Stop(L"BGM"))
		rb_raise(rb_eSinError, "Failed to stop bgm.");

	if (Close(L"BGM"))
		rb_raise(rb_eSinError, "Failed to close device.");

	return Qnil;
}

VALUE MRbAudio::bgm_fade(int argc, VALUE time)
{
	SafeFixnumValue(time);
#pragma message("		Unfinished Function " __FUNCTION__)
	return Qnil;
}

VALUE MRbAudio::bgs_play(int argc, VALUE * argv)
{
	SafeStringValue(argv[0]);
	wchar_t * filename = Kconv::UTF8ToUnicode(RSTRING_PTR(argv[0]));
	if (wcscmp(lastBGS, filename))
	{
		if (lastBGS[0] != 0)
		{
			 if (Close(L"BGS"))
				rb_raise(rb_eSinError, "Failed to close device.");
		}
		wcscpy_s(lastBGS, filename);

		if (Open(L"BGS", lastBGS))
			rb_raise(rb_eSinError, "Failed to open audio file: `%s'.", Kconv::UnicodeToAnsi(lastBGS));

		if (Play(L"BGS", L"repeat"))
			rb_raise(rb_eSinError, "Failed to play audio file: `%s'.", Kconv::UnicodeToAnsi(lastBGS));
	}
	int volume = 100;
	if (!NIL_P(argv[1]))
	{
		SafeFixnumValue(argv[1]);
		volume = FIX2INT(argv[1]);
	}
	if (SetVolume(L"BGS", volume))
		rb_raise(rb_eSinError, "Failed to set volume.");

	return Qnil;
}

VALUE MRbAudio::bgs_stop()
{
	if (Stop(L"BGS"))
		rb_raise(rb_eSinError, "Failed to stop bgs.");
	
	if (Close(L"BGS"))
		rb_raise(rb_eSinError, "Failed to close device.");

	return Qnil;
}

VALUE MRbAudio::bgs_fade(int argc, VALUE time)
{
	SafeFixnumValue(time);
#pragma message("		Unfinished Function " __FUNCTION__)
	return Qnil;
}

VALUE MRbAudio::me_play(int argc, VALUE *argv)
{
	SafeStringValue(argv[0]);
	wchar_t * filename = Kconv::UTF8ToUnicode(RSTRING_PTR(argv[0]));
	if (!wcscmp(lastME, filename))
	{
		wcscpy_s(lastME, filename);

		if (Open(L"ME", lastME))
			rb_raise(rb_eSinError, "Failed to open audio file: `%s'.", lastME);

		if (Play(L"ME", L""))
			rb_raise(rb_eSinError, "Failed to play audio file: `%s'.", lastME);
	}
	int volume = 100;
	if (!NIL_P(argv[1]))
	{
		SafeFixnumValue(argv[1]);
		volume = FIX2INT(argv[1]);
	}
	if (SetVolume(L"ME", volume))
		rb_raise(rb_eSinError, "Failed to set volume.");

	return Qnil;
}

VALUE MRbAudio::me_stop()
{
	if (Stop(L"ME"))
		rb_raise(rb_eSinError, "Failed to stop me.");

	return Qnil;
}

VALUE MRbAudio::me_fade(int argc, VALUE time)
{
	SafeFixnumValue(time);
#pragma message("		Unfinished Function " __FUNCTION__)
	return Qnil;
}

VALUE MRbAudio::se_play(int argc, VALUE *argv)
{
	int volume = 100;
	SafeStringValue(argv[0]);
	if (Open(L"SE", Kconv::UTF8ToUnicode(RSTRING_PTR(argv[0]))))
		rb_raise(rb_eSinError, "Failed to open audio file: `%s'.", Kconv::UTF8ToAnsi(RSTRING_PTR(argv[0])));

	if (Play(L"SE", L""))
		rb_raise(rb_eSinError, "Failed to play audio file: `%s'.", Kconv::UTF8ToAnsi(RSTRING_PTR(argv[0])));

	if (!NIL_P(argv[1]))
	{
		SafeFixnumValue(argv[1]);
		volume = FIX2INT(argv[1]);
	}

	if (SetVolume(L"SE", volume))
		rb_raise(rb_eSinError, "Failed to set volume.");

	return Qnil;
}

VALUE MRbAudio::se_stop()
{
	if (Stop(L"SE"))
		rb_raise(rb_eSinError, "Failed to stop se.");

	return Qnil;
}


void MRbAudio::InitLibrary()
{
	lastBGM[0] = 0;
	lastBGS[0] = 0;
	lastME[0]  = 0;
	lastSE[0]  = 0;

	mAudio = rb_define_module_under(rb_mSin, "Audio");
	
	rb_define_module_function(mAudio, "bgm_play", RbFunc(bgm_play), -1);
	rb_define_module_function(mAudio, "bgm_stop", RbFunc(bgm_stop), 0);
	rb_define_module_function(mAudio, "bgm_fade", RbFunc(bgm_fade), 1);

	rb_define_module_function(mAudio, "bgs_play", RbFunc(bgs_play), -1);
	rb_define_module_function(mAudio, "bgs_stop", RbFunc(bgs_stop), 0);
	rb_define_module_function(mAudio, "bgs_fade", RbFunc(bgs_fade), 1);

	rb_define_module_function(mAudio, "me_play", RbFunc(me_play), -1);
	rb_define_module_function(mAudio, "me_stop", RbFunc(me_stop), 0);
	rb_define_module_function(mAudio, "me_fade", RbFunc(me_fade), 1);

	rb_define_module_function(mAudio, "se_play", RbFunc(se_play), -1);
	rb_define_module_function(mAudio, "se_stop", RbFunc(se_stop), 0);
}

#endif