
MAJOR = 2   # => 主版本，即 YAR。历史版本：1. Draft
MINOR = 4   # => 次版本，新增功能时提升
TEENY = 0   # => 修订数，修正 bug 时提升
MICRO = 1   # => 编译数，编译时自动提升

data =<<_HERE_
#ifndef __SIN_REVISION_H__
#define __SIN_REVISION_H__

#define BUILD_DATA_STR  "1,0,0,1"
#define BUILD_DATA      1,0,0,1

#endif // __SIN_REVISION_H__
_HERE_

version = ""

if File.exist? "sin_revision.h"
  str = ""
  data = open("sin_revision.h", "rb") {|f| f.read }
  data.each_line do |line|
    if line =~ /#define BUILD_DATA_STR  "(.*)"/
      ary = $1.split(",")
      ary[3] = ary[3].to_i + 1
      version = "#{MAJOR},#{MINOR},#{TEENY},#{ary[3]}"
      line = "#define BUILD_DATA_STR  \"#{version}\"\n"
    elsif line =~ /#define BUILD_DATA      (.*)/
      ary = $1.split(",")
      ary[3] = ary[3].to_i + 1
      version = "#{MAJOR},#{MINOR},#{TEENY},#{ary[3]}"
      line = "#define BUILD_DATA      #{version}\n"
    end
    str += line
  end
  data = str
else
  str = ""
  data.each_line do |line|
    if line =~ /#define BUILD_DATA_STR  "(.*)"/
      version = "#{MAJOR},#{MINOR},#{TEENY},#{MICRO}"
      line = "#define BUILD_DATA_STR  \"#{version}\"\n"
    elsif line =~ /#define BUILD_DATA      (.*)/
      version = "#{MAJOR},#{MINOR},#{TEENY},#{MICRO}"
      line = "#define BUILD_DATA      #{version}\n"
    end
    str += line
  end
  data = str
end

open("sin_revision.h", "wb") {|f| f.write data }

puts "current version is #{version}"