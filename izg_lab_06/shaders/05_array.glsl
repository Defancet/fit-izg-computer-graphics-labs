#ifdef VERTEX_SHADER
#line 2
precision highp float;

uniform mat4 view;
uniform mat4 proj;

void main(){
  gl_Position = vec4(0.f,0.f,0.f,1.f);
  const vec2 vertices[] = vec2[](
    vec2(0.f,0.f),
    vec2(1.f,0.f),
    vec2(0.f,1.f),
    vec2(1.f,0.f),
    vec2(2.f,0.f),
    vec2(1.f,1.f)
  );
  mat4 vp = proj*view;
  if(gl_VertexID>=vertices.length())return;
  gl_Position = vp * vec4(vertices[gl_VertexID],0.f,1.f);
}
#endif



#ifdef FRAGMENT_SHADER
precision highp float;

out vec4 fColor;

void main(){
  fColor = vec4(0.f,0.5f,0.f,1.f);
}

#endif
