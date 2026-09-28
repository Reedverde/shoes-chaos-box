#include <cassert>
#include <iostream>
#include "../../src/main.cpp"
int main(){
 fake_now=10000; rotationQ16=3*65536+32000; decodedScene=30; beginScene(fake_now);
 assert(rotationQ16==3*65536+32000); assert(animationStep==0);
 assert(CircuitPlayground.strip.shows==1);
 assert(framePixels[0].green==213 && framePixels[0].blue==245);
 fake_now+=2500; drawSceneFrame(88); assert(framePixels[0].red>framePixels[0].green);
 fake_now+=2500; drawSceneFrame(88); assert(framePixels[0].green>framePixels[0].blue);
 fake_now+=10000; beginScene(fake_now);assert(framePixels[0].blue==245);
 // Subpixel wrap interpolates rather than teleporting at pixel 9->0.
 decodedScene=1;beginScene(fake_now);rotationQ16=9*65536+32768;
 clearPixels();setFramePixel(0,200,0,0);presentFrame(true);
 assert(CircuitPlayground.strip.pixels[9][0]>=99);assert(CircuitPlayground.strip.pixels[0][0]>=99);
 // A delayed loop catches up to elapsed time rather than slowing the chase.
 decodedScene=1;fake_now=20000;beginScene(fake_now);rotationQ16=0;
 fake_now+=450;drawSceneFrame(48);assert(rotationQ16==5*65536);
 // Rotation and phrase timing also survive the millisecond clock wrapping.
 fake_now=0xfffffff0U;beginScene(fake_now);rotationQ16=0;
 fake_now+=90;drawSceneFrame(48);assert(rotationQ16==65536);
 assert(animationStep==2);
 // One hardware show per completed frame, including idle and every scene.
 for(int id=0;id<32;++id){decodedScene=id;fake_now+=10000;int before=CircuitPlayground.strip.shows;beginScene(fake_now);assert(CircuitPlayground.strip.shows==before+1);fake_now+=16;drawSceneFrame(48);assert(CircuitPlayground.strip.shows==before+2);}
 // Arc Core has its own long-sync code and follows fast gold chase -> gold glow
 // -> blue powered-on pulse before repeating the complete 35.04-second cycle.
 fake_now=50000;beginArcCore(fake_now);assert(arcCoreActive && sceneActive);
 bool sawGold=false;for(const auto &p:framePixels)if(p.red>p.blue)sawGold=true;assert(sawGold);
 fake_now=54750;drawSceneFrame(48);for(const auto &p:framePixels)assert(p.red==255&&p.green==226&&p.blue==128);
 fake_now=55040;drawSceneFrame(48);assert(framePixels[0].blue>framePixels[0].green&&framePixels[0].green>framePixels[0].red);
 const uint8_t lowBlue=framePixels[0].blue;
 fake_now=58790;drawSceneFrame(48);assert(framePixels[0].blue>=lowBlue&&framePixels[0].green>70);
 fake_now=85040;drawSceneFrame(48);sawGold=false;for(const auto &p:framePixels)if(p.red>p.blue)sawGold=true;assert(sawGold);
 pulseStartedAt=fake_now;handleFallingEdge(fake_now+1);assert(!arcCoreActive&&!sceneActive);
 decodeState=DecodeState::WaitForSync;pulseStartedAt=1000;handleFallingEdge(1120);assert(decodeState==DecodeState::ArcArmed);
 // If the halo reboots while Arc is already holding the control line high,
 // it recovers after a short stable-high guard instead of remaining idle.
 sceneActive=false;arcCoreActive=false;decodeState=DecodeState::WaitForSync;
 bootArcRecoveryPending=true;bootHighStartedAt=90000;recoverArcFromHeldBootSignal(90749,true);assert(!arcCoreActive);
 recoverArcFromHeldBootSignal(90750,true);assert(arcCoreActive&&!bootArcRecoveryPending);
 int before=CircuitPlayground.strip.shows;drawIdle();assert(CircuitPlayground.strip.shows==before+1);
 std::cout<<"PASS: 32 scene profiles plus Arc Core fast-gold/blue timing and protocol\n";
}
