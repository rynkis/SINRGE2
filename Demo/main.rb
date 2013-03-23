#encoding:utf-8

#	修改 print 和 p 方法，可以用来输出 log
#~ module Kernel
  #~ alias o_print print
  #~ def print(*args)
    #~ args.each {|a|
      #~ open("log.txt", "a") {|f| f.write a.to_s}
      #~ o_print a
    #~ }
  #~ end
	
  #~ alias o_p p
  #~ def p(*args)
    #~ args.each {|a|
      #~ open("log.txt", "a") {|f| f.write a.to_s}
      #~ o_p a
    #~ }
  #~ end
#~ end

=begin
有什么类、方法可以用下面的方法找，写文档好累的= =
=end

#~ Object.constants.sort.each {|s|
  #~ print s#"#{s.to_s}\t=>\t#{eval(s.to_s)}"
  #~ print "\n"
#~ }
#~ print "===========================================\n"
#~ Seal.constants.sort.each {|s|
#~ print s
#~ print "\n"
#~ }
#~ print "===========================================\n"
#~ Seal.methods.sort.each {|s|
#~ print s
#~ print "\n"
#~ }

begin

	include Sin
	#~ require "Win32API"
	#~ PeekMessage      = Win32API.new("user32", "PeekMessage", "pllll", "i")
	#~ TranslateMessage = Win32API.new("user32", "TranslateMessage", "p", "l")
	#~ DispatchMessage  = Win32API.new("user32", "DispatchMessage", "p", "l")
	
	print "===========================================================\n"
	[:RUBY_COPYRIGHT,:RUBY_DESCRIPTION].each {|v|print "#{eval v.to_s}\n"}
	print "===========================================================\n\n"
	
	print "#{Time.now} Hello, 这是 #{RUBY_ENGINE} #{RUBY_VERSION} 哦！\n"
	
	print "#{Time.now} require socket.so => "
	p require("socket.so")
	print "#{Time.now} require win32ole.so => "
	p require("win32ole.so")
	print "#{Time.now} require dl.so => "
	p require("dl.so")
	
	print "#{Time.now} 设置窗口标题\n"
	NGE::Frame.title = "SINRGE2 Ver. Draft"
	#~ NGE::Frame.width = 1280
	#~ NGE::Frame.height = 720
	
	#	NGE 本身没支持全屏，so 这只一个接口……
	#~ NGE::Frame.fullscreen_start = true
	
	print "#{Time.now} 初始化 NGE2 模块\n"
	NGE.init
	Sin::Input.show_mouse(false)
	
	#~ print "#{Time.now} #{Dir.pwd}\n"

	##	Audio.rb 是为了提供 RGSS 的播放方式，目前还没完成
	##	所以还是得照着 Seal 的方法用
	#~ require "Audio.rb"
	#~ Audio.bgm_play("Audio/19 luminous sword.ogg", 80, 100)
	
	print "#{Time.now} 初始化 Seal 模块\n"
	Seal.startup("")
	
	print "#{Time.now} 创建 Source\n"
	@bgm = Seal::Source.new
	@se = Seal::Source.new
	
	print "#{Time.now} 打开音频文件流\n"
	@bgm.stream = Seal::Stream.new("Audio/Last Whisper (SLOS Arrange).ogg")
	#~ @bgm.buffer = Seal::Buffer.new("Audio/Last Whisper (SLOS Arrange).ogg")
	@bgm.looping = true
	@se.buffer = Seal::Buffer.new("Audio/se.ogg")
	#~ @se.gain = 150.0
	
	print "#{Time.now} 开始播放音频文件\n"
	@bgm.play
	
	print "#{Time.now} 开始加载图片文件\n"
	@img1 = NGE.image_load("Graphics/中文/测试pic.bmp")	# <= 测试中文path
	
	demo = ""
	open("Graphics/demo0.jpg", "rb") {|f| demo = f.read}
	@img2 = NGE.image_load_buf(demo, demo.size)#NGE.image_clone(@img1)
	
	NGE.image_fliph(@img1)
	NGE.image_flipv(@img2)
	
	color = 0xff0000ff
	NGE.image_fillrect(@img2, 0, 0, 64, 64, color)
	
	@img3 = NGE.image_create(800, 600)
	@font1 = NGE.create_font_freetype("Fonts/microhei.ttf", 14)
	NGE.font_setcolor(@font1, 0xffffffff)
	
	@str = "执着于老朽的文字诗篇；痴迷程序代码和主机游戏；"
	NGE.font_drawtext(@font1, @str, @str.length * 3, @img3, 32, 32, 0)
	@str = "爱世界，爱人民，不爱国；不坦率直白，造作即造文。"
	NGE.font_drawtext(@font1, @str, @str.length * 3, @img3, 32, 64, 0)
	@str = "I'm Sherry Lynch, I'm Shy07."
	NGE.font_drawtext(@font1, @str, @str.length, @img3, 32, 96, 0)
	
	#~ NGE.image_fillrect(@img3, 0, 0, 64, 128, color)
	#~ NGE.image_to_image_alpha(@img3, @img1, 0, 0, 128)
	
	@img4 = NGE.image_load("Graphics/icon256.png")
	@img5 = NGE.image_load("Graphics/demo2.tga")
	
	print "#{Time.now} 开始生成粒子系统\n"
	@pt1 = NGE.create_particle
	@ptimg = NGE.image_load("Graphics/par/particles.png")
	NGE.particle_set_image(@pt1, @ptimg)
	NGE.particle_set_clip(@pt1, 0, 0, 32, 32)
	NGE.particle_set_center(@pt1, 16, 16)
	
	@pt2 = NGE.create_particle
	NGE.particle_set_image(@pt2, @ptimg)
	NGE.particle_set_clip(@pt2, 96, 96, 128, 128)
	NGE.particle_set_center(@pt2, 16, 16)
	
	@pt4 = NGE.create_particle
	NGE.particle_set_image(@pt4, @ptimg)
	NGE.particle_set_clip(@pt4, 32, 32, 64, 64)
	NGE.particle_set_center(@pt4, 16, 16)
	
	@pt5 = NGE.create_particle
	NGE.particle_set_image(@pt5, @ptimg)
	NGE.particle_set_clip(@pt5, 64, 64, 96, 96)
	NGE.particle_set_center(@pt5, 16, 16)
	
	@pt7 = NGE.create_particle
	NGE.particle_set_image(@pt7, @ptimg)
	NGE.particle_set_clip(@pt7, 32, 0, 64, 32)
	NGE.particle_set_center(@pt7, 16, 16)
	
	@pt8 = NGE.create_particle
	NGE.particle_set_image(@pt8, @ptimg)
	NGE.particle_set_clip(@pt8, 0, 32, 32, 64)
	NGE.particle_set_center(@pt8, 16, 16)
	
	@ptsys1 = NGE.create_particle_system("Graphics/par/particle1.psi", @pt1)
	NGE.particle_system_fire_at(@ptsys1, 128, 128)
	@ptsys2 = NGE.create_particle_system("Graphics/par/particle2.psi", @pt2)
	NGE.particle_system_fire_at(@ptsys2, 400, 128)
	@ptsys3 = NGE.create_particle_system("Graphics/par/particle3.psi", @pt1)
	NGE.particle_system_fire_at(@ptsys3, 672, 128)
	@ptsys4 = NGE.create_particle_system("Graphics/par/particle4.psi", @pt4)
	NGE.particle_system_fire_at(@ptsys4, 128, 256)
	@ptsys5 = NGE.create_particle_system("Graphics/par/particle5.psi", @pt5)
	NGE.particle_system_fire_at(@ptsys5, 400, 256)
	@ptsys6 = NGE.create_particle_system("Graphics/par/particle6.psi", @pt1)
	NGE.particle_system_fire_at(@ptsys6, 672, 256)
	@ptsys7 = NGE.create_particle_system("Graphics/par/particle7.psi", @pt7)
	NGE.particle_system_fire_at(@ptsys7, 128, 472)
	@ptsys8 = NGE.create_particle_system("Graphics/par/particle8.psi", @pt2)
	NGE.particle_system_fire_at(@ptsys8, 400, 472)
	@ptsys9 = NGE.create_particle_system("Graphics/par/particle9.psi", @pt8)
	NGE.particle_system_fire_at(@ptsys9, 672, 472)
	
	@timer = NGE.create_timer
	NGE.timer_start(@timer)
	mlast = 0
	
	@colors = [0xff0000ff, 0xffff0000, 0xff00ff00, 0xff000000]
	angle = 0
	
	print "#{Time.now} 主循环\n"
	while true #@bgm.state == Seal::Source::State::PLAYING
		unless NGE::Frame.peek_message
			#~ NGE.screen_shot("snap.png")
			#~ NGE.image_save(NGE.screen_to_image, "snap.png", true, true)
			NGE.quit
			#~ Seal.cleanup
			exit
			break
		end
		NGE.limit_fps
		NGE.show_fps
		
		NGE.begin_scene
		NGE.image_to_screen(@img1, 0, 0)
		NGE.fill_polygon([200, 300, 400, 300], [10, 10, 70, 70], 4, 0xff8000ff)
		NGE.draw_circle(500, 64, 32, 0xff808080)
		NGE.render_quad(@img5, 0, 0, 0, 0, 160, 164, 1, 1, 360-angle, 0xc0ffffff)
		NGE.render_quad(@img2, 0, 0, 0, 0, 160, 164, 1, 1, angle, 0xc0ffffff)
		NGE.image_to_screen(@img4, 272, 172)
		NGE.fill_rect_grad(128, 32, 128, 64, @colors)
		NGE.fill_circle(564, 64, 32, 0xff808080)
		NGE.draw_polygon([200, 300, 400], [80, 80, 160], 3, 0xff00ff80)
		NGE.image_to_screen(@img3, 0, 0)
		NGE.particle_system_render(@ptsys1)
		NGE.particle_system_render(@ptsys2)
		NGE.particle_system_render(@ptsys3)
		NGE.particle_system_render(@ptsys4)
		NGE.particle_system_render(@ptsys5)
		NGE.particle_system_render(@ptsys6)
		NGE.particle_system_render(@ptsys7)
		NGE.particle_system_render(@ptsys8)
		NGE.particle_system_render(@ptsys9)
		NGE.end_scene
		
		
		Input.update
		
		angle += 0.5
		angle = 0 if angle >= 360.0
		
		dt = (NGE.get_ticks(@timer) - mlast) / 1000.0
		mlast = NGE.get_ticks(@timer)
		NGE.particle_system_update(@ptsys1, dt)
		NGE.particle_system_update(@ptsys2, dt)
		NGE.particle_system_update(@ptsys3, dt)
		NGE.particle_system_update(@ptsys4, dt)
		NGE.particle_system_update(@ptsys5, dt)
		NGE.particle_system_update(@ptsys6, dt)
		NGE.particle_system_update(@ptsys7, dt)
		NGE.particle_system_update(@ptsys8, dt)
		NGE.particle_system_update(@ptsys9, dt)
		pos = Input.mouse_pos
		NGE.particle_system_move_to(@ptsys5, pos[0], pos[1], false)
		
		@se.play if Input.click?(0x01) && Input.mouse_over?
	end
	
	print "#{Time.now} 出错了= =\n"
	loop { a = 1; sleep 1}
rescue
  p $!
	loop { a = 1; sleep 1}
end