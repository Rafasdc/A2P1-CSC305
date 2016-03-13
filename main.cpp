#include "Canvas.h"
#include <math.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


using namespace glm;
using namespace std;

vector<vec4> SquareVertices;
vector<vec4> SQ;

vec3 camPos(0,0,3);
vec3 up(0,1,0);
vec3 target(0,0,0);
vec3 front(0,0,-1);

mat4 model;
mat4 view;
mat4 pr;
mat4 Mv;

unsigned int width = 512;
unsigned int height = 512;

float vppos_x = 0;
float vppos_y = 0;
bool leftButtonPressed = false;
float linelength = 0;
int timercount = 0;
Canvas canvas;

void MouseMove(double x, double y)
{
   //the pointer has moved
   vppos_x = (float)(x) / 256 - 1;
   vppos_y = 1 - (float)(y) / 256;
}

void MouseButton(MouseButtons mouseButton, bool press)
{
    //What to do with the keys?
    if (mouseButton == LeftButton)
    {
        if (press == true) leftButtonPressed = true;
        else leftButtonPressed = false;
    }
}

void KeyPress(char keychar)
{
    //A key is pressed! print a message
    fprintf(stderr, "The \"%c\" key is pressed!\n", keychar);
}

void DrawCross(float x_center, float y_center)
{
    canvas.AddLine(x_center - linelength,
                   y_center - linelength,
                   x_center + linelength,
                   y_center + linelength);

    canvas.AddLine(x_center - linelength,
                   y_center + linelength,
                   x_center + linelength,
                   y_center - linelength);
}

void DrawSquare(float x_center, float y_center)
{
    canvas.AddLine(x_center - linelength, y_center - linelength, x_center - linelength, y_center + linelength);
    canvas.AddLine(x_center - linelength, y_center + linelength, x_center + linelength, y_center + linelength);
    canvas.AddLine(x_center + linelength, y_center + linelength, x_center + linelength, y_center - linelength);
    canvas.AddLine(x_center + linelength, y_center - linelength, x_center - linelength, y_center - linelength);
}

void OnPaint()
{
    Mv = model;
    canvas.Clear();
    DrawCross(vppos_x, vppos_y);

    if (leftButtonPressed == true)
    {

    }
    //Draw first face
    for (int i = 0; i < SquareVertices.size() -1 ; i++){

        vec4 startPoint = Mv * SquareVertices[i];
        vec4 endPoint = Mv * SquareVertices[i+1];
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

        vec4 startPoint = Mv * SQ[i];
        vec4 endPoint = Mv * SQ[i+1];
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

        vec4 startPoint = Mv * SQ[i];
        vec4 endPoint = Mv * SquareVertices[i];
        /*
        fprintf(stderr,"%.1f\n", endPoint.y);
        fprintf(stderr,"%.1f\n", endPoint.x);
        fprintf(stderr,"%.1f\n", startPoint.y);
        fprintf(stderr,"%.1f\n", startPoint.y);
        */
        canvas.AddLine(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
    }
}

void OnTimer()
{
    linelength = (float)(sin(timercount / 10.0) * 0.1 + 0.1);
    timercount ++;


    /*
    float radius = 10.0f;
    float camx = sin(timercount * radius);
    float camz = cos(timercount * radius);
    camPos.x = camx;
    camPos.z = camz;
    */

    model = rotate(model, radians(-20.0f), vec3(1.0f,0.0f,0.5f));
    //view = translate(view,vec3(0.0f,0.0f,-3.0f));
    view = lookAt(camPos,camPos+front,up);
    pr = perspective(radians(45.0f),(float)width/(float)height,0.1f,100.0f);
    Mv = pr * view * model;

}

int main(int, char **){



    //first face
    SquareVertices.push_back(vec4(0.25,0.25,0.25,1));//down
    SquareVertices.push_back(vec4(0.25,-0.25,0.25,1));//right
    SquareVertices.push_back(vec4(-0.25,-0.25,0.25,1));//right up
    SquareVertices.push_back(vec4(-0.25,0.25,0.25,1));//up
    SquareVertices.push_back(vec4(0.25,0.25,0.25,1));

    //second face
    SQ.push_back(vec4(0.25,0.25,-0.25,1));//down
    SQ.push_back(vec4(0.25,-0.25,-0.25,1));//right
    SQ.push_back(vec4(-0.25,-0.25,-0.25,1));//right up
    SQ.push_back(vec4(-0.25,0.25,-0.25,1));//up
    SQ.push_back(vec4(0.25,0.25,-0.25,1));


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


