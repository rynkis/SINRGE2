#encoding:utf-8

include SINRGE2

Frame.title = "DemoZazaka"
SINRGE2.init_video


@pln = Plane.new
@pln.bitmap = Bitmap.new("bg2")

@bmp = Bitmap.new("zazaka")
@spts = []

a5 = [
  Color.new(0, 0, 0, 0),
  Color.new(196, 128, 64, 128),
  Color.new(96, 128, 196, 128),
  Color.new(128, 196, 96, 128),
  Color.new(196, 96, 128, 128)
]

1000.times do |i|
  spt = Sprite.new
  spt.bitmap = @bmp
  spt.x = rand(800)
  spt.y = rand(600)
  spt.ox = 32
  spt.oy = 32
  spt.angle = rand(360)
  spt.zoom_x = rand(0.5..2.0)
  spt.zoom_y = spt.zoom_x
  spt.color = a5[rand(a5.size)]
  spt.z = i + 100

  dpos = [rand(-200..200), rand(-200..200)]
  dzoom = rand(-1.0..1.0)
  dagl = rand(-360..360)

  @spts << [spt, dpos, dzoom, dagl]
end

while true
  Graphics.update
  Input.update
  dt = timer_delta
  @spts.each do |spt|
    spt[0].x += spt[1][0] * dt
    spt[0].y += spt[1][1] * dt
    spt[1][0] = -spt[1][0] if spt[0].x > 800 || spt[0].x < 0
    spt[1][1] = -spt[1][1] if spt[0].y > 600 || spt[0].y < 0
    spt[0].zoom_x += spt[2] * dt
    spt[0].zoom_y = spt[0].zoom_x
    spt[2] = -spt[2] if spt[0].zoom_x > 2.0 || spt[0].zoom_x < 0.5
    spt[0].angle += Integer(spt[3] * dt)
  end
end












__END__
begin

  bigall = Bitmap.new("Graphics/par/particles.png")
  
  @pars = []
  map = [[96,32],[96,96],[0,0],[32,32],[64,64],[0,0],[32,0],[96,96],[0,32]]
  (0..8).each do |i|
      par = ParticleSystem.new("Graphics/par/particle#{i+1}.psi")
      
      btm01 = Bitmap.new(32,32)
      btm01.blt(0,0, bigall, Rect.new(map[i][0], map[i][1], 32, 32))
      
      par.bitmap = btm01
      
      par.fire_at(i % 3 * 256 + 128, i / 3 * 200 + 100)
      @pars << par
  end

  vp = Viewport.new(0, 0, 800, 600)
  # vp.color = Color.new(255, 0, 0, 128)

  spt = Sprite.new(vp)
  spt.bitmap = Bitmap.new(32, 32)
  spt.bitmap.blt(0,0, bigall, Rect.new(0, 32, 32, 32))
  spt.ox = 16
  spt.oy = 16
  spt.zoom_x = 3.2
  spt.zoom_y = 3.2
  spt.x = 400
  spt.y = 300

  while true
    Graphics.update
    Input.update
    
    @pars[4].move_to *Input.mouse_pos
    @pars.each{|par| par.update}

    break if Input.trigger?(13)
    
  end

rescue
  msgbox $!
end












spt = Sprite.new
spt.bitmap = Bitmap.new("Graphics/1.png")
# spt.ox = spt.width / 2
# spt.oy = spt.height / 2
# spt.x = Graphics.width / 2
# spt.y = Graphics.height / 2
p t1 =  Time.now
spt.bitmap.radial_blur(45, 3)
# spt.bitmap.blur
p t2 = Time.now


game_stop
