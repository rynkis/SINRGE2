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
  dt = time_delta
  @spts.each do |spt|
    spt[0].x += spt[1][0] * dt
    spt[0].y += spt[1][1] * dt
    spt[1][0] = -spt[1][0] if spt[0].x > 800 || spt[0].x < 0
    spt[1][1] = -spt[1][1] if spt[0].y > 600 || spt[0].y < 0
    spt[0].zoom_x += spt[2] * dt
    spt[0].zoom_y = spt[0].zoom_x
    spt[2] = -spt[2] if spt[0].zoom_x > 2.0 || spt[0].zoom_x < 0.5
    spt[0].angle += spt[3] * dt
  end
end

