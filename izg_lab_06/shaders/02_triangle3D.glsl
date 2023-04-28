#ifdef VERTEX_SHADER
#line 2
precision highp float;

uniform mat4 view;
uniform mat4 proj;

void main(){
  gl_Position = vec4(0.f,0.f,0.f,1.f);
  mat4 vp = proj*view;
  if(gl_VertexID==0)gl_Position = vp*vec4(0.f,0.f,0.f,1.f);
  if(gl_VertexID==1)gl_Position = vp*vec4(1.f,0.f,0.f,1.f);
  if(gl_VertexID==2)gl_Position = vp*vec4(0.f,1.f,0.f,1.f);
}
#endif



#ifdef FRAGMENT_SHADER
precision highp float;

out vec4 fColor;

void main(){
  fColor = vec4(1.f,0.f,0.f,1.f);
}

#endif
