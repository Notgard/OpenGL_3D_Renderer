#version 330 core

// Input from the vertex shader
layout(triangles) in;

// Output to the fragment shader
layout(triangle_strip, max_vertices = 3) out;

// Inputs from the vertex shader
in mat3 normal_matrix[];
in vec3 norm[];
in vec2 textureCoordV[];
in vec4 vertexColor[];
in vec3 worldPosition[];

in vec3 fnorm[];
in vec3 fuvs[];
in vec3 fpos[];
in mat4 fview[];
in mat3 fnormmat[];

in vec4 fcolor[];

// Outputs to the fragment shader
out mat3 out_normal_matrix;
out vec3 out_norm;
out vec2 out_textureCoordV;
out vec4 out_vertexColor;
out vec3 out_worldPosition;

out vec3 out_gnorm;
out vec3 out_guvs;
out vec3 out_gpos;
out mat4 out_gview;
out mat3 out_gnormmat;

out vec4 out_gcolor;

void main()
{
    for (int i = 0; i < 3; i++) // Loop over the three vertices of the triangle
    {
        // Pass through all the inputs to the outputs
        out_normal_matrix = normal_matrix[i];
        out_norm = norm[i];
        out_textureCoordV = textureCoordV[i];
        out_vertexColor = vertexColor[i];
        out_worldPosition = worldPosition[i];

        out_gnorm = fnorm[i];
        out_guvs = fuvs[i];
        out_gpos = fpos[i];
        out_gview = fview[i];
        out_gnormmat = fnormmat[i];

        out_gcolor = fcolor[i];

        // Set the position of the vertex
        gl_Position = gl_in[i].gl_Position;

        // Emit the vertex
        EmitVertex();
    }

    // End the primitive
    EndPrimitive();
}