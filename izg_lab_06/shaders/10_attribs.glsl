#ifdef VERTEX_SHADER
#line 2
precision highp float;

uniform mat4 view;
uniform mat4 proj;

out vec3 vColor;

void main(){
  gl_Position = vec4(0.f,0.f,0.f,1.f);
  mat4 vp = proj*view;
  if(gl_VertexID==0){
    vColor = vec3(1.f,0.f,0.f);
    gl_Position = vp*vec4(-1.f,-1.f,0.f,1.f);
  }
  if(gl_VertexID==1){
    vColor = vec3(0.f,1.f,0.f);
    gl_Position = vp*vec4(+1.f,-1.f,0.f,1.f);
  }
  if(gl_VertexID==2){
    vColor = vec3(0.f,0.f,1.f);
    gl_Position = vp*vec4(+0.f,+1.f,0.f,1.f);
  }
}
#endif



#ifdef FRAGMENT_SHADER
precision highp float;

out vec4 fColor;

in vec3 vColor;

void main(){
  fColor = vec4(vColor,1.f);
}

#endif
