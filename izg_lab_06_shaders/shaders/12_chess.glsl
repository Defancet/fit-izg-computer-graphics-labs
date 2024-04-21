#ifdef VERTEX_SHADER
#line 2
precision highp float;
uniform mat4 view;
uniform mat4 proj;

out vec2 vTexCoord;

void main(){
  gl_Position = vec4(0.f,0.f,0.f,1.f);

  vec2 vertices[] = vec2[](
    vec2(-1.f,-1.f),
    vec2(+1.f,-1.f),
    vec2(-1.f,+1.f),
    vec2(+1.f,+1.f)
  );
  mat4 vp = proj*view;
  
  uint indices[] = uint[](
    0u,1u,2u,2u,1u,3u
  );

  if(gl_VertexID >= indices.length())return;

  uint index = indices[gl_VertexID];

  gl_Position = vp * vec4(vertices[index],0.f,1.f);
  vTexCoord = vertices[index]*0.5f+0.5f;
}
#endif



#ifdef FRAGMENT_SHADER
precision highp float;

out vec4 fColor;

in vec2 vTexCoord;

void main(){
  float repeatCount = 8.f;
  vec2 repeatedUV = fract(vTexCoord*repeatCount);
  float chessPattern = float(repeatedUV.x>0.5f == repeatedUV.y>0.5f);
  fColor = vec4(vec3(chessPattern),1.f);
}

#endif
