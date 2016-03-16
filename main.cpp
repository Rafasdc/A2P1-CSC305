#include "Canvas.h"
#include <math.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "incbase.h"


using namespace glm;
using namespace std;

vec3 camPos(0.0f,0.0f,3.0f);
vec3 up(0.0f,1.0f,0.0f);
vec3 target(0.0f,0.0f,0.0f);


mat4 model;
mat4 modelSQ; //second square model
mat4 view;
mat4 viewSQ; //second square view
mat4 pr;
mat4 prSQ; //second square pr
mat4 Mv;
mat4 MvL; //lighting Mv no longer used


float lastX = 600;
float lastY = 600;
float yaws = -90.0f;
float pitchs = 0.0f;

float zoom = -85;

float vppos_x = 0;
float vppos_y = 0;
bool leftButtonPressed = false;
bool rightButtonPressed = false;
float linelength = 0;
int timercount = 0;
bool mousemoved = false;

unsigned int width = 1200;
unsigned int height = 1200;

float Rotation = 0;
float RotatingSpeed = 0.02;
Canvas canvas;


//used for squares, includes the normal
 const GLfloat vpoint[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
   };

 //used for sphere only to deform square into triangle
GLfloat square_vertices[36][3] = {
    {-0.5f, -0.5f, -0.5f},
    {0.5f, -0.5f, -0.5f},
    {0.5f,  0.5f, -0.5f},
    {0.5f,  0.5f, -0.5f},
    {-0.5f,  0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},

    {-0.5f, -0.5f,  0.5f},
    {0.5f, -0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f},
    {-0.5f, -0.5f,  0.5f},

    {-0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f},

    {0.5f,  0.5f,  0.5f},
    { 0.5f,  0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},

    {-0.5f, -0.5f, -0.5f},
    {0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f,  0.5f},
    { 0.5f, -0.5f,  0.5f},
    {-0.5f, -0.5f,  0.5f},
    {-0.5f, -0.5f, -0.5f},

    {-0.5f,  0.5f, -0.5f},
    {0.5f,  0.5f, -0.5f},
    {0.5f,  0.5f,  0.5f},
    {0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f, -0.5f},
};

//vertices with texture coordinates for square
//no normals for color
const GLfloat vtexcoord[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f

};


\


vector<GLfloat> vertices;
vector<GLfloat> normals;

//---------- Code to Subdivide Based by Trung Bun stackoverflow.com -------------
//Used to subdivide square vertices to procedually generate a sphere by generating
//more triangle per face, until it becomes a sphere object.

//normalizes the current vertices before addding to vector list
void normalize3f(float v[3]){
    GLfloat d = sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if (d == 0.0f){
    //    fprintf(stderr, "Zero length vector. \n");
        return;
    }
    v[0] /= d;
    v[1] /= d;
    v[2] /= d;
    normals.push_back(v[0]);
    normals.push_back(v[1]);
    normals.push_back(v[2]);
}

//adds the vertice
void addVertices(GLfloat v[]){
    for (int i = 0; i < 3; i++){
        vertices.push_back(v[i]);
    }
}
//subdivides
void subdivide(GLfloat v1[], GLfloat v2[], GLfloat v3[], int depth)
{
    GLfloat v12[3], v23[3], v31[3];

    if (depth == 0){
        addVertices(v1);
        addVertices(v2);
        addVertices(v3);
        return;
    }

    for (int i = 0; i < 3; i++){
        v12[i] = v1[i]+v2[i];
        v23[i] = v2[i]+v3[i];
        v31[i] = v3[i]+v1[i];
    }

    normalize3f(v12);
    normalize3f(v23);
    normalize3f(v31);
    subdivide(v1, v12, v31, depth-1);
    subdivide(v2, v23, v12, depth-1);
    subdivide(v3, v31, v23, depth-1);
    subdivide(v12, v23, v31, depth-1);
}

// -------------- -end code to subdivide -------------------------------




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
        in vec2 tex;\
        in vec3 vpoint; \
        uniform mat4 model; \
        uniform mat4 view;\
        uniform mat4 pr;\
        uniform mat4 Mv;\
        out vec3 Normal;\
        out vec3 FragPos; \
        out vec2 texCoord;\
        void main() { \
            gl_Position =  pr*view*model*vec4(position,1.0f);\
            Normal = normal;\
            FragPos = vec3(model*vec4(position,1.0f));\
            texCoord = vec2(tex.x, 1.0-tex.y);\
        } \
        ";

