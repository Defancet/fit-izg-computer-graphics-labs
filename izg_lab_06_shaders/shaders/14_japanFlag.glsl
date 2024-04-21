#ifdef VERTEX_SHADER
#line 2
precision highp float;
uniform mat4 view;
uniform mat4 proj;

out highp vec2 vCoord;
void main(){
  gl_Position = vec4(0.f,0.f,0.f,1.f);

  mat4 vp = proj*view;

  if(gl_VertexID==0)gl_Position=vec4(-1.0,-1.f,0.f,1.f);
  if(gl_VertexID==1)gl_Position=vec4(+1.0,-1.f,0.f,1.f);
  if(gl_VertexID==2)gl_Position=vec4(-1.0,+1.f,0.f,1.f);
  if(gl_VertexID==3)gl_Position=vec4(-1.0,+1.f,0.f,1.f);
  if(gl_VertexID==4)gl_Position=vec4(+1.0,-1.f,0.f,1.f);
  if(gl_VertexID==5)gl_Position=vec4(+1.0,+1.f,0.f,1.f);
  vCoord = gl_Position.xy*0.5f+0.5f;
  gl_Position = vp*(vec4(1.5f,1.f,1.f,1.f)*gl_Position);
}
#endif



#ifdef FRAGMENT_SHADER
precision highp float;

in highp vec2 vCoord;
out highp vec4 fColor;

vec4 japanTexture(in vec2 uv){
  vec2 suv = uv * vec2(3,2);
  float dist = distance(suv,vec2(1.5f,1.f));
  vec3 red    = vec3(0xBC,0x00,0x2D)/vec3(255);
  vec3 white  = vec3(0xFF,0xFF,0xFF)/vec3(255);
  return vec4(mix(red,white,float(dist > 3.f/5.f)),1.f);
}

void main(){
  fColor = japanTexture(vCoord);
}
#endif
