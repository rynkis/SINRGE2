#encoding:utf-8

include SINRGE2

Frame.start_width = 1280
Frame.start_height = 720
Frame.title = "DemoBaCL"
SINRGE2.init_video

class SINRGE2::LFont
  #
  # => cache_clear
  #
  def cache_clear
    if @cache
      @cache.each_value {|bmp| bmp.dispose }
      @cache.clear
    end
  end
  #
  # => dispose
  #
  alias demo_origin_dispose dispose
  def dispose
    cache_clear
    demo_origin_dispose
  end
  #
  # => char_bitmap
  #
  alias demo_origin_char_bitmap char_bitmap
  def char_bitmap(str)
    @cache ||= {}
    char = str.slice(0, 1)
    if @cache[char]
      @cache[char] = demo_origin_char_bitmap(char) if @cache[char].disposed?
    else
      @cache[char] = demo_origin_char_bitmap(char)
    end
    @cache[char]
  end
end

class Bulls_And_Cows
  #
  # => 常量
  #
  BLACK = Color.new(0, 0, 0)
  WHITE = Color.new(255, 255, 255)
  CHAR = "MNHQ$OC?7>!;-:. "
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
    @lfont12 = LFont.new("Unicode12", 12)
    @lfont12.color = BLACK
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
    (0..4).each {|i| draw_text("0", x:i*40, y:2, width:40, height:12, halign:1) }
    @bitmap.fill_rect(@index * 40 + 17, 15, 5, 1, BLACK)
    draw_text("0 A 0 B", x:0, y:19, width:80, height:12, halign:1)
    draw_text("0 Step", x:80, y:19, width:80, height:12, halign:1)
    draw_text("操作：上下键增减数字", x:0, y:36, width:160, height:12)
    draw_text("左右键移动光标", x:36, y:50, width:160, height:12)
    draw_text("空格键确定", x:36, y:64, width:160, height:12)
    draw_text("ESC键重置", x:36, y:78, width:160, height:12)
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
    draw_text(str + " #{a}A#{b}B", x:x, y:y, width:80, height:12, halign:1)
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
        draw_text("回答正确，按ESC重置", x:0, y:19, width:160, height:12, halign:1)
        str = "答案："
        @true_answer.each {|n| str += n.to_s }
        x = @step > 4 ? 80 : 0
        y = @step > 4 ? @step - 4 : @step
        y = y * 12 + 24
        draw_text(str, x:x, y:y, width:80, height:12, halign:1)
        @game_over = true
      elsif @step == 8
        # msgbox "游戏结束"
        @bitmap.fill_rect(  0, 19, 160, 12, WHITE)
        draw_text("游戏结束，按ESC重置", x:0, y:19, width:160, height:12, halign:1)
        str = "答案："
        @true_answer.each {|n| str += n.to_s }
        draw_text(str, x:80, y:72, width:80, height:12, halign:1)
        @game_over = true
      else
        draw_step(a, b)
        @bitmap.fill_rect(  0, 19, 160, 12, WHITE)
        draw_text("#{a} A #{b} B", x:0, y:19, width:80, height:12, halign:1)
        draw_text("#{@step} Step", x:80, y:19, width:80, height:12, halign:1)
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
      draw_text(@your_answer[@index].to_s, x:@index*40, y:2, width:40, height:12, halign:1)
      @dirty = true
    elsif Input.trigger?(0x28)
      @bitmap.fill_rect(@index * 40, 2, 40, 12, WHITE)
      @your_answer[@index] == 0 ? @your_answer[@index] = 9 : @your_answer[@index] -= 1
      draw_text(@your_answer[@index].to_s, x:@index*40, y:2, width:40, height:12, halign:1)
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
  def draw_text(str, x: nil, y: nil, width: nil, height: nil, rect: nil, halign: 0, valign: 0)
    if rect
      x = rect.x
      y = rect.y
      width = rect.width
      height = rect.height
    end
    texts = str.clone
    if halign == 1 || halign == 2
      tw = @lfont12.text_width(texts)
      if width - tw > 0
        x += (width - tw) / (3 - halign)
      end
    end
    if height - @lfont12.size > 0 && valign != 0
      y += (height - @lfont12.size) / (3 - valign)
    end
    while !texts.empty?
      text = texts.slice!(0, 1)
      bmp = @lfont12.char_bitmap(text)
      @bitmap.blt(x, y, bmp, bmp.rect)
      x += @lfont12.text_width(text)
    end
  end
end

Bulls_And_Cows.new.run
