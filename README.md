﻿# SINRGE2
Copyright (c) Lynch HSU (or Shy07) <Gernischt@gmail.com>

## About SINRGE2
SINRGE2 is the recursive acronym of "SINRGE2 Is Not Ruby Game Engine 2".  
As the name, SINRGE2 has no game engine tools such as map editor,
scripts editor, etc. SINRGE2 is just a library for Ruby programming
language.  
If you have experience with RGSS what is the ruby game scripts system
of RPG Maker XP or later. Then SINRGE2 would be very easy to use.  

## How to use
### Prepare
To use SINRGE2, you may compile SINRGE2 from source code or download
the binary files first. And edit the .ini file to tell SINRGE2.dll
some necessary parameters.
```Runme.ini
[setting]
Library=SINRGE2.dll # the path of SINRGE2.dll
ExtFunc=SealEx.dll  # the path of extension library and it could be empty  
Scripts=main.rb     # the path of main script file
Console=0           # show cmd window or not
                    # only '1' can open the console and any other value would close the console
```
### Basic Use

Initialize SINRGE2:

```ruby
SINRGE2.init_video # only initialize graphics moudle
#SINRGE2.init      # initialize both graphics and seal moudle
                   # premise is you have specified the ExtFunc in .ini
include SINRGE2
```

Hello world:  

You can print "Hello world" in cmd window by  
```ruby
print "Hello world"
```
or  
```ruby
p "Hello world"
puts "Hello world"
```
as usual.  

And in SINRGE2 you can print it on screen. This is a sample:  
```ruby
@spt = Sprite.new # create a sprite
@spt.bitmap = Bitmap.new(128, 32) # create a 128 * 32 bitmap
@spt.bitmap.draw_text(0, 0, 128, 32, "Hello world") # draw text
SINRGE2.stop # stop SINRGE2 with looping Graphics#update
```
You can get the same effect in RGSS too.  

You can find more info from [here](http://shy07.com/SINRGE2/index.html).  

## Platforms

Windows Only.

