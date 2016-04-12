#ifndef PERLIN_H
#define PERLIN_H

float lerp(float a0, float a1, float w){
    return (1.0-w)*a0 + w*a1;
}

float dotGridGradient(int ix, int iy, float x, float y){

}

#endif // PERLIN_H
