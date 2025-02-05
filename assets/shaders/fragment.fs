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

void calcAmbient(out vec3 ambient, out vec3 diffuse, out vec3 light_ambient, out vec3 light_diffuse, out float diff);
void calcSpecular(out vec3 specular, out vec3 light_specular, out vec3 reflectDir, out vec3 viewDir);
void calcEmbient(out vec3 embient);

vec3 CalcPointLight(PointLight light) {
   vec3 lightDir = normalize(light._position - FragPos);

   float diff = max(dot(lightDir, normalize(Normal)), 0.0);

   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = normalize(reflect(-lightDir, normalize(Normal)));
   
   vec3 ambient  = vec3(0.0);
   vec3 diffuse  = vec3(0.0);
   vec3 specular = vec3(0.0);
   vec3 embient  = vec3(0.0);

   calcAmbient(ambient, diffuse, light._ambient, light._diffuse, diff);
   calcSpecular(specular, light._specular, reflectDir, viewDir);
   calcEmbient(embient);

   
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

   calcAmbient(ambient, diffuse, light._ambient, light._diffuse, diff);
   calcSpecular(specular, light._specular, reflectDir, viewDir);
   calcEmbient(embient);


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

   calcAmbient(ambient, diffuse, light._ambient, light._diffuse, diff);
   calcSpecular(specular, light._specular, reflectDir, viewDir);
   calcEmbient(embient);

   ambient  += light._ambient *   vec3(ourColor);
   diffuse  += light._diffuse * diff *  vec3(ourColor);
   float spec = pow(max(dot(reflectDir, viewDir), 0.0), ourShininess);
   specular += light._specular * spec * vec3(ourColor);

   return (ambient + diffuse + specular + embient);
}


void calcAmbient(out vec3 ambient, out vec3 diffuse, out vec3 light_ambient, out vec3 light_diffuse, out float diff) {
   for (int i = 0;  i < materialDiffuseCount; ++i) {

      if (i == 0) {
         ambient += light_ambient *        vec3(texture(material._diffuse[0], TexCoord));
         diffuse += light_diffuse * diff * vec3(texture(material._diffuse[0], TexCoord));      
      }
      else if (i == 1) {
         ambient += light_ambient *        vec3(texture(material._diffuse[1], TexCoord));
         diffuse += light_diffuse * diff * vec3(texture(material._diffuse[1], TexCoord));      
      }
      else if (i == 2) {
         ambient += light_ambient *        vec3(texture(material._diffuse[2], TexCoord));
         diffuse += light_diffuse * diff * vec3(texture(material._diffuse[2], TexCoord));      
      }
      else if (i == 3) {
         ambient += light_ambient *        vec3(texture(material._diffuse[3], TexCoord));
         diffuse += light_diffuse * diff * vec3(texture(material._diffuse[3], TexCoord));      
      }
      else if (i == 4) {
         ambient += light_ambient *        vec3(texture(material._diffuse[4], TexCoord));
         diffuse += light_diffuse * diff * vec3(texture(material._diffuse[4], TexCoord));      
      }      
      else if (i == 5) {
         ambient += light_ambient *        vec3(texture(material._diffuse[5], TexCoord));
         diffuse += light_diffuse * diff * vec3(texture(material._diffuse[5], TexCoord));      
      }
      else if (i == 6) {
         ambient += light_ambient *        vec3(texture(material._diffuse[6], TexCoord));
         diffuse += light_diffuse * diff * vec3(texture(material._diffuse[6], TexCoord));      
      }      
      else if (i == 7) {
         ambient += light_ambient *        vec3(texture(material._diffuse[7], TexCoord));
         diffuse += light_diffuse * diff * vec3(texture(material._diffuse[7], TexCoord));      
      }  
      else if (i == 8) {
         ambient += light_ambient *        vec3(texture(material._diffuse[8], TexCoord));
         diffuse += light_diffuse * diff * vec3(texture(material._diffuse[8], TexCoord));      
      }  
      else if (i == 9) {
         ambient += light_ambient *        vec3(texture(material._diffuse[9], TexCoord));
         diffuse += light_diffuse * diff * vec3(texture(material._diffuse[9], TexCoord));      
      }                           
   }
   
}


void calcSpecular(out vec3 specular, out vec3 light_specular, out vec3 reflectDir, out vec3 viewDir) {
   for (int i = 0; i < materialSpecularCount; ++i) {
      float spec = pow(max(dot(reflectDir, viewDir), 0.0), material._shininess[i]);

      if (i == 0) {
         specular += light_specular * spec * vec3(texture(material._specular[0], TexCoord));
      }
      else if (i == 1) {
         specular += light_specular * spec * vec3(texture(material._specular[1], TexCoord));
      }
      else if (i == 2) {
         specular += light_specular * spec * vec3(texture(material._specular[2], TexCoord));
      }
      else if (i == 3) {
         specular += light_specular * spec * vec3(texture(material._specular[3], TexCoord));
      }
      else if (i == 4) {
         specular += light_specular * spec * vec3(texture(material._specular[4], TexCoord));
      }
      else if (i == 5) {
         specular += light_specular * spec * vec3(texture(material._specular[5], TexCoord));
      }
      else if (i == 6) {
         specular += light_specular * spec * vec3(texture(material._specular[6], TexCoord));
      }
   }
}


void calcEmbient(out vec3 embient) {
   for (int i = 0; i < materialEmbientCount; ++i) {
      if (i == 0) {
         embient  += vec3(texture(material._embient[0], TexCoord));
      }
      else if (i == 1) {
         embient  += vec3(texture(material._embient[1], TexCoord));
      }
      else if (i == 2) {
         embient  += vec3(texture(material._embient[2], TexCoord));
      }
      else if (i == 3) {
         embient  += vec3(texture(material._embient[3], TexCoord));
      }
      else if (i == 4) {
         embient  += vec3(texture(material._embient[4], TexCoord));
      }
      else if (i == 5) {
         embient  += vec3(texture(material._embient[5], TexCoord));
      }
      else if (i == 6) {
         embient  += vec3(texture(material._embient[6], TexCoord));
      }
   }   
}