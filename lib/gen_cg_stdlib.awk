BEGIN {
  parse_state = "PARSE_ROOT";
  entry_count = 0;
}

{
  if (($2 == "CgStandard") && ($3 == "Library"))
  {
    # page break. Ingnore
  }
  else if (parse_state == "PARSE_ROOT")
  {
    if (($1 == "Cg") && ($2 == "Toolkit"))
    {
      # page break. Ignore
    }
    else if ($1 == "NAME")
    {
      begin_entry();

      parse_state = "PARSE_NAME";
      entry_db[ entry_count, "help_text" ] = entry_db[ entry_count, "help_text" ] "\\n\\nNAME\\n";
    }
  }
  else if (parse_state == "PARSE_NAME")
  {
    if (($1 == "Cg") && ($2 == "Toolkit"))
    {
      # page break. Ignore
    }
    else if ($1 == "SYNOPSIS")
    {
      parse_state = "PARSE_SYNOPSIS";
      entry_db[ entry_count, "help_text" ] = entry_db[ entry_count, "help_text" ] "\\n\\nSYNOPSIS\\n";
    }
    else
    {
      parse_name();
    }
  }
  else if (parse_state == "PARSE_SYNOPSIS")
  {
    if (($1 == "Cg") && ($2 == "Toolkit"))
    {
      # page break. Ignore
    }
    else if ($1 == "PARAMETERS") # Optional. Missing in tex1DARRAYfetch anyway.
    {
      parse_state = "PARSE_PARAMETERS";
      entry_db[ entry_count, "help_text" ] = entry_db[ entry_count, "help_text" ] "\\n\\nPARAMETERS\\n";
    }
    else if ($1 == "DESCRIPTION")
    {
      parse_state = "PARSE_DESCRIPTION";
      entry_db[ entry_count, "help_text" ] = entry_db[ entry_count, "help_text" ] "\\n\\nDESCRIPTION\\n";
    }
    else
    {
      parse_synopsis();
    }
  }
  else if (parse_state == "PARSE_PARAMETERS")
  {
    if (($1 == "Cg") && ($2 == "Toolkit"))
    {
      # page break. Ignore
    }
    else if ($1 == "DESCRIPTION")
    {
      parse_state = "PARSE_DESCRIPTION";
      entry_db[ entry_count, "help_text" ] = entry_db[ entry_count, "help_text" ] "\\n\\nDESCRIPTION\\n";
    }
    else
    {
      parse_parameters();
    }
  }
  else if (parse_state == "PARSE_DESCRIPTION")
  {
    if (($1 == "Cg") && ($2 == "Toolkit"))
    {
      # page break. Ignore
    }
    else if (($1 == "REFERENCE") && ($2 == "IMPLEMENTATION")) # Optional
    {
      parse_state = "PARSE_REFERENCE_IMPLEMENTATION";
      entry_db[ entry_count, "help_text" ] = entry_db[ entry_count, "help_text" ] "\\n\\nREFERENCE IMPLEMENTATION\\n";
    }
    else if (($1 == "PROFILE") && ($2 == "SUPPORT"))
    {
      parse_state = "PARSE_PROFILE_SUPPORT";
      entry_db[ entry_count, "help_text" ] = entry_db[ entry_count, "help_text" ] "\\n\\nPROFILE SUPPORT\\n";
    }
    else
    {
      parse_description();
    }
  }
  else if (parse_state == "PARSE_REFERENCE_IMPLEMENTATION")
  {
    if (($1 == "Cg") && ($2 == "Toolkit"))
    {
      # page break. Ignore
    }
    else if (($1 == "PROFILE") && ($2 == "SUPPORT"))
    {
      parse_state = "PARSE_PROFILE_SUPPORT";
      entry_db[ entry_count, "help_text" ] = entry_db[ entry_count, "help_text" ] "\\n\\nPROFILE SUPPORT\\n";
    }
    else
    {
      parse_reference_implementation();
    }
  }
  else if (parse_state == "PARSE_PROFILE_SUPPORT")
  {
    if (($1 == "Cg") && ($2 == "Toolkit"))
    {
      # page break. Ignore
    }
    else if (($1 == "SEE") && ($2 == "ALSO"))
    {
      parse_state = "PARSE_SEE_ALSO";
      entry_db[ entry_count, "help_text" ] = entry_db[ entry_count, "help_text" ] "\\n\\nSEE ALSO\\n";
    }
    else
    {
      parse_profile_support();
    }
  }
  else if (parse_state == "PARSE_SEE_ALSO")
  {
    if (($1 == "Cg") && ($2 == "Toolkit"))
    {
      end_entry();
      parse_state = "PARSE_ROOT";
    }
    else
    {
      parse_see_also();
    }
  }
}

