#ifdef VERTEX_SHADER
#line 2
precision highp float;
uniform mat4 view;
uniform mat4 proj;

out highp vec2 vCoord;
flat out int matrialId;

void drawQuad(int vId,vec2 size,vec2 offset,mat4 mvp,int material){
  if(vId<0 || vId>=6)return;
  if(vId==0)gl_Position=vec4(-1.0,-1.f,0.f,1.f);
  if(vId==1)gl_Position=vec4(+1.0,-1.f,0.f,1.f);
  if(vId==2)gl_Position=vec4(-1.0,+1.f,0.f,1.f);
  if(vId==3)gl_Position=vec4(-1.0,+1.f,0.f,1.f);
  if(vId==4)gl_Position=vec4(+1.0,-1.f,0.f,1.f);
  if(vId==5)gl_Position=vec4(+1.0,+1.f,0.f,1.f);

  matrialId = material;
  vCoord = gl_Position.xy*0.5f+.5f;
  gl_Position = mvp*(gl_Position*vec4(size,1.f,1.f)+vec4(offset,0.f,0.f));
}

void main(){
  gl_Position = vec4(0.f,0.f,0.f,1.f);
  if(gl_VertexID>=12)return;

  mat4 mvp = proj*view;

  drawQuad(gl_VertexID  ,vec2(1.f,2.f/3.f),vec2(-2.f,0.f),mvp,0);

  drawQuad(gl_VertexID-6,vec2(1.f,2.f/3.f),vec2( 2.f,0.f),mvp,1);

}
#endif



#ifdef FRAGMENT_SHADER
precision highp float;

flat in int matrialId;
in highp vec2 vCoord;
out highp vec4 fColor;

highp vec4 vietnam(highp vec2 uv )
{
  uv *= vec2(3.f,2.f);
  const vec3  red    = vec3(0xda,0x25,0x1d)/vec3(0xff);
  const vec3  yellow = vec3(0xff,0xff,0x00)/vec3(0xff);
  const float radius = 6.f/20.f*2.f;
  const vec2  center = vec2(1.5f,1.f);
  const mat2  rot    = mat2(
    cos(radians(144.f)),-sin(radians(144.f)),
    sin(radians(144.f)), cos(radians(144.f))
  );

  bool  parity = true;
  bool  inter  = true;
  vec2  starA  = vec2(0,radius);
  for(uint i=0u;i<5u;++i){
    vec2 starB  = rot*starA;
    vec2 edge   = starB-starA;
    vec2 normal = vec2(-edge.y,edge.x);
    vec3 line   = vec3(normal,-dot(normal,starA+center));
    float dist  = dot(line,vec3(uv,1.f));
    starA       = starB;
    parity = parity != (dist < 0.f);
    inter  = inter  && (dist < 0.f);
  }

  float mixF = float((parity || inter) && distance(uv,center)<radius);
  return vec4(mix(red,yellow,mixF),1.0);
}

highp vec4 thailand(highp vec2 uv){
  uv *= vec2(9.f,6.f);
  highp vec3 white = vec3(0xf4,0xf5,0xf8)/vec3(255);
  highp vec3 red   = vec3(0xa5,0x19,0x31)/vec3(255); 
  highp vec3 blue  = vec3(0x2d,0x2a,0x4a)/vec3(255);
  highp float dist = abs(uv.y-3.f);
  if(dist<1.f)return vec4(blue,1.f);
  if(dist<2.f)return vec4(white,1.f);
  return vec4(red,1.f);
}

void main(){

  if(matrialId==0)fColor = vietnam (vCoord);
  if(matrialId==1)fColor = thailand(vCoord);
}
#endif
