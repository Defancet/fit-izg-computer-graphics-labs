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
    gl_Position = vec4(0.f, 0.f, 0.f, 1.f);

    mat4 vp = proj * view;

    //int qID = gl_VertexID/6;

    int offset = 0;

    offset += drawCube(offset, vp*S(1.17f, 0.65f, 0.06f)*T(0.f, 0.f, -1.45f), 0);

    offset += drawCube(offset, vp*S(1.3f, 0.07f, 0.04f)*T(0.f, 10.0f, -0.5f), 0);

    offset += drawCube(offset, vp*S(1.3f, 0.07f, 0.04f)*T(0.f, -10.0f, -0.5f), 0);

    offset += drawCube(offset, vp*S(0.07f, 0.75f, 0.04f)*T(17.6f, 0.f, -0.5f), 0);

    offset += drawCube(offset, vp*S(0.07f, 0.75f, 0.04f)*T(-17.6f, 0.f, -0.5f), 0);

    offset += drawCube(offset, vp*T(0.0f, -1.05f, -0.15f)*S(0.6f, 0.05f, 0.35f), 0);

    offset += drawCube(offset, vp*Rx(radians(105.f))*T(0.f, -0.1f, 0.6f)*S(0.25f, 0.06f, 0.5f), 0);

    offset += drawQuad(offset, vp*T(0.0f, 0.0f, 0.00f)*S(1.2f, 0.65f, -0.5f), 1);


}

#endif


#ifdef FRAGMENT_SHADER
precision highp float;

uniform float iTime;

flat in int vM;
in vec2 vCoord;

out vec4 fColor;

void main(){
    if(vM==0)fColor = vec4(0.25f,0.25f,0.25f,1);

    if(vM==1){
        vec2 modifiedCoord = vec2(vCoord.x + sin(iTime), vCoord.y);
        fColor = vec4(modifiedCoord, 1, 1);
    }
}

#endif