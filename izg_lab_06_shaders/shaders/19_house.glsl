#ifdef VERTEX_SHADER

uniform mat4 view;
uniform mat4 proj;
#line 5


//function to calculate other coords of walls
vec4 [6]getWalls(vec4 wall[6]){
    vec4 walls[6] = wall;
    walls[2] -= vec4(1,0,-1,0);
    walls[3] -= vec4(1,0,-1,0);
    walls[5] -= vec4(1,0,-1,0);
    return walls;
}

//function to calculate other coords of roof
vec4 [6]getRoof(vec4 roof[6], vec4 offset){
    vec4 newroof[6] = roof;
    for (int i = 0; i < 6; i++)
    {
        if (i == 2 || i == 5) continue;
        newroof[i] += offset;
    }
    return newroof;
}

out vec2 vCoord;
//function for setting vCoords of fragments
void setAttr(vec4 vertex, vec4 arr[6], int material, vec4 offset){
    if (material == 0 || material == 1) {
        if (vertex == arr[0]+offset) vCoord = vec2(0,0);
        if (vertex == arr[1]+offset) vCoord = vec2(1,0);
        if (vertex == arr[2]+offset) vCoord = vec2(0,1);
        if (vertex == arr[5]+offset) vCoord = vec2(1,1);
    } else {
        if (vertex == arr[0]) vCoord = vec2(0,0);
        if (vertex == arr[1]) vCoord = vec2(0,0);
        if (vertex == arr[4]) vCoord = vec2(0,0);
        if (vertex == arr[2]) vCoord = vec2(1,0);
    }
    return;
}


flat out int material;
void main(){

    if(gl_VertexID>=42){
        gl_Position = vec4(0.f,0.f,0.f,1.f);
        return;
    }

    mat4 mvp = proj*view;

    //coordinates of the ground
    vec4 ground[]= vec4[](
    vec4(-2,0,-2,1),
    vec4( 2,0,-2,1),
    vec4(-2,0, 2,1),
    vec4(-2,0, 2,1),
    vec4( 2,0,-2,1),
    vec4( 2,0, 2,1)
    );

    //coordinates of a single wall of the house
    vec4 wall[]=vec4[](
    vec4(-0.5,0,-0.5,1),
    vec4(-0.5,1,-0.5,1),
    vec4( 0.5,0,-0.5,1),
    vec4( 0.5,0,-0.5,1),
    vec4(-0.5,1,-0.5,1),
    vec4( 0.5,1,-0.5,1)
    );

    vec4 roof[]=vec4[](
    vec4(-0.5,1,-0.5,1),
    vec4( 0.5,1,-0.5,1),
    vec4(   0,2,   0,1),
    vec4(-0.5,1,-0.5,1),
    vec4(-0.5,1, 0.5,1),
    vec4(   0,2,   0,1)
    );

    //set attributes of the ground
    if(gl_VertexID<ground.length()){
        material = 0; //material 0 = the ground
        gl_Position = mvp*vec4(ground[gl_VertexID]);
        setAttr(ground[gl_VertexID], ground, material, vec4(0.f));
        return;
    }
    else if((gl_VertexID-12)<wall.length()){
        int ID = gl_VertexID-6;
        material = 1;
        if (ID < 6){
            gl_Position = mvp*vec4(wall[ID]);
            setAttr(wall[ID], wall, material, vec4(0.f));
        } else {
            ID -= 6;
            vec4 walls[6] = getWalls(wall);
            gl_Position = mvp*vec4(walls[ID]);
            setAttr(walls[ID], walls, material, vec4(0.f));
        }
        return;
    }
    else if((gl_VertexID-24)<wall.length()){
        int ID = gl_VertexID - 18;
        material = 1;
        if (ID < 6){
            vec4 offset = vec4(0,0,1,0);
            vec4 coords = vec4(wall[ID])+offset;
            gl_Position = mvp*coords;
            setAttr(coords, wall, material, offset);
        } else {
            ID -= 6;
            vec4 offset = vec4(1,0,0,0);
            vec4 walls[6] = getWalls(wall);
            vec4 coords = walls[ID]+vec4(1,0,0,0);
            gl_Position = mvp*coords;
            setAttr(coords, walls, material, offset);
        }
        return;
    }
    else if((gl_VertexID-42)<wall.length()){
        int ID = gl_VertexID - 30;
        material = 2;
        if (ID < 6){
            gl_Position = mvp*vec4(roof[ID]);
            setAttr(roof[ID], roof, material, vec4(0.f));
        } else {
            ID -= 6;
            if (ID < 3){
                vec4 offset = vec4(0,0,1,0);
                vec4 roof2[6] = getRoof(roof, offset);
                gl_Position = mvp*vec4(roof2[ID]);
                vec4 coords = roof2[ID];
                if (coords == roof2[0]) vCoord = vec2(0,0);
                if (coords == roof2[1]) vCoord = vec2(0,0);
                if (coords == roof2[2]) vCoord = vec2(1,0);
            } else {
                vec4 offset = vec4(1,0,0,0);
                vec4 roof2[6] = getRoof(roof, offset);
                gl_Position = mvp*vec4(roof2[ID]);
                vec4 coords = roof2[ID];
                if (coords == roof2[3]) vCoord = vec2(0,0);
                if (coords == roof2[4]) vCoord = vec2(0,0);
                if (coords == roof2[5]) vCoord = vec2(1,0);
            }
        }
    }
}
#endif


#ifdef FRAGMENT_SHADER
in vec2 vCoord;
flat in int material;
out highp vec4 fColor;
void main(){
    vec2 uv = vCoord;
    if (material == 0){
        uv = fract(vCoord*4);
        float d = distance(uv,vec2(0.5));
        float c = float(d<0.5);
        vec3 col = mix(vec3(0,0.5,0),vec3(0,1,0),c);
        fColor = vec4(col,1);
    } else if (material == 1){
        float c = float(uv.x > .1 && uv.x < .9 && uv.y > .1 && uv.y < .9);
        vec4 col = mix(vec4(0,0,0,1),vec4(0.5,0.5,0,0.5),c);
        fColor = vec4(col);
    } else if (material == 2){
        fColor = vec4(vCoord,0,1);
    }
}
#endif