function parse_name()
{
  if ( entry_db[ entry_count, "NAME_STATE" ] == 0 )
  {
    entry_db[ entry_count, "name" ]              = $1;
    entry_db[ entry_count, "short_description" ] = $0;
    entry_db[ entry_count, "NAME_STATE" ]        = 1;
  }
  else
  {
    entry_db[ entry_count, "short_description" ] = entry_db[ entry_count, "short_description" ] " " $0;
  }

  entry_db[ entry_count, "help_text" ] = entry_db[ entry_count, "help_text" ] $0 "\\n";
  entry_db[ entry_count, "tip_text" ]  = entry_db[ entry_count, "tip_text" ] $0 " ";
}

function parse_synopsis( _synopisis_ndx, _synopsis )
{
  _synopsis_ndx = entry_db[ entry_count, "synopsis_count" ];

  if ( index( $0, "(" ) > 0 )
  {
    entry_db[ entry_count, "synopsis", _synopsis_ndx ] = "";
  }

  entry_db[ entry_count, "synopsis", _synopsis_ndx ] = entry_db[ entry_count, "synopsis", _synopsis_ndx ] $0;

  if ( index( $0, ")" ) > 0 )
  {
    _synopsis = entry_db[ entry_count, "synopsis", _synopsis_ndx ];

    gsub( /,/," ", _synopsis )
    gsub( /\(/," ", _synopsis )
    gsub( /\)/," ", _synopsis )
    gsub( /;/,"", _synopsis )
    gsub( /^ /,"", _synopsis )
    gsub( / $/,"", _synopsis )
    gsub( / /," ", _synopsis )

    entry_db[ entry_count, "synopsis", _synopsis_ndx ] = _synopsis;
    entry_db[ entry_count, "synopsis_count" ]          = _synopsis_ndx+1;
  }

  entry_db[ entry_count, "help_text" ] = entry_db[ entry_count, "help_text" ] $0 "\\n";
}

function parse_parameters()
{
  entry_db[ entry_count, "help_text" ] = entry_db[ entry_count, "help_text" ] $0 "\\n";
}

function parse_description()
{
  entry_db[ entry_count, "help_text" ] = entry_db[ entry_count, "help_text" ] $0 "\\n";
}

function parse_reference_implementation()
{
  entry_db[ entry_count, "help_text" ] = entry_db[ entry_count, "help_text" ] $0 "\\n";
}

function parse_profile_support()
{
  entry_db[ entry_count, "help_text" ] = entry_db[ entry_count, "help_text" ] $0 "\\n";
}

function parse_see_also()
{
  entry_db[ entry_count, "help_text" ] = entry_db[ entry_count, "help_text" ] $0 "\\n";
}

function begin_entry()
{
  entry_db[ entry_count, "NAME_STATE" ]     = 0;
  entry_db[ entry_count, "synopsis_count" ] = 0;
  entry_db[ entry_count, "help_text" ]      = "";
  entry_db[ entry_count, "tip_text" ]       = "";
}

function end_entry()
{
  entry_count++;
}