const char * fshader_square = " \
        #version 330 core \n\
        out vec4 color; \
        in vec3 Normal; \
        in vec3 FragPos;\
        in vec2 texCoord;\
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
            color = vec4(toApply,1.0f);\
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
                    UV = vec2(octa_vpoints.x/2 + 0.5, octa_vpoints.y/2+0.5);\
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
                    float spec = pow(max(dot(viewPos,reflectD),0.0),1);\
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

//OpenGL context variables
GLuint programID = 0;
GLuint VertexArrayID = 0;
GLuint modelGL = 0;
GLuint viewGL = 0;
GLuint prGL = 0;
GLuint MvGL = 0;
GLuint objectColorGL = 0;
GLuint lightColorGL = 0;
GLuint lightSourceGL = 0;
GLuint viewPosGL = 0;

GLuint lightID = 0;
GLuint VertexArrayLight = 0;
GLuint MvLGL;

GLuint sphereID = 0;
GLuint VertexArraySphere = 0;
GLuint MvGL_sphere = 0;
GLuint modelGL_sphere = 0;
GLuint viewGL_sphere = 0;
GLuint prGL_sphere = 0;
GLuint objectColorGL_sphere = 0;
GLuint lightColorGL_sphere = 0;
GLuint lightSourceGL_sphere = 0;
GLuint viewPosGL_sphere = 0;
GLuint spheretex;

GLuint skyBoxID = 0;
GLuint VertexArraySkyBox = 0;
GLuint modelGL_sky = 0;
GLuint viewGL_sky = 0;
GLuint prGL_sky = 0;
GLuint skytex_bindingpoint;

GLuint skytexture;
GLuint sunTexture;




void InitializeCam(){
        //recursion depth 1 = ugly, 2 = normal, 3 = best, 4+ = lags
       for (int i = 0; i <36; i++){
            subdivide(square_vertices[i],square_vertices[i+1],square_vertices[i+2],3);
        }
       //fprintf(stderr,"%d\n",vertices.size());
       //fprintf(stderr,"%d\n",normals.size());

       //for (int i = 0; i< vertices.size(); i++){
       //    fprintf(stderr,"%.1f,%.1f,%.1f\n",vertices[i],vertices[i+1],vertices[i+2]);
       //}


    model = rotate(model, radians(25.0f), vec3(1.0f,0.0f,0.0f));//changed
    view = lookAt(camPos,target,up);
    pr = perspective(/* zoom */ radians(-85.0f),(float)width/(float)height,0.1f,100.0f);
    Mv = pr * view * model;
    mat4 modelL;
    modelL = translate(modelL, vec3(-0.75f,-1.95f,0.75f));
    modelL = scale(modelL,vec3(0.35f));
    MvL = pr* view * modelL;
    modelSQ = translate(modelSQ, vec3(0.25f,-0.35f,1.5f));
    modelSQ = scale(modelSQ,vec3(0.15f));
    viewSQ = lookAt(camPos,target,up);
    prSQ = perspective(/* zoom */ radians(-85.0f),(float)width/(float)height,0.1f,100.0f);

}
void InitializeGL()
{

    glEnable(GL_DEPTH_TEST); //The cube loses 2 faces if this is not enabled, generates depth correctly

    //Compile the shaders
    programID = compile_shaders(vshader_square, fshader_square);

    //Generate Vertex Array and bind the vertex buffer data

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    ///--- Generate memory for VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    /// The subsequent commands will affect the specified buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /// Pass the vertex positions to OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(vpoint), vpoint, GL_DYNAMIC_DRAW);

    ///--- find the binding point in the shader:
    /// "vpoint" in the vertex shader
    //glUseProgram(programID);
    //GLuint vpoint_id = glGetAttribLocation(programID, "vpoint");
    glVertexAttribPointer(0,
                          3, //size per vertex (3 floats for cord)
                          GL_FLOAT,
                          false, //don't normalize
                          6 * sizeof(GLfloat), //stride = 0
                          0); //offset = 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,
                          3, //size per vertex (3 floats for cord)
                          GL_FLOAT,
                          false, //don't normalize
                          6 * sizeof(GLfloat), //stride = 0
                          (GLvoid*)(3*sizeof(GLfloat))); //offset = 0
    glEnableVertexAttribArray(1);



    //Find the binding point for the uniform variable
    //glEnableVertexAttribArray(vpoint_id);
    MvGL = glGetUniformLocation(programID,"Mv");
    objectColorGL = glGetUniformLocation(programID,"objectColor");
    lightColorGL = glGetUniformLocation(programID,"lightColor");
    lightSourceGL = glGetUniformLocation(programID, "lightSource");
    modelGL = glGetUniformLocation(programID, "model");
    viewGL = glGetUniformLocation(programID,"view");
    prGL = glGetUniformLocation(programID,"pr");
    viewPosGL = glGetUniformLocation(programID,"viewPos");



    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

