#ifdef VERTEX_SHADER
precision highp float;

void main(){
  gl_Position = vec4(0.f,0.f,0.f,1.f);
  if(gl_VertexID==0)gl_Position = vec4(-1.f,-1.f,0.f,1.f);
  if(gl_VertexID==1)gl_Position = vec4(+1.f,-1.f,0.f,1.f);
  if(gl_VertexID==2)gl_Position = vec4(-1.f,+1.f,0.f,1.f);
  if(gl_VertexID==3)gl_Position = vec4(-1.f,+1.f,0.f,1.f);
  if(gl_VertexID==4)gl_Position = vec4(+1.f,-1.f,0.f,1.f);
  if(gl_VertexID==5)gl_Position = vec4(+1.f,+1.f,0.f,1.f);
}
#endif



#ifdef FRAGMENT_SHADER
precision highp float;

out vec4 fColor;

void main(){
  fColor = vec4(1.f,.5f,0.f,1.f);
}

#endif
