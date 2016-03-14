#include "Canvas.h"
#include <math.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "incbase.h"


using namespace glm;
using namespace std;

vec3 camPos(0.0f,0.0f,3.0f);
vec3 up(0.0f,1.0f,0.0f);
vec3 target(0.0f,0.0f,0.0f);


mat4 model;
mat4 modelSQ;
mat4 view;
mat4 viewSQ;
mat4 pr;
mat4 prSQ;
mat4 Mv;
mat4 MvL;


float lastX = 0;
float lastY = 0;
float yaws = 45.0f;
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

const GLfloat octa_vpoints[6][3] = {
     {1.0f,  0.0f,  0.0f},  //0.0f,  1.0f,  0.0f,
     {0.0f,  1.0f,  0.0f},  //0.0f,  1.0f,  0.0f,
     {0.0f,  0.0f,  1.0f},  //0.0f,  1.0f,  0.0f,
    {-1.0f,  0.0f,  0.0f},  //0.0f,  1.0f,  0.0f,
     {0.0f, -1.0f,  0.0f},  //0.0f,  1.0f,  0.0f,
     {0.0f,  0.0f, -1.0f}  //0.0f,  1.0f,  0.0f,

};


#define X .525731112119133606
#define Z .850650808352039932

static GLfloat vdata[12][3] = {
   {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
   {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
   {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};
static GLuint tindices[20][3] = {
   {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
   {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
   {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
   {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };


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
        in vec3 vpoint; \
        uniform mat4 model; \
        uniform mat4 view;\
        uniform mat4 pr;\
        uniform mat4 Mv;\
        out vec3 Normal;\
        out vec3 FragPos; \
        void main() { \
            gl_Position =  pr*view*model*vec4(position,1.0f);\
            Normal = normal;\
            FragPos = vec3(model*vec4(position,1.0f));\
        } \
        ";

const char * fshader_square = " \
        #version 330 core \n\
        out vec4 color; \
        in vec3 Normal; \
        in vec3 FragPos;\
         \
        uniform vec3 objectColor;\
        uniform vec3 lightColor; \
        uniform vec3 lightSource;\
        uniform vec3 viewPos; \
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
                void main() { \
                    gl_Position =  pr*view*model*vec4(octa_vpoints,1.0f);\
                } \
                ";

        const char * fshader_sphere = " \
                #version 330 core \n\
                out vec4 color; \
                void main() {\
                    color = vec4(1.0f);\
                }\
                ";
        const char * vshader_sky = " \
                #version 330 core \n\
                layout (location = 0) in vec3 position; \
                layout (location = 1) in vec3 normal;\
                in vec3 vpoint; \
                uniform mat4 model; \
                uniform mat4 view;\
                uniform mat4 pr;\
                uniform mat4 Mv;\
                out vec3 Normal;\
                out vec3 FragPos; \
                void main() { \
                    gl_Position =  pr*view*model*vec4(position,1.0f);\
                    Normal = normal;\
                    FragPos = vec3(model*vec4(position,1.0f));\
                } \
                ";

        const char * fshader_sky = " \
                #version 330 core \n\
                out vec4 color; \
                in vec3 Normal; \
                in vec3 FragPos;\
                 \
                uniform vec3 objectColor;\
                uniform vec3 lightColor; \
                uniform vec3 lightSource;\
                uniform vec3 viewPos; \
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

GLuint skyBoxID = 0;





void InitializeCam(){


    model = rotate(model, radians(25.0f), vec3(1.0f,0.0f,0.5f));
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

    glEnable(GL_DEPTH_TEST); //The cube loses 2 faces if this is not enabled.

    //Compile the shaders
    programID = compile_shaders(vshader_square, fshader_square);
    skyBoxID = compile_shaders(vshader_sky,fshader_sky);


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
    glUseProgram(programID);
    //GLuint vpoint_id = glGetAttribLocation(programID, "vpoint");
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3, //size per vertex (3 floats for cord)
                          GL_FLOAT,
                          false, //don't normalize
                          6 * sizeof(GLfloat), //stride = 0
                          0); //offset = 0
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



    //glEnableVertexAttribArray(0);
    //glBindVertexArray(0);

// ------------- light ---------------

    lightID = compile_shaders(vshader_light, fshader_sphere);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(octa_vpoints), octa_vpoints, GL_STATIC_DRAW);


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

    glUseProgram(programID);
    glBindVertexArray(VertexArrayID);
    glUniformMatrix4fv(viewGL,1,GL_FALSE,value_ptr(view));
    glUniformMatrix4fv(prGL,1,GL_FALSE,value_ptr(pr));
    glUniformMatrix4fv(MvGL,1,GL_FALSE,value_ptr(Mv));
    glUniform3f(objectColorGL, 1.0f,0.5f,0.31f);
    glUniform3f(lightColorGL, 1.0f,1.0f,1.0f);
    glUniform3f(lightSourceGL, -0.75f,-1.95f,0.75f);
    glUniformMatrix4fv(modelGL,1,GL_FALSE,value_ptr(model));
    glUniform3f(viewPosGL, camPos.x,camPos.y,camPos.z);
    glDrawArrays(GL_TRIANGLES,0,36);
    //Clean up the openGL context for other drawings
    glUseProgram(0);
    glBindVertexArray(0);

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

    glUseProgram(skyBoxID);
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


    glUseProgram(lightID);
    glBindVertexArray(VertexArrayID);
    glUniformMatrix4fv(MvLGL,1,GL_FALSE,value_ptr(MvL));
    glDrawArrays(GL_TRIANGLES,0,36);
    glUseProgram(0);
    glBindVertexArray(0);




    glUseProgram(sphereID);
    glBindVertexArray(VertexArraySphere);
    mat4 modelS;
    modelS = translate(modelS, vec3(-1.25f,-0.35f,0.75f));
    glUniformMatrix4fv(MvGL_sphere,1,GL_FALSE,value_ptr(MvL));
    glUniformMatrix4fv(viewGL_sphere,1,GL_FALSE,value_ptr(view));
    glUniformMatrix4fv(prGL_sphere,1,GL_FALSE,value_ptr(pr));
    glUniformMatrix4fv(modelGL_sphere,1,GL_FALSE,value_ptr(modelS));
    //glDrawArrays(GL_TRIANGLES,0,6);
    glUseProgram(0);
    glBindVertexArray(0);








}

void HandleLeftClick(){
    if (leftButtonPressed){
        float xoffset = vppos_x - lastX;
        float yoffset = lastY - vppos_y;

        xoffset *= 45;
        yoffset *= 45;

        yaws += xoffset;
        pitchs += yoffset;

        vec3 F;
        F.x = cos(radians(yaws) * cos(radians(pitchs)));
        F.y = sin(radians(pitchs));
        F.z = sin(radians(yaws)) * cos(radians(pitchs));
        normalize(F);
        camPos = F;
        //model = rotate(model, radians(25.0f), vec3(1.0f,0.0f,0.5f));
        view = lookAt(camPos,vec3(0,0,0),up);
        //pr = perspective(/* zoom */ radians(-85.0f),(float)width/(float)height,0.1f,100.0f);
        //Mv = pr * view * model;



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
    canvas.SetTimer(0.05, OnTimer);
    //Show Window
    canvas.Initialize(width, height, "Rotating Square Demo");
    //Do our initialization
    InitializeCam();
    InitializeGL();
    canvas.Show();

    return 0;
}

