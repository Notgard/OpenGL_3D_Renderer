#version 330 core

out vec4 FragColor;

in mat3 normal_matrix;
in vec3 norm;
in vec2 textureCoordV;
in vec4 vertexColor;
in vec3 worldPosition;

in vec3 gnorm;
in vec3 guvs;
in vec3 gpos;
in mat4 gview;
in mat3 gnormmat;

in vec4 gcolor;

uniform float textureMapping;

uniform vec3  cameraPosition;
uniform vec3  lightPosition;
uniform vec3  lightColor;
uniform float lightIntensity;

uniform vec4  lightAmbient;
uniform vec4  lightDiffuse;
uniform float lightSpecular;

uniform sampler2D diffuse_map;
uniform sampler2D specex_map;
uniform sampler2D normal_map;

uniform int hasDiffuse = 0;
uniform int hasNormal = 0;
uniform int hasSpecex = 0;

uniform int textureMappingBool;

vec4 computePhong(vec3 light, vec4 lightcol, float lightint, vec3 pos, vec3 norm, vec3 camPos, float specPower, vec4 ambiant, vec4 diffuse, vec4 specStrength)
{
    vec3 normal = normalize(norm);
    vec3 lightDir = normalize(light - pos);
    vec3 viewDir = normalize(camPos - pos);
    
    // **Diffuse Lighting**
    float diffAngle = max(dot(normal, lightDir), 0.0);
    vec4 diffuseColor = diffuse * diffAngle;

    // **Blinn-Phong Specular**
    vec3 halfwayDir = normalize(viewDir + lightDir);
    float specAngle = max(dot(normal, halfwayDir), 0.0);
    float specular = pow(specAngle, specPower);
    vec4 specularColor = lightint * lightcol * specular * specStrength;

    // **Light Attenuation**
    //float distance = length(light - pos);
    //float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
    
    // **Final Lighting Computation**
    return (ambiant * lightcol + diffuseColor * lightcol) + specularColor;
}

//TODO: make sure we get the same render for the geometry variables and the vertex variables
// then, we might be able to play around with th geometry correctly
// For now, we first need to fix multi texture buffering on the opengl c++ side
//second, fix the texturemapping and outside uniforms that are not being passed correctly
void main()
{
    vec3 normal;
    vec4 diffuse;
    vec4 specular_strength;
    //vec3 position = worldPosition;
    vec3 position = gpos;

    int mapping = int(textureMapping);

    // **Load Diffuse Texture**
    if(textureMappingBool == 1 && hasDiffuse == 1) {
        diffuse = texture(diffuse_map, textureCoordV);
        //GEOMdiffuse = texture(diffuse_map, guvs.xy);
    } else {
        diffuse = lightDiffuse;
    }
    
    // **Load Normal Map Correctly**
    if(textureMappingBool == 1 && hasNormal == 1) {
        vec3 texture_normal = texture(normal_map, textureCoordV).rgb;
        //GEOMvec3 texture_normal = texture(normal_map, guvs.xy).rgb;
        texture_normal = normalize(texture_normal * 2.0 - 1.0);
        normal = normalize(normal_matrix * texture_normal);  // Transform to world space
        //GEOMnormal = normalize(gnormmat * texture_normal);  // Transform to world space
    } else {
        normal = normalize(norm);
        //GEOMnormal = normalize(gnorm);
    }

    // **Load Specular Map**
    if(textureMappingBool == 1 && hasSpecex == 1) {
        specular_strength = texture(specex_map, textureCoordV);
        //GEOMspecular_strength = texture(specex_map, guvs.xy);
    } else {
        specular_strength = vec4(1.0);
    }

    // **Compute Phong Lighting**
    vec4 phong_lighting = computePhong(
        lightPosition,
        vec4(lightColor, 1.0),
        lightIntensity,
        position,
        normal,
        cameraPosition,
        lightSpecular,
        lightAmbient,
        diffuse, 
        specular_strength
    );

    //FragColor = vec4(textureMapping);
    FragColor = phong_lighting * vertexColor;
    //FragColor = texture(diffuse_map, textureCoordV);
    //GEOMFragColor = phong_lighting * gcolor;
}
