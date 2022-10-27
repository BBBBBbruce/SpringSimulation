#version 330 core
out vec4 FragColor;
#define PI 3.14159265


uniform sampler2D diffuse;
uniform sampler2D metallic;
uniform sampler2D normal;
uniform sampler2D rough;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform float f0 = 0.8;//0.5-1.0
uniform float k = 0.2;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform samplerCube skybox;



vec3 getNormalFromMap()
{   

    vec3 tangentNormal = texture(normal, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(FragPos);
    vec3 Q2  = dFdy(FragPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}


float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

void main()
{	
    vec3 I = normalize(FragPos - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    vec3 lightColor = texture(skybox, R).rgb;

    vec3 albedo     = pow(texture(diffuse, TexCoords).rgb, vec3(2.2));
    //float metallic  = texture(metallic, TexCoords).r;
    float metallic = 0;
    float roughness = texture(rough, TexCoords).r;
  

    vec3 N = getNormalFromMap();
    vec3 V = normalize(viewPos - FragPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);

    // calculate per-light radiance
    vec3 L = normalize(lightPos - FragPos);
    vec3 H = normalize(V + L);

    //float distance = length(single_light.position - FragPos);
    //float attenuation = 1.0 / (distance * distance);
    //vec3 radiance = single_light.position * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);   
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
    vec3 nominator    = NDF * G * F; 
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
    vec3 specular = nominator / denominator;
        
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);        

    // add to outgoing radiance Lo
    //Lo += (kD * albedo / PI + specular) * 1 * NdotL;
    float distance = length(lightPos - FragPos);
    //float attenuation = 1.0 / (distance * distance);
    float attenuation = 1.0;
    vec3 radiance = lightColor * attenuation;

    Lo += (kD * albedo / PI + specular) * radiance * NdotL;// note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again

    vec3 ambient = vec3(0.9) * albedo;
    
    vec3 color = ambient + Lo*4;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    //FragColor = vec4(color, 1.0);
    FragColor = vec4(1.0);
}