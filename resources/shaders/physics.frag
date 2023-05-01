#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 normals;

struct directionalLightStruct {
    float ambientIntensity;
    float diffuseIntensity;
    vec4 color;
    vec4 direction;
};

layout(binding = 1) uniform DirectionalLight { directionalLightStruct base; }
dLight;

layout(location = 0) out vec4 outColor;

vec4 CalcLightByDirection(vec4 color, float ambientIntensity,
                          float diffuseIntensity, vec4 direction) {
    vec4 ambientColor = color * ambientIntensity;
    // base off of the angle of the light towards a surface
    vec3 fix = vec3(-direction.x, -direction.y, -direction.z);
    float diffuseFactor = max(dot(normalize(normals), normalize(fix)), 0.0f);
    // diffuse
    vec3 diffCol = color.xyz * diffuseIntensity * diffuseFactor;
    vec4 diffuseColor = vec4(diffCol, 1.0f);

    // specular
    /*
    vec4 specularColor = vec4(0, 0, 0, 0);
    if (diffuseFactor > 0.0f) {

        vec3 fragToEye = normalize(camPos.pos - fragPos.xyz);
        vec3 halfwayDir = normalize(-direction.xyz + fragToEye);

        vec3 reflectedVert =
            normalize(reflect(direction.xyz, normalize(normals)));

        // float specFactor = dot(fragToEye, reflectedVert);
        float specFactor = max(dot(normals, halfwayDir), 0.0f);

        if (specFactor > 0.0f) {
            specFactor = pow(specFactor, mat.base.shininess);
            specularColor = vec4(color.xyz * diffuseIntensity *
                                     mat.base.specularIntensity * specFactor,
                                 1.0f);
        }
    }
    */

    return (ambientColor + diffuseColor /* + specularColor */);
}

vec4 CalcDirectionalLight() {
    return CalcLightByDirection(dLight.base.color, dLight.base.ambientIntensity,
                                dLight.base.diffuseIntensity,
                                dLight.base.direction);
}

void main() {
    vec4 lightColor = vec4(0, 0, 0, 1);
    lightColor = CalcDirectionalLight();

    outColor = vec4((fragColor * (lightColor).xyz), 1.0);
}