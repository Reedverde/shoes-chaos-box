"""Exercise real ESP32 pulse senders against the actual halo loop and renderer."""
from pathlib import Path
import re
import subprocess
import tempfile

firmware = Path(__file__).resolve().parents[3]
sender = (firmware / 'idf_wearable/src/main.c').read_text()
receiver = firmware / 'circuit_playground_halo/src/main.cpp'
headers = Path(__file__).resolve().parent
constants = '\n'.join(re.findall(r'^#define (?:HALO_\w+|PIN_HALO_TRIGGER) .+$', sender, re.M))
functions = sender[sender.index('static void halo_start_scene('):sender.index('static void dfplayer_command(')]
prefix = r'''
#include <cassert>
#include <iostream>
void advanceMs(unsigned n) { while(n--) { ++fake_now; loop(); } }
void gpio_set_level(int, int level) { fake_input=level; loop(); }
void esp_rom_delay_us(unsigned us) { assert(us%1000==0); advanceMs(us/1000); }
#define pdMS_TO_TICKS(x) (x)
void vTaskDelay(unsigned ms) { advanceMs(ms); }
#define ESP_LOGI(...) ((void)0)
'''
tests = r'''
void resetReceiver() {
 decodeState=DecodeState::WaitForSync; previousInput=false; fake_input=0;
 sceneActive=false; arcCoreActive=false; bootArcRecoveryPending=false;
 effectVisible=false; afterglowUntil=fake_now; idleMode=IdleMode::Home;
 decodedBitCount=0; decodedScene=0; inputLowStartedAt=fake_now;
}
int main() {
 fake_now=10000; resetReceiver(); setup();
 halo_idle(false);
 assert(idleMode==IdleMode::Home && !sceneActive && fake_input==0);
 // Home has all three hues, low brightness, continuous fractional movement.
 fake_now=24000; drawIdle();
 assert(CircuitPlayground.strip.pixels[0][2]>CircuitPlayground.strip.pixels[0][0]);
 assert(CircuitPlayground.strip.pixels[3][0]>CircuitPlayground.strip.pixels[3][1]);
 assert(CircuitPlayground.strip.pixels[6][1]>CircuitPlayground.strip.pixels[6][2]);
 unsigned redBefore=CircuitPlayground.strip.pixels[0][0];
 fake_now+=1200; drawIdle(); assert(CircuitPlayground.strip.pixels[0][0]!=redBefore);
 for(unsigned t=0;t<24000;t+=16) {
   fake_now=48000+t; drawIdle();
   assert(CircuitPlayground.brightness>=18 && CircuitPlayground.brightness<=32);
 }
 // QR latches low, pulses together, and never rotates or changes its green hue.
 halo_idle(true); assert(idleMode==IdleMode::Qr && !sceneActive && fake_input==0);
 for(unsigned t=0;t<5000;t+=16) {
   advanceMs(16); drawIdle();
   assert(CircuitPlayground.brightness>=8 && CircuitPlayground.brightness<=20);
   for(int i=0;i<10;++i) {
     assert(CircuitPlayground.strip.pixels[i][0]==0);
     assert(CircuitPlayground.strip.pixels[i][1]==220);
     assert(CircuitPlayground.strip.pixels[i][2]==65);
   }
 }
 // Repeating QR for late-power recovery preserves its phase and mode.
 halo_idle(true); assert(idleMode==IdleMode::Qr && !arcCoreActive);
 halo_start_arc_core(); assert(arcCoreActive && sceneActive);
 halo_idle(false); assert(!sceneActive && !arcCoreActive && idleMode==IdleMode::Home);
 // Both idle states can precede any of the existing 32 positional scene IDs.
 for(int i=0;i<32;++i) {
   halo_idle(i%2); halo_start_scene(i);
   assert(sceneActive && !arcCoreActive && scenePalette==i);
   halo_idle(false); assert(!sceneActive && idleMode==IdleMode::Home);
 }
 // Interrupted packets must not swallow the next special/idle command.
 resetReceiver(); decodeState=DecodeState::ReadBits; decodedBitCount=2;
 halo_start_arc_core(); assert(arcCoreActive);
 halo_idle(true); assert(!arcCoreActive && idleMode==IdleMode::Qr);
 resetReceiver(); decodeState=DecodeState::ReadBits; decodedBitCount=1;
 advanceMs(101); assert(decodeState==DecodeState::WaitForSync);
 // Halo starting late while QR is selected gets the correct mode on refresh.
 resetReceiver(); setup(); halo_idle(true); assert(idleMode==IdleMode::Qr);
 // Clock rollover retains bounded brightness and the requested idle mode.
 fake_now=0xfffffff0U; drawIdle(); advanceMs(64); drawIdle();
 assert(idleMode==IdleMode::Qr && CircuitPlayground.brightness<=20);
 std::cout<<"PASS: actual ESP32/halo link, home/QR effects, 32 scenes, Arc, packet recovery\n";
}
'''
with tempfile.TemporaryDirectory() as td:
    p = Path(td)
    (p/'test.cpp').write_text('#include "'+str(receiver)+'"\n'+constants+'\n'+prefix+functions+tests)
    subprocess.run(['c++','-std=c++17','-I'+str(headers),str(p/'test.cpp'),'-o',str(p/'test')],check=True)
    subprocess.run([str(p/'test')],check=True)
