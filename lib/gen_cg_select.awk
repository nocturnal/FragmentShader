BEGIN {
  select_count  = 0;
  compare_count = 0;
  comparez_count = 0;
  FS = "\t";
}

{
  if ($1 == "select")
  {
    _node_name = $2;
    _name      = $3;
    _operator  = $4;

    select_data[ select_count, "name" ]      = _name;
    select_data[ select_count, "node_name" ] = _node_name;
    select_data[ select_count, "operator" ]  = _operator;
    select_count++;  
  }

  if ($1 == "compare")
  {
    _node_name = $2;
    _name      = $3;
    _operator  = $4;

    compare_data[ compare_count, "name" ]      = _name;
    compare_data[ compare_count, "node_name" ] = _node_name;
    compare_data[ compare_count, "operator" ]  = _operator;
    compare_count++;  
  }

  if ($1 == "comparez")
  {
    _node_name = $2;
    _name      = $3;
    _operator  = $4;

    comparez_data[ comparez_count, "name" ]      = _name;
    comparez_data[ comparez_count, "node_name" ] = _node_name;
    comparez_data[ comparez_count, "operator" ]  = _operator;
    comparez_count++;  
  }
}

END {
  output_select_data();
  output_compare_data();
  output_comparez_data();
  output_node_table();
}

function output_node_table( _node_name, _name, _operator )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("register {\n");
  printf("  __name = 'Cg [Select]',\n");
  printf("  __prologue = '',\n");
  printf("  __epilogue = '',\n");

  for (i=0;i<select_count;i++)
  {
    _node_name = select_data[ i, "node_name" ];
    _name      = select_data[ i, "name" ];
    _operator  = select_data[ i, "operator" ];

    printf("  ['%s'] = %s,\n",_name,_node_name);
  }

  for (i=0;i<compare_count;i++)
  {
    _node_name = compare_data[ i, "node_name" ];
    _name      = compare_data[ i, "name" ];
    _operator  = compare_data[ i, "operator" ];

    printf("  ['%s'] = %s,\n",_name,_node_name);
  }

  for (i=0;i<comparez_count;i++)
  {
    _node_name = comparez_data[ i, "node_name" ];
    _name      = comparez_data[ i, "name" ];
    _operator  = comparez_data[ i, "operator" ];

    printf("  ['%s'] = %s,\n",_name,_node_name);
  }

  printf("}\n");
  printf("\n");
}

