-- The properties panel
local g_props
-- The graph
local g_graph
-- The selected object
local g_selected
-- The clipboard
local g_clipboard
-- The undo list
local g_undo

local function do_delete(what)
	if what.instance_of(Node) then
		local node = what
		local list = {}
		for edge in g_graph.edges_iter() do
			for port in node.inputs_iter() do
				if edge.target == port then
					list[edge] = true
				end
			end
			for port in node.outputs_iter() do
				if edge.source == port then
					list[edge] = true
				end
			end
		end
		g_undo.add(
			function() g_graph.add_node(node) for edge in pairs(list) do g_graph.add_edge(edge) end end,
			function() for edge in pairs(list) do g_graph.del_edge(edge) end g_graph.del_node(node) end
		)
		if what == g_selected then
			g_props.clear()
			g_props.redraw()
			g_selected = nil
			c_lib.menu_ctrl('copy', false)
			c_lib.set_page('help')
		end
		g_graph.redraw()
	elseif what.instance_of(Port) then
		local edges
		if what.instance_of(Input) then
			edges = g_graph.find_edges(nil, what)
		else
			edges = g_graph.find_edges(what, nil)
		end
		g_undo.add(
			function() for index, edge in ipairs(edges) do g_graph.add_edge(edge) end end,
			function() for index, edge in ipairs(edges) do g_graph.del_edge(edge) end end
		)
		g_graph.redraw()
	end
end

function left_click(x, y, shift, control, alt)
	local under = g_graph.inside(x, y)
	if under == g_selected then
		return
	end
	if not shift and under ~= g_selected then
		g_selected = under
		g_props.clear()
		if g_selected then
			if g_selected.config then
				g_selected.config(g_props)
			elseif g_selected.owner and g_selected.owner.config then
				g_selected.owner.config(g_props)
			end
			c_lib.menu_ctrl('copy', g_selected.instance_of(Node))
		end
		g_props.redraw()
		c_lib.set_page('props')
		if g_selected.instance_of(Node) then
			c_lib.set_status(2)
			c_lib.set_status(3, 'Press delete to remove (will remove all connections)')
		elseif g_selected.instance_of(Input) then
			c_lib.set_status(2, 'Shift-click in a output port to connect')
			c_lib.set_status(3, 'Press delete to remove incoming connection')
		elseif g_selected.instance_of(Output) then
			c_lib.set_status(2, 'Shift-click in a input port to connect')
			c_lib.set_status(3, 'Press delete to remove all outgoing connections')
		else
			c_lib.set_status(2)
			c_lib.set_status(3)
		end
		g_graph.redraw()
		return
	end
	if shift and under and g_selected and under.owner ~= g_selected.owner then
		local source, target
		if under.instance_of(Input) and g_selected.instance_of(Output) then
			source, target = g_selected, under
		elseif under.instance_of(Output) and g_selected.instance_of(Input) then
			source, target = under, g_selected
		else
			return
		end
		local edge = Edge(source, target)
		local del_list, del = g_graph.find_edges(nil, target)
		if #del_list ~= 0 then
			del = del_list[1]
		end
		if del then
			g_undo.add(
				function() g_graph.del_edge(edge) g_graph.add_edge(del) end,
				function() g_graph.del_edge(del) g_graph.add_edge(edge) end
			)
		else
			g_undo.add(
				function() g_graph.del_edge(edge) end,
				function() g_graph.add_edge(edge) end
			)
		end
	end
end

function motion(x, y, shift, control, alt)
	local under = g_graph.inside(x, y)
	if not under.instance_of(Graph) then
		if under.get_comment then
			local out = StringOutputStream()
			under.get_comment(out)
			local comment = out.close()
			c_lib.set_tooltip(comment)
		end
		if under.get_tip then
			local out = StringOutputStream()
			under.get_tip(out)
			out.write(' | click to select')
			local tip = out.close()
			c_lib.set_status(1, tip)
		end
	else
		c_lib.set_tooltip('')
		c_lib.set_status(1)
	end
end

function add_node(namespace, name)
	local classes = _G[namespace]
	local class = classes and classes[name]
	if class then
		local node = class()
		if g_graph.prologue == '' then
			g_graph.prologue = classes.__prologue
		end
		if g_graph.epilogue == '' then
			g_graph.epilogue = classes.__epilogue
		end
		if not node.name then
			node.name = name
		end
		g_selected = node
		c_lib.menu_ctrl('copy', true)
		g_undo.add(
			function() g_graph.del_node(node) end,
			function() g_graph.add_node(node) end
		)
		g_props.clear()
		node.config(g_props)
		g_props.redraw()
		c_lib.set_page('props')
	end
end

function get_help(namespace, name)
	local classes = _G[namespace]
	if name then
		local class = classes and classes[name]
		if class and class.get_help then
			c_lib.set_page('help')
			local out = StringOutputStream()
			class.get_help(nil, out)
			return out.close()
		end
	elseif classes.__help then
		c_lib.set_page('help')
		return classes.__help
	end
	c_lib.set_page('props')
	return ''
end

function finish(dc)
	if g_selected and (g_selected.instance_of(Node) or g_selected.instance_of(Port)) then
		dc.invert(g_selected.x0, g_selected.y0, g_selected.x1, g_selected.y1)
	end
end

function delete()
	if g_selected then
		do_delete(g_selected)
	end
end

function new()
	g_graph = Graph()
	g_props = Properties(g_graph)
	g_undo = Undo()
	g_selected = nil
	g_clipboard = nil
	c_lib.menu_ctrl('copy', false)
	c_lib.menu_ctrl('paste', false)
	c_lib.set_page('help')
	g_graph.update()
end

function undo()
	g_undo.undo()
	g_selected = nil
end

function redo()
	g_undo.redo()
	g_selected = nil
end

function cut()
	if g_selected and g_selected.instance_of(Node) then
		g_clipboard = g_selected
		do_delete(g_selected)
		g_selected = nil
		c_lib.menu_ctrl('copy', false)
		c_lib.menu_ctrl('paste', true)
	end
end

function copy()
	if g_selected and g_selected.instance_of(Node) then
		g_clipboard = g_selected
		c_lib.menu_ctrl('paste', true)
	end
end

function paste()
	if g_clipboard then
		local node = g_clipboard.clone()
		g_undo.add(
			function() g_graph.del_node(node) end,
			function() g_graph.add_node(node) end
		)
	end
end

function duplicate()
	if g_selected and g_selected.instance_of(Node) then
		local node = g_selected.clone()
		g_undo.add(
			function() g_graph.del_node(node) end,
			function() g_graph.add_node(node) end
		)
	end
end

function save(name)
	local out = FileOutputStream(name)
	g_graph.save(out)
	out.close()
end

function open(name)
	new()
	local inp = FileInputStream(name)
	g_graph.open(inp)
	inp.close()
end
