#ifdef VERTEX_SHADER
//#define MAKE_STUDENT_RELEASE
#line 3
precision highp float;
uniform float iTime;
uniform mat4 view;
uniform mat4 proj;
flat out int id;

out highp vec2 vCoord;


void main(){
  gl_Position = vec4(0.f,0.f,0.f,1.f);

  uint indices[] = uint[](
  0u,1u,2u,2u,1u,3u,
  4u,5u,6u,6u,5u,7u,
  0u,4u,2u,2u,4u,6u,
  1u,5u,3u,3u,5u,7u,
  0u,1u,4u,4u,1u,5u,
  2u,3u,6u,6u,3u,7u
  );

  if(gl_VertexID>=indices.length())return;

  if(gl_VertexID>=0 && gl_VertexID<= 5)
  id = 0;
  if(gl_VertexID>=6 && gl_VertexID<= 11)
  id = 1;
  if(gl_VertexID>=12 && gl_VertexID<= 17)
  id = 2;
  if(gl_VertexID>=18 && gl_VertexID<= 23)
  id = 3;
  if(gl_VertexID>=24 && gl_VertexID<= 29)
  id = 4;
  if(gl_VertexID>=30 && gl_VertexID<= 35)
  id = 5;

  vec3 pos;
  for(uint i=0u;i<3u;++i)
  pos[i] = float((indices[gl_VertexID]>>i)&1u);

  mat3 rotation;
  rotation[0] = vec3(cos(iTime),0.0f,-sin(iTime));
  rotation[1] = vec3(0.0f,1.0f , 0.0f);
  rotation[2] = vec3(sin(iTime),0.0f , cos(iTime));




  gl_Position = proj*view*vec4(rotation*pos*2.f-1.f,1.f);

}
#endif



#ifdef FRAGMENT_SHADER
precision highp float;
out highp vec4 fColor;

uniform highp float iTime;
flat in int id;
in highp vec2 vCoord;


void main(){
  if(id == 0)
  fColor = vec4(1.f,0.0f,0.0f,1.0f);
  if(id == 1)
  fColor = vec4(1.f,1.0f,0.0f,1.0f);
  if(id == 2)
  fColor = vec4(1.f,1.0f,1.0f,1.0f);
  if(id == 3)
  fColor = vec4(0.f,1.0f,1.0f,1.0f);
  if(id == 4)
  fColor = vec4(0.f,0.0f,1.0f,1.0f);
  if(id == 5)
  fColor = vec4(0.f,0.0f,0.0f,1.0f);


}
#endif