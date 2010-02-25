BEGIN {
  op2_all_count     = 0;
  op1_pre_all_count = 0;
  op2_int_count     = 0;
  op1_post_int_count= 0;
}

{
  if ($1 == "op2_all")
  {
    _node_name = $2;
    _name      = $3;
    _operator  = $4;

    op2_all_data[ op2_all_count, "name" ]      = _name;
    op2_all_data[ op2_all_count, "node_name" ] = _node_name;
    op2_all_data[ op2_all_count, "operator" ]  = _operator;
    op2_all_count++;  
  }
  else if ($1 == "op1_pre_all")
  {
    _node_name = $2;
    _name      = $3;
    _operator  = $4;

    op1_pre_all_data[ op1_pre_all_count, "name" ]      = _name;
    op1_pre_all_data[ op1_pre_all_count, "node_name" ] = _node_name;
    op1_pre_all_data[ op1_pre_all_count, "operator" ]  = _operator;
    op1_pre_all_count++;  
  }
  else if ($1 == "op2_int")
  {
    _node_name = $2;
    _name      = $3;
    _operator  = $4;

    op2_int_data[ op2_int_count, "name" ]      = _name;
    op2_int_data[ op2_int_count, "node_name" ] = _node_name;
    op2_int_data[ op2_int_count, "operator" ]  = _operator;
    op2_int_count++;  
  }
  else if ($1 == "op1_post_int")
  {
    _node_name = $2;
    _name      = $3;
    _operator  = $4;

    op1_post_int_data[ op1_post_int_count, "name" ]      = _name;
    op1_post_int_data[ op1_post_int_count, "node_name" ] = _node_name;
    op1_post_int_data[ op1_post_int_count, "operator" ]  = _operator;
    op1_post_int_count++;  
  }
}

END {

  output_op2_all_data();
  output_op1_pre_all_data()
  output_op2_int_data()
  output_op1_post_int_data()
  output_node_table();
}

function output_node_table( _node_name, _name, _operator )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("register {\n");
  printf("  __name = 'Cg [Arithmetic Operators]',\n");
  printf("  __prologue = '',\n");
  printf("  __epilogue = '',\n");

  for (i=0;i<op2_all_count;i++)
  {
    _node_name = op2_all_data[ i, "node_name" ];
    _name      = op2_all_data[ i, "name" ];
    _operator  = op2_all_data[ i, "operator" ];

    printf("  ['%s'] = %s,\n",_name,_node_name);
  }

  for (i=0;i<op1_pre_all_count;i++)
  {
    _node_name = op1_pre_all_data[ i, "node_name" ];
    _name      = op1_pre_all_data[ i, "name" ];
    _operator  = op1_pre_all_data[ i, "operator" ];

    printf("  ['%s'] = %s,\n",_name,_node_name);
  }

  for (i=0;i<op2_int_count;i++)
  {
    _node_name = op2_int_data[ i, "node_name" ];
    _name      = op2_int_data[ i, "name" ];
    _operator  = op2_int_data[ i, "operator" ];

    printf("  ['%s'] = %s,\n",_name,_node_name);
  }

  for (i=0;i<op1_post_int_count;i++)
  {
    _node_name = op1_post_int_data[ i, "node_name" ];
    _name      = op1_post_int_data[ i, "name" ];
    _operator  = op1_post_int_data[ i, "operator" ];

    printf("  ['%s'] = %s,\n",_name,_node_name);
  }

  printf("}\n");
  printf("\n");
}