function output_select_data( _node_name, _name, _operator )
{
  for (i=0;i<select_count;i++)
  {
    _node_name = select_data[ i, "node_name" ];
    _name      = select_data[ i, "name" ];
    _operator  = select_data[ i, "operator" ];

    printf("-- #################################################### --\n");
    printf("\n");
    printf("local %s = class(Node)\n", _node_name );
    printf("\n");
    printf("function %s:get_help(out)\n", _node_name );
    printf("    out.write( 'd = if ( x %s y ) then a else b' )\n", _operator );
    printf("    out.write( '\\n' )\n", _operator );
    printf("    out.write( 'Supported Types:\\n' )\n", _operator );
    printf("    out.write( 'float\\n' ) \n");
    printf("    out.write( 'half\\n' ) \n");
    printf("    out.write( 'fixed\\n' ) \n");
    printf("    out.write( 'int\\n' ) \n");
    printf("    out.write( 'float4\\n' ) \n");
    printf("    out.write( 'half4\\n' ) \n");
    printf("    out.write( 'fixed4\\n' ) \n");
    printf("    out.write( 'int4\\n' ) \n");
    printf("    out.write( 'float3\\n' ) \n");
    printf("    out.write( 'half3\\n' ) \n");
    printf("    out.write( 'fixed3\\n' ) \n");
    printf("    out.write( 'int3\\n' ) \n");
    printf("    out.write( 'float2\\n' ) \n");
    printf("    out.write( 'half2\\n' ) \n");
    printf("    out.write( 'fixed2\\n' ) \n");
    printf("    out.write( 'int2\\n' ) \n");
    printf("end\n");
    printf("\n");
    printf("function %s:new()\n", _node_name );
    printf("    Node.new(self)\n");
    printf("    self.x = self.add_port(Input('x'))\n");
    printf("    self.y = self.add_port(Input('y'))\n");
    printf("    self.a = self.add_port(Input('a'))\n");
    printf("    self.b = self.add_port(Input('b'))\n");
    printf("    self.d = self.add_port(Output('d'))\n");
    printf("end\n");
    printf("\n");
    printf("function %s:get_type(port)\n", _node_name );
    printf("\n");
    printf("    local tx = self.x.get_output().get_type()\n");
    printf("    local ty = self.y.get_output().get_type()\n");
    printf("    local ta = self.a.get_output().get_type()\n");
    printf("    local tb = self.b.get_output().get_type()\n");
    printf("\n");
    printf("    if (   ( ta == 'float' ) \n");
    printf("       or ( ta == 'half' ) \n");
    printf("       or ( ta == 'fixed' ) \n");
    printf("       or ( ta == 'int' ) \n");
    printf("       or ( ta == 'float4' ) \n");
    printf("       or ( ta == 'half4' ) \n");
    printf("       or ( ta == 'fixed4' ) \n");
    printf("       or ( ta == 'int4' ) \n");
    printf("       or ( ta == 'float3' ) \n");
    printf("       or ( ta == 'half3' ) \n");
    printf("       or ( ta == 'fixed3' ) \n");
    printf("       or ( ta == 'int3' ) \n");
    printf("       or ( ta == 'float2' ) \n");
    printf("       or ( ta == 'half2' ) \n");
    printf("       or ( ta == 'fixed2' ) \n");
    printf("       or ( ta == 'int2' )) then \n"); 
    printf("        \n");
    printf("        if ((tx == ty) and (ta == tb)) then\n");
    printf("            return ta\n");
    printf("        end\n");
    printf("    end\n");
    printf("\n");
    printf("    return nil\n");
    printf("end\n");
    printf("\n");
    printf("function %s:config(props)\n", _node_name );
    printf("    Node.config(self, props)\n");
    printf("    props.delete('Name')\n");
    printf("end\n");
    printf("\n");
    printf("function %s:gen_code(out, scratch)\n", _node_name );
    printf("    local ta   = self.a.get_output().get_type()\n");
    printf("    local t    = ta\n");
    printf("    local x    = 'r' .. self.x.get_output().hex()\n");
    printf("    local y    = 'r' .. self.y.get_output().hex()\n");
    printf("    local a    = 'r' .. self.a.get_output().hex()\n");
    printf("    local b    = 'r' .. self.b.get_output().hex()\n");
    printf("    local d    = 'r' .. self.d.hex()\n");
    printf("\n");
    printf("    if (   ( ta == 'float' ) \n");
    printf("       or ( ta == 'half' ) \n");
    printf("       or ( ta == 'float4' ) \n");
    printf("       or ( ta == 'float3' ) \n");
    printf("       or ( ta == 'half3' ) \n");
    printf("       or ( ta == 'float2' ) \n");
    printf("       or ( ta == 'half2' )) then \n");
    printf("        out.write( '\t' .. t .. ' ' .. d .. ' = lerp(' .. b .. ', ' .. a .. ', ' .. t .. '(' .. x .. ' %s ' .. y .. ') );' .. '\\n' );\n", _operator );
    printf("    else\n");
    printf("        out.write( '\t' .. t .. ' ' .. d .. ' = (' .. t .. '(' .. x .. ' %s ' .. y .. ') * ' .. a .. ') ' )\n",_operator );
    printf("        out.write( '+ (' .. t .. '(!(' .. x .. ' %s ' .. y .. ')) * ' .. b .. ') ' )\n",_operator);
    printf("        out.write( ';\\n' )\n");
    printf("    end\n");

    printf("end\n");
    printf("\n");
    printf("function %s:get_tip(out)\n", _node_name );
    printf("    out.write( 'd = if ( x %s y ) then a else b' )", _operator );
    printf("end\n");
    printf("\n");
  }
}

