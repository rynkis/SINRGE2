#encoding:utf-8
include SINRGE2

Frame.title = "DemoLFont"
SINRGE2.init_video

buffer = open("Unicode12", "rb") {|f| f.read }
buffer = Zlib::Inflate.inflate(buffer)
@lfont12 = LFont.new(buffer, 12, true)
@lfont12.color = Color.new(144, 0, 32)

buffer = open("Unicode14", "rb") {|f| f.read }
buffer = Zlib::Inflate.inflate(buffer)
@lfont14 = LFont.new(buffer, 14, true)
@lfont14.color = Color.new(32, 64, 128)

buffer = open("Unicode16", "rb") {|f| f.read }
buffer = Zlib::Inflate.inflate(buffer)
@lfont16 = LFont.new(buffer, 16, true)

@spt = Sprite.new
@spt.bitmap = Bitmap.new(800, 600)

def draw_lfont_text(lfont, texts, x: nil, y: nil, width: nil, height: nil, rect: nil, halign: 0, valign: 1)
  if rect
    x = rect.x
    y = rect.y
    width = rect.width
    height = rect.height
  end

  if halign == 1 || halign == 2
    temp = texts.clone
    tw = 0
    while !temp.empty?
      text = temp.slice!(0, 1)
      tw += lfont.text_size(text)
    end
    if width - tw > 0
      x += (width - tw) / (3 - halign)
    end
  end
  if height - lfont.size > 0 && valign != 0
    y += (height - lfont.size) / (3 - valign)
  end
  while !texts.empty?
    text = texts.slice!(0, 1)
    bmp = lfont.font_bitmap(text)
    @spt.bitmap.blt(x, y, bmp, bmp.rect)
    x += lfont.text_size(text)
    bmp.dispose
  end
end

texts = "16size：执着于老朽的文字诗篇；痴迷程序代码和主机游戏；"
draw_lfont_text(@lfont16, texts, x: 32, y: 32, width: 800, height: 20)
texts = "14size：爱世界，爱人民，不爱国；不坦率直白，造作即造文。"
draw_lfont_text(@lfont14, texts, x: 32, y: 52, width: 800, height: 20)
texts = "12size：I'm Sherry Lynch, I'm Shy07."
draw_lfont_text(@lfont12, texts, rect: Rect.new(32, 72, 800, 20))

game_stop













