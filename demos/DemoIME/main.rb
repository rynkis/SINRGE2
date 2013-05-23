#encoding:utf-8
include SINRGE2

Frame.title = "DemoIME"
SINRGE2.init_video

require "__list__"

# ary = ("A".."Z").to_a
# print ary

str = nil

@spt = Sprite.new
@spt.bitmap = Bitmap.new(*Graphics.screen_size)
@spt.bitmap.fill_rect(@spt.bitmap.rect, 0xff900020)

@ime = SINIME.new
@ime.restart

while true
  Graphics.update
  Input.update
  @ime.update
  print @ime.return_texts unless @ime.return_texts(false).empty?
  # str = Input.character
  # puts str if str
end













