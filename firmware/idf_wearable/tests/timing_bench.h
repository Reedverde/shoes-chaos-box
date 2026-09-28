// Opt-in repair validation; no media writes, excluded from release builds.
#include "bench_expected.h"
static uint32_t bench_hash(void) {
    uint32_t h=2166136261U;
    for(int half=0;half<2;++half)
        for(size_t i=0;i<RAW_HALF_FRAME_BYTES;++i)
            h=(h^raw_frame_buffers[half][i])*16777619U;
    return h;
}
static void timing_bench(void) {
    vTaskDelay(pdMS_TO_TICKS(3000));
    ESP_LOGI(TAG,"VALIDATE_START files=%u",(unsigned)(sizeof expected_frames/sizeof expected_frames[0]));
    for(size_t i=0;i<sizeof expected_frames/sizeof expected_frames[0];++i) {
        char path[80];snprintf(path,sizeof path,"/visual/%s",expected_frames[i].path);
        if(!load_sd_raw(path)||bench_hash()!=expected_frames[i].hash) {
            ESP_LOGE(TAG,"VALIDATE_FAIL %s",path);
            visual_sd_card->host.set_card_clk(visual_sd_card->host.slot,4000);
            ESP_LOGI(TAG,"BENCH_DONE failed=1 restored_khz=4000");return;
        }
        if(i%100==99) ESP_LOGI(TAG,"VALIDATE_PROGRESS checked=%u",(unsigned)(i+1));
    }
    ESP_LOGI(TAG,"VALIDATE_PASS all image hashes match staging");
    // Exercise the real player, including DFPlayer and halo, for every scene.
    for(size_t i=0;i<media_scene_count;++i) {
        ESP_LOGI(TAG,"VALIDATE_PLAY %s",media_scenes[i].id);
        play_media_scene(&media_scenes[i]);
        draw_idle();vTaskDelay(pdMS_TO_TICKS(2500));
    }
    ESP_LOGI(TAG,"BENCH_DONE validated_khz=16000");
}
