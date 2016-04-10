#include "glm/glm.hpp"

#ifndef SPLINE_H
#define SPLINE_H

float bPoint(float t, float a, float b, float c, float d){
    float A = ( d - (3.0 * c) + (3.0 * b) - a );
    float B = ( (3.0 * c) - (6.0 * b) + (3.0 * a) );
    float C = ( (3.0 * b) - (3.0 * a) );
    float D = ( a );

    return ( A*t*t*t + B*t*t + C*t + D  );

}
float bTangent(float t, float a, float b, float c, float d){
    float A = ( d - (3.0 * c) + (3.0 * b) - a );
    float B = ( (3.0 * c) - (6.0 * b) + (3.0 * a) );
    float C = ( (3.0 * b) - (3.0 * a) );
    float D = ( a );

    return ( ( 3.0 * A * t* t ) + ( 2.0 * B * t ) + C );
}

float bSecond(float t, float a, float b, float c, float d){
    float A = ( d - (3.0 * c) + (3.0 * b) - a );
    float B = ( (3.0 * c) - (6.0 * b) + (3.0 * a) );
    float C = ( (3.0 * b) - (3.0 * a) );
    float D = ( a );

    return ( ( 6.0 * A * t ) + ( 2.0 * B) );
}


#endif // SPLINE_H