END {
  g_result_nodes_count = 0;

  for (i=0;i<entry_count;i++) 
  {
    # printf("NAME\n");
    # printf("%s\n",entry_db[i,"short_description"]);
    # printf("--\n");
    # printf("entry[%d] = %s (%s)\n",i,entry_db[i,"name"],entry_db[i,"short_description"]);

    for (j=0;j< entry_db[ i, "synopsis_count" ];j++)
    {
      # printf("-- %s\n",entry_db[ i, "synopsis", j]);
    }

    output_entry(i);
  }

  printf("\n\n");
  printf("-- #################################################### --\n");
  printf("register {\n");
  printf("  __name = 'Cg [stdlib]',\n");
  printf("  __prologue = '',\n");
  printf("  __epilogue = '',\n");
  for (i=0;i<g_result_nodes_count;i++)
  {
    printf("  ['%s'] = %s,\n",g_result_nodes_db[i,"display_name"],g_result_nodes_db[i,"node_name"]);
  }
  printf("}\n");
}

function output_entry( _entry_ndx, _synopsis_ndx, _synopsis_str, _synopsis_tokens, _synopsis_token_count, _synopsis_length_found_count, _synopsis_length_found, _i, _synopsis_count, _similar_synopsis_found )
{
    _synopsis_count              = entry_db[ _entry_ndx, "synopsis_count" ];
    _synopsis_length_found_count = 0;

    for (_synopsis_ndx=0;_synopsis_ndx< _synopsis_count;_synopsis_ndx++)
    {
      _synopsis_str         = entry_db[ _entry_ndx, "synopsis", _synopsis_ndx ];
      _synopsis_token_count = split( _synopsis_str, _synopsis_tokens, " " );
      _similar_synopsis_found = 0;

      for ( _i=0;_i<_synopsis_length_found_count;_i++ )
      {
        if ( _synopsis_length_found[ _i ] == _synopsis_token_count )
        {
          # This synopsis length (parameter count) has already been managed
          _similar_synopsis_found = 1;
          break;
        }
      }

      if (!_similar_synopsis_found)
      {
        # Found a new synopsis length

        output_entry_unique_synopsis_length( _entry_ndx, _synopsis_token_count );

        _synopsis_length_found[ _synopsis_length_found_count ] = _synopsis_token_count;
        _synopsis_length_found_count++;
      }

      # printf("-- %s\n",entry_db[ i, "synopsis", j]);
    }
   
}

