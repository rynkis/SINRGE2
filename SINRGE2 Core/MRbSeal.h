/*
** SINRGE2
** Copyright (C) 2013 Su Zhang, Shy07
** Gernischt@gmail.com
**
** Ruby Moudle Seal
*/
#ifndef _MODULE_RB_SEAL_H_
#define _MODULE_RB_SEAL_H_

#include "RbClassBase.h"
#include "sin_seal_ex.h"

namespace
{
	func_seal_startup						seal_startup;
	func_seal_cleanup						seal_cleanup;
	func_seal_get_per_src_effect_limit		seal_get_per_src_effect_limit;
	func_seal_get_version					seal_get_version;

	func_seal_init_src						seal_init_src;
	func_seal_destroy_src					seal_destroy_src;
	func_seal_play_src						seal_play_src;
	func_seal_pause_src						seal_pause_src;
	func_seal_stop_src						seal_stop_src;
	func_seal_rewind_src					seal_rewind_src;
	func_seal_set_src_buf					seal_set_src_buf;
	func_seal_set_src_stream				seal_set_src_stream;
	func_seal_update_src					seal_update_src;
	func_seal_detach_src_audio				seal_detach_src_audio;
	func_seal_set_src_queue_size			seal_set_src_queue_size;
	func_seal_set_src_chunk_size			seal_set_src_chunk_size;
	func_seal_set_src_pos					seal_set_src_pos;
	func_seal_set_src_vel					seal_set_src_vel;
	func_seal_set_src_pitch					seal_set_src_pitch;
	func_seal_set_src_gain					seal_set_src_gain;
	func_seal_set_src_auto					seal_set_src_auto;
	func_seal_set_src_relative				seal_set_src_relative;
	func_seal_set_src_looping				seal_set_src_looping;
	func_seal_get_src_queue_size			seal_get_src_queue_size;
	func_seal_get_src_chunk_size			seal_get_src_chunk_size;
	func_seal_get_src_buf					seal_get_src_buf;
	func_seal_get_src_stream				seal_get_src_stream;
	func_seal_get_src_pos					seal_get_src_pos;
	func_seal_get_src_vel					seal_get_src_vel;
	func_seal_get_src_pitch					seal_get_src_pitch;
	func_seal_get_src_gain					seal_get_src_gain;
	func_seal_is_src_auto					seal_is_src_auto;
	func_seal_is_src_relative				seal_is_src_relative;
	func_seal_is_src_looping				seal_is_src_looping;
	func_seal_get_src_type					seal_get_src_type;
	func_seal_get_src_state					seal_get_src_state;

	func_seal_set_listener_pos				seal_set_listener_pos;
	func_seal_set_listener_gain				seal_set_listener_gain;
	func_seal_set_listener_vel				seal_set_listener_vel;
	func_seal_set_listener_orien			seal_set_listener_orien;
	func_seal_get_listener_pos				seal_get_listener_pos;
	func_seal_get_listener_gain				seal_get_listener_gain;
	func_seal_get_listener_vel				seal_get_listener_vel;
	func_seal_get_listener_orien			seal_get_listener_orien;

	func_seal_init_buf						seal_init_buf;
	func_seal_destroy_buf					seal_destroy_buf;
	func_seal_load2buf						seal_load2buf;
	func_seal_get_buf_size					seal_get_buf_size;
	func_seal_get_buf_freq					seal_get_buf_freq;
	func_seal_get_buf_bps					seal_get_buf_bps;
	func_seal_get_buf_nchannels				seal_get_buf_nchannels;

	func_seal_open_stream					seal_open_stream;
	func_seal_rewind_stream					seal_rewind_stream;
	func_seal_close_stream					seal_close_stream;

	func_seal_init_rvb						seal_init_rvb;
	func_seal_destroy_rvb					seal_destroy_rvb;
	func_seal_load_rvb						seal_load_rvb;
	func_seal_set_rvb_density				seal_set_rvb_density;
	func_seal_set_rvb_diffusion				seal_set_rvb_diffusion;
	func_seal_set_rvb_gain					seal_set_rvb_gain;
	func_seal_set_rvb_hfgain				seal_set_rvb_hfgain;
	func_seal_set_rvb_decay_time			seal_set_rvb_decay_time;
	func_seal_set_rvb_hfdecay_ratio			seal_set_rvb_hfdecay_ratio;
	func_seal_set_rvb_reflections_gain		seal_set_rvb_reflections_gain;
	func_seal_set_rvb_reflections_delay		seal_set_rvb_reflections_delay;
	func_seal_set_rvb_late_gain				seal_set_rvb_late_gain;
	func_seal_set_rvb_late_delay			seal_set_rvb_late_delay;
	func_seal_set_rvb_air_absorbtion_hfgain	seal_set_rvb_air_absorbtion_hfgain;
	func_seal_set_rvb_room_rolloff_factor	seal_set_rvb_room_rolloff_factor;
	func_seal_set_rvb_hfdecay_limited		seal_set_rvb_hfdecay_limited;
	func_seal_get_rvb_density				seal_get_rvb_density;
	func_seal_get_rvb_diffusion				seal_get_rvb_diffusion;
	func_seal_get_rvb_gain					seal_get_rvb_gain;
	func_seal_get_rvb_hfgain				seal_get_rvb_hfgain;
	func_seal_get_rvb_decay_time			seal_get_rvb_decay_time;
	func_seal_get_rvb_hfdecay_ratio			seal_get_rvb_hfdecay_ratio;
	func_seal_get_rvb_reflections_gain		seal_get_rvb_reflections_gain;
	func_seal_get_rvb_reflections_delay		seal_get_rvb_reflections_delay;
	func_seal_get_rvb_late_gain				seal_get_rvb_late_gain;
	func_seal_get_rvb_late_delay			seal_get_rvb_late_delay;
	func_seal_get_rvb_air_absorbtion_hfgain	seal_get_rvb_air_absorbtion_hfgain;
	func_seal_get_rvb_room_rolloff_factor	seal_get_rvb_room_rolloff_factor;
	func_seal_is_rvb_hfdecay_limited		seal_is_rvb_hfdecay_limited;

	func_seal_init_efs						seal_init_efs;
	func_seal_destroy_efs					seal_destroy_efs;
	func_seal_set_efs_effect				seal_set_efs_effect;
	func_seal_feed_efs						seal_feed_efs;
	func_seal_set_efs_gain					seal_set_efs_gain;
	func_seal_set_efs_auto					seal_set_efs_auto;
	func_seal_get_efs_effect				seal_get_efs_effect;
	func_seal_get_efs_gain					seal_get_efs_gain;

	func_seal_is_efs_auto					seal_is_efs_auto;
	func_seal_get_err_msg					seal_get_err_msg;
	func_seal_sleep							_seal_sleep;

	func_seal_move_listener					seal_move_listener;
	func_seal_move_src						seal_move_src;
}

struct MRbSeal
{
	static bool	InitLibrary();
};

#endif //_MODULE_RB_SEAL_H_