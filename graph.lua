-- Make some functions local
local floor = math.floor
local match = string.match
local gmatch = string.gmatch
local substr = string.sub
local lower = string.lower
local length = string.len
local gsub = string.gsub
local gfind = string.gfind

Properties = class()

function Properties:new(graph)
	self.graph = graph
	self.clear()
end

function Properties:redraw()
	c_lib.props_redraw()
end

function Properties:clear()
	c_lib.props_clear()
end

function Properties:delete(label)
	c_lib.props_delete(label)
end

function Properties:text_input(label, getter, setter, validator)
	c_lib.props_text_input(label, getter, function(v) local update = getter() ~= v; setter(v); if update then self.graph.update() end end, validator)
end

function Properties:memo_input(label, getter, setter, validator)
	c_lib.props_memo_input(label, getter, function(v) local update = getter() ~= v; setter(v); if update then self.graph.update() end end, validator)
end

function Properties:check_box(label, getter, setter)
	c_lib.props_check_box(label, getter, function(v) local update = getter() ~= v; setter(v); if update then self.graph.update() end end, function(v) return true end)
end

function Properties:list(label, choices, getter, setter)
	c_lib.props_list(label, choices, getter, function(v) local update = getter() ~= v; setter(v); if update then self.graph.update() end end, function(v) return true end)
end

InputStream = class()

function InputStream:read_all()
	error('must implement the read_all() method in class ' .. self.class_name())
end

function InputStream:close()
	error('must implement the close() method in class ' .. self.class_name())
end

FileInputStream = class(InputStream)

function FileInputStream:new(file)
	if type(file) == 'string' then
		local err
		file, err = io.open(file, 'rb')
		if file == nil then
			error(err)
		end
	end
	self.file = file
end

function FileInputStream:read_all()
	return self.file:read('*a')
end

function FileInputStream:close()
	self.file:close()
end

OutputStream = class()

function OutputStream:write(...)
	--local args = {}
	for i = 1, #arg do
		arg[i] = tostring(arg[i])
	end
	self.write_str(table.concat(arg))
end

function OutputStream:write_str(str)
	error('must implement the write_str() method in class ' .. self.class_name())
end

function OutputStream:close()
	error('must implement the close() method in class ' .. self.class_name())
end

FileOutputStream = class(OutputStream)

function FileOutputStream:new(file)
	if type(file) == 'string' then
		local err
		file, err = io.open(file, 'wb')
		if file == nil then
			error(err)
		end
	end
	self.file = file
end

function FileOutputStream:write_str(str)
	self.file:write(str)
end

function FileOutputStream:close()
	self.file:close()
end

StringOutputStream = class(OutputStream)

function StringOutputStream:new()
	self.output = {}
end

