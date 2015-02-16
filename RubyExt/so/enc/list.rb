
str = "[\n"

Dir.glob("*.so") { |file| str += "\"#{file}\",\n" }

str += "]\n"

open("list.txt", "wb") {|io| io.write str }
