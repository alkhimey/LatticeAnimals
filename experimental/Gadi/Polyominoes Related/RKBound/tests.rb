require 'test/unit'
require 'rk_bound'

class Matrix3DTests < Test::Unit::TestCase
	def test_direct_access
		m = Matrix3D.new(4,4,4,3) #a 4x4x4 matrix filled with 3's
		m[0,0,0] = 8
		assert_equal([8,3,3,3], m.row([0,0],0))
		assert_equal([8,3,3,3], m.row([0,0],1))
		assert_equal([8,3,3,3], m.row([0,0],2))
		m[0,0,0] = 3
		m[0,1,1] = 2
		assert_equal([3,3,3,3],m.row([0,0],0))
		assert_equal([3,2,3,3],m.row([0,1],1))
		m.set_row([0,1],1,[6,7,8,9])
		assert_equal(6,m[0,0,1])
		assert_equal(7,m[0,1,1])
		assert_equal(8,m[0,2,1])
		assert_equal(9,m[0,3,1])
	end
end