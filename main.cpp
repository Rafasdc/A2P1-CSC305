#include "Canvas.h"
#include <math.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


using namespace glm;
using namespace std;

vector<vec3> SquareVertices;
vector<vec3> SQ;

vec3 camPos(0.0f,0.0f,3.0f);
vec3 up(0.0f,1.0f,0.0f);
vec3 target(0.0f,0.0f,0.0f);

mat4 model;
mat4 view;
mat4 pr;
mat4 Mv;

float lastX = 256;
float lastY = 256;
float yaws = -90.0f;
float pitchs = 0.0f;

unsigned int width = 512;
unsigned int height = 512;
float zoom = -85;
float rotateCube = 25;

float vppos_x = 0;
float vppos_y = 0;
bool leftButtonPressed = false;
bool rightButtonPressed = false;
float linelength = 0;
int timercount = 0;
Canvas canvas;
bool mousemoved = false;

void MouseMove(double x, double y)
{
    lastX = vppos_x;
    lastY = vppos_y;
    //fprintf(stderr, "last_x is %.5f\n", lastX);
    //fprintf(stderr, "last_y is %.5f\n", lastY);

   //the pointer has moved
   vppos_x = (float)(x) / 256 - 1;
   vppos_y = 1 - (float)(y) / 256;
   //fprintf(stderr, "vppos_x is %.5f\n", vppos_x);
   //fprintf(stderr, "vppos_y is %.5f\n", vppos_y);
   mousemoved= true;


}

void MouseButton(MouseButtons mouseButton, bool press)
{
    //What to do with the keys?
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
    //A key is pressed! print a message
    fprintf(stderr, "The \"%c\" key is pressed!\n", keychar);
}

void OnPaint()
{
    canvas.Clear();

    //Draw first face
    for (int i = 0; i < SquareVertices.size() -1 ; i++){

        vec4 startPoint = Mv * vec4(SquareVertices[i], 1.0f);
        vec4 endPoint = Mv * vec4(SquareVertices[i+1], 1.0f);
        /*
        fprintf(stderr,"%.1f\n", endPoint.y);
        fprintf(stderr,"%.1f\n", endPoint.x);
        fprintf(stderr,"%.1f\n", startPoint.y);
        fprintf(stderr,"%.1f\n", startPoint.y);
        */
        canvas.AddLine(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
    }
    //Draw second face
    for (int i = 0; i < SQ.size() -1 ; i++){

        vec4 startPoint = Mv * vec4(SQ[i], 1.0f);
        vec4 endPoint = Mv * vec4(SQ[i+1], 1.0f);
        /*
        fprintf(stderr,"%.1f\n", endPoint.y);
        fprintf(stderr,"%.1f\n", endPoint.x);
        fprintf(stderr,"%.1f\n", startPoint.y);
        fprintf(stderr,"%.1f\n", startPoint.y);
        */
        canvas.AddLine(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
    }
    //connect them to create cube
    for (int i = 0; i < SQ.size() -1 ; i++){

        vec4 startPoint = Mv * vec4(SQ[i], 1.0f);
        vec4 endPoint = Mv * vec4(SquareVertices[i], 1.0f);
        /*
        fprintf(stderr,"%.1f\n", endPoint.y);
        fprintf(stderr,"%.1f\n", endPoint.x);
        fprintf(stderr,"%.1f\n", startPoint.y);
        fprintf(stderr,"%.1f\n", startPoint.y);
        */
        canvas.AddLine(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
    }
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
    linelength = (float)(sin(timercount / 10.0) * 0.1 + 0.1);
    timercount ++;

    HandleLeftClick();
    mousemoved = false;
    HandleRightClick();

}

int main(int, char **){

    //model = translate(model, -camPos);
    model = rotate(model, radians(0.1f), vec3(1.0f,0.0f,0.5f));
    view = lookAt(camPos,target,up);
    pr = perspective(/* zoom */ radians(-85.0f),(float)width/(float)height,0.1f,100.0f);
    Mv = pr * view * model;

    //first face
    SquareVertices.push_back(vec3(0.25,0.25,0.25));//down
    SquareVertices.push_back(vec3(0.25,-0.25,0.25));//right
    SquareVertices.push_back(vec3(-0.25,-0.25,0.25));//right up
    SquareVertices.push_back(vec3(-0.25,0.25,0.25));//up
    SquareVertices.push_back(vec3(0.25,0.25,0.25));

    //second face
    SQ.push_back(vec3(0.25,0.25,-0.25));//down
    SQ.push_back(vec3(0.25,-0.25,-0.25));//right
    SQ.push_back(vec3(-0.25,-0.25,-0.25));//right up
    SQ.push_back(vec3(-0.25,0.25,-0.25));//up
    SQ.push_back(vec3(0.25,0.25,-0.25));


    //Link the call backs
    canvas.SetMouseMove(MouseMove);
    canvas.SetMouseButton(MouseButton);
    canvas.SetKeyPress(KeyPress);
    canvas.SetOnPaint(OnPaint);
    canvas.SetTimer(0.1, OnTimer);//trigger OnTimer every 0.1 second
    //Show Window
    canvas.Show(width, height, "Canvas Demo");
    return 0;
}


