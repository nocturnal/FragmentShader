{{
['end_comment']='*/',
['epilogue']='',
['gen_comments']=true,
['__class']='Graph',
['nodes']={2},
['edges']={3},
['y0']=0,
['prologue']='',
['y1']=404,
['x1']=196,
['x0']=0,
['start_comment']='/*',
},
{
[1]={4},
[2]={5},
[3]={6},
[4]={7},
[5]={8},
[6]={9},
[7]={10},
['01BFAA68']={7},
['017F16A8']={4},
['017F7DF0']={5},
['01DC4D40']={8},
['01C9F318']={10},
['01EC4098']={9},
['0164E558']={6},
},
{
[{11}]=1,
[{12}]=1,
[{13}]=1,
[{14}]=1,
[{15}]=1,
[{16}]=1,
},
{
['__class']='Cg [_C3E3 fragment].Decal TexCoord',
['comment']='',
['owner']={1},
['d']={17},
['x0']=0,
['name']='Decal TexCoord',
['inputs']={18},
['color']='black',
['outputs']={19},
['y1']=40,
['x1']=88,
['y0']=0,
},
{
['a']={20},
['comment']='',
['__class']='Cg [_C3E3 fragment].Output Color',
['owner']={1},
['x0']=93,
['name']='Output Color',
['inputs']={21},
['color']='black',
['outputs']={22},
['y1']=404,
['x1']=167,
['y0']=364,
},
{
['color']='black',
['d']={23},
['y1']=40,
['x1']=185,
['outputs']={24},
['__class']='Cg [Misc].Constant, User (Vector2)',
['y0']=0,
['type_choices']={25},
['owner']={1},
['name']='float2(0.5,0.5)',
['inputs']={26},
['value_0']='0.5',
['comment']='',
['type_ndx']=1,
['x0']=106,
['value_1']='0.5',
},
{
['a']={27},
['b']={28},
['color']='black',
['d']={29},
['y1']=136,
['x1']=121,
['outputs']={30},
['__class']='Cg [Arithmetic Operators].Subtract',
['y0']=76,
['owner']={1},
['comment']='',
['x0']=67,
['name']='Subtract',
['inputs']={31},
},
{
['color']='black',
['y1']=232,
['x1']=121,
['outputs']={32},
['__class']='Cg [stdlib].length( v )',
['in_v']={33},
['owner']={1},
['comment']='',
['y0']=172,
['x0']=67,
['out']={34},
['inputs']={35},
['name']='length',
},
{
['color']='black',
['d']={36},
['y1']=222,
['x1']=196,
['value']='0.5',
['outputs']={37},
['__class']='Cg [Misc].Constant, User (Splat)',
['y0']=182,
['type_choices']={38},
['owner']={1},
['name']='float(0.5)',
['comment']='',
['type_ndx']=1,
['x0']=139,
['inputs']={39},
},
{
['color']='black',
['d']={40},
['y1']=328,
['x1']=190,
['outputs']={41},
['__class']='Cg [Select].Compare Greater Than',
['x']={42},
['y0']=268,
['type_choices']={43},
['owner']={1},
['name']='Compare Greater Than',
['type_ndx']=1,
['comment']='',
['y']={44},
['x0']=70,
['inputs']={45},
},
{
['target']={27},
['source']={17},
['__class']='Edge',
},
{
['target']={33},
['source']={29},
['__class']='Edge',
},
{
['target']={28},
['source']={23},
['__class']='Edge',
},
{
['target']={44},
['source']={36},
['__class']='Edge',
},
{
['target']={42},
['source']={34},
['__class']='Edge',
},
{
['target']={20},
['source']={40},
['__class']='Edge',
},
{
['owner']={4},
['__class']='Output',
['x0']=0,
['y1']=40,
['y0']=20,
['name']='d',
['x1']=88,
},
{
},
{
[1]={17},
},
{
['x1']=167,
['owner']={5},
['__class']='Input',
['x0']=93,
['y1']=384,
['y0']=364,
['name']='a',
},
{
[1]={20},
},
{
},
{
['owner']={6},
['__class']='Output',
['x0']=106,
['y1']=40,
['y0']=20,
['name']='d',
['x1']=185,
},
{
[1]={23},
},
{
[1]='float2',
[2]='half2',
[3]='fixed2',
[4]='int2',
[5]='bool2',
},
{
},
{
['x1']=94,
['owner']={7},
['__class']='Input',
['x0']=67,
['y1']=96,
['y0']=76,
['name']='a',
},
{
['x1']=121,
['owner']={7},
['__class']='Input',
['x0']=94,
['y1']=96,
['y0']=76,
['name']='b',
},
{
['owner']={7},
['__class']='Output',
['x0']=67,
['y1']=136,
['y0']=116,
['name']='d',
['x1']=121,
},
{
[1]={29},
},
{
[1]={27},
[2]={28},
},
{
[1]={34},
},
{
['x1']=121,
['owner']={8},
['__class']='Input',
['x0']=67,
['y1']=192,
['y0']=172,
['name']='v',
},
{
['owner']={8},
['__class']='Output',
['x0']=67,
['y1']=232,
['y0']=212,
['name']='out',
['x1']=121,
},
{
[1]={33},
},
{
['owner']={9},
['__class']='Output',
['x0']=139,
['y1']=222,
['y0']=202,
['name']='d',
['x1']=196,
},
{
[1]={36},
},
{
[1]='float',
[2]='half',
[3]='fixed',
[4]='int',
[5]='bool',
[6]='float4',
[7]='half4',
[8]='fixed4',
[9]='int4',
[10]='bool4',
[11]='float3',
[12]='half3',
[13]='fixed3',
[14]='int3',
[15]='bool3',
[16]='float2',
[17]='half2',
[18]='fixed2',
[19]='int2',
[20]='bool2',
[21]='float4x4',
[22]='half4x4',
[23]='fixed4x4',
[24]='int4x4',
[25]='bool4x4',
[26]='float4x3',
[27]='half4x3',
[28]='fixed4x3',
[29]='int4x3',
[30]='bool4x3',
[31]='float4x2',
[32]='half4x2',
[33]='fixed4x2',
[34]='int4x2',
[35]='bool4x2',
[36]='float3x4',
[37]='half3x4',
[38]='fixed3x4',
[39]='int3x4',
[40]='bool3x4',
[41]='float3x3',
[42]='half3x3',
[43]='fixed3x3',
[44]='int3x3',
[45]='bool3x3',
[46]='float3x2',
[47]='half3x2',
[48]='fixed3x2',
[49]='int3x2',
[50]='bool3x2',
[51]='float2x4',
[52]='half2x4',
[53]='fixed2x4',
[54]='int2x4',
[55]='bool2x4',
[56]='float2x3',
[57]='half2x3',
[58]='fixed2x3',
[59]='int2x3',
[60]='bool2x3',
[61]='float2x2',
[62]='half2x2',
[63]='fixed2x2',
[64]='int2x2',
[65]='bool2x2',
},
{
},
{
['owner']={10},
['__class']='Output',
['x0']=70,
['y1']=328,
['y0']=308,
['name']='d',
['x1']=190,
},
{
[1]={40},
},
{
['x1']=130,
['owner']={10},
['__class']='Input',
['x0']=70,
['y1']=288,
['y0']=268,
['name']='x',
},
{
[1]='float',
[2]='half',
[3]='fixed',
[4]='int',
[5]='bool',
[6]='float4',
[7]='half4',
[8]='fixed4',
[9]='int4',
[10]='bool4',
[11]='float3',
[12]='half3',
[13]='fixed3',
[14]='int3',
[15]='bool3',
[16]='float2',
[17]='half2',
[18]='fixed2',
[19]='int2',
[20]='bool2',
},
{
['x1']=190,
['owner']={10},
['__class']='Input',
['x0']=130,
['y1']=288,
['y0']=268,
['name']='y',
},
{
[1]={42},
[2]={44},
},
}