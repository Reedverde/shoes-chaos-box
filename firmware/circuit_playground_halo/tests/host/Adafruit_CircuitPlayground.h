#pragma once
#include <cstdint>
struct StripMock {int shows=0;uint8_t pixels[10][3]={};void setPixelColor(int p,uint8_t r,uint8_t g,uint8_t b){pixels[p][0]=r;pixels[p][1]=g;pixels[p][2]=b;}void show(){++shows;}};
struct BoardMock{StripMock strip;int brightness=0;void begin(){}void setBrightness(int b){brightness=b;}};
static BoardMock CircuitPlayground;
