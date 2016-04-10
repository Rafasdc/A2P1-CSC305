#ifndef SHADERS_H
#define SHADERS_H

#include "glm/gtc/noise.hpp"

const char * vshader_light = " \
        #version 330 core \n\
        in vec3 vpoint; \
        uniform mat4 MvL; \
        void main() { \
            normalize(vpoint);\
            float x = vpoint.x*sqrt(1-(vpoint.y*vpoint.y)/2 - (vpoint.z*vpoint.z)/2 + (vpoint.y*vpoint.y*vpoint.z*vpoint.z)/3);\
            float y = vpoint.y*sqrt(1-(vpoint.z*vpoint.z)/2 - (vpoint.x*vpoint.x)/2+(vpoint.z*vpoint.z*vpoint.x*vpoint.x)/3);\
            float z = vpoint.z*sqrt(1-(vpoint.x*vpoint.x)/2 - (vpoint.y*vpoint.y)/2 + (vpoint.x*vpoint.x*vpoint.y*vpoint.y)/3);\
            gl_Position = MvL* vec4(x,y,z,1.0f);\
        } \
        ";

const char * fshader_light = " \
        #version 330 core \n\
        out vec3 color; \
        void main() { color = vec3(1, 1, 1);} \
        ";

const char * vshader_square = " \
        #version 330 core \n\
        layout (location = 0) in vec3 position; \
        layout (location = 1) in vec3 normal;\
        layout (location = 2) in vec2 texcoord;\
        in vec3 vpoint; \
        uniform mat4 model; \
        uniform mat4 view;\
        uniform mat4 pr;\
        uniform mat4 Mv;\
        out vec3 Normal;\
        out vec3 FragPos; \
        out vec2 UV;\
        void main() { \
            gl_Position =  pr*view*model*vec4(position,1.0f);\
            Normal = normal;\
            FragPos = vec3(model*vec4(position,1.0f));\
            UV = +vec2(texcoord);\
        } \
        ";

const char * fshader_square = " \
        #version 330 core \n\
        out vec4 color; \
        in vec3 Normal; \
        in vec3 FragPos;\
        in vec2 UV;\
         \
        uniform vec3 objectColor;\
        uniform vec3 lightColor; \
        uniform vec3 lightSource;\
        uniform vec3 viewPos;\
        uniform sampler2D tex; \
        \
        void main() {\
            float ambientConstant = 0.1f;\
            vec3 ambient = ambientConstant * lightColor;\
            \
            vec3 norm = normalize(Normal);\
            vec3 lightDirection = normalize(lightSource - FragPos);\
            float diff = max(dot(norm,lightDirection),0.0);\
            vec3 diffuse = diff * lightColor;\
            \
            float specIntensity = 0.5f;\
            vec3 viewD = normalize(viewPos-FragPos);\
            vec3 reflectD = reflect(-lightDirection,norm);\
            float spec = pow(max(dot(viewPos,reflectD),0.0),32);\
            vec3 finalSpecular = specIntensity * spec * lightColor;\
            \
            vec3 toApply = (ambient+diffuse+finalSpecular) * objectColor;\
            color = texture(tex,UV) + vec4(toApply,1.0f);\
        }\
        ";



        const char * vshader_sphere = " \
                #version 330 core \n\
                in vec3 octa_vpoints; \
                uniform mat4 model; \
                uniform mat4 view;\
                uniform mat4 pr;\
                uniform mat4 Mv;\
                out vec3 Normal;\
                out vec3 FragPos;\
                out vec2 UV;\
                void main() { \
                    gl_Position =  pr*view*model*vec4(octa_vpoints,1.0f);\
                    Normal = octa_vpoints;\
                    FragPos = vec3(model*vec4(octa_vpoints,1.0f));\
                    UV = vec2(octa_vpoints.x/2 + 0.5, octa_vpoints.y/2 + 0.5);\
                } \
                ";

        const char * fshader_sphere = " \
                #version 330 core \n\
                out vec4 color;\
                in vec3 Normal;\
                in vec3 FragPos;\
                in vec2 UV; \
                uniform vec3 lightSource;\
                uniform vec3 lightColor;\
                uniform vec3 objectColor;\
                uniform vec3 viewPos;\
                uniform sampler2D tex;\
                void main() {\
                    float ambientConstant = 0.1f;\
                    vec3 ambient = ambientConstant * lightColor;\
                \
                    vec3 norm = normalize(Normal);\
                    vec3 lightDirection = normalize(lightSource - FragPos);\
                    float diff = max(dot(norm,lightDirection),0.0);\
                    vec3 diffuse = diff * lightColor;\
                \
                    float specIntensity = 0.25f;\
                    vec3 viewD = normalize(viewPos-FragPos);\
                    vec3 reflectD = reflect(-lightDirection,norm);\
                    float spec = pow(max(dot(viewPos,reflectD),0.0),2);\
                    vec3 finalSpecular = specIntensity * spec * lightColor;\
                \
                    vec3 toApply = (ambient+diffuse+finalSpecular) * objectColor;\
                    color = mix(texture(tex,UV),vec4(toApply,1.0f),0.5);\
                }\
                ";
        const char * vshader_sky = " \
                #version 330 core \n\
                layout (location = 0) in vec3 position; \
                layout (location = 2) in vec2 vtexcoord;\
                out vec2 uv; \
                uniform mat4 model; \
                uniform mat4 view;\
                uniform mat4 pr;\
                void main() { \
                    gl_Position =  pr*view*model*vec4(position,1.0f);\
                    uv = vec2(vtexcoord.x, 1.0 - vtexcoord.y);\
                } \
                ";

        const char * fshader_sky = " \
                #version 330 core \n\
                out vec3 color;\
                in vec2 uv; \
                uniform sampler2D tex;\
                void main() {\
                    color = texture(tex,uv).rgb;\
                }\
                ";





#endif // SHADERS_H
