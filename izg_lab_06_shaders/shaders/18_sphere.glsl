#ifdef VERTEX_SHADER
#line 2
precision highp float;
uniform mat4 view;
uniform mat4 proj;

out vec2 vTexCoord;
out vec3 vNormal  ;

uint sphere(in uint offset,in mat4 mvp,in uint nCols,in uint nRows,in vec3 center,in float radius){
  uint nCells = nCols * nRows;
  uint verticesPerCell = 6u;
  uint nVertices = nCells * verticesPerCell;
  uint verticesPerRow = nCols * verticesPerCell;

  uint vID = uint(gl_VertexID);
  if(vID < offset || vID >= offset+nVertices)return nVertices;

  uint cellID = vID / verticesPerCell;
  uint verID  = vID % verticesPerCell;

  uint rowID = cellID / nCols;
  uint colID = cellID % nCols;

  uvec2 verOffset[] = uvec2[](
    uvec2(0,0),
    uvec2(1,0),
    uvec2(0,1),
    uvec2(0,1),
    uvec2(1,0),
    uvec2(1,1)
  );

  float zAngleNorm = float(colID + verOffset[verID].x * 0.8f) / float(nCols);
  float yAngleNorm = float(rowID + verOffset[verID].y * 0.8f) / float(nRows);


  float yAngle = radians(yAngleNorm * 180.f - 90.f);
  float zAngle = radians(zAngleNorm * 360.f);

  vec2 xyCircle = vec2(         cos(zAngle),sin(zAngle));
  vec3 sphere   = vec3(xyCircle*cos(yAngle),sin(yAngle));

  vTexCoord = vec2(zAngleNorm,yAngleNorm);
  vNormal   = sphere;
  gl_Position = mvp * vec4(sphere*radius+center,1.f);

  return nVertices;
}

void main(){
  gl_Position = vec4(0.f,0.f,0.f,1.f);
  
  mat4 vp = proj*view;

  uint offset = 0u;

  offset+=sphere(offset,vp,10u,10u,vec3(0.0f,0.f,0.f),1.0f);
  offset+=sphere(offset,vp,10u,10u,vec3(1.5f,1.f,0.f),0.2f);

}
#endif



#ifdef FRAGMENT_SHADER
precision highp float;

in vec2 vTexCoord;
in vec3 vNormal;

out vec4 fColor;

void main(){
  fColor = vec4(vNormal,1.f);
}

#endif