function output_op2_all_data( _node_name, _name, _operator )
{
  for (i=0;i<op2_all_count;i++)
  {
    _node_name = op2_all_data[ i, "node_name" ];
    _name      = op2_all_data[ i, "name" ];
    _operator  = op2_all_data[ i, "operator" ];

    printf("-- #################################################### --\n");
    printf("\n");
    printf("local %s = class(Node)\n", _node_name );
    printf("\n");
    printf("function %s:get_help(out)\n", _node_name );
    printf("	out.write( 'd = x %s y' )\n", _operator );
    printf("	out.write( '\\n' )\n", _operator );
    printf("	out.write( 'Supported Types:\\n' )\n", _operator );
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
    printf("    out.write( 'float4x4\\n' ) \n");
    printf("    out.write( 'half4x4\\n' ) \n");
    printf("    out.write( 'fixed4x4\\n' ) \n");
    printf("    out.write( 'int4x4\\n' ) \n");
    printf("    out.write( 'float4x3\\n' ) \n");
    printf("    out.write( 'half4x3\\n' ) \n");
    printf("    out.write( 'fixed4x3\\n' ) \n");
    printf("    out.write( 'int4x3\\n' ) \n");
    printf("    out.write( 'float4x2\\n' ) \n");
    printf("    out.write( 'half4x2\\n' ) \n");
    printf("    out.write( 'fixed4x2\\n' ) \n");
    printf("    out.write( 'int4x2\\n' ) \n");
    printf("    out.write( 'float3x4\\n' ) \n");
    printf("    out.write( 'half3x4\\n' ) \n");
    printf("    out.write( 'fixed3x4\\n' ) \n");
    printf("    out.write( 'int3x4\\n' ) \n");
    printf("    out.write( 'float3x3\\n' ) \n");
    printf("    out.write( 'half3x3\\n' ) \n");
    printf("    out.write( 'fixed3x3\\n' ) \n");
    printf("    out.write( 'int3x3\\n' ) \n");
    printf("    out.write( 'float3x2\\n' ) \n");
    printf("    out.write( 'half3x2\\n' ) \n");
    printf("    out.write( 'fixed3x2\\n' ) \n");
    printf("    out.write( 'int3x2\\n' ) \n");
    printf("    out.write( 'float2x4\\n' ) \n");
    printf("    out.write( 'half2x4\\n' ) \n");
    printf("    out.write( 'fixed2x4\\n' ) \n");
    printf("    out.write( 'int2x4\\n' ) \n");
    printf("    out.write( 'float2x3\\n' ) \n");
    printf("    out.write( 'half2x3\\n' ) \n");
    printf("    out.write( 'fixed2x3\\n' ) \n");
    printf("    out.write( 'int2x3\\n' ) \n");
    printf("    out.write( 'float2x2\\n' ) \n");
    printf("    out.write( 'half2x2\\n' ) \n");
    printf("    out.write( 'fixed2x2\\n' ) \n");
    printf("    out.write( 'int2x2\\n' )  \n");
    printf("end\n");
    printf("\n");
    printf("function %s:new()\n", _node_name );
    printf("	Node.new(self)\n");
    printf("	self.a = self.add_port(Input('a'))\n");
    printf("	self.b = self.add_port(Input('b'))\n");
    printf("	self.d = self.add_port(Output('d'))\n");
    printf("end\n");
    printf("\n");
    printf("function %s:get_type(port)\n", _node_name );
    printf("\n");
    printf("	local tx = self.a.get_output().get_type()\n");
    printf("	local ty = self.b.get_output().get_type()\n");
    printf("\n");
    printf("        if (   ( tx == 'float' ) \n");
    printf("            or ( tx == 'half' ) \n");
    printf("            or ( tx == 'fixed' ) \n");
    printf("            or ( tx == 'int' ) \n");
    printf("            or ( tx == 'float4' ) \n");
    printf("            or ( tx == 'half4' ) \n");
    printf("            or ( tx == 'fixed4' ) \n");
    printf("            or ( tx == 'int4' ) \n");
    printf("            or ( tx == 'float3' ) \n");
    printf("            or ( tx == 'half3' ) \n");
    printf("            or ( tx == 'fixed3' ) \n");
    printf("            or ( tx == 'int3' ) \n");
    printf("            or ( tx == 'float2' ) \n");
    printf("            or ( tx == 'half2' ) \n");
    printf("            or ( tx == 'fixed2' ) \n");
    printf("            or ( tx == 'int2' ) \n");
    printf("            or ( tx == 'float4x4' ) \n");
    printf("            or ( tx == 'half4x4' ) \n");
    printf("            or ( tx == 'fixed4x4' ) \n");
    printf("            or ( tx == 'int4x4' ) \n");
    printf("            or ( tx == 'float4x3' ) \n");
    printf("            or ( tx == 'half4x3' ) \n");
    printf("            or ( tx == 'fixed4x3' ) \n");
    printf("            or ( tx == 'int4x3' ) \n");
    printf("            or ( tx == 'float4x2' ) \n");
    printf("            or ( tx == 'half4x2' ) \n");
    printf("            or ( tx == 'fixed4x2' ) \n");
    printf("            or ( tx == 'int4x2' ) \n");
    printf("            or ( tx == 'float3x4' ) \n");
    printf("            or ( tx == 'half3x4' ) \n");
    printf("            or ( tx == 'fixed3x4' ) \n");
    printf("            or ( tx == 'int3x4' ) \n");
    printf("            or ( tx == 'float3x3' ) \n");
    printf("            or ( tx == 'half3x3' ) \n");
    printf("            or ( tx == 'fixed3x3' ) \n");
    printf("            or ( tx == 'int3x3' ) \n");
    printf("            or ( tx == 'float3x2' ) \n");
    printf("            or ( tx == 'half3x2' ) \n");
    printf("            or ( tx == 'fixed3x2' ) \n");
    printf("            or ( tx == 'int3x2' ) \n");
    printf("            or ( tx == 'float2x4' ) \n");
    printf("            or ( tx == 'half2x4' ) \n");
    printf("            or ( tx == 'fixed2x4' ) \n");
    printf("            or ( tx == 'int2x4' ) \n");
    printf("            or ( tx == 'float2x3' ) \n");
    printf("            or ( tx == 'half2x3' ) \n");
    printf("            or ( tx == 'fixed2x3' ) \n");
    printf("            or ( tx == 'int2x3' ) \n");
    printf("            or ( tx == 'float2x2' ) \n");
    printf("            or ( tx == 'half2x2' ) \n");
    printf("            or ( tx == 'fixed2x2' ) \n");
    printf("            or ( tx == 'int2x2' ) ) then\n");
    printf("        \n");
    printf("	    if ((tx == ty)) then\n");
    printf("		    return tx\n");
    printf("	    end\n");
    printf("	end\n");
    printf("\n");
    printf("	return nil\n");
    printf("end\n");
    printf("\n");
    printf("function %s:config(props)\n", _node_name );
    printf("	Node.config(self, props)\n");
    printf("	props.delete('Name')\n");
    printf("end\n");
    printf("\n");
    printf("function %s:gen_code(out, scratch)\n", _node_name );
    printf("	local tx   = self.a.get_output().get_type()\n");
    printf("	local t    = tx\n");
    printf("	local x    = 'r' .. self.a.get_output().hex()\n");
    printf("	local y    = 'r' .. self.b.get_output().hex()\n");
    printf("	local d    = 'r' .. self.d.hex()\n");
    printf("\n");
    printf("	out.write( '\t' .. t .. ' ' .. d .. ' = ' .. x .. ' %s ' .. y .. ';\\n' )\n", _operator );
    printf("end\n");
    printf("\n");
    printf("function %s:get_tip(out)\n", _node_name );
    printf("	out.write( 'd = x %s y' )\n", _operator );
    printf("end\n");
    printf("\n");
  }
}

