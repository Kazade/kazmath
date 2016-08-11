local lkazmath = require "lkazmath"

local dataMap = {
	kmVec2 = {x=1, y=2},
	kmVec3 = {x=1, y=2, z=3},
	kmVec4 = {x=1, y=2, z=3, w=4},
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
		table.insert(array, i)
	end

	obj:Fill(unpack(array))
	for i, v in pairs(obj:ToArray()) do
		assert(array[i] == v)
	end

	for filedName, v in pairs(obj:ToTbl()) do
		assert(obj[filedName] == v)
	end
end

