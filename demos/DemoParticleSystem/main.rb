#encoding:utf-8

include SINRGE2

Frame.title = "DemoParticleSystem"
SINRGE2.init_video

begin

  bigall = Bitmap.new("par/particles.png")
  
  @pars = []
  map = [[96,32],[96,96],[0,0],[32,32],[64,64],[0,0],[32,0],[96,96],[0,32]]
  (0..8).each do |i|
    par = ParticleSystem.new("par/particle#{i+1}.psi")
      
    btm01 = Bitmap.new(32,32)
    btm01.blt(0,0, bigall, Rect.new(map[i][0], map[i][1], 32, 32))
      
    par.bitmap = btm01
      
    par.fire_at(i % 3 * 256 + 128, i / 3 * 200 + 100)
    @pars << par
  end

  while true
    Graphics.update
    Input.update
    
    @pars[4].move_to *Input.mouse_pos
    @pars.each{|par| par.update }

    break if Input.trigger?(13)
    
    if Input.click? 0x01
      @pars[4].stop
    elsif Input.click? 0x02
      @pars[4].stop(true)
    elsif Input.click? 0x04
      @pars[4].fire
    end

  end

rescue
  msgbox $!
end

