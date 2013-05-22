#encoding:utf-8
include SINRGE2

Frame.title = "DemoLFont"
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

buffer = open("Unicode12", "rb") {|f| f.read }
buffer = Zlib::Inflate.inflate(buffer)
@lfont12 = LFont.new(buffer, 12, true)
@lfont12.color = Color.new(144, 0, 32)

@lfont14 = LFont.new("Unicode14", 14)
@lfont14.color = Color.new(32, 64, 128)

@lfont16 = LFont.new("Unicode16", 16)

@spt = Sprite.new
@spt.bitmap = Bitmap.new(800, 600)

def draw_lfont_text(lfont, str, x: nil, y: nil, width: nil, height: nil, rect: nil, halign: 0, valign: 1)
  if rect
    x = rect.x
    y = rect.y
    width = rect.width
    height = rect.height
  end
  texts = str.clone
  if halign == 1 || halign == 2
    tw = lfont.text_width(texts)
    if width - tw > 0
      x += (width - tw) / (3 - halign)
    end
  end
  if height - lfont.size > 0 && valign != 0
    y += (height - lfont.size) / (3 - valign)
  end
  while !texts.empty?
    text = texts.slice!(0, 1)
    bmp = lfont.char_bitmap(text)
    @spt.bitmap.blt(x, y, bmp, bmp.rect)
    x += lfont.text_width(text)
  end
end

texts = "16size：执着于老朽的文字诗篇；痴迷程序代码和主机游戏；"
draw_lfont_text(@lfont16, texts, x: 32, y: 32, width: 800, height: 20)
texts = "14size：爱世界，爱人民，不爱国；不坦率直白，造作即造文。"
draw_lfont_text(@lfont14, texts, x: 32, y: 52, width: 800, height: 20)
texts = "12size：I'm Sherry Lynch, I'm Shy07."
draw_lfont_text(@lfont12, texts, rect: Rect.new(32, 72, 800, 20))

SINRGE2.stop













