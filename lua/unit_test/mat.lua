local lkazmath = require "lkazmath"

local dataMap = {
	kmMat3 = {},
	kmMat4 = {},
}

for i=1, 9 do
	table.insert(dataMap.kmMat3, i)
end

for i=1, 16 do
	table.insert(dataMap.kmMat4, i)
end

function table_size(tbl)
	local cnt = 0
	for _, _ in pairs(tbl) do
		cnt = cnt + 1
	end
	return cnt
end

for className, data in pairs (dataMap) do
	local newFuncName = string.format("%sNew", className)
	local newWithFuncName = string.format("%sNewWithArray", className)
	local obj = lkazmath[newFuncName]()
	local obj2 = lkazmath[newWithFuncName](data)
	for i, v in pairs(obj:ToArray()) do
		assert(v == 0)
	end
	for i, v in pairs(obj2:ToArray()) do
		assert(v == i)
	end
	obj:Fill(obj2:ToArray())
	assert(obj:AreEqual(obj2) == true)
end