// ------------- light ---------------

    lightID = compile_shaders(vshader_light, fshader_light);
    glGenVertexArrays(1, &VertexArrayLight);
    glBindVertexArray(VertexArrayLight);


    glUseProgram(lightID);
    //glEnableVertexAttribArray(vpoint_id);
    glVertexAttribPointer(0,
                          3, //size per vertex (3 floats for cord)
                          GL_FLOAT,
                          false, //don't normalize
                          6* sizeof(GLfloat), //stride = 0
                          0); //offset = 0
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);



// ------------ sphere ---------------





    sphereID = compile_shaders(vshader_sphere, fshader_sphere);

    glGenVertexArrays(1, &VertexArraySphere);
    glBindVertexArray(VertexArraySphere);

    ///--- Generate memory for VBO
    //GLuint VBO_Sphere;
    glGenBuffers(1, &VBO);
    /// The subsequent commands will affect the specified buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    /// Pass the vertex positions to OpenGL
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat),&vertices[0], GL_STATIC_DRAW);


    glUseProgram(sphereID);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3, //size per vertex (3 floats for cord)
                          GL_FLOAT,
                          false, //don't normalize
                          0, //stride = 0
                          0); //offset = 0
    glEnableVertexAttribArray(0);
    MvGL_sphere = glGetUniformLocation(sphereID,"Mv");
    modelGL_sphere = glGetUniformLocation(sphereID, "model");
    viewGL_sphere = glGetUniformLocation(sphereID,"view");
    prGL_sphere = glGetUniformLocation(sphereID,"pr");
    objectColorGL_sphere = glGetUniformLocation(sphereID,"objectColor");
    lightColorGL_sphere = glGetUniformLocation(sphereID,"lightColor");
    lightSourceGL_sphere = glGetUniformLocation(sphereID, "lightSource");
    viewPosGL_sphere = glGetUniformLocation(sphereID,"viewPos");
    spheretex = glGetUniformLocation(sphereID,"tex");

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


// ---------- skybox ----------------
    skyBoxID = compile_shaders(vshader_sky,fshader_sky);
    glGenVertexArrays(1, &VertexArraySkyBox);
    glBindVertexArray(VertexArraySkyBox);


    glUseProgram(skyBoxID);

    GLuint VBO_sky;
    glGenBuffers(1,&VBO_sky);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_sky);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vtexcoord),vtexcoord,GL_STATIC_DRAW);

    glVertexAttribPointer(0,
                          3, //size per vertex (3 floats for cord)
                          GL_FLOAT,
                          false, //don't normalize
                          5 * sizeof(GLfloat), //stride = 0
                          (GLvoid*)0); //offset = 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2,
                          2, //size per vertex (3 floats for cord)
                          GL_FLOAT,
                          false, //don't normalize
                          5 * sizeof(GLfloat), //stride = 0
                          (GLvoid*)(3*sizeof(GLfloat))); //offset = 0
    glEnableVertexAttribArray(2);



    skytex_bindingpoint = glGetUniformLocation(skyBoxID,"tex");
    modelGL_sky = glGetUniformLocation(skyBoxID, "model");
    viewGL_sky = glGetUniformLocation(skyBoxID,"view");
    prGL_sky = glGetUniformLocation(skyBoxID,"pr");

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