function output_entry_unique_synopsis_length( _entry_ndx, _synopsis_length, _node_name, _display_name, _synopsis_ndx, _synopsis_str, _synopsis_tokens, _synopsis_token_count, _parameter_names, _i, _parameter_count, _parameter_type )
{
  # Unique node for each unique parameters count (synopsis length)

  _parameter_count = (_synopsis_length/2)-1;
  _node_name       = "cg_" entry_db[ _entry_ndx, "name" ] "_" _parameter_count;
  
  # Build list of parameter names
  # Build display name from parameter names

  _display_name = entry_db[ _entry_ndx, "name" ] "( ";
  for (_synopsis_ndx=0;_synopsis_ndx< entry_db[ _entry_ndx, "synopsis_count" ];_synopsis_ndx++)
  {
    _synopsis_str         = entry_db[ _entry_ndx, "synopsis", _synopsis_ndx ];
    _synopsis_token_count = split( _synopsis_str, _synopsis_tokens, " " );

    if ( _synopsis_token_count == _synopsis_length )
    {
      for (_i=0;_i<_parameter_count;_i++)
      {
        _parameter_names[_i] = _synopsis_tokens[ 1+((_i+1)*2)+1 ];
        _display_name        = _display_name _parameter_names[_i];

        if (_i != (_parameter_count-1))
        {
          _display_name = _display_name ", ";
        } 
      }
     
      _display_name = _display_name " )";
      break;
    }
  }

  # ------------------------------------
  # -- ADD NODE TO SCRIPT RETURN LIST --
  # ------------------------------------

  g_result_nodes_db[ g_result_nodes_count, "node_name" ]    = _node_name;
  g_result_nodes_db[ g_result_nodes_count, "display_name" ] = _display_name;
  g_result_nodes_count++;

  # ------------------------------------
  # ------------------------------------
  

  # Create node

  printf("-- #################################################### --\n");
  printf("-- %s\n",_display_name);
  printf("-- #################################################### --\n");
  printf("local %s = class(Node)\n", _node_name );

  printf("function %s:get_help(out)\n", _node_name );
  printf("	out.write( '%s' )\n", entry_db[ _entry_ndx, "help_text" ] );
  printf("end\n");
  printf("\n");

  printf("function %s:new()\n", _node_name );
  printf("	Node.new(self)\n");
  printf("	self.name = '%s'\n",entry_db[ _entry_ndx, "name" ]);

  # ----------------------------------------------------------------------------------
  # ---- TODO: Need to do something special for frexp (needs additional output node)
  # ----------------------------------------------------------------------------------

  for (_i=0;_i<_parameter_count;_i++)
  {
      printf("	self.in_%s = self.add_port(Input('%s'))\n", _parameter_names[_i], _parameter_names[_i] );
  }
  printf("	self.out = self.add_port(Output('out'))\n");
  printf("end\n");
  printf("\n");

  printf("function %s:get_type(port)\n", _node_name );
  printf("\n");
  for (_i=0;_i<_parameter_count;_i++)
  {
      printf("	local type_%s = self.in_%s.get_output().get_type()\n", _parameter_names[_i], _parameter_names[_i] );
  }

  for (_synopsis_ndx=0;_synopsis_ndx< entry_db[ _entry_ndx, "synopsis_count" ];_synopsis_ndx++)
  {
    _synopsis_str         = entry_db[ _entry_ndx, "synopsis", _synopsis_ndx ];
    _synopsis_token_count = split( _synopsis_str, _synopsis_tokens, " " );

    if ( _synopsis_token_count == _synopsis_length )
    {
      printf("	if (");
      for (_i=0;_i<_parameter_count;_i++)
      {
        _parameter_type = _synopsis_tokens[ 1+((_i+1)*2)+0 ];

        printf("(type_%s == '%s')",_parameter_names[_i],_parameter_type);
        if (_i != (_parameter_count-1))
        { 
          printf("\n	and ");
        }
      }
      printf(") then\n");
      printf("		return '%s'\n",_synopsis_tokens[1]);
      printf("	end\n");
    }
  }

  printf("	return nil\n");
  printf("end\n");
  printf("\n");

  printf("function %s:config(props)\n", _node_name );
  printf("	Node.config(self, props)\n");
  printf("	props.delete('Name')\n");
  printf("end\n");
  printf("\n");

  printf("function %s:gen_code(out, scratch)\n", _node_name );
  printf("	local out_type   = self.get_type()\n");
  printf("	local d    = 'r' .. self.out.hex()\n");
  for (_i=0;_i<_parameter_count;_i++)
  {
      printf("	local in_%s = 'r' .. self.in_%s.get_output().hex()\n", _parameter_names[_i], _parameter_names[_i] );
  }
  printf("\n");
  printf("	out.write( '\t' .. out_type .. ' ' .. d .. ' = %s( ' ", entry_db[_entry_ndx,"name"] );
  for (_i=0;_i<_parameter_count;_i++)
  {
      printf(".. in_%s", _parameter_names[_i]);
      if (_i!=(_parameter_count-1))
      {
        printf(".. ', ' ");
      }
  }
  printf(".. ' );\\n')\n");
  printf("end\n");
  printf("\n");

  printf("function %s:get_tip(out)\n", _node_name );
  printf("	out.write( '%s' )\n", entry_db[ _entry_ndx, "tip_text" ] );
  printf("end\n");
  printf("\n");
}
