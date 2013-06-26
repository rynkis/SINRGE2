#encoding:utf-8

include SINRGE2

Frame.start_width = 640
Frame.start_height = 480
Frame.title = "DemoBaC"
SINRGE2.init_video

class Bulls_And_Cows
  #
  # => 常量
  #
  BLACK = Color.new(0, 0, 0, 128)
  SIMHEI24 = Font.new("simhei", 24)
  #
  # => 初始化对像
  #
  def initialize
    @sprite = Sprite.new
    @bitmap = Bitmap.new(640, 480)
    # @bitmap.font.size = 24
    # @bitmap.font.shadow = false
    @bitmap.fill_rect(@bitmap.rect, BLACK)
    @sprite.bitmap = @bitmap
    restart
  end
  #
  # => 释放
  #
  def dispose
    @bitmap.dispose
    @sprite.dispose
  end
  #
  # => 重新开始
  #
  def restart
    loop do
      @true_answer = [rand(10), rand(10), rand(10), rand(10)]
      @true_answer.uniq!
      break if @true_answer.size == 4
    end
    @your_answer = [0, 0, 0, 0]
    @step  = 0
    @index = 0
    @game_over = false
    @bitmap.fill_rect(@bitmap.rect, BLACK)
    draw_text(0, 0, 640, 24, "上下键增减数字，左右键移动光标，空格键确定, ESC键重置", 1)
    (0..4).each {|i| draw_text(i * 160,  64, 160, 24, 0.to_s, 1)}
    draw_text(@index * 160, 90, 160, 24, "-", 1)
    draw_text(  0, 128, 320, 24, "0 A 0 B", 1)
    draw_text(320, 128, 320, 24, "0 Step", 1)
  end
  #
  # => 检索AB
  #
  def check_ab
    a = b = 0
    (0..3).each do |i|
      if @your_answer[i] == @true_answer[i]
        a += 1
      elsif @true_answer.include?(@your_answer[i])
        b += 1
      end
    end
    return a, b
  end
  #
  # => 绘制步骤
  #
  def draw_step(a, b)
    x = @step <= 4 ? 0 : 320
    y = @step <= 4 ? @step : @step - 4
    str = ""
    @your_answer.each {|n| str += n.to_s }
    draw_text(x, y * 24 + 240, 320, 24, str + " #{a}A#{b}B", 1)
  end
  #
  #
  #
  def draw_text(*args)
    args.insert 0, SIMHEI24
    @bitmap.draw_text *args
  end
  #
  # => 更新
  #
  def update
    restart if Input.trigger?(0x1B)
    return if @game_over
    if Input.trigger?(0x20)
      return if @your_answer.uniq.size < 4
      a, b = check_ab
      @step += 1
      if a == 4
        msgbox "回答正确\n"
        draw_text(0, 184, 640, 24, "回答正确", 1)
        str = "正确答案："
        @true_answer.each {|n| str += n.to_s }
        x = @step <= 4 ? 0 : 320
        y = @step <= 4 ? @step : @step-4
        draw_text(x, y * 24 + 240, 320, 24, str, 1)
        @game_over = true
      elsif @step == 8
        msgbox "游戏结束\n"
        draw_text(0, 184, 640, 24, "游戏结束", 1)
        str = "正确答案："
        @true_answer.each {|n| str += n.to_s }
        draw_text(320, (@step-4) * 24 + 240, 320, 24, str, 1)
        @game_over = true
      else
        draw_step(a, b)
        @bitmap.fill_rect(  0, 128, 640, 24, BLACK)
        draw_text(  0, 128, 320, 24, "#{a} A #{b} B", 1)
        draw_text(320, 128, 320, 24, "#{@step} Step", 1)
      end
    elsif Input.trigger?(0x25)
      @bitmap.fill_rect(@index * 160, 90, 160, 24, BLACK)
      @index == 0 ? @index = 3 : @index -= 1
      draw_text(@index * 160, 90, 160, 24, "-", 1)
    elsif Input.trigger?(0x27)
      @bitmap.fill_rect(@index * 160, 90, 160, 24, BLACK)
      @index == 3 ? @index = 0 : @index += 1
      draw_text(@index * 160, 90, 160, 24, "-", 1)
    elsif Input.trigger?(0x26)
      @bitmap.fill_rect((@index) * 160, 64, 160, 24, BLACK)
      @your_answer[@index] == 9 ? @your_answer[@index] = 0 : @your_answer[@index] += 1
      draw_text(@index * 160, 64, 160, 24, @your_answer[@index].to_s, 1)
    elsif Input.trigger?(0x28)
      @bitmap.fill_rect((@index) * 160, 64, 160, 24, BLACK)
      @your_answer[@index] == 0 ? @your_answer[@index] = 9 : @your_answer[@index] -= 1
      draw_text(@index * 160, 64, 160, 24, @your_answer[@index].to_s, 1)
    elsif Input.trigger?(0x10) # 作弊 = =
      str = "正确答案："
      @true_answer.each {|n| str += n.to_s }
      msgbox str
    end
  end
  #
  # => 运行游戏
  #
  def run
    loop do
      Graphics.update
      Input.update
      self.update
    end
  end
end

Bulls_And_Cows.new.run
