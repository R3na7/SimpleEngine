#version 330 core
out vec4 FragColor;

struct Material {
   sampler2D _diffuse[10];
   sampler2D _specular[10];
   sampler2D _embient[10];

   float _shininess[10];
};

struct PointLight {
   vec3 _position;

   float _constant;
   float _linear;
   float _quadratic;

   vec3 _ambient;
   vec3 _diffuse;
   vec3 _specular;
};

struct DirectionLight {
   vec3 _direction;

   vec3 _ambient;
   vec3 _diffuse;
   vec3 _specular;
};

struct SpotLight {
   vec3 _position;
   vec3 _direction;
   float _cutOff;
   float _outerCutOff;

   float _constant;
   float _linear;
   float _quadratic;

   vec3 _ambient;
   vec3 _diffuse;
   vec3 _specular;
};

uniform PointLight     pointLights    [10];
uniform DirectionLight directionLights[10];
uniform SpotLight      spotLights     [10];
uniform Material       material;

uniform int pointLightCount;
uniform int directionLightCount;
uniform int spotLightCount;
uniform int materialDiffuseCount;
uniform int materialSpecularCount;
uniform int materialEmbientCount;

uniform vec3 viewPos;
uniform vec4 ourColor;
uniform float ourShininess;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

vec3 CalcPointLight(PointLight light);
vec3 CalcSpotLight(SpotLight light);
vec3 CalcDirectionLight(DirectionLight light);

void main()
{  
   vec3 result = vec3(0.0);

   for (int i = 0; i < spotLightCount; ++i) {
      result += CalcSpotLight(spotLights[i]);
   }
   
   for (int i = 0; i < pointLightCount; ++i) {
      result += CalcPointLight(pointLights[i]);
   }

   for (int i = 0; i < directionLightCount; ++i) {
      result += CalcDirectionLight(directionLights[i]);
   }

   

   FragColor = vec4(result, 1.0);
}


vec3 CalcPointLight(PointLight light) {
   vec3 lightDir = normalize(light._position - FragPos);

   float diff = max(dot(lightDir, normalize(Normal)), 0.0);

   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = normalize(reflect(-lightDir, normalize(Normal)));
   
   vec3 ambient  = vec3(0.0);
   vec3 diffuse  = vec3(0.0);
   vec3 specular = vec3(0.0);
   vec3 embient  = vec3(0.0);

   for (int i = 0;  i < materialDiffuseCount; ++i) {
      ambient += light._ambient *        vec3(texture(material._diffuse[i], TexCoord));
      diffuse += light._diffuse * diff * vec3(texture(material._diffuse[i], TexCoord));
   }
   
   for (int i = 0; i < materialSpecularCount; ++i) {
      float spec = pow(max(dot(reflectDir, viewDir), 0.0), material._shininess[i]);
      specular += light._specular * spec * vec3(texture(material._specular[i], TexCoord));
   }

   for (int i = 0; i < materialEmbientCount; ++i) {
      embient  += vec3(texture(material._embient[i], TexCoord));
   }   

   
   ambient  += light._ambient *   vec3(ourColor);
   diffuse  += light._diffuse * diff *  vec3(ourColor);
   float spec = pow(max(dot(reflectDir, viewDir), 0.0), ourShininess);
   specular += light._specular * spec * vec3(ourColor);



   float dist = length(light._position - FragPos);
   float attenuation = 1.0 / (light._constant + 
                              light._linear * dist + 
                              light._quadratic * (dist * dist));

   ambient *= attenuation;
   diffuse *= attenuation;
   specular*= attenuation;

   return (ambient + diffuse + specular + embient);
}
vec3 CalcSpotLight(SpotLight light) {
   vec3 lightDir = normalize(light._position - FragPos);

   float diff = max(dot(lightDir, normalize(Normal)), 0.0);

   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = normalize(reflect(-lightDir, normalize(Normal)));
   
   vec3 ambient  = vec3(0.0);
   vec3 diffuse  = vec3(0.0);
   vec3 specular = vec3(0.0);
   vec3 embient  = vec3(0.0);

   if (dot(-light._direction, lightDir) > light._outerCutOff) {
      for (int i = 0;  i < materialDiffuseCount; ++i) {
         ambient += light._ambient *        vec3(texture(material._diffuse[i], TexCoord));
         diffuse += light._diffuse * diff * vec3(texture(material._diffuse[i], TexCoord));
      }

      for (int i = 0; i < materialSpecularCount; ++i) {
         float spec = pow(max(dot(reflectDir, viewDir), 0.0), material._shininess[i]);
         specular += light._specular * spec * vec3(texture(material._specular[i], TexCoord));
      }

      for (int i = 0; i < materialEmbientCount; ++i) {
         embient  += vec3(texture(material._embient[i], TexCoord));
      }   


      float dist = length(light._position - FragPos);
      float attenuation = 1.0 / (light._constant + 
                                 light._linear * dist + 
                                 light._quadratic * (dist * dist));

      ambient *= attenuation;
      diffuse *= attenuation;
      specular*= attenuation;

      float theta = dot(-light._direction, lightDir);
      float epsilon = light._cutOff - light._outerCutOff;
      float intesivity = clamp((theta - light._outerCutOff) / epsilon, 0.0, 1.0);

      ambient *= intesivity;
      diffuse *= intesivity;
      specular*= intesivity;
   }

   ambient  += light._ambient *   vec3(ourColor);
   diffuse  += light._diffuse * diff *  vec3(ourColor);
   float spec = pow(max(dot(reflectDir, viewDir), 0.0), ourShininess);
   specular += light._specular * spec * vec3(ourColor);

   return (ambient + diffuse + specular + embient);
}
vec3 CalcDirectionLight(DirectionLight light) {
   
   float diff = max(dot(light._direction, Normal), 0.0);

   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = normalize(reflect(-light._direction, Normal));

   vec3 ambient  = vec3(0.0);
   vec3 diffuse  = vec3(0.0);
   vec3 specular = vec3(0.0);
   vec3 embient  = vec3(0.0);

   for (int i = 0;  i < materialDiffuseCount; ++i) {
      ambient += light._ambient *        vec3(texture(material._diffuse[i], TexCoord));
      diffuse += light._diffuse * diff * vec3(texture(material._diffuse[i], TexCoord));
   }

   for (int i = 0; i < materialSpecularCount; ++i) {
      float spec = pow(max(dot(reflectDir, viewDir), 0.0), material._shininess[i]);
      specular += light._specular * spec * vec3(texture(material._specular[i], TexCoord));
   }

   for (int i = 0; i < materialEmbientCount; ++i) {
      embient  += vec3(texture(material._embient[i], TexCoord));
   }  

   ambient  += light._ambient *   vec3(ourColor);
   diffuse  += light._diffuse * diff *  vec3(ourColor);
   float spec = pow(max(dot(reflectDir, viewDir), 0.0), ourShininess);
   specular += light._specular * spec * vec3(ourColor);

   return (ambient + diffuse + specular + embient);
}
