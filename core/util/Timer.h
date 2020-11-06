#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <string>

namespace Sim
{
    class Timer
    {
    public:
        typedef std::chrono::high_resolution_clock Clock;
        //typedef std::chrono::milliseconds milliseconds;
        typedef std::chrono::duration<double> duration;

        Clock::time_point t0;
        Clock::time_point t1;
        std::string process;
        void begin(std::string name)
        {
            process = name;
            t0 = Clock::now();

        }

        void end()
        {
            t1 = Clock::now();
            duration time_span = std::chrono::duration_cast<duration>(t1 - t0);
            std::cout << process << " uses time about  "<< time_span.count() << " seconds. \n";
            /*milliseconds ms = std::chrono::duration_cast<milliseconds>(t1 - t0);
            std::cout << process << " uses time about  " << ms.count() << "ms\n";*/
        }
    };
    
    
}
