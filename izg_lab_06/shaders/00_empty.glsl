#ifdef VERTEX_SHADER
precision highp float;

uniform mat4 view = mat4(1);
uniform mat4 proj = mat4(1);

uniform float iTime;

flat out int vM;
out vec2 vCoord;

int drawQuad(int offset,mat4 mvp,int m){
  vec2 verts[] = vec2[](
  vec2(0,0),
  vec2(1,0),
  vec2(0,1),
  vec2(0,1),
  vec2(1,0),
  vec2(1,1)
  );

  int vID = gl_VertexID - offset;

  if(vID < 0 || vID>=verts.length())return verts.length();

  gl_Position = mvp * vec4(verts[vID]*2-1,0.f,1.f);
  vM = m;
  vCoord = verts[vID];

  return verts.length();
}

int drawCube(int offset,mat4 mvp,int m){
  uint indices[] = uint[](
  0u,1u,2u,2u,1u,3u,
  4u,5u,6u,6u,5u,7u,
  0u,4u,2u,2u,4u,6u,
  1u,5u,3u,3u,5u,7u,
  0u,1u,4u,4u,1u,5u,
  2u,3u,6u,6u,3u,7u
  );

  int vID = gl_VertexID - offset;

  if(vID < 0 || vID>=indices.length())return indices.length();

  vec3 pos;
  for(uint i=0u;i<3u;++i)
  pos[i] = float((indices[vID]>>i)&1u);

  gl_Position = mvp*vec4(pos*2.0f-1.f,1.f);
  vM = m;


  return indices.length();
}

int drawSphere(in int offset,in mat4 mvp,in int nCols,in int nRows,int m){
  int nCells = nCols * nRows;
  int verticesPerCell = 6;
  int nVertices = nCells * verticesPerCell;
  int verticesPerRow = nCols * verticesPerCell;

  int vID = gl_VertexID-offset;
  if(vID < 0 || vID >= nVertices)return nVertices;

  int cellID = vID / verticesPerCell;
  int verID  = vID % verticesPerCell;

  int rowID = cellID / nCols;
  int colID = cellID % nCols;

  ivec2 verOffset[] = ivec2[](
  ivec2(0,0),
  ivec2(1,0),
  ivec2(0,1),
  ivec2(0,1),
  ivec2(1,0),
  ivec2(1,1)
  );

  float zAngleNorm = float(colID + verOffset[verID].x * 1.f) / float(nCols);
  float yAngleNorm = float(rowID + verOffset[verID].y * 1.f) / float(nRows);


  float yAngle = radians(yAngleNorm * 180.f - 90.f);
  float zAngle = radians(zAngleNorm * 360.f);

  vec2 xyCircle = vec2(         cos(zAngle),sin(zAngle));
  vec3 sphere   = vec3(xyCircle*cos(yAngle),sin(yAngle));

  vCoord = vec2(zAngleNorm,yAngleNorm);
  //vNormal   = sphere;
  gl_Position = mvp * vec4(sphere,1.f);
  vM = m;

  return nVertices;
}

mat4 T(float x,float y,float z){
  mat4 r = mat4(1);
  r[3] = vec4(x,y,z,1);
  return r;
}


mat4 Ry(float a){
  mat4 r = mat4(1);
  float ca = cos(a);
  float sa = sin(a);
  r[0][0] = ca;
  r[0][2] = sa;
  r[2][0] = -sa;
  r[2][2] = ca;

  return r;
}

mat4 Rx(float a){
  mat4 r = mat4(1);
  float ca = cos(a);
  float sa = sin(a);
  r[1][1] = ca;
  r[1][2] = sa;
  r[2][1] = -sa;
  r[2][2] = ca;

  return r;
}

mat4 Rz(float a){
  mat4 r = mat4(1);
  float ca = cos(a);
  float sa = sin(a);
  r[0][0] = ca;
  r[0][1] = sa;
  r[1][0] = -sa;
  r[1][1] = ca;

  return r;
}


mat4 S(float x,float y,float z){
  mat4 r = mat4(1);
  r[0][0] = x;
  r[1][1] = y;
  r[2][2] = z;
  return r;
}

mat4 S(vec3 s){
  mat4 r = mat4(1);
  r[0][0] = s.x;
  r[1][1] = s.y;
  r[2][2] = s.z;
  return r;
}

mat4 S(float x){
  mat4 r = mat4(1);
  r[0][0] = x;
  r[1][1] = x;
  r[2][2] = x;
  return r;
}

void main(){
  gl_Position = vec4(0.f,0.f,0.f,1.f);

  mat4 vp = proj * view;

  //int qID = gl_VertexID/6;

  int offset = 0;
  offset += drawQuad(offset,vp,0);

  offset += drawQuad(offset,vp*T(10,sin(iTime),0),1);
  offset += drawQuad(offset,vp*T(4,4,0),2);
  offset += drawQuad(offset,vp*T(6,4,0),3);
  offset += drawQuad(offset,vp*T(9,4,0),4);
  offset += drawQuad(offset,vp*T(4,6,0),5);
  offset += drawQuad(offset,vp*T(6,6,0),6);
  offset += drawQuad(offset,vp*T(8,6,0),7);
  offset += drawQuad(offset,vp*T(10,6,0),8);
  offset += drawQuad(offset,vp*T(2,8,0),9);
  offset += drawCube(offset,vp*T(-5,sin(iTime),0),1);
  offset += drawSphere(offset,vp*T(0,3,0),10,10,0);
}
#endif



#ifdef FRAGMENT_SHADER
precision highp float;

uniform float iTime;

flat in int vM;
in vec2 vCoord;

out vec4 fColor;

vec4 circle(vec2 uv){
  float d = length(uv-.5);
  float r = (sin(iTime*4)*.5+.5)*.3+0.2;
  if(d > r)discard;
  return vec4(0,1,0,1);
}

void main(){
  if(vM==0)fColor = vec4(1,0,1,1);
  if(vM==1)fColor = vec4(0,0,1,1);
  if(vM==100)fColor = vec4(0,0,0,1);
  if(vM==101)fColor = vec4(1,0,0,1);
  if(vM==2)fColor = vec4(vCoord,0,1);
}

#endif
