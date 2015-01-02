#!/usr/bin/env ruby
require 'Qt'

class Coordinates
	attr_accessor :x,:y
	def initialize(x,y)
		self.x=x
		self.y=y
	end
end

class Polyomino
	attr_accessor :squares
	def initialize
		self.squares=Array.new
	end
	def addSquare(square)
		squares << square
	end
	def addSquare(x,y)
		squares << Coordinates.new(x,y)
	end
end

class PolyominoBoard < Qt::Widget
	$SquareSize=90
	$PolyominoColor=Qt::darkCyan

	def initialize(parent, name)
		super(parent,name)
		setBackgroundMode(NoBackground)
		@myPolyomino=Polyomino.new
		@myPolyomino.squares << Coordinates.new(0,0) << Coordinates.new(0,1) << Coordinates.new(1,1) << Coordinates.new(1,2)
	end
	def paintEvent(event)
		painter=Qt::Painter.new(self)
		doubleBuffer=Qt::Pixmap.new(self.width,self.height)
		doubleBufferPainter=Qt::Painter.new(doubleBuffer)
		doubleBuffer.fill(white)
		tempBuffer=Qt::Pixmap.new($SquareSize*5,$SquareSize*5)
		tempBuffer.fill(white)
		tempBufferPainter=Qt::Painter.new(tempBuffer)
		
		paintPolyomino(tempBufferPainter,@myPolyomino)

		drawBufferAt(doubleBufferPainter,tempBuffer,Coordinates.new(30,30))

		painter.drawPixmap(0,0,doubleBuffer);
		painter.end
		tempBufferPainter.end
		doubleBufferPainter.end
	end
	def drawBufferAt(doubleBufferPainter,tempBuffer,screenCoords)
		doubleBufferPainter.drawPixmap(screenCoords.x,screenCoords.y,tempBuffer)
	end
	def paintPolyomino(painter, polyomino)
		painter.setBrush($PolyominoColor)
		painter.setPen(Qt::NoPen)
		polyomino.squares.each do |square|
			painter.drawRect(square.x*$SquareSize,square.y*$SquareSize, $SquareSize,$SquareSize)
		end
	end

end

app = Qt::Application.new(ARGV)
widget = PolyominoBoard.new(nil, "Board")
#widget = Qt::Widget.new
widget.setGeometry(50, 500, 400, 400)
widget.caption = "Hello World!"

app.mainWidget = widget
widget.show
app.exec