function output_op2_int_data( _node_name, _name, _operator )
{
  for (i=0;i<op2_int_count;i++)
  {
    _node_name = op2_int_data[ i, "node_name" ];
    _name      = op2_int_data[ i, "name" ];
    _operator  = op2_int_data[ i, "operator" ];

    printf("-- #################################################### --\n");
    printf("\n");
    printf("local %s = class(Node)\n", _node_name );
    printf("\n");
    printf("function %s:get_help(out)\n", _node_name );
    printf("	out.write( 'd = x %s y' )\n", _operator );
    printf("	out.write( '\\n' )\n", _operator );
    printf("	out.write( 'Supported Types:\\n' )\n", _operator );
    printf("    out.write( 'int\\n' ) \n");
    printf("    out.write( 'int4\\n' ) \n");
    printf("    out.write( 'int3\\n' ) \n");
    printf("    out.write( 'int2\\n' ) \n");
    printf("    out.write( 'int4x4\\n' ) \n");
    printf("    out.write( 'int4x3\\n' ) \n");
    printf("    out.write( 'int4x2\\n' ) \n");
    printf("    out.write( 'int3x4\\n' ) \n");
    printf("    out.write( 'int3x3\\n' ) \n");
    printf("    out.write( 'int3x2\\n' ) \n");
    printf("    out.write( 'int2x4\\n' ) \n");
    printf("    out.write( 'int2x3\\n' ) \n");
    printf("    out.write( 'int2x2\\n' )  \n");
    printf("end\n");
    printf("\n");
    printf("function %s:new()\n", _node_name );
    printf("	Node.new(self)\n");
    printf("	self.a = self.add_port(Input('a'))\n");
    printf("	self.b = self.add_port(Input('b'))\n");
    printf("	self.d = self.add_port(Output('d'))\n");
    printf("end\n");
    printf("\n");
    printf("function %s:get_type(port)\n", _node_name );
    printf("\n");
    printf("	local tx = self.a.get_output().get_type()\n");
    printf("	local ty = self.b.get_output().get_type()\n");
    printf("\n");
    printf("        if (   ( tx == 'int' ) \n");
    printf("            or ( tx == 'int4' ) \n");
    printf("            or ( tx == 'int3' ) \n");
    printf("            or ( tx == 'int2' ) \n");
    printf("            or ( tx == 'int4x4' ) \n");
    printf("            or ( tx == 'int4x3' ) \n");
    printf("            or ( tx == 'int4x2' ) \n");
    printf("            or ( tx == 'int3x4' ) \n");
    printf("            or ( tx == 'int3x3' ) \n");
    printf("            or ( tx == 'int3x2' ) \n");
    printf("            or ( tx == 'int2x4' ) \n");
    printf("            or ( tx == 'int2x3' ) \n");
    printf("            or ( tx == 'int2x2' ) ) then\n");
    printf("        \n");
    printf("	    if ((tx == ty)) then\n");
    printf("		    return tx\n");
    printf("	    end\n");
    printf("	end\n");
    printf("\n");
    printf("	return nil\n");
    printf("end\n");
    printf("\n");
    printf("function %s:config(props)\n", _node_name );
    printf("	Node.config(self, props)\n");
    printf("	props.delete('Name')\n");
    printf("end\n");
    printf("\n");
    printf("function %s:gen_code(out, scratch)\n", _node_name );
    printf("	local tx   = self.a.get_output().get_type()\n");
    printf("	local t    = tx\n");
    printf("	local x    = 'r' .. self.a.get_output().hex()\n");
    printf("	local y    = 'r' .. self.b.get_output().hex()\n");
    printf("	local d    = 'r' .. self.d.hex()\n");
    printf("\n");
    printf("	out.write( '\t' .. t .. ' ' .. d .. ' = ' .. x .. ' %s ' .. y .. ';\\n' )\n", _operator );
    printf("end\n");
    printf("\n");
    printf("function %s:get_tip(out)\n", _node_name );
    printf("	out.write( 'd = x %s y' )\n", _operator );
    printf("end\n");
    printf("\n");
  }
}