function output_compare_data( _node_name, _name, _operator )
{
  for (i=0;i<compare_count;i++)
  {
    _node_name = compare_data[ i, "node_name" ];
    _name      = compare_data[ i, "name" ];
    _operator  = compare_data[ i, "operator" ];

    printf("-- #################################################### --\n");
    printf("\n");
    printf("local %s = class(Node)\n", _node_name );
    printf("\n");
    printf("function %s:get_help(out)\n", _node_name );
    printf("    out.write( 'd = if ( x %s y ) then (1) else (0)' )\n", _operator );
    printf("    out.write( '\\n' )\n", _operator );
    printf("    out.write( 'Supported Types:\\n' )\n", _operator );
    printf("    out.write( 'float\\n' ) \n");
    printf("    out.write( 'half\\n' ) \n");
    printf("    out.write( 'fixed\\n' ) \n");
    printf("    out.write( 'int\\n' ) \n");
    printf("    out.write( 'float4\\n' ) \n");
    printf("    out.write( 'half4\\n' ) \n");
    printf("    out.write( 'fixed4\\n' ) \n");
    printf("    out.write( 'int4\\n' ) \n");
    printf("    out.write( 'float3\\n' ) \n");
    printf("    out.write( 'half3\\n' ) \n");
    printf("    out.write( 'fixed3\\n' ) \n");
    printf("    out.write( 'int3\\n' ) \n");
    printf("    out.write( 'float2\\n' ) \n");
    printf("    out.write( 'half2\\n' ) \n");
    printf("    out.write( 'fixed2\\n' ) \n");
    printf("    out.write( 'int2\\n' ) \n");
    printf("end\n");
    printf("\n");
    printf("function %s:new()\n", _node_name );
    printf("    Node.new(self)\n");
    printf("    self.x = self.add_port(Input('x'))\n");
    printf("    self.y = self.add_port(Input('y'))\n");
    printf("    self.d = self.add_port(Output('d'))\n");

    printf("        self.type_choices = { 'float',\n");
    printf("                              'half',  \n");
    printf("                              'fixed',  \n");
    printf("                              'int',  \n");
    printf("                              'bool',  \n");
    printf("                              'float4',  \n");
    printf("                              'half4',  \n");
    printf("                              'fixed4',  \n");
    printf("                              'int4',  \n");
    printf("                              'bool4',  \n");
    printf("                              'float3',  \n");
    printf("                              'half3',  \n");
    printf("                              'fixed3',  \n");
    printf("                              'int3',  \n");
    printf("                              'bool3',  \n");
    printf("                              'float2',  \n");
    printf("                              'half2',  \n");
    printf("                              'fixed2',  \n");
    printf("                              'int2',  \n");
    printf("                              'bool2' }\n");
    printf("        self.type_ndx = 1\n");

    printf("end\n");
    printf("\n");
    printf("function %s:get_type(port)\n", _node_name );
    printf("    return self.type_choices[self.type_ndx]\n");
    printf("end\n");
    printf("\n");

    printf("function %s:config(props)\n", _node_name );
    printf("    Node.config(self, props)\n");
    printf("    props.delete('Name')\n");
    printf("\n");
    printf("    props.list(\n");
    printf("    'Type',\n");
    printf("    self.type_choices,\n");
    printf("    function() return self.type_ndx end,\n");
    printf("    function(v) self.type_ndx = v end\n");
    printf("    )\n");
    printf("end\n");

    printf("\n");
    printf("function %s:gen_code(out, scratch)\n", _node_name );
    printf("    local ta   = self.type_choices[self.type_ndx]\n");
    printf("    local t    = ta\n");
    printf("    local x    = 'r' .. self.x.get_output().hex()\n");
    printf("    local y    = 'r' .. self.y.get_output().hex()\n");
    printf("    local a    = ta .. '(1)'\n");
    printf("    local b    = ta .. '(0)'\n");
    printf("    local d    = 'r' .. self.d.hex()\n");
    printf("\n");
    printf("    out.write( '\t' .. t .. ' ' .. d .. ' = ' .. t .. '(' .. x .. ' %s ' .. y .. ');\\n')\n",_operator );
    printf("end\n");

    printf("\n");
    printf("function %s:get_tip(out)\n", _node_name );
    printf("    out.write( 'd = if ( x %s y ) then (1) else (0)' )", _operator );
    printf("end\n");
    printf("\n");
  }
}

