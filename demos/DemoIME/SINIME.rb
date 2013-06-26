#encoding:utf-8
#
#
#
class SINIME < Sprite
  #
  # => 常量
  #
  include MsgAndKeyConstant
  HALF_SYMBOL = [
    "~" , "`" , "!" , "@" , "#" , "$" , "%%", "^" , "&" , "*" , "(" , ")" ,
    "_" , "-" , "+" , "=" , "{" , "[" , "}" , "]" , "|" , "\\", ":" , ";" ,
    "\"", "'" , "<" , "," , ">" , "." , "?" , "/"
  ]
  FULL_SYMBOL = [
    "～", "·" , "！", "@" , "#" , "￥", "%%", "……", "&" , "×" , "（", "）",
    "——", "-" , "+" , "=" , "『", "【", "』", "】", "|" , "、", "：", "；",
    "“" , "‘" , "《", "，", "》", "。", "？", "/"
  ]
  FULL_SYMBOL_EX = [ "”", "’" ]
  #
  # => 类变量
  #
  @@background = Bitmap.new(328, 52)
  @@background.fill_rect(@@background.rect, Color.new(255, 255, 255))
  @@SIMHEI20 = Font.new("simhei", 20)
  #
  # => 初始化对象
  #
  def initialize(viewport = nil)
    super(viewport)
    self.x = Graphics.width - @@background.width
    self.y = Graphics.height - @@background.height
    self.z = 100
    self.bitmap = @@background
    self.visible = false
    @active = false
    @letter_rect = Rect.new(6, 2, 201, 24)
    @hanzi_rect = Rect.new(6, 26, 240, 24)
    @ime_rect = Rect.new(207, 2,120, 24)
    @mode_rect = Rect.new(246, 26, 80, 24)
    @return_text = ""
    @in_dquot = false
    @in_squot = false
    create_contents
    clear
    @e_mode = true
    enorcn
  end
  #
  # => contents
  #
  def contents
    @contents.bitmap
  end
  #
  # => active
  #
  def active
    @active
  end
  #
  # => active=
  #
  def active=(value)
    @active = value
    self.visible = value
    @contents.visible = value
  end
  #
  # => 重新输入
  #
  def restart
    clear
    self.active = true
  end
  #
  # => 清除变量
  #
  def clear
    @temp_text1  = ""
    @temp_text2  = ""
    @temp_pinyin = []
    @temp_hanzi  = []
    @page = 0
    contents.clear_rect(@letter_rect)
    contents.fill_rect(6, 2, 1, 20, Color.new(128, 128, 128))
    contents.clear_rect(@hanzi_rect)
    # @return_text = ""
  end
  #
  # => 释放
  #
  def dispose
    @contents.dispose
    super
  end
  #
  # => 生成窗口内容
  #
  def create_contents
    @contents = Sprite.new(self.viewport)
    @contents.bitmap = self.bitmap.clone
    contents.clear
    # contents.font.size  = 20
    # contents.font.shadow = false
    # contents.font.color = Color.new(0, 0, 255)
    # draw_text(@ime_rect, "简易输入法", 2)
    # contents.font.color = Color.new(0, 0, 0)
    @@SIMHEI20.color = Color.new(0, 0, 255)
    draw_text(@ime_rect, "简易输入法", 2)
    @@SIMHEI20.color = Color.new(0, 0, 0)
    @contents.x = self.x
    @contents.y = self.y
    @contents.z = self.z + 1
    @contents.visible = false
  end
  #
  #
  #
  def draw_text(*args)
    args.insert 0, @@SIMHEI20
    contents.draw_text *args
  end
  #
  #
  #
  def text_size(str)
    contents.text_size(@@SIMHEI20, str)
  end
  #
  # => 更新画面
  #
  def update
    super
    update_input if @active
  end
  #
  # => 更新输入
  #
  def update_input
    if @e_mode
      if Input.trigger?(VK_LCONTROL)
        enorcn
      elsif Input.trigger?(VK_SPACE)
        @return_text = " "
      elsif Input.trigger?(VK_RETURN)
        @return_text = "\n"
      else
        c = Input.character
        @return_text = c if c != nil
      end
    else
      if Input.trigger?(VK_SPACE)
        if @temp_hanzi[@page * 5] != nil
          @return_text = @temp_hanzi[@page * 5]
        else
          @return_text = @temp_text1.empty? ? " " : @temp_text1
        end
        clear
        return
      end
      if Input.trigger?(VK_RETURN)
        if @temp_text1.empty?
          @return_text = "\n"
        else
          @return_text = @temp_text1
          clear
        end
        return
      end
      if Input.trigger?(VK_OEM_MINUS)
        c = Input.character
        return check_return_text(c) if @temp_text1.empty?
        return if @page == 0
        @page -= 1
        refresh_hanzi
        return
      end
      if Input.trigger?(VK_OEM_PLUS)
        c = Input.character
        return check_return_text(c) if @temp_text1.empty?
        if @page + 1 < @temp_hanzi.size / 5.0
          @page += 1
          refresh_hanzi
        end
        return
      end
      5.times do |i|
        if Input.trigger?(VK_1 + i)
          c = Input.character
          return check_return_text(c) if @temp_text1.empty?
          if @temp_hanzi[@page * 5 + i]
            @return_text = @temp_hanzi[@page * 5 + i]
            clear
          end
          return
        end
      end
      if Input.trigger?(VK_LCONTROL)
        enorcn
      elsif Input.trigger?(VK_BACK)
        @temp_text1 = ""
        @temp_pinyin.pop
        @temp_pinyin.each {|s| @temp_text1 += s }
        @page = 0
        update_hanzi_group
      else
        c = Input.character
        return if check_return_text(c)
        if c != nil && @temp_pinyin.size < 20
          @temp_text1 += c
          @temp_pinyin << c
          @page = 0
          update_hanzi_group
        end
      end
    end
  end
  #
  # => 刷新字母区
  #
  def refresh_letter
    contents.clear_rect(@letter_rect)
    draw_text(@letter_rect, @temp_text1)
    rect = text_size(@temp_text1)
    contents.fill_rect(rect.width + 6, 4, 1, 20, Color.new(128, 128, 128))
  end
  #
  # => 刷新汉字区
  #
  def refresh_hanzi
    @temp_text2.clear
    5.times do |i|
      id = i + @page * 5
      break if @temp_hanzi[id].nil?
      @temp_text2 += "#{i + 1}.#{@temp_hanzi[id]} "
    end
    refresh_letter
    contents.clear_rect(@hanzi_rect)
    draw_text(@hanzi_rect, @temp_text2)
  end
  #
  # => 更新汉字组
  #
  def update_hanzi_group
    @temp_hanzi = []
    id = Pinyin.index(@temp_pinyin[0])
    if id.nil?
      @temp_hanzi = []
    else
      Pinyin[id].size.times do |group_index|
        if @temp_pinyin == Pinyin[id][group_index]
          @temp_hanzi = Hanzi[id][group_index]
          break
        end
      end
    end
    refresh_hanzi
  end
  #
  # => 中英切换
  #
  def enorcn
    @e_mode = !@e_mode
    str = @e_mode ? "英" : "中"
    contents.clear_rect(@mode_rect)
    # contents.font.color = Color.new(0, 0, 255)
    @@SIMHEI20.color = Color.new(0, 0, 255)
    draw_text(@mode_rect, str, 2)
    # contents.font.color = Color.new(0, 0, 0)
    @@SIMHEI20.color = Color.new(0, 0, 0)
    clear
  end
  #
  # => 输入中？
  #
  def inputting?
    @temp_text1.empty?
  end
  #
  # => 返回文本
  #
  def return_texts(remove = true)
    str = @return_text
    @return_text = "" if remove
    return str
  end
  #
  # => 检查返回文本
  #
  def check_return_text(c)
    if HALF_SYMBOL.include?(c)
      id = HALF_SYMBOL.index(c)
      if c == "\""
        if @in_dquot
          @return_text = FULL_SYMBOL_EX[0]
        else
          @return_text = FULL_SYMBOL[id]
        end
        @in_dquot = !@in_dquot
        return true
      elsif c == "'"
        if @in_squot
          @return_text = FULL_SYMBOL_EX[1]
        else
          @return_text = FULL_SYMBOL[id]
        end
        @in_squot = !@in_squot
        return true
      else
        @return_text = id ? c : FULL_SYMBOL[id]
        return true
      end
    end
    return false
  end
end
