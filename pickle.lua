----------------------------------------------
-- Pickle.lua
-- A table serialization utility for lua
-- Steve Dekorte, http://www.dekorte.com, Apr 2000
-- Freeware
----------------------------------------------

local Pickle = {
	clone = function (t) local nt={}; for i, v in pairs(t) do nt[i] = v end return nt end
}

function Pickle:pickle_(root)
	if type(root) ~= 'table' then 
		error('can only pickle tables')
	end
	self._tableToRef = {}
	self._refToTable = {}
	local savecount = 0
	self:ref_(root)
	local s = ''

	while table.getn(self._refToTable) > savecount do
		savecount = savecount + 1
		local t = self._refToTable[savecount]
		s = s .. '{\n'
		for i, v in pairs(t) do
			local ti, tv = type(i), type(v)
			if (ti == 'string' or ti == 'number' or ti == 'boolean' or ti == 'table') and (tv == 'string' or tv == 'number' or tv == 'boolean' or tv == 'table') then
				s = s .. '[' .. self:value_(i) .. ']=' .. self:value_(v) .. ',\n'
			end
		end
		s = s .. '},\n'
	end

	return '{' .. s .. '}'
end

function Pickle:value_(v)
	local	vtype = type(v)
	if	vtype == 'string' then return '\'' .. string.gsub(v, '.', function (char) local code = string.byte(char, 1, 1) if code < 32 or code == 127 or char == '\'' then code = '\\' .. code while string.len(code) < 3 do code = '0' .. code end return '\\' .. code else return char end end) .. '\''
	elseif	vtype == 'number' then return v
	elseif vtype == 'boolean' then return v and 'true' or 'false'
	elseif	vtype == 'table' then return '{' .. self:ref_(v) .. '}'
	else	return nil --error('pickle a '..type(v)..' is not supported')
	end  
end

function Pickle:ref_(t)
	local ref = self._tableToRef[t]
	if not ref then 
		if t == self then error('can\'t pickle the pickle class') end
		table.insert(self._refToTable, t)
		ref = table.getn(self._refToTable)
		self._tableToRef[t] = ref
	end
	return ref
end

----------------------------------------------
-- pickle
----------------------------------------------

function pickle(t)
	return Pickle:clone():pickle_(t)
end

----------------------------------------------
-- unpickle
----------------------------------------------

function unpickle(s)
	if type(s) ~= 'string' then
		error('can only unpickle strings')
	end
	local tables = loadstring('return ' .. s)()
  
	for tnum = 1, table.getn(tables) do
		local t = tables[tnum]
		local tcopy = {};
		for i, v in pairs(t) do
			tcopy[i] = v
		end
		for i, v in pairs(tcopy) do
			local ni, nv
			if type(i) == 'table' then ni = tables[i[1]] else ni = i end
			if type(v) == 'table' then nv = tables[v[1]] else nv = v end
			t[i] = nil
			t[ni] = nv
		end
	end
	return tables[1]
end
