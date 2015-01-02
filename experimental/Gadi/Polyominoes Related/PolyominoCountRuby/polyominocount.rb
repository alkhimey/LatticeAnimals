#!/usr/bin/env ruby

class Integer
	def aPowerOf?(x)
		puts "aPowerOf called for #{self} with x=#{x}"
		temp=self
		return nil if temp < 1
		while temp>1
			return nil if temp % x != 0 #not divisible by x but bigger than 1 - not a prime of x
			temp /= x
		end
		return true
	end
end

class Grid
	attr_reader	:canonicalCellNumber, :n
	def initialize (n, d)
		@n=n
		@d=d
		@freeSize=2*n-2
		@canonicalCellNumber=coordinatesToNumber(canonicalCell)
		@cells=Array.new
	end
	def canonicalCell
		cell=[@n-2]*@d
		cell[@d-1]=0
		return cell
	end
	def coordinatesToNumber(coordinates)
		number=0
		t=1
		coordinates.each{|x| number += t*x; t *=@freeSize}
		return number
	end
	def numberToCoordinates(number)
		coordinates=Array.new
		@d.times {|i| coordinates[i]=number % @freeSize; number /= @freeSize}
		return coordinates
	end
	def neighbors(cell)
		neighborsArray=Array.new
		t=1
		@d.times {neighborsArray.push(cell + t,cell - t); t *=@freeSize}
		return neighborsArray
	end
	def addCell (newCell)
		#adds newCell to the current polyomino and returns all new neighbors
		newNeighborsArray=neighbors(newCell).find_all {|x| legalAndNewNeighbor(x)}
		@cells.push(newCell)
		return newNeighborsArray
	end
	def legalAndNewNeighbor(x)
		return nil if x<canonicalCellNumber 		 #not legal
		return nil if @cells.find{|y| areNeighbors(x,y) or x == y} #not new
		return true
	end
	def areNeighbors(x,y)
		#return ((x-y).abs.aPowerOf?(@freeSize))
		neighbors(x).include?(y)
	end
	def polyominoSize
		return @cells.size
	end
	def removeCell(cell)
		@cells -= [cell]
	end
end

def count (n,d)
	myGrid = Grid.new(n,d)
	countArray=[0]*n
	untriedSet=[myGrid.canonicalCellNumber]
	countRecurse(myGrid, countArray, untriedSet)
end

def countRecurse(myGrid, countArray, untriedSet)
	while not untriedSet.empty?
		newCell=untriedSet.shift 							#remove a cell from the untried set
		untriedSet += newNeighbors = myGrid.addCell(newCell)				#add it to the current polyomino, add new neighbors to the untried set
		countArray[myGrid.polyominoSize-1]+=1							#count a new polyomino
		countRecurse(myGrid, countArray, untriedSet.dup) if myGrid.polyominoSize<myGrid.n	#still need to recurse
		myGrid.removeCell(newCell)
		untriedSet-= newNeighbors
	end
	return countArray
end

if ARGV.size != 5
	puts "Usage: PolyominoCount n d split totalChunks currentChunk"
	exit
end
	n=ARGV[0]
	d=ARGV[1]
	puts "Counting #{d}-dimensional polyominoes of size #{n}"
	oldTime=Time.now
	puts count(n.to_i,d.to_i).inspect
	newTime=Time.now
	puts "Time passed: #{newTime.to_i-oldTime.to_i}"
