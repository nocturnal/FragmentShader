BEGIN {
  g_node_count  = 0;
  g_prefix = "";
  g_prefix_node_name = "";
}

{
  
  if ($1 == "PREFIX")
  {
    prefix_text = $0;
    gsub( /^PREFIX +/,"", prefix_text)
    g_prefix = prefix_text;
  }
  else if ($1 == "PREFIX_NODE_NAME")
  {
    g_prefix_node_name = $2;
  }
  else if ($1 == "NAME")
  {
    name_text = $0;
    gsub( /^NAME +/,"", name_text)
    g_node_db[ g_node_count, "name" ] = name_text;
    g_node_count++;
  }
  else if ($1 == "NODE_NAME")
  {
    g_node_db[ g_node_count-1, "node_name" ] = $2;
  }
  else if ($1 == "TYPE") 
  {
    g_node_db[ g_node_count-1, "type" ] = $2;
  }

  else if ($1 == "VARTYPE") 
  {
    g_node_db[ g_node_count-1, "var_type" ] = $2;
  }

  else if ($1 == "VARSEL") 
  {
    g_node_db[ g_node_count-1, "var_count" ] = NF-1;

    for (i=0;i<NF-1;i++)
    {
      g_node_db[ g_node_count-1, "var_choices", i ] = $(2+i);
    }
  }

  else if ($1 == "HELP")
  {
    help_text = $0;
    gsub( /^HELP +/,"", help_text)
    g_node_db[ g_node_count-1, "help" ] = help_text;
  }
}

END {

  for (i=0;i<g_node_count;i++)
  {
    _node_name = g_prefix_node_name "_" g_node_db[ i, "node_name" ];
    _name      = g_node_db[ i, "name" ];
    _type      = g_node_db[ i, "type" ];
    _var_type  = g_node_db[ i, "var_type" ];
    _var_count = g_node_db[ i, "var_count" ];
    _help      = g_node_db[ i, "help" ];
    
    printf("-- #################################################### --\n");
    printf("\n");
    printf("local %s = class(Node)\n",_node_name);
    printf("\n");
    printf("function %s:get_help(out)\n",_node_name);
    printf("  out.write[[%s]]\n",_help);
    printf("end\n");
    printf("\n");
    printf("function %s:new()\n",_node_name);
    printf("  Node.new(self)\n");
    
    if ( _type == "IN" )
    {
      printf("  self.d = self.add_port(Output('d'))\n");
    }
    else if ( _type == "OUT" )
    {
      printf("  self.a = self.add_port(Input('a'))\n");
    }
    
    if (_var_count > 1)
    {
      printf("  self.choices= { ");
      for (j=0;j<_var_count;j++)
      {
        printf("'%s',",g_node_db[i,"var_choices",j]);
      }
      printf("}\n");
      printf("  self.choice_ndx = 1\n");
    
      printf("  self.name = '[%s] (%s) %s (%s)'\n",_type,_var_type,_name,g_node_db[i,"var_choices",0]);
    }
    else
    {
      printf("  self.name = '[%s] (%s) %s'\n",_type,_var_type,_name);
    }
    
    printf("end\n");
    printf("\n");
    
    printf("function %s:get_type(port)\n",_node_name);
    printf("  return '%s'\n",_var_type);
    printf("end\n");
    printf("\n");
    
    printf("function %s:config(props)\n",_node_name);
    printf("  Node.config(self, props)\n");
    printf("  props.delete('Name')\n");
    printf("\n");
    
    if (_var_count > 1)
    {
      printf("  props.list(\n");
      printf("    'Select',\n");
      printf("    self.choices,\n");
      printf("    function() return self.choice_ndx end,\n");
      printf("    function(v) self.name = '(%s) %s (' .. self.choices[v] .. ')' self.choice_ndx = v end\n",_var_type,_name);
      printf("  )\n");
    }
    
    printf("end\n");
    printf("\n");
    
    printf("function %s:gen_code(out, scratch)\n",_node_name);

    if ( _type == "IN" )
    {
      if (_var_count > 1)
      {
        printf("  out.write( '\\t#define r' .. self.d.hex() .. ' ' .. self.choices[self.choice_ndx] .. '\\n' )\n");
      }
      else
      {
        printf("  out.write( '\\t#define r' .. self.d.hex() .. ' %s\\n' )\n",g_node_db[i,"var_choices",0]);
      }
    }
    else if ( _type == "OUT" )
    {
      if (_var_count > 1)
      {
        printf("  local a    = 'r' .. self.a.get_output().hex()\n");
        printf("  local c    = self.choices[ self.choice_ndx ];\n");
        printf("  out.write( '\\t' .. c .. ' = ' .. a .. ';\\n' )\n");
      }
      else
      {
        printf("  local a    = 'r' .. self.a.get_output().hex()\n");
        printf("  out.write( '\\t%s = ' .. a .. ';\\n' )\n",g_node_db[i,"var_choices",0]);
      }
    }
    printf("end\n");
    printf("\n");

    printf("function %s:get_tip(out)\n",_node_name);
    printf("  out.write[[%s]]\n",_help);
    printf("end\n");
    printf("\n");

    # printf("%s %s %s [ ",g_node_db[i,"type"],g_node_db[i,"var_type"],g_node_db[i,"name"]);
    # for (j=0;j<g_node_db[i,"var_count"];j++)
    # {
    #   printf("%s, ",g_node_db[i,"var_choices",j]);
    # }
    # printf("] (%s)\n",g_node_db[i,"help"]);
  }


  printf("\n");
  printf("-- #################################################### --\n");
  printf("\n");
  printf("register {\n");
  printf("  __name = 'Cg [_%s]',\n",g_prefix);
  printf("  __prologue = '',\n");
  printf("  __epilogue = '',\n");
  printf("\n");
  for (i=0;i<g_node_count;i++)
  {
    _node_name = g_prefix_node_name "_" g_node_db[ i, "node_name" ];
    _name      = g_node_db[ i, "name" ];
    _type      = g_node_db[ i, "type" ];
    _var_type  = g_node_db[ i, "var_type" ];
    _var_count = g_node_db[ i, "var_count" ];

    if (_var_count > 1)
    {
      printf("  ['[%s] (%s) %s (%s)'] = %s,\n",_type,_var_type,_name,g_node_db[i,"var_choices",0],_node_name);
    }
    else
    {
      printf("  ['[%s] (%s) %s'] = %s,\n",_type,_var_type,_name,_node_name);
    }
  }

  printf("}\n");
}
