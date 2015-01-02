#!/usr/bin/env ruby

require "mathn"

class MoreMath
	@@bincoeff=[]
	def MoreMath.bincoeff (n,k)
		@@bincoeff[n]=[] unless (@@bincoeff.size>n and @@bincoeff[n]!=nil)
		return @@bincoeff[n][k] unless (@@bincoeff[n].size<=k or @@bincoeff[n][k]==nil)
		return 0 if k>n or k<0
		temp=1
		k.times {|i| temp*=(n-i); temp/= (i+1)}
		@@bincoeff[n][k]=temp
		return temp
	end
end

class Grid
	attr_accessor 	:cells
	attr_reader	:totalSize
	def initialize(n,d) #for counting d-dimensional polyominoes of size n
		@n=n
		@d=d
		@freeSize=2*n-2
		@totalSize=coordinatesToCellNumber(lastCoordinate)
		@canonicalCell=coordinatesToCellNumber(canonicalCellCoordinates)
	end
	def lastCoordinate
		temp=[2*@n-3]*@d
		temp[@d-1]=@n-1	
		return temp
	end
	def canonicalCellCoordinates
		temp=[@n-2]*@d
		temp[@d-1]=0
		return temp
	end
	def nextCoordinate(coordinate)
		tempCheck=nil;
		coordinate.collect {|i|
			if i>=@freeSize and tempCheck==nil
				0
			else
				if tempCheck==nil 
					tempCheck=1
					i+1
				else
					i
				end
			end
			}
	end
	def eachCell
		pos=[0]*@d
		while pos!=lastCoordinate
			yield(pos)
			pos=nextCoordinate(pos)
		end
		yield(pos)
	end
	def cellNumberToCoordinates(cellNumber)
		temp=[0]*@d
		temp.collect!{|value| tempValue=cellNumber % @freeSize; cellNumber/=@freeSize; tempValue}
	end
	def coordinatesToCellNumber(coordinates)
		number=0
		coordinates.reverse.each{|value| number *= @freeSize; number+=value}
		number
	end
	def inspect
		@d.to_s + "-dimensional grid for polyominoes of size " + @n.to_s + "\n" +
		"polyominoes in range 0.." + @totalSize.to_s + " with FreeSize=" + @freeSize.to_s + "\n" +
		"canonical cell is located at " + canonicalCellCoordinates.inspect + " and is therefore number " +
		@canonicalCell.to_s + "\n"
	end

	def neighbor(x,direction, dimension)
		temp=cellNumberToCoordinates(x)
		temp[dimension]+=direction
		return coordinatesToCellNumber(temp) if (0..@freeSize-1).include?(temp[dimension])
		return nil
	end
	def calculatePerformence (cellArray)
		#checks how much cellArray is "close" to being a valid polyomino
		#it is not hard to see that a cell array is a valid polyomino of size n iff its score is 0
		cellGraph=transformCellArrayToGraph(cellArray)
		connectedComponents=cellGraph.depthFirstSearch()
		score = 1
		score-=connectedComponents.size
		
		canonicalCellLocation=cellArray.index(@canonicalCell)
		score-=((connectedComponents.find {|x| x.index(canonicalCellLocation)}).size-@n).abs if canonicalCellLocation
		score-=(cellArray.size-@n).abs
		return score
	end

	def transformCellArrayToGraph(cellArray)
		cellGraph = Graph.new(cellArray.size)
		cellGraph.verticesLabels=cellArray.collect {|x| cellNumberToCoordinates(x).inspect + " (#{x.to_s})"}
		cellArray.each do |x| #adding edges to neighbors that are in cellArray
			neighbors=Array.new
			@d.times {|t| neighbors.push(neighbor(x, 1,t),neighbor(x,-1,t))}
			trueNeighbors=neighbors & cellArray
			trueNeighbors.each{|y| cellGraph.addEdge(cellArray.index(x),cellArray.index(y))}
		end
		return cellGraph
	end

end

class Array
	def findPlace
		each_index {|i| return i if yield(self[i])}
		return nil
	end
	def normalize!
		sum=0
		each {|value| sum=sum+value}
		collect! {|value| value.to_f/sum}
	end
	def normalize
		sum=0
		each {|value| sum=sum+value}
		collect {|value| value.to_f / sum}
	end
	def pickAtRandom
		random=rand(0)
		sum=0
		each_index{|i| sum+=self[i]; return i if sum>=random}		
	end
	def sum
		temp=0
		each {|x| temp+=x}
		return temp
	end
	def p_B(b,k)
		#given a subset B of the stochastic array and an integer k bigger than the subset size
		#returns the probebility that B will be a subset of a chosen k-subset
		values=self.values_at(*b)
		m=b.size
		n=size
#		puts "((#{MoreMath.bincoeff(n-m,k-m).to_f}*#{values.sum})+(#{MoreMath.bincoeff(n-m-1,k-m-1).to_f}*(#{1-values.sum})) ) / #{MoreMath.bincoeff(n-1,k-1).to_f}"
		((MoreMath.bincoeff(n-m,k-m).to_f*values.sum)+(MoreMath.bincoeff(n-m-1,k-m-1).to_f*(1-values.sum)) ) / MoreMath.bincoeff(n-1,k-1).to_f
		
	end
	def pickSomeAtRandom(k)
		n=size

		tempArray=self.dup
		result=[]

		k.times do |m| # m is the number of items already picked
			oldPB=p_B(result,k)
			each_index {|i| if result.include?(i) #calculates probebilities
						tempArray[i]=0
					else
						tempArray[i]=p_B(result+[i],k) / (oldPB*(k-m))
					end
					}
			result += [tempArray.pickAtRandom]
		end

		return result
	end
	def pickRandomIndependently
		tempArray=Array.new
		each_index {|i| random=rand(0); tempArray << i if random<self[i]}
		tempArray
	end
