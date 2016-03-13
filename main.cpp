#include "Canvas.h"
#include <math.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


using namespace glm;
using namespace std;

vec3 camPos(0.0f,0.0f,3.0f);
vec3 up(0.0f,1.0f,0.0f);
vec3 target(0.0f,0.0f,0.0f);
vec3 lPos(1.2f,1.0f,2.0f);

mat4 model;
mat4 view;
mat4 pr;
mat4 Mv;
mat4 MvL;

float lastX = 256;
float lastY = 256;
float yaws = -90.0f;
float pitchs = 0.0f;

float zoom = -85;
float rotateCube = 25;

float vppos_x = 0;
float vppos_y = 0;
bool leftButtonPressed = false;
bool rightButtonPressed = false;
float linelength = 0;
int timercount = 0;
bool mousemoved = false;

unsigned int width = 512;
unsigned int height = 512;

float Rotation = 0;
float RotatingSpeed = 0.02;
Canvas canvas;

const GLfloat vpoint[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
   };

const char * vshader_light = " \
        #version 330 core \n\
        in vec3 vpoint; \
        uniform mat4 MvL; \
        void main() { \
            gl_Position =  MvL*vec4(vpoint,1.0f);\
        } \
        ";

const char * fshader_light = " \
        #version 330 core \n\
        out vec3 color; \
        void main() { color = vec3(1, 0, 0);} //Set pixel to red \
        ";

const char * vshader_square = " \
        #version 330 core \n\
        in vec3 vpoint; \
        uniform mat4 Mv; \
        void main() { \
            gl_Position =  Mv*vec4(vpoint,1.0f);\
        } \
        ";

const char * fshader_square = " \
        #version 330 core \n\
        out vec3 color; \
        uniform vec3 objectColor;\
        uniform vec3 lightColor;\
        void main() {\
            color = vec3(1, 0, 0);\
        } //Set pixel to red \
        ";






//OpenGL context variables
GLuint programID = 0;
GLuint lightID = 0;
GLuint VertexArrayID = 0;
GLuint MvGL = 0;
GLuint MvLGL = 0;
GLuint objectColorGL = 0;
GLuint lightcolorGL = 0;


void InitializeCam(){

    model = rotate(model, radians(0.1f), vec3(1.0f,0.0f,0.5f));
    view = lookAt(camPos,target,up);
    pr = perspective(/* zoom */ radians(-85.0f),(float)width/(float)height,0.1f,100.0f);
    Mv = pr * view * model;
    mat4 modelL = model;
    modelL = translate(model, lPos);
    modelL = scale(model,vec3(0.01f));
    MvL = pr* view * modelL;
}
void InitializeGL()
{

    //Compile the shaders
    programID = compile_shaders(vshader_square, fshader_square);
    lightID = compile_shaders(vshader_light, fshader_light);

    //Generate Vertex Array and bind the vertex buffer data
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glGenVertexArrays(1,&lightID);
    glBindVertexArray(lightID);

    ///--- Generate memory for vertexbuffer
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    /// The subsequent commands will affect the specified buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    /// Pass the vertex positions to OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(vpoint), vpoint, GL_STATIC_DRAW);

    ///--- find the binding point in the shader:
    /// "vpoint" in the vertex shader
    glUseProgram(programID);
    GLuint vpoint_id = glGetAttribLocation(programID, "vpoint");
    glEnableVertexAttribArray(vpoint_id);
    glVertexAttribPointer(vpoint_id,
                          3, //size per vertex (3 floats for cord)
                          GL_FLOAT,
                          false, //don't normalize
                          0, //stride = 0
                          0); //offset = 0
    //Find the binding point for the uniform variable
    MvGL = glGetUniformLocation(programID,"Mv");
    MvLGL = glGetUniformLocation(programID,"MvL");
    objectColorGL = glGetUniformLocation(lightID,"objectColor");
    lightcolorGL = glGetUniformLocation(lightID,"lightColor");
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

}

void MouseMove(double x, double y)
{
    lastX = vppos_x;
    lastY = vppos_y;
   //the pointer has moved
   vppos_x = (float)(x) / 256 - 1;
   vppos_y = 1 - (float)(y) / 256;
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
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(programID);
    //glBindVertexArray(VertexArrayID);
    glBindVertexArray(programID);
    glUniformMatrix4fv(MvGL,1,GL_FALSE,value_ptr(Mv));
    glDrawArrays(GL_TRIANGLES, 0 /*buffer offset*/, 36 /*#vertices*/);
    //Clean up the openGL context for other drawings
    glUseProgram(0);
    glBindVertexArray(0);


    glUseProgram(lightID);
    //glBindVertexArray(VertexArrayID);
    glBindVertexArray(lightID);
    glUniform3f(objectColorGL,1.0f,0.5f,0.31f);
    glUniform3f(lightcolorGL, 1.0f,1.0f,1.0f);
    glUniformMatrix4fv(MvLGL,1,GL_FALSE,value_ptr(MvL));
    glDrawArrays(GL_TRIANGLES,0,36);


    glUseProgram(0);
    glBindVertexArray(0);

}

void HandleLeftClick(){
    if (leftButtonPressed){
        float xoffset = vppos_x - lastX;
        float yoffset = lastY - vppos_y;

        xoffset *= 45.5;
        yoffset *= 45.5;

        yaws += xoffset;
        pitchs += yoffset;

        vec3 F;
        F.x = cos(radians(yaws) * cos(radians(pitchs)));
        F.y = sin(radians(pitchs));
        F.z = sin(radians(yaws)) * cos(radians(pitchs));
        normalize(F);
        camPos = F;
        view = lookAt(camPos,vec3(0,0,0),up);
        Mv = pr * view * model;

    }

}

void HandleRightClick(){
    if (rightButtonPressed){
        if (vppos_y > lastY){
            zoom += 5;
            pr = perspective(/* zoom */ radians(zoom),(float)width/(float)height,0.1f,100.0f);
            Mv = pr * view * model;
        } else {
            zoom -= 5;
            pr = perspective(/* zoom */ radians(zoom),(float)width/(float)height,0.1f,100.0f);
            Mv = pr * view * model;
        }
    }
}

void OnTimer()
{
    HandleLeftClick();
    HandleRightClick();
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

