"""Compile the real app_task with simulated GPIO/queue/media boundaries."""
from pathlib import Path
import subprocess,tempfile
src=(Path(__file__).resolve().parents[2]/'src/main.c').read_text()
app=src[src.index('static void app_task(void *unused)'):src.index('\nvoid app_main(void)')]
enum=src[src.index('typedef enum {\n    TRIGGER_LOCAL_BUTTON'):src.index('} trigger_source_t;')+len('} trigger_source_t;')]
stubs=r'''
#include <cstdint>
#include <cstdio>
#include <vector>
#include <string>
#include <cassert>
struct Done{};
using TickType_t=uint32_t;
#define pdMS_TO_TICKS(x) (x)
#define pdTRUE 1
#define ESP_LOGI(...) ((void)0)
#define PIN_PREVIOUS_BUTTON 22
#define PIN_TRIGGER_BUTTON 13
#define TRIGGER_LOCKOUT_MS 10000
#define VOLUME_OVERLAY_MS 1200
static TickType_t now=1000,finish=0;
struct Event{TickType_t at;trigger_source_t source;};
static std::vector<Event> events;
static size_t nextEvent;
static bool queued=false,trigger_locked=false,arc_core_active=false;
static bool arcFails=false,interruptScene=false;
static trigger_source_t pending;
static int trigger_queue=1;
static std::string trace;
struct media_scene_t {const char* id;unsigned audio_id,lockout_ms;};
static media_scene_t media_scenes[]={{"S021",21,10000}};
static size_t media_scene_count=1,playback_order_cursor=0,playback_order_count=32;
static TickType_t xTaskGetTickCount(){return now;}
static void vTaskDelay(TickType_t t){now+=t;}
static int gpio_get_level(int){return 1;}
static int xQueueReceive(int,trigger_source_t* out,int){
 if(queued){*out=pending;queued=false;return pdTRUE;}
 if(nextEvent<events.size()&&now>=events[nextEvent].at){*out=events[nextEvent++].source;return pdTRUE;}
 if(now>=finish)throw Done{};
 return 0;
}
static void xQueueOverwrite(int,trigger_source_t* source){pending=*source;queued=true;}
static void xQueueReset(int){queued=false;}
static void request_trigger(trigger_source_t source){pending=source;queued=true;}
static void draw_idle(){trace+='H';}
static void draw_qr_code(){trace+='Q';}
static void dfplayer_stop(){}
static void halo_stop(){}
static void volume_adjust(int){}
static void start_scan_if_needed(){}
static size_t next_random_scene_index(){return playback_order_cursor++;}
static size_t previous_scene_index(){return playback_order_cursor ? --playback_order_cursor : 0;}
static bool play_media_scene(const media_scene_t*){
 trace+='S';
 if(interruptScene){assert(nextEvent<events.size());assert(events[nextEvent].source==TRIGGER_PEDAL_RIGHT);now=events[nextEvent++].at;return true;}
 return false;
}
static bool play_master_scene(){return play_media_scene(nullptr);}
static bool play_arc_core_loop(trigger_source_t* exit){
 trace+='A';if(arcFails)return false;
 assert(nextEvent<events.size());now=events[nextEvent].at;*exit=events[nextEvent++].source;return true;
}
'''
tests=r'''
static void run(std::vector<Event> e,const char* expected,size_t expectedCursor,bool interrupt=false,bool fail=false){
 events=e;nextEvent=0;queued=false;now=1000;finish=e.back().at+300;
 trace.clear();interruptScene=interrupt;arcFails=fail;trigger_locked=false;playback_order_cursor=0;
 try{app_task(nullptr);}catch(Done&){}
 if(trace!=expected){fprintf(stderr,"expected %s, got %s\n",expected,trace.c_str());assert(false);}
 assert(playback_order_cursor==expectedCursor);
}
int main(){
 // Two complete cycles, then left pedal plays normally without stale lockout.
 run({{1000,TRIGGER_PEDAL_RIGHT},{1300,TRIGGER_PEDAL_RIGHT},{1600,TRIGGER_PEDAL_RIGHT},
      {1900,TRIGGER_PEDAL_RIGHT},{2200,TRIGGER_PEDAL_RIGHT},{2500,TRIGGER_PEDAL_RIGHT},
      {2800,TRIGGER_PEDAL_LEFT}},"HQAHQAHSH",1);
 // Interrupting a scene enters QR and continues through Arc to home.
 run({{1000,TRIGGER_PEDAL_LEFT},{1300,TRIGGER_PEDAL_RIGHT},
      {1600,TRIGGER_PEDAL_RIGHT},{1900,TRIGGER_PEDAL_RIGHT}},"HSQAH",1,true);
 // Missing/unreadable Arc returns home; subsequent right press still gives QR.
 run({{1000,TRIGGER_PEDAL_RIGHT},{1300,TRIGGER_PEDAL_RIGHT},
      {1600,TRIGGER_PEDAL_RIGHT}},"HQAHQ",0,false,true);
 // The existing Mode 4 direct special still toggles home on the next click.
 run({{1000,TRIGGER_ARC_CORE},{1300,TRIGGER_ARC_CORE}},"HAH",0);
 // Left pedal can leave Arc and immediately play a normal scene.
 run({{1000,TRIGGER_PEDAL_RIGHT},{1300,TRIGGER_PEDAL_RIGHT},
      {1600,TRIGGER_PEDAL_LEFT}},"HQAHSH",1);
 puts("PASS: QR/Arc overlays preserve scene cursor; repeated cycles and exits work");
}
'''
with tempfile.TemporaryDirectory() as td:
 p=Path(td);(p/'test.cpp').write_text('#include <cstdint>\n'+enum+stubs+app+tests)
 subprocess.run(['c++','-std=c++17',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