//---------------------- Textures ----------------------------

    Texture texSky = LoadPNGTexture("sky.png");
    glGenTextures(1,&skytexture);
    glBindTexture(GL_TEXTURE_2D,skytexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,texSky.width,texSky.height,0,GL_RGBA,GL_UNSIGNED_BYTE,texSky.dataptr);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);

    Texture texSun = LoadPNGTexture("sun.png");
    glGenTextures(1,&sunTexture);
    glBindTexture(GL_TEXTURE_2D,sunTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,texSun.width,texSun.height,0,GL_RGBA,GL_UNSIGNED_BYTE,texSun.dataptr);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);

    /*
    Texture texMetal = LoadPNGTexture("sun.png");
    glGenTextures(1,&metalTexture);
    glBindTexture(GL_TEXTURE_2D, metalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,texMetal.width,texSun.height,0,GL_RGBA,GL_UNSIGNED_BYTE,texMetal.dataptr);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
    */







}

void MouseMove(double x, double y)
{

    lastX = vppos_x;
    lastY = vppos_y;
   //the pointer has moved
   vppos_x = (float)(x) / (width/2) - 1;
   vppos_y = 1 - (float)(y) / (height/2);
   mousemoved= true;

}

void MouseButton(MouseButtons mouseButton, bool press)
{
    if (mouseButton == LeftButton)
    {
        if (press == true) leftButtonPressed = true;
        else leftButtonPressed = false;
    }
    if (mouseButton == RightButton)
    {
        if (press == true) rightButtonPressed = true;
        else rightButtonPressed = false;
    }
}

void KeyPress(char keychar)
{

}

void OnPaint()
{


    //Binding the openGL context
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); //for drawing testing purposes

    //main sphere
    glUniform1i(spheretex,0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sunTexture);
    glUseProgram(sphereID);
    glBindVertexArray(VertexArraySphere);
    glUniformMatrix4fv(viewGL_sphere,1,GL_FALSE,value_ptr(view));
    glUniformMatrix4fv(prGL_sphere,1,GL_FALSE,value_ptr(pr));
    glUniformMatrix4fv(MvGL_sphere,1,GL_FALSE,value_ptr(Mv));
    glUniform3f(objectColorGL_sphere, 1.0f,0.5f,0.31f);
    glUniform3f(lightColorGL_sphere, 1.0f,1.0f,1.0f);
    glUniform3f(lightSourceGL_sphere, -0.75f,-1.95f,0.75f);
    glUniformMatrix4fv(modelGL_sphere,1,GL_FALSE,value_ptr(model));
    glUniform3f(viewPosGL_sphere, camPos.x,camPos.y,camPos.z);
    glDrawArrays(GL_TRIANGLES,0,vertices.size());
    //Clean up the openGL context for other drawings
    glUseProgram(0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D,0);

    //revolving cube around main sphere
    glUseProgram(programID);
    glBindVertexArray(VertexArrayID);
    glUniformMatrix4fv(viewGL,1,GL_FALSE,value_ptr(viewSQ));
    glUniformMatrix4fv(prGL,1,GL_FALSE,value_ptr(prSQ));
    glUniformMatrix4fv(MvGL,1,GL_FALSE,value_ptr(Mv));
    glUniform3f(objectColorGL, 0.3f,0.5f,0.31f);
    glUniform3f(lightColorGL, 1.0f,1.0f,1.0f);
    glUniform3f(lightSourceGL, -0.75f,-1.95f,0.75f);
    glUniformMatrix4fv(modelGL,1,GL_FALSE,value_ptr(modelSQ));
    glUniform3f(viewPosGL, camPos.x,camPos.y,camPos.z);
    glDrawArrays(GL_TRIANGLES, 0 , 36);
    //Clean up the openGL context for other drawings
    glUseProgram(0);
    glBindVertexArray(0);

    //skybox
    glUniform1i(skytex_bindingpoint,0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skytexture);
    glUseProgram(skyBoxID);
    glBindVertexArray(VertexArraySkyBox);
    mat4 modelS;
    modelS = translate(modelS, vec3(0.0f,0.0f,-2.0f));
    //modelS = rotate(modelS, radians(25.0f), vec3(1.0f,0.0f,0.5f));
    mat4 prS;
    prS = perspective(radians(10.75f),(float)width/(float)height,0.1f,100.0f);
    mat4 viewS;
    viewS = lookAt(vec3(camPos.x,camPos.y, -7.75f),target,up);
    glUniformMatrix4fv(viewGL_sky,1,GL_FALSE,value_ptr(viewS));
    glUniformMatrix4fv(prGL_sky,1,GL_FALSE,value_ptr(prS));
    glUniformMatrix4fv(modelGL_sky,1,GL_FALSE,value_ptr(modelS));
    glDrawArrays(GL_TRIANGLES, 0 , 36);
    //Clean up the openGL context for other drawings
    glUseProgram(0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D,0);




    //Light source
    //for viewing purposes
    glUseProgram(lightID);
    glBindVertexArray(VertexArrayID);
    glUniformMatrix4fv(MvLGL,1,GL_FALSE,value_ptr(MvL));
    glDrawArrays(GL_TRIANGLES,0,36);
    glUseProgram(0);
    glBindVertexArray(0);



    //secondary sphere acts like a moon
    /*
    glUniform1i(spheretex,0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sunTexture);
    */
    glUseProgram(sphereID);
    glBindVertexArray(VertexArraySphere);
    glUniformMatrix4fv(MvGL_sphere,1,GL_FALSE,value_ptr(MvL));
    glUniformMatrix4fv(viewGL_sphere,1,GL_FALSE,value_ptr(view));
    glUniformMatrix4fv(prGL_sphere,1,GL_FALSE,value_ptr(pr));
    glUniformMatrix4fv(modelGL_sphere,1,GL_FALSE,value_ptr(modelSQ));
    glUniform3f(objectColorGL_sphere, 0.7f,0.5f,1.0f);
    glUniform3f(lightColorGL_sphere, 1.0f,1.0f,1.0f);
    glUniform3f(lightSourceGL_sphere, -0.75f,-1.95f,0.75f);
    glUniform3f(viewPosGL_sphere, camPos.x,camPos.y,camPos.z);
    glDrawArrays(GL_TRIANGLES,0,vertices.size());
    glUseProgram(0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D,0);

}
void HandleLeftClick(){
    if (leftButtonPressed){
        float xoffset = vppos_x - lastX;
        float yoffset = lastY - vppos_y;

        xoffset *= 0.25f;
        yoffset *= 0.25f;
        mat4 rotateEulerX;
        mat4 rotateEulerY;

        yaws += xoffset;
        pitchs += yoffset;
        if (vppos_x > lastX){
            rotateEulerX= eulerAngleXY(radians(pitchs*0.05),radians(yaws*0.025));
            view = view*rotateEulerX;
        } else if (vppos_x < lastX){
            rotateEulerX= eulerAngleXY(radians(-pitchs*0.05),radians(-yaws*0.025));
            view = view*rotateEulerX;
        }
        if (vppos_y > lastY){
            rotateEulerY = eulerAngleXY(radians(yaws*0.05),radians(pitchs*0.025));
            view = view * rotateEulerY;
        } else if (vppos_y < lastY){
            rotateEulerY = eulerAngleXY(radians(-yaws*0.05),radians(-pitchs*0.025));
            view = view * rotateEulerY;
        }
    }

}

