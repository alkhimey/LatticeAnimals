#!/usr/bin/env ruby
def num_to_coords(num, base,d,n)
	coords=[]
	while num>0
		coords.unshift(num % base)
		num /= base
	end
	while coords.size < d
		coords.unshift(0)
	end
	coords[0]+=(n-2)
	return coords.collect{|x| x-(n-2)}
end

n=8
d=3
free_size=2*n-2

zero_location=(1...d).inject(0){|sum, count| sum+(free_size**(count-1))*(n-2)}

input=File.open("input", "r"){|file| file.read}
input.each_line do |line|
	puts line.chomp.split("-").collect{|x| num_to_coords(x.to_i,free_size,d,n)}.inspect
end