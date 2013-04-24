SINRGE2 Ver. YAR
======
Copyright (C) 2013 Sherry Lynch(or Shy07) <Gernischt@gmail.com>

Sorry, only chinese version readme available now.

关于 SINRGE2
=====

SINRGE2，SINRGE2 Is Not Ruby Game Engine 2 的递归缩写，是由 Sherry Lynch（或称 Shy07）个人开发的一个用于游戏开发的 Ruby 实现。当然，也离不开 RGE2 开发团队的贡献。

之所以会开发这个项目，是因为 RGE2 的开发长期停滞，发布无望，所以试图自己动手，丰衣足食。  

初版，Ver. Draft 是采用 NGE2 作为图像底层，但是 Windows 版本一直不稳定，在官方论坛咨询也不得其果。因此，马不停蹄开始了第二版本的开发。

第二版，Ver. YAR 是在研究了 FET、RGE2 等多个项目后开始制作的。YAR 以 HGE 作为图像底层，使用了大量的 RGE2 代码，可以说是对 RGE2 进行补完或分支版本开发。YAR，即 Yet Another RGE2。

YAR，原本计划实现 NGE2 的函数。但是考虑到 Ruby 彻底面向对象的特性，使用 NGE2 的函数来开发会显得很奇怪。因此，个人决定放弃。如果有人曾对此作期待的话，那么个人在此向您道歉。


YAR 的特性
=====



依赖
=====

Ruby 2.0.0 p0 https://github.com/ruby/ruby  
--附加依赖：zlib

HGE 1.81 https://github.com/kvakvs/hge

Seal 0.1.2 「焦尾」 https://github.com/zhangsu/seal  
--附加依赖：libmpg123、libogg、 libvorbis、OpenAL32


