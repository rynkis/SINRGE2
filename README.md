SINRGE2 Ver. YAR
=================
Copyright (C) 2013 Sherry Lynch(or Shy07) <Gernischt@gmail.com>

Sorry, only chinese version readme available now.

关于 SINRGE2
==================

SINRGE2，SINRGE2 Is Not Ruby Game Engine 2 的递归缩写，是由 Sherry Lynch（或称 Shy07）个人开发的一个用于游戏开发的 Ruby 实现。

这是SINRGE2的第二个版本，目前正在开发中。

这个版本放弃了NGE2的代码，转用HGE作为基础。所以，从这个版本开始意味着SINRGE2只能运行在Windows上。  
考虑到作者对NGE2的函数颇为熟悉，因此这个版本保留了NGE2的部分函数。并进行了适当改进。  
例如：image_to_image，现在直接支持像素Alpha混合，因此放弃了image_to_image_alpha这个函数。  
当然，今后也会添加适当的函数支持NGE2原有的功能。  

依赖
==================

Ruby 2.0.0 p0 http://www.ruby-lang.org/en/  
--附加依赖：zlib

HGE 1.81 http://hge.relishgames.com/doc/index.html  

Seal 0.1.2 「焦尾」 https://github.com/zhangsu/seal  
--附加依赖：libmpg123、libogg、 libvorbis、OpenAL32


