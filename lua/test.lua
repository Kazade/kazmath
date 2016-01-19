l = require "lkazmath"

do
	local vec2 = l.kmVec2NewWithTbl({x=1,y=2})
	local tbl = vec2:ToTbl()
	assert(tbl.x == 1)
	assert(tbl.y == 2)
	assert(#(vec2:ToArray()) == 2)
end

do
	local vec3 = l.kmVec3New()
	local tbl = vec3:ToTbl()
	assert(tbl.x == 0)
	assert(tbl.y == 0)
	assert(tbl.z == 0)
	assert(#(vec3:ToArray()) == 3)
	vec3:Fill(5, 6, 7)
	table.foreach(vec3:ToArray(), print)
	table.foreach(vec3:ToTbl(), print)
end

do
	local array = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6}
	local mat4 = l.kmMat4NewWithArray(array)
	local newarray = mat4:ToArray()
	assert(#array == #newarray)
end


