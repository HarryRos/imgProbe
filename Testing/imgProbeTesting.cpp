#include "include\hwInterface.hpp"

#include <iostream>
#include <future>
#include <string>
#include <chrono>

int main()
{
    // Initialisation
    std::string outDir = "sensorData";
    size_t imgDim = 5;
    IMAGE_TYPE probeType = US;

    // Create a new probe object
    imgSensor::ImgProbe imgProbe(probeType, imgDim, outDir);

    while (true)
    {
        try
        {
            std::cout << "Main thread: Calling sensor for new image." << std::endl;
            auto imgCaptureTask = std::async(std::launch::async, &imgSensor::ImgProbe::captureImg, &imgProbe);

            // std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "Main thread: Proceessing alternative task 1." << std::endl;
            // std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "Main thread: Proceessing alternative task 2." << std::endl;

            std::cout << "Main thread: Finished alternative tasks and waiting for sensor." << std::endl;
            imgCaptureTask.wait();
            if (!imgCaptureTask.get())
                std::cout << "Main thread: WARNING - Image capture failed." << std::endl;
            else
                std::cout << "Main thread: Image captured successfully." << std::endl;
            std::cout << std::endl;

        }
        catch (const std::system_error &e)
        {
            std::cerr << "Could not initialise new image capture thread: " << e.what() << std::endl;
        }
        catch (const std::bad_alloc &e)
        {
            std::cerr << "Could not allocate memory for new image capture thread: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "ERROR: Could not initialise new image capture thread. Unknown error occured." << std::endl;
        }
    }
    return 0;
}