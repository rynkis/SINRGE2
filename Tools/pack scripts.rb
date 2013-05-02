#encoding:utf-8
#
# => You can make scripts package by this file
#
ary = []

scripts = open("__list__.rb", "rb") {|f| f.read }

scripts = scripts.split("\n")

scripts.each do |line|
  if line =~ /require "(.*).rb"/
    name = $1
    str = open(name + ".rb", "rb") {|f| f.read }
    ary << [name, str]
  end
end

open("../Data/Scripts.rbdata", "wb") { |f|  Marshal.dump(ary, f) }

exit
