#encoding:utf-8
include SINRGE2

Frame.title = "DemoLFont"
SINRGE2.init_video

UNICODE12 = Font.new("Unicode12", 12, true)
UNICODE12.color = Color.new(144, 0, 32)

UNICODE14 = Font.new("Unicode14", 14, true)
UNICODE14.color = Color.new(32, 64, 128)

UNICODE16 = Font.new("Unicode16", 16, true)

@spt = Sprite.new
@spt.bitmap = Bitmap.new(800, 600)

texts = "16size：执着于老朽的文字诗篇；痴迷程序代码和主机游戏；"
@spt.bitmap.draw_text(UNICODE16, 32, 32, 800, 20, texts)
texts = "14size：爱世界，爱人民，不爱国；不坦率直白，造作即造文。"
@spt.bitmap.draw_text(UNICODE14, 32, 52, 800, 20, texts)
texts = "12size：I'm Sherry Lynch, I'm Shy07."
@spt.bitmap.draw_text(UNICODE12, Rect.new(32, 72, 800, 20), texts)

SINRGE2.stop













