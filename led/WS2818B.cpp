#include "WS2818B.hpp"

WS2818B::WS2818B(Timer *timer, uint8_t channel_index, uint16_t chain_count) :
    Component("WS2818B"),
    timer(timer),
    channel_index(channel_index),
    chain_count(chain_count){
    // Create Component folder in FS
    Create_virtual_file("LED_count", this, &WS2818B::LED_count);

    // Setup init values of timer
    if (timer->Size() == 32) {
        timer_max_count = UINT32_MAX;
    } else if (timer->Size() == 16) {
        timer_max_count = UINT16_MAX;
    }

    // Calculate length of logic levels
    float nanosecond_per_tick = 1000000000 / timer->Input_frequency();
    tick_LOW  = logic_LOW / nanosecond_per_tick;
    tick_HIGH = logic_HIGH / nanosecond_per_tick;

    Init_timer();
}

int WS2818B::Init_timer(){
    timer->Mode(Timer::Modes::Timer_IRQ);
    timer->Prescaler(0);
    timer->Counter(timer_max_count - 1);

    timer->channel[channel_index].Mode(TIM_channel::Modes::Toggle);
    timer->channel[channel_index].Pulse(timer_max_count);

    timer->IRQ->Register(this, &WS2818B::Timer_stop);
    return 0;
}

int WS2818B::Color(Color::Colors color, float intensity){
    Color::RGB LED = Color::RGB(color);
    return Color(LED.red, LED.green, LED.blue, intensity);
}

int WS2818B::Color(uint8_t red, uint8_t green, uint8_t blue, float intensity){
    // Restrict intensity from 0 to 1.0
    intensity = min(max(intensity,0.0f),1.0f);
    // Generate protocol timings
    vector<uint32_t> protocol_timing = Protocol_generator(red * intensity, green * intensity, blue * intensity);
    Push_protocol(protocol_timing);
    return protocol_timing.size();
}

void WS2818B::Push_protocol(vector<uint32_t> &protocol_timing){
    timer->Start();
    timer->channel[channel_index].Start_DMA((uint32_t *) protocol_timing.data(), protocol_timing.size());
}

void WS2818B::Timer_stop(){
    device()->mcu->TIM_16->Stop();
    timer->Counter(0);
}

vector<uint32_t> WS2818B::Protocol_generator(uint8_t red, uint8_t green, uint8_t blue){
    vector<uint32_t> WS2818_bytes;
    uint8_t colors[3] = { red, green, blue };

    // Initialize vector
    WS2818_bytes.reserve(48);
    WS2818_bytes.push_back(0);

    // Generate all colors
    for (size_t color_index = 0; color_index < 3; color_index++) {
        for (int i = 7; i >= 0; i--) {
            if (colors[color_index] & (1 << i)) {
                Generate_HIGH(WS2818_bytes);
            } else {
                Generate_LOW(WS2818_bytes);
            }
        }
    }
    // Add stop element at end of field
    WS2818_bytes.back() = timer_max_count;

    return WS2818_bytes;
}

void WS2818B::Generate_HIGH(vector<uint32_t> &frame){
    frame.push_back(frame.back() + tick_HIGH);
    frame.push_back(frame.back() + tick_LOW);
}

void WS2818B::Generate_LOW(vector<uint32_t> &frame){
    frame.push_back(frame.back() + tick_LOW);
    frame.push_back(frame.back() + tick_HIGH);
}
