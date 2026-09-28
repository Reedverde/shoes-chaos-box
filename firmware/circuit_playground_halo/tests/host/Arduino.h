#pragma once
#include <cstdint>
static uint32_t fake_now=0;
inline uint32_t millis(){return fake_now;}
inline void delay(unsigned){}
inline void pinMode(int,int){}
inline int digitalRead(int){return 0;}
constexpr int A1=1, INPUT_PULLDOWN=0, HIGH=1;
struct SerialMock {void begin(int){} template<class T>void print(T){} template<class T>void println(T){}};
static SerialMock Serial;
