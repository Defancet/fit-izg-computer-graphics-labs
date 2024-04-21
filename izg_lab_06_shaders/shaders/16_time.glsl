#ifdef VERTEX_SHADER
#line 2
precision highp float;
uniform highp mat4 view;
uniform highp mat4 proj;

out highp vec2 vCoord;
void main(){
  gl_Position = vec4(0.f,0.f,0.f,1.f);

  //fullscreen quad
  highp vec2 verts[]= vec2[](
    vec2(-1.f,-1.f),
    vec2(+1.f,-1.f),
    vec2(-1.f,+1.f),
    vec2(-1.f,+1.f),
    vec2(+1.f,-1.f),
    vec2(+1.f,+1.f)
  );

  if(gl_VertexID>=verts.length())return;

  gl_Position = vec4(verts[gl_VertexID],0.f,1.f);
  vCoord = verts[gl_VertexID]*.5f+.5f;
}
#endif



#ifdef FRAGMENT_SHADER
precision highp float;

uniform highp float iTime;
in highp vec2 vCoord;
out highp vec4 fColor;
void main(){
  fColor = vec4(abs(sin(vCoord.x*radians(360.f)+iTime)*.1f+.5f-vCoord.y)<0.01f);
}
#endif
