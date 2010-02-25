BEGIN {
  FS = "\t";

  op_count = 0;
}

{
  if (NF == 4)
  {
    op_name          = $1;
    op_param_pattern = $2;
    op_description   = $3;
    op_formula       = $4;

    gsub(/\"/,"", op_param_pattern);
    gsub(/ /,"", op_name);
    gsub(/\\n/,"\n", op_description);
    gsub(/\\n/,"\n", op_formula);

    if ( op_param_pattern == "rt, ra, rb")
    {
      gen_op_rt_ra_rb( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "rc, ra, rb")
    {
      gen_op_rc_ra_rb( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "p_rt")
    {
      gen_op_p_rt( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "c_ra")
    {
      gen_op_c_ra( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "rt, ra, rb, rc")
    {
      gen_op_rt_ra_rb_rc( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "rt, ra")
    {
      gen_op_rt_ra( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "rt, ra, s10")
    {
      gen_op_rt_ra_s10( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "rt, ra, scale7")
    {
      gen_op_rt_ra_scale7( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "rt, ra, u3")
    {
      gen_op_rt_ra_u3( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "rt, ra, u5")
    {
      gen_op_rt_ra_u5( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "rt, ra, u6")
    {
      gen_op_rt_ra_u6( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "rt, ra, s7")
    {
      gen_op_rt_ra_s7( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "rt, ra, s3")
    {
      gen_op_rt_ra_s3( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "rt, u7(ra)")
    {
      gen_op_rt_u7_ra( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "rt, s14(ra)")
    {
      gen_op_rt_s14_ra( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "rc, s14(ra)")
    {
      gen_op_rc_s14_ra( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "rta, u16")
    {
      gen_op_rta_u16( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "rt, u16")
    {
      gen_op_rt_u16( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "rt, u18")
    {
      gen_op_rt_u18( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "rt, s16")
    {
      gen_op_rt_s16( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
    }

    else if ( op_param_pattern == "rt, s18")
    {
      gen_op_rt_s18( op_name, op_description, op_formula );
      gen_op_rt_s18_literal( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
      op_names[ op_count ] = op_name "_literal";
      op_count++;
    }

    else if ( op_param_pattern == "rc, s18")
    {
      gen_op_rc_s18( op_name, op_description, op_formula );
      gen_op_rc_s18_literal( op_name, op_description, op_formula );
      op_names[ op_count ] = op_name;
      op_count++;
      op_names[ op_count ] = op_name "_literal";
      op_count++;
    }


    # printf("%s '%s' %s %s\n",op_name,op_param_pattern,op_description,op_formula);
  }

  else if (NF == 2)
  {
    if ($1 == "NODENAME")
    {
      node_name = $2;
    }
  }
}

END {
  printf("register {\n");
  printf("  __name = '%s',\n",node_name);
  printf("  __prologue = '',\n");
  printf("  __epilogue = '',\n");
  for (i=0;i<op_count;i++)
  {
    printf("  ['%s'] = si_%s,\n",op_names[i],op_names[i]);
  }
  printf("}\n");
}

function gen_op_rt_ra_rb( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.ra = self.add_port(Input('ra'))\n");
  printf("  self.rb = self.add_port(Input('rb'))\n");
  printf("  self.rt = self.add_port(Output('rt'))\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rt = 'r' .. self.rt.hex()\n");
  printf("  local ra = 'r' .. self.ra.get_output().hex()\n");
  printf("  local rb = 'r' .. self.rb.get_output().hex()\n");
  printf("  out.write( '\\t .reg ' .. rt .. '\\n')\n");
  printf("  out.write( '\\t %s ' .. rt .. ', ' .. ra .. ', ' .. rb .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rc_ra_rb( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.rc = self.add_port(Input('rc'))\n");
  printf("  self.ra = self.add_port(Input('ra'))\n");
  printf("  self.rb = self.add_port(Input('rb'))\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rc = 'r' .. self.rc.get_output().hex()\n");
  printf("  local ra = 'r' .. self.ra.get_output().hex()\n");
  printf("  local rb = 'r' .. self.rb.get_output().hex()\n");
  printf("  out.write( '\\t %s ' .. rc .. ', ' .. ra .. ', ' .. rb .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rt_ra_rb_rc( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.ra = self.add_port(Input('ra'))\n");
  printf("  self.rb = self.add_port(Input('rb'))\n");
  printf("  self.rc = self.add_port(Input('rc'))\n");
  printf("  self.rt = self.add_port(Output('rt'))\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rt = 'r' .. self.rt.hex()\n");
  printf("  local ra = 'r' .. self.ra.get_output().hex()\n");
  printf("  local rb = 'r' .. self.rb.get_output().hex()\n");
  printf("  local rc = 'r' .. self.rc.get_output().hex()\n");
  printf("  out.write( '\\t .reg ' .. rt .. '\\n')\n");
  printf("  out.write( '\\t %s ' .. rt .. ', ' .. ra .. ', ' .. rb .. ', ' .. rc .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_p_rt( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.rt = self.add_port(Output('rt'))\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rt = 'r' .. self.rt.hex()\n");
  printf("  out.write( '\\t .define ' .. rt .. ' %s\\n')\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_c_ra( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.ra = self.add_port(Input('ra'))\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local ra = 'r' .. self.ra.get_output().hex()\n");
  printf("  out.write( '\\t .define %s ' .. ra .. '\\n')\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rt_ra( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.ra = self.add_port(Input('ra'))\n");
  printf("  self.rt = self.add_port(Output('rt'))\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rt = 'r' .. self.rt.hex()\n");
  printf("  local ra = 'r' .. self.ra.get_output().hex()\n");
  printf("  out.write( '\\t .reg ' .. rt .. '\\n')\n");
  printf("  out.write( '\\t %s ' .. rt .. ', ' .. ra .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rt_ra_s10( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.ra  = self.add_port(Input('ra'))\n");
  printf("  self.rt  = self.add_port(Output('rt'))\n");
  printf("  self.s10 = 0\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("  props.text_input(\n");
  printf("                    's10',\n");
  printf("                    function() return self.s10 end,\n");
  printf("                    function(v) self.s10 = v end,\n");
  printf("                    function(v) return math.evaluate(v) end\n");
  printf("                  )\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rt  = 'r' .. self.rt.hex()\n");
  printf("  local ra  = 'r' .. self.ra.get_output().hex()\n");
  printf("  local s10 = math.evaluate(self.s10)\n");
  printf("  out.write( '\\t .reg ' .. rt .. '\\n')\n");
  printf("  out.write( '\\t %s ' .. rt .. ', ' .. ra .. ', ' .. s10 .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rt_ra_scale7( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.ra  = self.add_port(Input('ra'))\n");
  printf("  self.rt  = self.add_port(Output('rt'))\n");
  printf("  self.scale7 = 0\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("  props.text_input(\n");
  printf("                    'scale7',\n");
  printf("                    function() return self.scale7 end,\n");
  printf("                    function(v) self.scale7 = v end,\n");
  printf("                    function(v) return math.evaluate(v) end\n");
  printf("                  )\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rt  = 'r' .. self.rt.hex()\n");
  printf("  local ra  = 'r' .. self.ra.get_output().hex()\n");
  printf("  local scale7 = math.evaluate(self.scale7)\n");
  printf("  out.write( '\\t .reg ' .. rt .. '\\n')\n");
  printf("  out.write( '\\t %s ' .. rt .. ', ' .. ra .. ', ' .. scale7 .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rt_ra_u3( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.ra  = self.add_port(Input('ra'))\n");
  printf("  self.rt  = self.add_port(Output('rt'))\n");
  printf("  self.u3 = 0\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("  props.text_input(\n");
  printf("                    'u3',\n");
  printf("                    function() return self.u3 end,\n");
  printf("                    function(v) self.u3 = v end,\n");
  printf("                    function(v) return math.evaluate(v) end\n");
  printf("                  )\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rt  = 'r' .. self.rt.hex()\n");
  printf("  local ra  = 'r' .. self.ra.get_output().hex()\n");
  printf("  local u3 = math.evaluate(self.u3)\n");
  printf("  out.write( '\\t .reg ' .. rt .. '\\n')\n");
  printf("  out.write( '\\t %s ' .. rt .. ', ' .. ra .. ', ' .. u3 .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rt_ra_u5( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.ra  = self.add_port(Input('ra'))\n");
  printf("  self.rt  = self.add_port(Output('rt'))\n");
  printf("  self.u5 = 0\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("  props.text_input(\n");
  printf("                    'u5',\n");
  printf("                    function() return self.u5 end,\n");
  printf("                    function(v) self.u5 = v end,\n");
  printf("                    function(v) return math.evaluate(v) end\n");
  printf("                  )\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rt  = 'r' .. self.rt.hex()\n");
  printf("  local ra  = 'r' .. self.ra.get_output().hex()\n");
  printf("  local u5 = math.evaluate(self.u5)\n");
  printf("  out.write( '\\t .reg ' .. rt .. '\\n')\n");
  printf("  out.write( '\\t %s ' .. rt .. ', ' .. ra .. ', ' .. u5 .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rt_ra_u6( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.ra  = self.add_port(Input('ra'))\n");
  printf("  self.rt  = self.add_port(Output('rt'))\n");
  printf("  self.u6 = 0\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("  props.text_input(\n");
  printf("                    'u6',\n");
  printf("                    function() return self.u6 end,\n");
  printf("                    function(v) self.u6 = v end,\n");
  printf("                    function(v) return math.evaluate(v) end\n");
  printf("                  )\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rt  = 'r' .. self.rt.hex()\n");
  printf("  local ra  = 'r' .. self.ra.get_output().hex()\n");
  printf("  local u6 = math.evaluate(self.u6)\n");
  printf("  out.write( '\\t .reg ' .. rt .. '\\n')\n");
  printf("  out.write( '\\t %s ' .. rt .. ', ' .. ra .. ', ' .. u6 .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rt_ra_s7( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.ra  = self.add_port(Input('ra'))\n");
  printf("  self.rt  = self.add_port(Output('rt'))\n");
  printf("  self.s7 = 0\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("  props.text_input(\n");
  printf("                    's7',\n");
  printf("                    function() return self.s7 end,\n");
  printf("                    function(v) self.s7 = v end,\n");
  printf("                    function(v) return math.evaluate(v) end\n");
  printf("                  )\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rt  = 'r' .. self.rt.hex()\n");
  printf("  local ra  = 'r' .. self.ra.get_output().hex()\n");
  printf("  local s7 = math.evaluate(self.s7)\n");
  printf("  out.write( '\\t .reg ' .. rt .. '\\n')\n");
  printf("  out.write( '\\t %s ' .. rt .. ', ' .. ra .. ', ' .. s7 .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rt_ra_s3( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.ra  = self.add_port(Input('ra'))\n");
  printf("  self.rt  = self.add_port(Output('rt'))\n");
  printf("  self.s3 = 0\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("  props.text_input(\n");
  printf("                    's3',\n");
  printf("                    function() return self.s3 end,\n");
  printf("                    function(v) self.s3 = v end,\n");
  printf("                    function(v) return math.evaluate(v) end\n");
  printf("                  )\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rt  = 'r' .. self.rt.hex()\n");
  printf("  local ra  = 'r' .. self.ra.get_output().hex()\n");
  printf("  local s3 = math.evaluate(self.s3)\n");
  printf("  out.write( '\\t .reg ' .. rt .. '\\n')\n");
  printf("  out.write( '\\t %s ' .. rt .. ', ' .. ra .. ', ' .. s3 .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rt_u7_ra( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.ra  = self.add_port(Input('ra'))\n");
  printf("  self.rt  = self.add_port(Output('rt'))\n");
  printf("  self.u7 = 0\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("  props.text_input(\n");
  printf("                    'u7',\n");
  printf("                    function() return self.u7 end,\n");
  printf("                    function(v) self.u7 = v end,\n");
  printf("                    function(v) return math.evaluate(v) end\n");
  printf("                  )\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rt  = 'r' .. self.rt.hex()\n");
  printf("  local ra  = 'r' .. self.ra.get_output().hex()\n");
  printf("  local u7 = math.evaluate(self.u7)\n");
  printf("  out.write( '\\t .reg ' .. rt .. '\\n')\n");
  printf("  out.write( '\\t %s ' .. rt .. ', ' .. u7 .. '(' .. ra .. ')\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rt_s14_ra( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.ra  = self.add_port(Input('ra'))\n");
  printf("  self.rt  = self.add_port(Output('rt'))\n");
  printf("  self.s14 = 0\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("  props.text_input(\n");
  printf("                    's14',\n");
  printf("                    function() return self.s14 end,\n");
  printf("                    function(v) self.s14 = v end,\n");
  printf("                    function(v) return math.evaluate(v) end\n");
  printf("                  )\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rt  = 'r' .. self.rt.hex()\n");
  printf("  local ra  = 'r' .. self.ra.get_output().hex()\n");
  printf("  local s14 = math.evaluate(self.s14)\n");
  printf("  out.write( '\\t .reg ' .. rt .. '\\n')\n");
  printf("  out.write( '\\t %s ' .. rt .. ', ' .. s14 .. '(' .. ra .. ')\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rc_s14_ra( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.rc  = self.add_port(Input('rc'))\n");
  printf("  self.ra  = self.add_port(Input('ra'))\n");
  printf("  self.s14 = 0\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("  props.text_input(\n");
  printf("                    's14',\n");
  printf("                    function() return self.s14 end,\n");
  printf("                    function(v) self.s14 = v end,\n");
  printf("                    function(v) return math.evaluate(v) end\n");
  printf("                  )\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rc  = 'r' .. self.rc.get_output().hex()\n");
  printf("  local ra  = 'r' .. self.ra.get_output().hex()\n");
  printf("  local s14 = math.evaluate(self.s14)\n");
  printf("  out.write( '\\t %s ' .. rc .. ', ' .. s14 .. '(' .. ra .. ')\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rta_u16( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.ra  = self.add_port(Input('ra'))\n");
  printf("  self.rt  = self.add_port(Output('rt'))\n");
  printf("  self.u16 = 0\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("  props.text_input(\n");
  printf("                    'u16',\n");
  printf("                    function() return self.u16 end,\n");
  printf("                    function(v) self.u16 = v end,\n");
  printf("                    function(v) return math.evaluate(v) end\n");
  printf("                  )\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rt  = 'r' .. self.rt.hex()\n");
  printf("  local ra  = 'r' .. self.ra.get_output().hex()\n");
  printf("  local u16 = math.evaluate(self.u16)\n");
  printf("  out.write( '\\t .define ' .. rt .. ' ' .. ra .. '\\n')\n");
  printf("  out.write( '\\t %s ' .. rt .. ', ' .. u16 .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rt_u16( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.rt  = self.add_port(Output('rt'))\n");
  printf("  self.u16 = 0\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("  props.text_input(\n");
  printf("                    'u16',\n");
  printf("                    function() return self.u16 end,\n");
  printf("                    function(v) self.u16 = v end,\n");
  printf("                    function(v) return math.evaluate(v) end\n");
  printf("                  )\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rt  = 'r' .. self.rt.hex()\n");
  printf("  local u16 = math.evaluate(self.u16)\n");
  printf("  out.write( '\\t .reg ' .. rt .. '\\n')\n");
  printf("  out.write( '\\t %s ' .. rt .. ', ' .. u16 .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rt_u18( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.rt  = self.add_port(Output('rt'))\n");
  printf("  self.u18 = 0\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("  props.text_input(\n");
  printf("                    'u18',\n");
  printf("                    function() return self.u18 end,\n");
  printf("                    function(v) self.u18 = v end,\n");
  printf("                    function(v) return math.evaluate(v) end\n");
  printf("                  )\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rt  = 'r' .. self.rt.hex()\n");
  printf("  local u18 = math.evaluate(self.u18)\n");
  printf("  out.write( '\\t .reg ' .. rt .. '\\n')\n");
  printf("  out.write( '\\t %s ' .. rt .. ', ' .. u18 .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rt_s16( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.rt  = self.add_port(Output('rt'))\n");
  printf("  self.s16 = 0\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("  props.text_input(\n");
  printf("                    's16',\n");
  printf("                    function() return self.s16 end,\n");
  printf("                    function(v) self.s16 = v end,\n");
  printf("                    function(v) return math.evaluate(v) end\n");
  printf("                  )\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rt  = 'r' .. self.rt.hex()\n");
  printf("  local s16 = math.evaluate(self.s16)\n");
  printf("  out.write( '\\t .reg ' .. rt .. '\\n')\n");
  printf("  out.write( '\\t %s ' .. rt .. ', ' .. s16 .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rt_s18( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.rt  = self.add_port(Output('rt'))\n");
  printf("  self.s18 = 0\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("  props.text_input(\n");
  printf("                    's18',\n");
  printf("                    function() return self.s18 end,\n");
  printf("                    function(v) self.s18 = v end,\n");
  printf("                    function(v) return math.evaluate(v) end\n");
  printf("                  )\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rt  = 'r' .. self.rt.hex()\n");
  printf("  local s18 = math.evaluate(self.s18)\n");
  printf("  out.write( '\\t .reg ' .. rt .. '\\n')\n");
  printf("  out.write( '\\t %s ' .. rt .. ', ' .. s18 .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rt_s18_literal( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s_literal = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s_literal:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s_literal:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.rt  = self.add_port(Output('rt'))\n");
  printf("  self.s18 = 0\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s_literal:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("  props.text_input(\n");
  printf("                    's18',\n");
  printf("                    function() return self.s18 end,\n");
  printf("                    function(v) self.s18 = v end,\n");
  printf("                    function(v) return v end\n");
  printf("                  )\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s_literal:gen_code(out, scratch)\n",_op_name);
  printf("  local rt  = 'r' .. self.rt.hex()\n");
  printf("  local s18 = self.s18\n");
  printf("  out.write( '\\t .reg ' .. rt .. '\\n')\n");
  printf("  out.write( '\\t %s ' .. rt .. ', ' .. s18 .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s_literal:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s_literal:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rc_s18( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.rt  = self.add_port(Input('rc'))\n");
  printf("  self.s18 = 0\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("  props.text_input(\n");
  printf("                    's18',\n");
  printf("                    function() return self.s18 end,\n");
  printf("                    function(v) self.s18 = v end,\n");
  printf("                    function(v) return math.evaluate(v) end\n");
  printf("                  )\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s:gen_code(out, scratch)\n",_op_name);
  printf("  local rc  = 'r' .. self.rc.get_output().hex()\n");
  printf("  local s18 = math.evaluate(self.s18)\n");
  printf("  out.write( '\\t %s ' .. rc .. ', ' .. s18 .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

function gen_op_rc_s18_literal( _op_name, _op_description, _op_formula )
{
  printf("-- #################################################### --\n");
  printf("\n");
  printf("local si_%s_literal = class(Node)\n",_op_name);
  printf("\n");

  printf("function si_%s_literal:get_help(out)\n",_op_name)
  printf("  out.write[[ %s \n\n %s ]]\n",_op_description,_op_formula);
  printf("end\n");
  printf("\n");

  printf("function si_%s_literal:new()\n",_op_name);
  printf("  Node.new(self)\n");
  printf("  self.rc  = self.add_port(Input('rc'))\n");
  printf("  self.s18 = 0\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s_literal:config(props)\n",_op_name);
  printf("  Node.config(self, props)\n");
  printf("  props.delete('Name')\n");
  printf("  props.text_input(\n");
  printf("                    's18',\n");
  printf("                    function() return self.s18 end,\n");
  printf("                    function(v) self.s18 = v end,\n");
  printf("                    function(v) return v end\n");
  printf("                  )\n");
  printf("end\n");
  printf("\n");

  printf("function si_%s_literal:gen_code(out, scratch)\n",_op_name);
  printf("  local rc  = 'r' .. self.rc.get_output().hex()\n");
  printf("  local s18 = self.s18\n");
  printf("  out.write( '\\t %s ' .. rc .. ', ' .. s18 .. '\\n' )\n",_op_name);
  printf("end\n");
  printf("\n");

  printf("function si_%s_literal:get_tip(out)\n",_op_name);
  printf("  out.write('%s')\n",_op_description);
  printf("end\n");
  printf("\n");

  printf("function si_%s_literal:get_type(port)\n",_op_name);
  printf("  return 'qword'\n");
  printf("end\n");
  printf("\n");
}