end

class SampleArray < Array
	def inspect
		tempString=String.new
		each {|x| tempString= tempString + x.inspect + "\n"}
		return tempString
	end
end

class Graph
	attr_accessor 	:numberOfVertices
	attr_accessor 	:verticesLabels
	attr_reader	:numberOfEdges
	def initialize(numberOfVertices, isDirected=nil)
		@numberOfVertices=numberOfVertices
		@numberOfEdges=0
		@edgesList=Array.new(numberOfVertices)
		@edgesList.each_index {|i| @edgesList[i]=Array.new}
		@isDirected=isDirected
		@verticesLabels=Array.new
		numberOfVertices.times {|i| @verticesLabels[i]=i.to_s} # default labels
	end
	def addEdge(v1,v2)
		(@edgesList[v1]).push(v2).uniq!
		(@edgesList[v2]).push(v1).uniq! unless @isDirected
		@numberOfEdges+=1
		return self
	end
	def inspect
		tempString = "Graph of " + @numberOfVertices.to_s + " vertices and " + @numberOfEdges.to_s + " edges. Edge list: \n"
		@edgesList.each_index do |i| 
			tempString+=i.to_s + " - " + @verticesLabels[i] + ": "
			@edgesList[i].each {|j| tempString+= "(" + i.to_s + "," + j.to_s + ") "}
			tempString+="\n"
		end
		return tempString
	end
	def depthFirstSearch (startVertex = 0)
		#runs a DFS from startVertex, returns the list of vertices found
		def dfsVisit(vertex, colorsArray,vertexList)
			colorsArray[vertex]=1 #gray
			vertexList.push(vertex)
			@edgesList[vertex].each {|x| dfsVisit(x,colorsArray,vertexList) if colorsArray[x] == 0}
		end
		colorsArray=[0]*@numberOfVertices #white
		vertexList=Array.new
		return vertexList if self.numberOfEdges==0
		while startVertex do
			vertexList.unshift(Array.new) #new connected component array
			dfsVisit(startVertex,colorsArray,vertexList[0])
			startVertex = colorsArray.findPlace{|x| x == 0} #finding a new vertex
		end
		return vertexList
	end
end

def f(x,v)
#the probebility density function: given a subset, returns its probebility
	prod=1
	v.each_index do |i|
		if x.include?(i) 
			prod*=v[i]
		else
			prod*=(1-v[i])
		end
	end
	prod
end

def W(x,u,v)
#this function computes the cross-entropy, as described in the article: W(x)=f(x)/g(x)
	f(x,u)/f(x,v)
end

#now comes the algorithm itself
#preliminaris
$d=2 		#dimension
$n=5 		#poloymino size
$N=100 		#sample size
$N1=1000	#final sample size
$rho=0.2	#sample quantile
$targetScore = 0

grid=Grid.new($n,$d)
puts grid.inspect

X=SampleArray.new
gamma=[0]
#step 1: "Define v_0=u. Set t:=1"
v=[([0.5]*grid.totalSize)]
t=0

while gamma[t] < $targetScore
	t+=1
	#step 2: "Generate random sample X_1,...,X_N according to the pdf f( ;v_{t-1})"
	$N.times{|i| X[i]=v[t-1].pickRandomIndependently}
		#puts "Chose randomally the sets:\n#{X.inspect}"
	#Calculate the performances S(X_i) for all i...
	s=[]
	X.each_index {|i| s.push([i,grid.calculatePerformence(X[i])])}
	#...and order them from smallest to biggest.
	s.sort!{|a,b| a[1] <=> b[1]}
	puts "S=#{s.inspect}"
	#Let gamma_t be the (1-rho) sample quantile of performances: gamma_t=S_{ceil((1-rho)*N)}
	gamma[t]=(s[((1-$rho)*$N).ceil-1])[1]
	validSamples=(s.find_all{|x| x[1]>=gamma[t]}).collect{|x| x[0]}
	puts "Gamma[#{t}]=#{gamma[t]}"
	#step 3: Use the same sample to calculate v_{t,j}
	v[t]=Array.new(grid.totalSize)
	v[t].each_index do |j| 
		nominator=validSamples.collect{|i| temp=0; if X[i].include?(j) then temp=W(X[i],v[0],v[t-1]) end; temp}
		denominator=validSamples.collect{|i| W(X[i],v[0],v[t-1]) }
		v[t][j]=nominator.sum / denominator.sum
	end
#	puts "V[#{t}]=#{v[t].inspect}"
end

$N1.times{|i| X[i]=v[t].normalize.pickSomeAtRandom($n)}
s=[]
X.each_index {|i| s.push([i,grid.calculatePerformence(X[i])])}
validSamples=(s.find_all{|x| x[1]>=2*$targetScore}).collect{|x| x[0]}
l=(validSamples.collect{|i| W(X[i],v[0],v[t]) }).sum.to_f / $N1

puts "l=#{l}"