function output_comparez_data( _node_name, _name, _operator )
{
  for (i=0;i<comparez_count;i++)
  {
    _node_name = comparez_data[ i, "node_name" ];
    _name      = comparez_data[ i, "name" ];
    _operator  = comparez_data[ i, "operator" ];

    printf("-- #################################################### --\n");
    printf("\n");
    printf("local %s = class(Node)\n", _node_name );
    printf("\n");
    printf("function %s:get_help(out)\n", _node_name );
    printf("    out.write( 'd = if ( x %s 0 ) then (1) else (0)' )\n", _operator );
    printf("    out.write( '\\n' )\n", _operator );
    printf("    out.write( 'Supported Types:\\n' )\n", _operator );
    printf("    out.write( 'float\\n' ) \n");
    printf("    out.write( 'half\\n' ) \n");
    printf("    out.write( 'fixed\\n' ) \n");
    printf("    out.write( 'int\\n' ) \n");
    printf("    out.write( 'float4\\n' ) \n");
    printf("    out.write( 'half4\\n' ) \n");
    printf("    out.write( 'fixed4\\n' ) \n");
    printf("    out.write( 'int4\\n' ) \n");
    printf("    out.write( 'float3\\n' ) \n");
    printf("    out.write( 'half3\\n' ) \n");
    printf("    out.write( 'fixed3\\n' ) \n");
    printf("    out.write( 'int3\\n' ) \n");
    printf("    out.write( 'float2\\n' ) \n");
    printf("    out.write( 'half2\\n' ) \n");
    printf("    out.write( 'fixed2\\n' ) \n");
    printf("    out.write( 'int2\\n' ) \n");
    printf("end\n");
    printf("\n");
    printf("function %s:new()\n", _node_name );
    printf("    Node.new(self)\n");
    printf("    self.x = self.add_port(Input('x'))\n");
    printf("    self.d = self.add_port(Output('d'))\n");

    printf("        self.type_choices = { 'float',\n");
    printf("                              'half',  \n");
    printf("                              'fixed',  \n");
    printf("                              'int',  \n");
    printf("                              'bool',  \n");
    printf("                              'float4',  \n");
    printf("                              'half4',  \n");
    printf("                              'fixed4',  \n");
    printf("                              'int4',  \n");
    printf("                              'bool4',  \n");
    printf("                              'float3',  \n");
    printf("                              'half3',  \n");
    printf("                              'fixed3',  \n");
    printf("                              'int3',  \n");
    printf("                              'bool3',  \n");
    printf("                              'float2',  \n");
    printf("                              'half2',  \n");
    printf("                              'fixed2',  \n");
    printf("                              'int2',  \n");
    printf("                              'bool2' }\n");
    printf("        self.type_ndx = 1\n");

    printf("end\n");
    printf("\n");
    printf("function %s:get_type(port)\n", _node_name );
    printf("    return self.type_choices[self.type_ndx]\n");
    printf("end\n");
    printf("\n");

    printf("function %s:config(props)\n", _node_name );
    printf("    Node.config(self, props)\n");
    printf("    props.delete('Name')\n");
    printf("\n");
    printf("    props.list(\n");
    printf("    'Type',\n");
    printf("    self.type_choices,\n");
    printf("    function() return self.type_ndx end,\n");
    printf("    function(v) self.type_ndx = v end\n");
    printf("    )\n");
    printf("end\n");

    printf("\n");
    printf("function %s:gen_code(out, scratch)\n", _node_name );
    printf("    local ta   = self.type_choices[self.type_ndx]\n");
    printf("    local t    = ta\n");
    printf("    local x    = 'r' .. self.x.get_output().hex()\n");
    printf("    local tx   = self.x.get_output().get_type()\n");
    printf("    local d    = 'r' .. self.d.hex()\n");
    printf("\n");
    printf("    out.write( '\t' .. t .. ' ' .. d .. ' = ' .. t .. '(' .. x .. ' %s ' .. tx  .. '(0));\\n')\n",_operator );
    printf("end\n");

    printf("\n");
    printf("function %s:get_tip(out)\n", _node_name );
    printf("    out.write( 'd = if ( x %s 0 ) then (1) else (0)' )", _operator );
    printf("end\n");
    printf("\n");
  }
}

