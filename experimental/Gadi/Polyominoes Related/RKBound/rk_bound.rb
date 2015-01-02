#attempt to imitate the Rivest-Klarner upper bound in three dimensions
require 'rubygems'
require 'RMagick'
class Matrix3D
	def initialize
		@squares = {}
		@size = {:x => 0, :y => 0, :z => 0}
	end
	def Matrix3D.[](size_x, size_y, size_z, initial_value = nil)

		for i in 0...size_x
			for j in 0...size_y
				for k in 0...size_z
					@squares[[i,j,k]] = initial_value
				end
			end
		end
		@size = {:x => size_x, :y => size_y, :z => size_z}
	end
# 	def row(indexes, dimension)
# # 		raise "index out of bound (0..#{@size[dimension]-1})" if (index < 0 or index >= @size[dimension])
# 		row = []
# 		@size[dimension].times do |i|
# 			final_index = []
# 			temp_indexes = indexes.dup
# 			3.times do |j|
# 				final_index << ((j == dimension)?(i):(temp_indexes.shift))
# 			end
# 			row << @squares[final_index]
# 		end
# 		return row
# 	end
# 	def set_row(indexes, dimension, new_row)
# 		@size[dimension].times do |i|
# 			final_index = []
# 			temp_indexes = indexes.dup
# 			3.times do |j|
# 				final_index << ((j == dimension)?(i):(temp_indexes.shift))
# 			end
# 			@squares[final_index] = new_row.shift
# 		end
# 	end
	def dim_to_num(dim)
		case dim
			when Integer 	then return dim
			when :x		then return 0
			when :y		then return 1
			when :z		then return 2
		end
	end
	def each_cell_index
		
	end
	def fill
		
	end
	def all_dimensions
		[:x,:y,:z]
	end
	def in_bounds?(a,dimension)
		a >=0 and a < @size[dimension]
	end
	def all_in_bounds?(x,y,z)
		in_bounds?(x,:x) and in_bounds?(y,:y) and in_bounds?(z,:z)
	end
	def set_sizes(sizes)
		all_dimensions.each{|d| @size[d] = sizes[dim_to_num(d)] + 1 unless in_bounds?(sizes[dim_to_num(d)],d)}
	end
	def [](x,y,z)
		raise "out of bounds" unless all_in_bounds?(x,y,z)
		@squares[[x,y,z]]
	end
	def []=(x,y,z,val)
		@squares[[x,y,z]] = val
		set_sizes([x,y,z])
		fill
	end

# 	def rotate(rotation_plane)
# 		remaining_dimensions = (0..2).to_a - [rotation_plane]
# 		x = remaining_dimensions.first
# 		y = remaining_dimensions.last #using x and y beause they are simple names
# 		raise "cannot rotate" unless @size[x] == @size[y]
# 		@size[rotation_plane].times do |i|
# 			temp_rows = []
# 			@size[x].times do {|j| temp_rows << row()}
# 		end
# 	end

	def inspect
		puts @squares.inspect
		puts @size.inspect
		s=""
		@size[:z].times do |k|
			@size[:x].times do |i|
				@size[:y].times do |j|
					s += "#{@squares[[i,j,k]]}"
					s += " " unless j == @size[:y] - 1
				end
				s += "\n" unless i == @size[:x] - 1
			end
			s += "\n\n" unless k == @size[:z] - 1
		end
		return s
	end
end
m = Matrix3D.new
m[1,0,1] = 8
puts m.inspect
# m = Matrix3D.new(4,4,4,3)
# m[1,0,1] = 8
# puts m.inspect

class Neighborhood
	attr_accessor :squares
	@@square_size = 100

	def initialize
		self.squares = {}
		for i in 0..2
			for j in 0..2
				for k in 0..2
					self.squares[[i,j,k]] = :free
				end
			end
		end
		self.squares[[1,1,1]] = :center
		self.squares[[1,1,0]] = :known
		self.squares[[0,1,1]] = :known
		self.squares[[1,0,1]] = :known
	end
	def Neighborhood.type_to_char(type)
		case type
			when :free: 	return ' '
			when :center: 	return 's'
			when :known:	return '*'
			when :accepted:	return 'v'
			when :rejected:	return 'x'
		end
	end
	def Neighborhood.draw_square(type)
		canvas = Magick::Image.new(@@square_size,@@square_size) {self.background_color = 'transparent'}
		gc = Magick::Draw.new
		gc.stroke('black')
		gc.stroke_width(2)
		gc.fill('white')
		
		top = @@square_size-1
		gc.rectangle(0,0,top, top)
		center = @@square_size / 3
		#{Neighborhood.type_to_char(type)}
		gc.pointsize(2 * @@square_size / 3)
		gc.text(center, 2 * center, Neighborhood.type_to_char(type))
		gc.draw(canvas)
#  		canvas.display
		return canvas
	end
	def draw
		final_image=Magick::ImageList.new

		images = self.squares.collect{|coords, type| [coords,Neighborhood.draw_square(type)]}
		images.each do |image_pair|
			image = image_pair.last
			coords = image_pair.first
			offset = Magick::Rectangle.new(0,0,0,0)
			offset.x = coords[0] * @@square_size + coords[2] * @@square_size * 4
                        offset.y = coords[1] * @@square_size
			image.page = offset
			final_image << image
		end
		t = final_image.mosaic
		t.background_color = 'white'
		t.display
		t
	end
end

# n = Neighborhood.new

# n.draw
