#encoding:utf-8

include SINRGE2

Frame.start_width = 1280
Frame.start_height = 720
Frame.title = "DemoBaCL"
SINRGE2.init_video

class Bulls_And_Cows
  #
  # => 常量
  #
  BLACK = Color.new(0, 0, 0)
  WHITE = Color.new(255, 255, 255)
  UNICODE12 = Font.new("Unicode12", 12, true)
  #
  # => 初始化对像
  #
  def initialize
    @viewport = Viewport.new(0, 0, Graphics.width, Graphics.height)
    @viewport.z = -1
    @viewport.color = WHITE
    # @sprite = Sprite.new
    @bitmap = Bitmap.new(160, 90)
    @bitmap.fill_rect(@bitmap.rect, WHITE)
    # @sprite.bitmap = @bitmap
    # @lfont12 = LFont.new("Unicode12", 12)
    # @lfont12.color = BLACK
    UNICODE12.color = BLACK
    @charmap = Charmap.new(160, 90, 8)
    @charmap.bitmap = Bitmap.new("chars.png")
    restart
  end
  #
  # => 释放
  #
  def dispose
    @bitmap.dispose
    @charmap.dispose
    # @sprite.dispose
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
    @bitmap.fill_rect(@bitmap.rect, WHITE)
    (0..4).each {|i| draw_text(i*40, 2, 40, 12, "0", 1) }
    @bitmap.fill_rect(@index * 40 + 17, 15, 5, 1, BLACK)
    draw_text( 0, 19, 80, 12, "0 A 0 B", 1)
    draw_text(80, 19, 80, 12, "0 Step", 1)
    draw_text( 0, 36, 160, 12, "操作：上下键增减数字")
    draw_text(36, 50, 160, 12, "左右键移动光标")
    draw_text(36, 64, 160, 12, "空格键确定")
    draw_text(36, 78, 160, 12, "ESC键重置")
    @show_help = true
    @dirty = true
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
    x = @step > 4 ? 80 : 0
    y = @step > 4 ? @step - 4 : @step
    y = y * 12 + 24
    str = ""
    @your_answer.each {|n| str += n.to_s }
    draw_text(x, y, 80, 12, str + " #{a}A#{b}B", 1)
  end
  #
  # => 更新
  #
  def update
    restart if Input.trigger?(0x1B)
    return if @game_over
    if Input.trigger?(0x20)
      return if @your_answer.uniq.size < 4
      if @show_help
        @bitmap.fill_rect(0, 36, 160, 54, WHITE)
        @show_help = false
      end
      a, b = check_ab
      @step += 1
      if a == 4
        # msgbox "回答正确"
        @bitmap.fill_rect(  0, 19, 160, 12, WHITE)
        draw_text( 0, 19, 160, 12, "回答正确，按ESC重置", 1)
        str = "答案："
        @true_answer.each {|n| str += n.to_s }
        x = @step > 4 ? 80 : 0
        y = @step > 4 ? @step - 4 : @step
        y = y * 12 + 24
        draw_text(str, x, y, 80, 12, 1)
        @game_over = true
      elsif @step == 8
        # msgbox "游戏结束"
        @bitmap.fill_rect(  0, 19, 160, 12, WHITE)
        draw_text( 0, 19, 160, 12, "游戏结束，按ESC重置", 1)
        str = "答案："
        @true_answer.each {|n| str += n.to_s }
        draw_text(str, 80, 72, 80, 12, 1)
        @game_over = true
      else
        draw_step(a, b)
        @bitmap.fill_rect(  0, 19, 160, 12, WHITE)
        draw_text( 0, 19, 80, 12, "#{a} A #{b} B", 1)
        draw_text(80, 19, 80, 12, "#{@step} Step", 1)
      end
      @dirty = true
    elsif Input.trigger?(0x25)
      @bitmap.fill_rect(@index * 40 + 17, 15, 5, 1, WHITE)
      @index == 0 ? @index = 3 : @index -= 1
      @bitmap.fill_rect(@index * 40 + 17, 15, 5, 1, BLACK)
      @dirty = true
    elsif Input.trigger?(0x27)
      @bitmap.fill_rect(@index * 40 + 17, 15, 5, 1, WHITE)
      @index == 3 ? @index = 0 : @index += 1
      @bitmap.fill_rect(@index * 40 + 17, 15, 5, 1, BLACK)
      @dirty = true
    elsif Input.trigger?(0x26)
      @bitmap.fill_rect(@index * 40, 2, 40, 12, WHITE)
      @your_answer[@index] == 9 ? @your_answer[@index] = 0 : @your_answer[@index] += 1
      draw_text(@index*40, 2, 40, 12, @your_answer[@index].to_s, 1)
      @dirty = true
    elsif Input.trigger?(0x28)
      @bitmap.fill_rect(@index * 40, 2, 40, 12, WHITE)
      @your_answer[@index] == 0 ? @your_answer[@index] = 9 : @your_answer[@index] -= 1
      draw_text(@index*40, 2, 40, 12, @your_answer[@index].to_s, 1)
      @dirty = true
    elsif Input.trigger?(0x10) # 作弊 = =
      str = "正确答案："
      @true_answer.each {|n| str += n.to_s }
      msgbox str
    end
  end
  #
  # => 
  #
  def update_charmap
    if @dirty
      @charmap.reset(@bitmap)
      @dirty = false
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
      self.update_charmap
    end
  end
  #
  # => 
  #
  def draw_text(*args)
    args.insert 0, UNICODE12
    @bitmap.draw_text *args
  end
end

Bulls_And_Cows.new.run