function output_op1_pre_all_data( _node_name, _name, _operator )
{
  for (i=0;i<op1_pre_all_count;i++)
  {
    _node_name = op1_pre_all_data[ i, "node_name" ];
    _name      = op1_pre_all_data[ i, "name" ];
    _operator  = op1_pre_all_data[ i, "operator" ];

    printf("-- #################################################### --\n");
    printf("\n");
    printf("local %s = class(Node)\n", _node_name );
    printf("\n");
    printf("function %s:get_help(out)\n", _node_name );
    printf("	out.write( 'd = %s(x)' )\n", _operator );
    printf("	out.write( '\\n' )\n", _operator );
    printf("	out.write( 'Supported Types:\\n' )\n", _operator );
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
    printf("    out.write( 'float4x4\\n' ) \n");
    printf("    out.write( 'half4x4\\n' ) \n");
    printf("    out.write( 'fixed4x4\\n' ) \n");
    printf("    out.write( 'int4x4\\n' ) \n");
    printf("    out.write( 'float4x3\\n' ) \n");
    printf("    out.write( 'half4x3\\n' ) \n");
    printf("    out.write( 'fixed4x3\\n' ) \n");
    printf("    out.write( 'int4x3\\n' ) \n");
    printf("    out.write( 'float4x2\\n' ) \n");
    printf("    out.write( 'half4x2\\n' ) \n");
    printf("    out.write( 'fixed4x2\\n' ) \n");
    printf("    out.write( 'int4x2\\n' ) \n");
    printf("    out.write( 'float3x4\\n' ) \n");
    printf("    out.write( 'half3x4\\n' ) \n");
    printf("    out.write( 'fixed3x4\\n' ) \n");
    printf("    out.write( 'int3x4\\n' ) \n");
    printf("    out.write( 'float3x3\\n' ) \n");
    printf("    out.write( 'half3x3\\n' ) \n");
    printf("    out.write( 'fixed3x3\\n' ) \n");
    printf("    out.write( 'int3x3\\n' ) \n");
    printf("    out.write( 'float3x2\\n' ) \n");
    printf("    out.write( 'half3x2\\n' ) \n");
    printf("    out.write( 'fixed3x2\\n' ) \n");
    printf("    out.write( 'int3x2\\n' ) \n");
    printf("    out.write( 'float2x4\\n' ) \n");
    printf("    out.write( 'half2x4\\n' ) \n");
    printf("    out.write( 'fixed2x4\\n' ) \n");
    printf("    out.write( 'int2x4\\n' ) \n");
    printf("    out.write( 'float2x3\\n' ) \n");
    printf("    out.write( 'half2x3\\n' ) \n");
    printf("    out.write( 'fixed2x3\\n' ) \n");
    printf("    out.write( 'int2x3\\n' ) \n");
    printf("    out.write( 'float2x2\\n' ) \n");
    printf("    out.write( 'half2x2\\n' ) \n");
    printf("    out.write( 'fixed2x2\\n' ) \n");
    printf("    out.write( 'int2x2\\n' )  \n");
    printf("end\n");
    printf("\n");
    printf("function %s:new()\n", _node_name );
    printf("	Node.new(self)\n");
    printf("	self.a = self.add_port(Input('a'))\n");
    printf("	self.d = self.add_port(Output('d'))\n");
    printf("end\n");
    printf("\n");
    printf("function %s:get_type(port)\n", _node_name );
    printf("\n");
    printf("	local tx = self.a.get_output().get_type()\n");
    printf("\n");
    printf("        if (   ( tx == 'float' ) \n");
    printf("            or ( tx == 'half' ) \n");
    printf("            or ( tx == 'fixed' ) \n");
    printf("            or ( tx == 'int' ) \n");
    printf("            or ( tx == 'float4' ) \n");
    printf("            or ( tx == 'half4' ) \n");
    printf("            or ( tx == 'fixed4' ) \n");
    printf("            or ( tx == 'int4' ) \n");
    printf("            or ( tx == 'float3' ) \n");
    printf("            or ( tx == 'half3' ) \n");
    printf("            or ( tx == 'fixed3' ) \n");
    printf("            or ( tx == 'int3' ) \n");
    printf("            or ( tx == 'float2' ) \n");
    printf("            or ( tx == 'half2' ) \n");
    printf("            or ( tx == 'fixed2' ) \n");
    printf("            or ( tx == 'int2' ) \n");
    printf("            or ( tx == 'float4x4' ) \n");
    printf("            or ( tx == 'half4x4' ) \n");
    printf("            or ( tx == 'fixed4x4' ) \n");
    printf("            or ( tx == 'int4x4' ) \n");
    printf("            or ( tx == 'float4x3' ) \n");
    printf("            or ( tx == 'half4x3' ) \n");
    printf("            or ( tx == 'fixed4x3' ) \n");
    printf("            or ( tx == 'int4x3' ) \n");
    printf("            or ( tx == 'float4x2' ) \n");
    printf("            or ( tx == 'half4x2' ) \n");
    printf("            or ( tx == 'fixed4x2' ) \n");
    printf("            or ( tx == 'int4x2' ) \n");
    printf("            or ( tx == 'float3x4' ) \n");
    printf("            or ( tx == 'half3x4' ) \n");
    printf("            or ( tx == 'fixed3x4' ) \n");
    printf("            or ( tx == 'int3x4' ) \n");
    printf("            or ( tx == 'float3x3' ) \n");
    printf("            or ( tx == 'half3x3' ) \n");
    printf("            or ( tx == 'fixed3x3' ) \n");
    printf("            or ( tx == 'int3x3' ) \n");
    printf("            or ( tx == 'float3x2' ) \n");
    printf("            or ( tx == 'half3x2' ) \n");
    printf("            or ( tx == 'fixed3x2' ) \n");
    printf("            or ( tx == 'int3x2' ) \n");
    printf("            or ( tx == 'float2x4' ) \n");
    printf("            or ( tx == 'half2x4' ) \n");
    printf("            or ( tx == 'fixed2x4' ) \n");
    printf("            or ( tx == 'int2x4' ) \n");
    printf("            or ( tx == 'float2x3' ) \n");
    printf("            or ( tx == 'half2x3' ) \n");
    printf("            or ( tx == 'fixed2x3' ) \n");
    printf("            or ( tx == 'int2x3' ) \n");
    printf("            or ( tx == 'float2x2' ) \n");
    printf("            or ( tx == 'half2x2' ) \n");
    printf("            or ( tx == 'fixed2x2' ) \n");
    printf("            or ( tx == 'int2x2' ) ) then\n");
    printf("        \n");
    printf("		return tx\n");
    printf("	end\n");
    printf("\n");
    printf("	return nil\n");
    printf("end\n");
    printf("\n");
    printf("function %s:config(props)\n", _node_name );
    printf("	Node.config(self, props)\n");
    printf("	props.delete('Name')\n");
    printf("end\n");
    printf("\n");
    printf("function %s:gen_code(out, scratch)\n", _node_name );
    printf("	local tx   = self.a.get_output().get_type()\n");
    printf("	local t    = tx\n");
    printf("	local x    = 'r' .. self.a.get_output().hex()\n");
    printf("	local d    = 'r' .. self.d.hex()\n");
    printf("\n");
    printf("	out.write( '\t' .. t .. ' ' .. d .. ' = %s(' .. x .. ');\\n' )\n", _operator );
    printf("end\n");
    printf("\n");
    printf("function %s:get_tip(out)\n", _node_name );
    printf("	out.write( 'd = %s(x)' )\n", _operator );
    printf("end\n");
    printf("\n");
  }
}

