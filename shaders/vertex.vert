#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coords;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out mat3 normal_matrix;
out vec3 norm;
out vec2 textureCoordV;
out vec4 vertexColor;
out vec3 worldPosition;

out VertexData
{
    vec3 Normal;
    vec4 color;
    vec2 texCoord;
    mat4 projection;
    mat4 mvpMat;
} vertex;

out vec3 fnorm;
out vec3 fuvs;
out vec3 fpos;
out mat4 fview;
out mat3 fnormmat;

void main()
{
    mat4 modelView = view * model;
    mat3 normMatrix = transpose(inverse(mat3(model)));

    vec3 model_space_position = vec3(model * vec4(position, 1.0));
    
    vec3 model_space_normal = vec3(normalize(normMatrix * normal));

    //setting up the outputs    
    norm = model_space_normal;
    textureCoordV = tex_coords;
    worldPosition = model_space_position;
    vertexColor = color;
    normal_matrix = normMatrix;

    gl_Position = projection * view * model * vec4(position, 1.0);

    fnorm = model_space_normal;
    fuvs = vec3(tex_coords, 1.0);
    fpos = model_space_position;
    fview = view;
    fnormmat = normMatrix;

    vertex.Normal = model_space_normal;
    vertex.texCoord = tex_coords.xy;
    vertex.color = color;
    vertex.projection = projection;
    vertex.mvpMat = projection * view * model;
}