#encoding: utf-8
#
#
# 
module MsgAndKeyConstant

  # 消息
  WM_SETFOCUS       = 0x0007
  WM_KILLFOCUS      = 0x0008
  WM_ENABLE         = 0x000A
  WM_SETREDRAW      = 0x000B
  WM_SETTEXT        = 0x000C
  WM_GETTEXT        = 0x000D
  WM_GETTEXTLENGTH  = 0x000E
  WM_PAINT          = 0x000F
  WM_CLOSE          = 0x0010
  WM_SETCURSOR      = 0x0020

  WM_SYSKEYDOWN     = 0x0104
  WM_SYSKEYUP       = 0x0105
  WM_KEYDOWN        = 0x0100
  WM_KEYUP          = 0x0101
    
  WM_MOUSEMOVE      = 0x0200
  WM_LBUTTONDOWN    = 0x0201
  WM_LBUTTONUP      = 0x0202
  WM_LBUTTONDBLCLK  = 0x0203
  WM_RBUTTONDOWN    = 0x0204
  WM_RBUTTONUP      = 0x0205
  WM_RBUTTONDBLCLK  = 0x0206
  WM_MBUTTONDOWN    = 0x0207
  WM_MBUTTONUP      = 0x0208
  WM_MBUTTONDBLCLK  = 0x0209    
  WM_MOUSEWHEEL     = 0x020A
    
  # 虚拟键
  VK_LBUTTON        = 0x01
  VK_RBUTTON        = 0x02
  VK_MBUTTON        = 0x04

  VK_BACK           = 0x08
  VK_TAB            = 0x09
  VK_RETURN         = 0x0D
  VK_ENTER          = VK_RETURN
  VK_SHIFT          = 0x10
  VK_CONTROL        = 0x11
  VK_MENU           = 0x12

  VK_ESCAPE         = 0x1B
  VK_SPACE          = 0x20

  VK_LEFT           = 0x25
  VK_UP             = 0x26
  VK_RIGHT          = 0x27
  VK_DOWN           = 0x28
    
  VK_LWIN           = 0x5B
  VK_RWIN           = 0x5C
    
  VK_NUMPAD0        = 0x60
  VK_NUMPAD1        = 0x61
  VK_NUMPAD2        = 0x62
  VK_NUMPAD3        = 0x63
  VK_NUMPAD4        = 0x64
  VK_NUMPAD5        = 0x65
  VK_NUMPAD6        = 0x66
  VK_NUMPAD7        = 0x67
  VK_NUMPAD8        = 0x68
  VK_NUMPAD9        = 0x69
    
  VK_F1             = 0x70
  VK_F2             = 0x71
  VK_F3             = 0x72
  VK_F4             = 0x73
  VK_F5             = 0x74
  VK_F6             = 0x75
  VK_F7             = 0x76
  VK_F8             = 0x77
  VK_F9             = 0x78
  VK_F10            = 0x79
  VK_F11            = 0x7A
  VK_F12            = 0x7B
    
  VK_LSHIFT         = 0xA0
  VK_RSHIFT         = 0xA1
  VK_LCONTROL       = 0xA2
  VK_RCONTROL       = 0xA3
  VK_LMENU          = 0xA4
  VK_RMENU          = 0xA5
    
  VK_0              = 0x30
  VK_1              = 0x31
  VK_2              = 0x32
  VK_3              = 0x33
  VK_4              = 0x34
  VK_5              = 0x35
  VK_6              = 0x36
  VK_7              = 0x37
  VK_8              = 0x38
  VK_9              = 0x39
  VK_A              = 0x41
  VK_B              = 0x42
  VK_C              = 0x43
  VK_D              = 0x44
  VK_E              = 0x45
  VK_F              = 0x46
  VK_G              = 0x47
  VK_H              = 0x48
  VK_I              = 0x49
  VK_J              = 0x4A
  VK_K              = 0x4B
  VK_L              = 0x4C
  VK_M              = 0x4D
  VK_N              = 0x4E
  VK_O              = 0x4F
  VK_P              = 0x50
  VK_Q              = 0x51
  VK_R              = 0x52
  VK_S              = 0x53
  VK_T              = 0x54
  VK_U              = 0x55
  VK_V              = 0x56
  VK_W              = 0x57
  VK_X              = 0x58
  VK_Y              = 0x59
  VK_Z              = 0x5A

  VK_OEM_1          = 0xBA  # ';:' for US
  VK_OEM_PLUS       = 0xBB  # '+' any country
  VK_OEM_COMMA      = 0xBC  # ',' any country
  VK_OEM_MINUS      = 0xBD  # '-' any country
  VK_OEM_PERIOD     = 0xBE  # '.' any country
  VK_OEM_2          = 0xBF  # '/?' for US
  VK_OEM_3          = 0xC0  # '`~' for US

  VK_OEM_4          = 0xDB  #  '[{' for US
  VK_OEM_5          = 0xDC  #  '\|' for US
  VK_OEM_6          = 0xDD  #  ']}' for US
  VK_OEM_7          = 0xDE  #  ''"' for US
end