function output_op1_post_int_data( _node_name, _name, _operator )
{
  for (i=0;i<op1_post_int_count;i++)
  {
    _node_name = op1_post_int_data[ i, "node_name" ];
    _name      = op1_post_int_data[ i, "name" ];
    _operator  = op1_post_int_data[ i, "operator" ];

    printf("-- #################################################### --\n");
    printf("\n");
    printf("local %s = class(Node)\n", _node_name );
    printf("\n");
    printf("function %s:get_help(out)\n", _node_name );
    printf("	out.write( 'd = x%s' )\n", _operator );
    printf("	out.write( '\\n' )\n", _operator );
    printf("	out.write( 'Supported Types:\\n' )\n", _operator );
    printf("    out.write( 'int\\n' ) \n");
    printf("    out.write( 'int4\\n' ) \n");
    printf("    out.write( 'int3\\n' ) \n");
    printf("    out.write( 'int2\\n' ) \n");
    printf("    out.write( 'int4x4\\n' ) \n");
    printf("    out.write( 'int4x3\\n' ) \n");
    printf("    out.write( 'int4x2\\n' ) \n");
    printf("    out.write( 'int3x4\\n' ) \n");
    printf("    out.write( 'int3x3\\n' ) \n");
    printf("    out.write( 'int3x2\\n' ) \n");
    printf("    out.write( 'int2x4\\n' ) \n");
    printf("    out.write( 'int2x3\\n' ) \n");
    printf("    out.write( 'int2x2\\n' )  \n");
    printf("end\n");
    printf("\n");
    printf("function %s:new()\n", _node_name );
    printf("	Node.new(self)\n");
    printf("	self.a = self.add_port(Input('a'))\n");
    printf("	self.d = self.add_port(Output('d'))\n");
    printf("end\n");
    printf("\n");
    printf("function %s:get_type(port)\n", _node_name );
    printf("\n");
    printf("	local tx = self.a.get_output().get_type()\n");
    printf("\n");
    printf("        if (   ( tx == 'int' ) \n");
    printf("            or ( tx == 'int4' ) \n");
    printf("            or ( tx == 'int3' ) \n");
    printf("            or ( tx == 'int2' ) \n");
    printf("            or ( tx == 'int4x4' ) \n");
    printf("            or ( tx == 'int4x3' ) \n");
    printf("            or ( tx == 'int4x2' ) \n");
    printf("            or ( tx == 'int3x4' ) \n");
    printf("            or ( tx == 'int3x3' ) \n");
    printf("            or ( tx == 'int3x2' ) \n");
    printf("            or ( tx == 'int2x4' ) \n");
    printf("            or ( tx == 'int2x3' ) \n");
    printf("            or ( tx == 'int2x2' ) ) then\n");
    printf("        \n");
    printf("		return tx\n");
    printf("	end\n");
    printf("\n");
    printf("	return nil\n");
    printf("end\n");
    printf("\n");
    printf("function %s:config(props)\n", _node_name );
    printf("	Node.config(self, props)\n");
    printf("	props.delete('Name')\n");
    printf("end\n");
    printf("\n");
    printf("function %s:gen_code(out, scratch)\n", _node_name );
    printf("	local tx   = self.a.get_output().get_type()\n");
    printf("	local t    = tx\n");
    printf("	local x    = 'r' .. self.a.get_output().hex()\n");
    printf("	local d    = 'r' .. self.d.hex()\n");
    printf("\n");
    printf("	out.write( '\t' .. t .. ' ' .. d .. ' = ' .. x .. '%s;\\n' )\n", _operator );
    printf("end\n");
    printf("\n");
    printf("function %s:get_tip(out)\n", _node_name );
    printf("	out.write( 'd = x%s' )\n", _operator );
    printf("end\n");
    printf("\n");
  }
}