function StringOutputStream:write_str(str)
	self.output[#self.output + 1] = str
end

function StringOutputStream:close()
	return table.concat(self.output)
end

CodeOutputStream = class(OutputStream)

function CodeOutputStream:new()
	c_lib.set_code('')
end

function CodeOutputStream:write_str(str)
	c_lib.add_code(str)
end

function CodeOutputStream:close()
end

Rect = class()

function Rect:new(...)
	self.set_pos(arg[1], arg[2], arg[3], arg[4])
end

function Rect:fix(x0, y0, x1, y1)
	if x0 > x1 then
		x0, x1 = x1, x0
	end
	if y0 > y1 then
		y0, y1 = y1, y0
	end
	return x0, y0, x1, y1
end

function Rect:inside(x, y)
	return x >= self.x0 and x <= self.x1 and y >= self.y0 and y <= self.y1
end

function Rect:set_pos(x0, y0, x1, y1)
	if x0 and y0 then
		-- all coordinates provided
		self.x0, self.y0, self.x1, self.y1 = self.fix(x0, y0, x1, y1)
	elseif x0 then
		-- one rect provided
		self.y0 = x0.y0
		self.x1 = x0.x1
		self.y1 = x0.y1
		self.x0 = x0.x0
		self.x0, self.y0, self.x1, self.y1 = self.fix(x0.x0, x0.y0, x0.x1, x0.y1)
	else
		-- start empty
		self.x0 = 1e100
		self.y0 = 1e100
		self.x1 = -1e100
		self.y1 = -1e100
	end
end

function Rect:union(x0, y0, x1, y1)
	if not y0 then
		y0 = x0.y0
		x1 = x0.x1
		y1 = x0.y1
		x0 = x0.x0
	end
	x0, y0, x1, y1 = self.fix(x0, y0, x1, y1)
	if x0 < self.x0 then
		self.x0 = x0
	end
	if y0 < self.y0 then
		self.y0 = y0
	end
	if x1 > self.x1 then
		self.x1 = x1
	end
	if y1 > self.y1 then
		self.y1 = y1
	end
end

Port = class(Rect)

function Port:new(name, owner)
	self.name = name
	self.id = name
	self.owner = owner
end

function Port:get_id()
	if not self.id then
		self.id = self.name
	end
	return self.id
end

function Port:set_name(name)
	self.name = name
end

function Port:get_name()
	return self.name
end

Input = class(Port)

function Input:get_output()
	return self.owner.owner.find_output(self)
end

function Input:get_tip(out)
	out.write('Input port ', self.name)
end

Output = class(Port)

function Output:get_type()
	return self.owner.get_type(self)
end

function Output:get_tip(out)
	out.write('Output port ', self.name)
end

Node = class(Rect)

function Node:new(name, type, owner)
	Rect.new(self)
	self.name = name
	self.type = type
	self.owner = owner
	self.comment = ''
	self.color = 'black'
	self.inputs = {}
	self.outputs = {}
end

function Node:add_port(port)
	port.owner = self
	if port.instance_of(Input) then
		self.inputs[#self.inputs + 1] = port
	else
		self.outputs[#self.outputs + 1] = port
	end
	return port
end

function Node:inputs_iter()
	return
		function(data)
			local value = data.tab[data.i]
			data.i = data.i + 1
			return value
		end,
		{ i = 1, tab = self.inputs }
end

function Node:outputs_iter()
	return
		function(data)
			local value = data.tab[data.i]
			data.i = data.i + 1
			return value
		end,
		{ i = 1, tab = self.outputs }
end

function Node:num_ports()
	return #self.inputs + #self.outputs
end

function Node:get_port(index)
	if index <= #self.inputs then
		return self.inputs[index]
	end
	return self.outputs[index - #self.inputs]
end

function Node:inside(x, y)
	for port in self.inputs_iter() do
		if port.inside(x, y) then
			return port
		end
	end
	for port in self.outputs_iter() do
		if port.inside(x, y) then
			return port
		end
	end
	if Rect.inside(self, x, y) then
		return self
	end
end

function Node:to_dot(out)
	local function lbl(s)
		local r = string.gsub(s, '%{', '\\{')
		r = string.gsub(r, '%}', '\\}')
		r = string.gsub(r, '%|', '\\|')
		r = string.gsub(r, '%<', '\\<')
		r = string.gsub(r, '%>', '\\>')
		return r
	end
	out.write('n', self.hex(), ' [color="', self.color, '" label="{')
	if #self.inputs ~= 0 then
		out.write('{')
		local first = true
		for port in self.inputs_iter() do
			if first then
				first = false
			else
				out.write('|')
			end
			out.write('<i_', port.hex(), '> ', lbl(port.get_name()))
		end
		out.write('}|')
	end
	out.write(lbl(self.name))
	if #self.outputs ~= 0 then
		out.write('|{')
		local first = true
		for port in self.outputs_iter() do
			if first then
				first = false
			else
				out.write('|')
			end
			out.write('<o_', port.hex(), '> ', lbl(port.get_name()))
		end
		out.write('}')
	end
	out.write('}"];')
end

function Node:get_tip(out)
	out.write('Node ', self.name)
end

function Node:config(props)
	props.text_input(
		'Name',
		function() return self.name end,
		function(v) self.name = v end,
		function(v) return #v ~= 0 end
	)
	props.memo_input(
		'Comment',
		function() return self.comment end,
		function(v) self.comment = v end,
		function(v) return true end
	)
	for port in self.inputs_iter() do
		props.text_input(
			'Input port ' .. port.get_id(),
			function() return port.get_name() end,
			function(v) port.set_name(v) end,
			function(v) return #v ~= 0 end
		)
	end
	for port in self.outputs_iter() do
		props.text_input(
			'Output port ' .. port.get_id(),
			function() return port.get_name() end,
			function(v) port.set_name(v) end,
			function(v) return #v ~= 0 end
		)
	end
end

function Node:get_comment(out)
	out.write(self.comment)
end

function Node:get_type(port)
	error('must implement the get_type() method in class ' .. self.class_name())
end

function Node:gen_code(out, scratch)
	error('must implement the gen_code() method in class ' .. self.class_name())
end

Return = class(Node)

Edge = class()

function Edge:new(source, target)
	if not source.instance_of(Output) then
		error('source node must be a Node')
	elseif not target.instance_of(Input) then
		error('target node must be an Input')
	end
	self.source = source
	self.target = target
end

function Edge:to_dot(out)
	local source = self.source
	local target = self.target
	out.write('n', source.owner.hex(), ':o_', source.hex(), ' -> n', target.owner.hex(), ':i_' .. target.hex())
end

Graph = class(Rect)

function Graph:new()
	self.nodes = {}
	self.edges = {}
	self.prologue = ''
	self.epilogue = ''
	self.gen_comments = true
	self.start_comment = '/*'
	self.end_comment = '*/'
	c_lib.set_modified(false)
end

function Graph:config(props)
	props.memo_input(
		'Prologue',
		function() return self.prologue end,
		function(v) self.prologue = string.gsub(v, '\\n', '\n') self.update() end,
		function(v) return true end
	)
	props.memo_input(
		'Epilogue',
		function() return self.epilogue end,
		function(v) self.epilogue = string.gsub(v, '\\n', '\n') self.update() end,
		function(v) return true end
	)
	props.check_box(
		'Generate comments',
		function() return self.gen_comments end,
		function(v) self.gen_comments = v self.update() end
	)
	props.text_input(
		'Start comment',
		function() return self.start_comment end,
		function(v) self.start_comment = v self.update() end,
		function(v) return true end
	)
	props.text_input(
		'End comment',
		function() return self.end_comment end,
		function(v) self.end_comment = v self.update() end,
		function(v) return true end
	)
end

function Graph:add_node(node)
	local nodes = self.nodes
	nodes[#nodes + 1] = node
	nodes[node.hex()] = node
	node.owner = self
	self.update()
	c_lib.set_modified(true)
end

function Graph:add_edge(edge)
	self.edges[edge] = 1
	self.update()
	c_lib.set_modified(true)
end
	
function Graph:del_node(node)
	local nodes = self.nodes
	local index
	for i, n in ipairs(nodes) do
		if n == node then
			index = i
			break
		end
	end
	if index then
		for i = index, #nodes do
			nodes[i] = nodes[i + 1]
		end
		nodes[node.hex()] = nil
		local delete = {}
		for edge in pairs(self.edges) do
			if edge.source == node or edge.target == node then
				delete[edge] = true
			end
		end
		for edge in pairs(delete) do
			self.edges[edge] = nil
		end
		self.update()
		c_lib.set_modified(true)
	end
end

function Graph:del_edge(edge)
	self.edges[edge] = nil
	self.update()
	c_lib.set_modified(true)
end

function Graph:nodes_iter()
	return
		function(data)
			local value = data.tab[data.i]
			data.i = data.i + 1
			return value
		end,
		{ i = 1, tab = self.nodes }
end
	
function Graph:edges_iter()
	return pairs(self.edges)
end

function Graph:inside(x, y)
	for node in self.nodes_iter() do
		local found = node.inside(x, y)
		if found then
			return found
		end
	end
	return self
end

function Graph:find_edges(source, target)
	local edges = {}
	if source and target then
		for edge in self.edges_iter() do
			if edge.source == source and edge.target == target then
				edges[#edges + 1] = edge
			end
		end
	elseif source then
		for edge in self.edges_iter() do
			if edge.source == source then
				edges[#edges + 1] = edge
			end
		end
	elseif target then
		for edge in self.edges_iter() do
			if edge.target == target then
				edges[#edges + 1] = edge
			end
		end
	end
	return edges
end

function Graph:find_output(input)
	for edge in self.edges_iter() do
		if edge.target == input then
			return edge.source
		end
	end
end

function Graph:to_dot(out)
	out.write('digraph G {\n')
	out.write('\trankdir=TB;\n')
	out.write('\tnode [shape=record, fontname=Helvetica, fontsize=10];\n')
	for node in self.nodes_iter() do
		out.write('\t')
		node.to_dot(out)
		out.write('\n')
	end
	for edge in self.edges_iter() do
		out.write('\t')
		edge.to_dot(out)
		out.write('\n')
	end
	out.write('}\n')
end

function Graph:update()
	self.gen_code()
	local out = StringOutputStream()
	self.to_dot(out)
	local dot = out.close()
	--print(dot)
	c_lib.set_dot_data(dot)
	local xdot = c_lib.get_xdot_data()
	--print(xdot)
	self.set_pos()
	for hex, rects in gmatch(xdot, 'n(%x%x%x%x%x%x%x%x).-rects="(.-)"') do
		local node = self.nodes[hex]
		local border = Rect()
		local f, s, v = gmatch(rects, '(%d+),(%d+),(%d+),(%d+)')
		for port in node.inputs_iter() do
			local x0, y0, x1, y1 = f(s, v)
			v = x0
			x0, y0, x1, y1 = x0 + 0, y0 + 0, x1 + 0, y1 + 0
			port.set_pos(x0, y0, x1, y1)
			self.union(port)
			border.union(port)
		end
		local x0, y0, x1, y1 = f(s, v)
		v = x0
		x0, y0, x1, y1 = x0 + 0, y0 + 0, x1 + 0, y1 + 0
		self.union(x0, y0, x1, y1)
		border.union(x0, y0, x1, y1)
		for port in node.outputs_iter() do
			local x0, y0, x1, y1 = f(s, v)
			v = x0
			x0, y0, x1, y1 = x0 + 0, y0 + 0, x1 + 0, y1 + 0
			port.set_pos(x0, y0, x1, y1)
			self.union(port)
			border.union(port)
		end
		node.set_pos(border.x0, border.y0, border.x1, border.y1)
	end
	c_lib.graph_redraw()
end

function Graph:redraw()
	c_lib.graph_redraw()
end

function Graph:gen_code()
	local function push(stack, node)
		for index, pushed in ipairs(stack) do
			if node == pushed then
				return
			end
		end
		for port in node.inputs_iter() do
			for edge in self.edges_iter() do
				if edge.target == port then
					push(stack, edge.source.owner)
				end
			end
		end
		stack[#stack + 1] = node
	end
	c_lib.set_code('')
	local nok = {}
	for node in self.nodes_iter() do
		node.color = 'black'
		for port in node.inputs_iter() do
			local edges = self.find_edges(nil, port)
			if #edges == 0 then
				c_lib.add_code('Port ', port.owner.name, ':', port.name, ' is disconnected\n')
				node.color = 'red4'
				nok[node] = true
			end
		end
		for port in node.outputs_iter() do
			local edges = self.find_edges(port, nil)
			if #edges == 0 then
				c_lib.add_code('Port ', port.owner.name, ':', port.name, ' is disconnected\n')
				node.color = 'red4'
				nok[node] = true
			end
		end
	end
	if next(nok) == nil then
		for node in self.nodes_iter() do
			for port in node.outputs_iter() do
				if node.get_type(port) == nil then
					c_lib.add_code('Type mismatch in node ', node.name, '\n')
					node.color = 'red4'
					nok[node] = true
				end
			end
		end
	end
	local stack = {}
	for node in self.nodes_iter() do
		if not node.instance_of(Return) then
			push(stack, node)
		end
	end
	local epfound = false
	for node in self.nodes_iter() do
		if node.instance_of(Return) then
			if epfound then
				c_lib.add_code('There are multiple returns')
				node.color = 'red4'
				nok[node] = true
			else
				epfound = true
				push(stack, node)
			end
		end
	end
	if next(nok) ~= nil then
		return
	end
	--print('Generating code...')
	local out = CodeOutputStream()
	if self.gen_comments then
		out.write(self.start_comment, ' prologue ', self.end_comment, '\n')
	end
	out.write(self.prologue, '\n')
	local scratch = {}
	for i = 1, #stack do
		if self.gen_comments then
			out.write('\t', self.start_comment, ' ', stack[i].name, ' ', self.end_comment, '\n')
		end
		stack[i].gen_code(out, scratch)
	end
	if self.gen_comments then
		out.write(self.start_comment, ' epilogue ', self.end_comment, '\n')
	end
	out.write(self.epilogue, '\n')
end

function Graph:save(out)
	local function set_class(tab, visited)
		if visited[tab] then
			return
		end
		visited[tab] = true
		if tab.class_name then
			tab.__class = tab.class_name()
		end
		for key, value in pairs(tab) do
			if type(key) == 'table' then
				set_class(key, visited)
			end
			if type(value) == 'table' then
				set_class(value, visited)
			end
		end
	end
	set_class(self, {})
	out.write(pickle(self))
	c_lib.set_modified(false)
end

function Graph:open(inp)
	local graph = unpickle(inp.read_all())
	local function set_class(tab, visited)
		if visited[tab] then
			return
		end
		visited[tab] = true
		if tab.__class then
			local class = class_by_name(tab.__class)
			if class == nil then
				error('Unknown class ' .. tab.__class)
			end
			class.make_instance(tab)
			tab.__class = nil
		end
		for key, value in pairs(tab) do
			if type(key) == 'table' then
				set_class(key, visited)
			end
			if type(value) == 'table' then
				set_class(value, visited)
			end
		end
	end
	set_class(graph, {})
	local del = {}
	for key in pairs(self) do
		del[key] = true
	end
	for key in pairs(del) do
		self[key] = nil
	end
	for key, value in pairs(graph) do
		self[key] = value
	end
	local del = {}
	for key in pairs(self.nodes) do
		if type(key) == 'string' then
			del[key] = true
		end
	end
	for key in pairs(del) do
		self.nodes[key] = nil
	end
	for index, node in ipairs(self.nodes) do
		self.nodes[node.hex()] = node
	end
	self.update()
	c_lib.set_modified(false)
end

Undo = class()

function Undo:new()
	self.list = { n = 0 }
	c_lib.menu_ctrl('undo', false)
	c_lib.menu_ctrl('redo', false)
end

function Undo:add(undo, redo)
	self.list.n = self.list.n + 1
	self.list[self.list.n] = { undo = undo, redo = redo }
	local last = self.list.n + 1
	while self.list[last] ~= nil do
		last = last + 1
	end
	for index = self.list.n + 1, last - 1 do
		self.list[index] = nil
	end
	c_lib.menu_ctrl('undo', true)
	c_lib.menu_ctrl('redo', false)
	redo()
end

function Undo:undo()
	local index = self.list.n
	if index ~= 0 then
		local undo = self.list[index].undo
		self.list.n = index - 1
		c_lib.menu_ctrl('undo', index > 1)
		c_lib.menu_ctrl('redo', true)
		undo()
	end
end

function Undo:redo()
	local index = self.list.n + 1
	if self.list[index] ~= nil then
		self.list.n = index
		c_lib.menu_ctrl('undo', true)
		c_lib.menu_ctrl('redo', self.list[index + 1] ~= nil)
		self.list[index].redo()
	end
end
