#!/usr/bin/env ruby

require 'optparse'
require 'set'
#require 'profile'

#coding types:
# 1) 	?x?	2)	?2?	3)	?1x	4)	?12
#	*s1		*s1		*sx		*sx	
#	***		***		***		***
class Array
	def count_item(x)
		self.inject(0){|sum, item| x==item ? sum+1 : sum}
	end
	def count_items(items_array)
		self.inject(0){|sum, item| sum+items_array.count_item(item)}
	end
end

class Set
	def to_s
		self.to_a.inspect
	end
end

class Square
	@@neighbors=Hash.new #optimization
	include Comparable
	attr_accessor :coords
	def initialize(coords)
		self.coords=coords
		@my_hash=self.coords.hash
	end
	def calculate_neighbors
		neighbors_array=[]
		self.coords.each_index do |i|
			temp_coords=self.coords.dup
			temp_coords[i] +=1
			neighbors_array << Square.new(temp_coords)

			temp_coords=self.coords.dup
			temp_coords[i] -=1
			neighbors_array << Square.new(temp_coords)
		end
		return neighbors_array
	end
	def neighbors
		@@neighbors[self]||=calculate_neighbors #optimization
	end
	def hash
		@my_hash #optimization
# 		return self.coords.hash
	end
	def eql?(other)
		return self.coords==other.coords
	end
	def ==(other)
		return self.coords==other.coords
	end
	def <=> (other)
		self.coords <=> other.coords
	end
	def inspect
		coords.inspect
	end
end

class Grid
	attr_reader :squares
	def initialize(dimensions)
		@dimensions=dimensions
		@squares = Set.new
	end
	def origin
		return Square.new([0]*@dimensions)
	end
	def << (square)
		case square
 			when Square: 	@squares << square
 			when Array: 	@squares << Square.new(square)
		end
		return self
	end
	def remove_square(square)
		squares.delete(square)
	end

  def to_s
    raise "Cannot visualize non-2D grid" unless @dimensions == 2
    cols = @squares.collect { |sqr| sqr.coords[0] }.max + 1
    rows = @squares.collect { |sqr| sqr.coords[1] }.max + 1
    display_arr = []
    rows.times do
      display_arr << Array.new(cols)
    end

    @squares.each do |sqr|
      display_arr[sqr.coords[1]][sqr.coords[0]] = true
    end

    display_arr.collect do |row|
      row.collect do |col|
        if col
          "x"
        else
          "."
        end
      end.join("")
    end.join("\n")
  end

	def new_neighbors(square)
		#neighbors of square that are not neighbors of any other polyomino square
		#assumes square is not yet in the polyomino
		old_neighbors=@squares.collect{|s| s.neighbors}.flatten.uniq
		return square.neighbors.reject{|s| old_neighbors.include?(s) or @squares.include?(s) or s<self.origin}
	end
end


# def parse_options
#   options = {}
# 
#   opts = OptionParser.new
#   opts.on("-q", "--quiet") do
#     options[:quiet] = true
#   end
#   opts.on("-t", "--trees") do
# 	options[:trees] = true
#   end
#   opts.on("-n N", "(mandatory)", Integer) do |n|
#     options[:n] = n
#   end
#   opts.on("-d D", "(mandatory)", Integer) do |d|
#     options[:d] = d
#   end
#   opts.on("-v", "--verbose") do
# 	options[:verbose]=true
#   end
# 
#   begin
#     opts.parse!
#     raise unless options[:n] and options[:d]
#   rescue
#     puts opts
#     exit 1
#   end
# 
#   options
# end

if $0 == __FILE__
#   options = parse_options

end
