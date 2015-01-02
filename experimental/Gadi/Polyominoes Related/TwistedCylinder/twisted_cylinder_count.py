W = 10

class Square:
    def __init__(self, x,y):
        self.x = x
        self.y = y
    def neighbors(self):
        neighbors = []
        #up
        neighbors.append(Square(self.x,self.y+1))
        
        #down
        if self.y > 0 and self.x >= 0:
            neighbors.append(Square(self.x,self.y-1))
        
        #left
        if self.x == 0 and self.y > 0:
            neighbors.append(Square(W-1,self.y-1))
	else:
            if self.x > 0:
                neighbors.append(Square(self.x - 1,self.y))
	
        #right
	if self.x == W-1:
             neighbors.append(Square(0,self.y + 1))
	else:
             neighbors.append(Square(self.x + 1,self.y))
                	
        return neighbors
    def __repr__(self):
        return "(%d,%d)" % (self.x,self.y)


t = Square(9,0)
print t.neighbors()