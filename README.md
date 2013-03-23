SINRGE2 Ver.Draft
=================
Copyright (C) 2013 Sherry Lynch(or Shy07) <Gernischt@gmail.com>

Sorry, only chinese version readme available now.

关于 SINRGE2
==================

SINRGE2，SINRGE2 Is Not Ruby Game Engine 2 的递归缩写，是由 Sherry Lynch（或称 Shy07）个人开发的一个用于游戏开发的 Ruby 实现。

首先，这是一个 Bug 满满的版本，如果要投入实用的话……抱歉，浪费您的时间了……但如果您同样喜欢 Ruby，并且期待能有一个解决方案可以取代 RGSS，那么请您花点时间阅读下去，并希望您今后能够继续关注这个项目。

Ver.Draft，SINRGE2 的第一个版本，是通过直接把 NGE2 的函数绑定到 Ruby 2.0 来完成的。因此，在调用 NGE2 函数的时候会失去 Ruby 原来那样优雅的感觉，关于这一点真是十分遗憾。当然，这也是把第一个版本称作“Draft”的原因。Draft，即草稿，表示仅是作者为了熟悉用 C++ 扩展 Ruby 的试作型产品。

另外，Ruby 2.0 和 NGE2 本身是支持 Linux 的。不过由于作者认为 Linux 虽然非常适合做开发，但并不是很适合玩游戏。因此作者是在 Windows 下进行开发调试的，SINRGE2的部分代码，例如 Input 部分，只能在 Windows 上运行，这一点也请谅解。当然，若有兴趣，也可自行移植到相应的平台。

Ver.Draft 特性
==================

对比 RGSS 增加了：

* 内部直接支持鼠标/全键盘
* 支持 C 扩展库（socket 等 so 库）
* 自定义分辨率
* 使用 2.0 版本的 Ruby 
* 功能更为强大的音频播放类库 焦尾（Seal）

继承自 NGE2 的特性：

* 支持JPG,BMP(24,32位),PNG,TGA（24,32位）图片读取。
* 支持HZK，GBK点阵字体，FreeType字体显示。
* 支持高效的图片缩放旋转，alpha混色效果。
* 多种显示函数，提供不同的性能。
* 直线，矩形，三角形，圆形等几何图形的绘制。
* 支持HGE的粒子库。
* J2ME的开发者能很容易上手，支持图像裁剪SetClip，绘图函数DrawRegion。
* 提供硬件加速图片到图片绘制image_to_image。

不被支持的 NGE2 特性：

* 支持16位（RGBA5650 RGBA5551 RGBA4444）/32位(RGBA_8888)显示。
* 图片读取后转化到上述显示模式。
* 动画支持，用于播放动画，支持gif。
* 音频支持，用于播放声音，mp3，wav，可扩展。

使用 SINRGE2
==================

首先请自行编译 SINRGE2，项目工程推荐使用 VS2010 打开。然后在 SINRGE2.exe 所在目录新建一个 mian.rb 文件。当然你也可以新建一个 ini 文件，并可以配置以下选项。（ini 文件名必须与 SINRGE2.exe 相同）

```ini
[Setting]
Library=SINRGE2.dll's path
Scripts=your ruby scripts file's path
```
之后编写你喜欢的 Ruby 代码，运行 SINRGE2.exe 即可。

未来的计划
==================

Ver.Draft 是为了向 NGE2 致敬而保留了 NGE2 的函数调用方式，SINRGE2 的下一个版本将会完全放弃现在的代码，重新在 NGE2 的基础上进行抽象封装，争取实现接近 RGSS，但更为自由的 Ruby 类库。

同时，因为作者个人能力不足，所以也在此邀请喜爱 Ruby 并愿意为其开发扩展的同好加入。
相信有你们的加入，SINRGE2 将会变得更好用。

