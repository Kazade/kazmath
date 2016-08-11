l = require "lkazmath"

do
	local vec2 = l.kmVec2NewWithTbl({x=1,y=2})
	local tbl = vec2:ToTbl()
	assert(tbl.x == 1)
	assert(tbl.y == 2)
	assert(#(vec2:ToArray()) == 2)
	assert(vec2.x == tbl.x)
	assert(vec2.y == tbl.y)
	vec2.x = 0
	assert(vec2.x == 0)
	-- table.foreach(vec2:ToTbl(), print)
end

do
	local vec3 = l.kmVec3New()
	local tbl = vec3:ToTbl()
	assert(tbl.x == 0)
	assert(tbl.y == 0)
	assert(tbl.z == 0)
	assert(#(vec3:ToArray()) == 3)
	vec3:Fill(5, 6, 7)
	assert(vec3.x == 5)
	assert(vec3.y == 6)
	assert(vec3.z == 7)
	vec3.x = 0
	local array = vec3:ToArray()
	assert(array[1] == 0)
	assert(array[2] == 6)
	assert(array[3] == 7)
end

do
	local array = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6}
	local mat4 = l.kmMat4NewWithArray(array)
	local newarray = mat4:ToArray()
	assert(#array == #newarray)
end

do
	local ray2 = l.kmRay2New()
	ray2.px = 1
	ray2.py = 2
	ray2.vx = 3
	ray2.vy = 4
	assert(ray2.px == 1)
	assert(ray2.py == 2)
	assert(ray2.vx == 3)
	assert(ray2.vy == 4)
end


