local lkazmath = require "lkazmath"

local dataMap = {
	kmRay2 = {px=1, py=2, vx=3, vy=4},
	kmRay3 = {px=1, py=2, pz=3, vx=4, vy=5, vz=6},
}

function table_size(tbl)
	local cnt = 0
	for _, _ in pairs(tbl) do
		cnt = cnt + 1
	end
	return cnt
end

for className, data in pairs (dataMap) do
	local newFuncName = string.format("%sNew", className)
	local obj = lkazmath[newFuncName]()
	for filedName, _ in pairs(data) do
		assert(obj[filedName] == 0)
	end
	for filedName, value in pairs(data) do
		obj[filedName] = value
		assert(obj[filedName] == value)
	end

	local array = {}
	for i=1, table_size(data) do
		table.insert(array, 0)
	end

	obj:Fill(unpack(array))
	for filedName, _ in pairs(data) do
		assert(obj[filedName] == 0)
	end
end