void HandleRightClick(){
    if (rightButtonPressed){


        if (vppos_y > lastY){
            zoom += 5;
            pr = perspective(/* zoom */ radians(zoom),(float)width/(float)height,0.1f,100.0f);
            //Mv = pr * view * model;
        } else {
            zoom -= 5;
            pr = perspective(/* zoom */ radians(zoom),(float)width/(float)height,0.1f,100.0f);
            //Mv = pr * view * model;
        }
    }
}

void OnTimer()
{
    timercount++;

    HandleLeftClick();
    mousemoved = false;
    HandleRightClick();
    modelSQ = rotate(modelSQ, radians(25.0f), vec3(1.0f,1.0f,1.5f));

    GLfloat radius = 2.5f;
    GLfloat camX = cos(glfwGetTime()*radius);
    GLfloat camZ = sin(glfwGetTime()*radius);
    viewSQ = lookAt(vec3(camX,0.0,camZ), vec3(0.0f,0.0f,3.0f),up);

}

int main(int, char **){

    //Link the call backs
    canvas.SetMouseMove(MouseMove);
    canvas.SetMouseButton(MouseButton);
    canvas.SetKeyPress(KeyPress);
    canvas.SetOnPaint(OnPaint);
    canvas.SetTimer(0.1, OnTimer);
    //Show Window
    canvas.Initialize(width, height, "Rotating Square Demo");
    //Do our initialization
    InitializeCam();
    InitializeGL();
    canvas.Show();

    return 0;
}

