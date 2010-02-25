function class(...)
	-- create an empty class
	local new_class = {}
	-- copy all methods from the super classes
	for index = 1, #arg do
		for name, method in pairs(arg[index]) do
			new_class[name] = method
		end
	end
	-- insert an additional method to check the class of the instance
	new_class.instance_of = function(self, check)
		if check == new_class then
			return true
		end
		for index = 1, #arg do
			if arg[index].instance_of(self, check) then
				return true
			end
		end
		return false
	end
	-- insert an additional method to return an hex id of the instance
	new_class.hex = function(self)
		return string.match(tostring(self), 'table: (.*)')
	end
	-- insert an additional method to clone the instance
	new_class.clone = function(self)
		local function dup(obj, dupped)
			if type(obj) == 'table' then
				if dupped[obj] then
					return dupped[obj]
				end
				local tab = {}
				dupped[obj] = tab
				for key, value in pairs(obj) do
					tab[dup(key, dupped)] = dup(value, dupped)
				end
				return tab
			end
			return obj
		end
		local clone, dupped = new_class(), {}
		for key, value in pairs(self) do
			if not clone[key] then
				clone[key] = dup(value, dupped)
			end
		end
		return clone
	end
	-- insert an additional method to return the class name of an instance (only works if the class is accessible from the global space)
	new_class.class_name = function()
		local function find(space, name, visited)
			if visited[space] then
				return nil
			end
			visited[space] = true
			for key, value in pairs(space) do
				if value == new_class then
					return name .. '.' .. key
				end
				if type(value) == 'table' then
					local found = find(value, name .. '.' .. key, visited)
					if found ~= nil then
						return found
					end
				end
			end
			return nil
		end
		local found = find(_G, '_G', {})
		if found then
			return string.sub(found, 4, -1)
		end
		return nil
	end
	-- insert an additional method to make any given table an instance of the class
	new_class.make_instance = function(self)
		-- create closures that automatically pass the instance as the first parameter for all methods
		for name, method in pairs(new_class) do
			self[name] = function(...)
				return method(self, unpack(arg))
			end
		end
		-- remove the new and the make_instance methods method
		self.new = nil
		self.make_instance = nil
	end
	-- create a __call metamethod that creates a new instance of the class
	local meta = {}
	meta.__call = function(...)
		-- create an empty instance
		local self = {}
		new_class.make_instance(self)
		-- the first argument is the class, shift left all other arguments
		for i = 2, #arg do
			arg[i - 1] = arg[i]
		end
		arg[#arg] = nil
		-- call the new method to initialize the instance
		new_class.new(self, unpack(arg))
		-- return the newly created instance
		return self
	end
	--[[meta.__index = function(tab, key)
		print('__index', tab, key)
		return rawget(tab, key)
	end
	meta.__newindex = function(tab, key, value)
		print('__newindex', tab, key, value)
		return rawset(tab, key, value)
	end]]
	-- set the metatable of the class
	debug.setmetatable(new_class, meta)
	-- return it
	return new_class
end

function class_by_name(name)
	local library = _G
	while string.find(name, '%.') do
		local piece
		piece, name = string.match(name, '(.-)%.(.*)')
		library = library[piece]
		if library == nil then
			return nil
		end
	end
	return library[name]
end

--[[A = class()

function A:new(name)
	self.name = 'A: ' .. name
end

function A:print()
	print(self.name)
end

function A:methods()
	return
		function(state)
			local key, value = state.key
			repeat
				key, value = next(self, key)
			until key == nil or type(value) == 'function'
			state.key = key
			return value
		end,
		{ key = nil }
end

B = class(A)

function B:new(name)
	self.name = 'B: ' .. name
end

a = A('a')
b = B('b')

a.print()
b.print()

print(a.isa(A), a.isa(B), b.isa(A), b.isa(B))

for method in b:methods() do
	print(method)
end

for k, v in pairs(b) do
	print(k, v)
end
]]