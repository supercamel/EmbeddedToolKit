#include <chrono>
#include <thread>
#include <etk/etk.h>
 
using namespace std;
 
etk::Time _now;
auto start_time = std::chrono::high_resolution_clock::now();
 
 
etk::Time now()
{
    auto end_time = std::chrono::high_resolution_clock::now();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
    auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    
    _now.seconds() = seconds;
    _now.micros() = (micros%1000000);
    return _now;
}
 
void sleep_ms(uint32_t ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
 
void sleep_us(uint32_t us)
{
    std::this_thread::sleep_for(std::chrono::microseconds(us));
}
 



