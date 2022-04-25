#include "include\hwInterface.hpp"

#include <iostream>
#include <future>
#include <string>
#include <chrono>

#include <cassert>

bool test(IMAGE_TYPE probeType, size_t imgDim, std::string outDir, INDUCED_ERROR err)
{
    INDUCED_ERROR errState = err;  // Manually induced error.
    // Create a new probe object
    imgSensor::ImgProbe imgProbe(probeType, imgDim, outDir);
    // Manually induce error to the sensor.
    induceError(err);
    // Capture an image and print the response
    return imgProbe.captureImg();
}

int main()
{
    bool response;

    try
    {
        std::cout << "Test1 - valid inputs: US" << std::endl;
        response = test(US, 5, "sensorData", NONE);
        assert(response);
        std::cout << std::endl;

        std::cout << "Test2 - valid inputs: CT" << std::endl;
        response = test(CT, 5, "sensorData", NONE);
        assert(response);
        std::cout << std::endl;

        std::cout << "Test3 - valid inputs: Large image size." << std::endl;
        response = test(US, 500, "sensorData", NONE);
        assert(response);
        std::cout << std::endl;

        std::cout << "Test4 - valid inputs: Non-existent output dir." << std::endl;
        response = test(US, 5, "./sensorData2", NONE);
        assert(response);
        std::cout << std::endl;

        std::cout << "Test5 - invalid inputs: Invalid output dir name." << std::endl;
        response = test(US, 5, "./sensor<data", NONE);
        assert(!response);
        std::cout << std::endl;

        std::cout << "Test6 - invalid inputs: Huge image size." << std::endl;
        response = test(US, 5000, "sensorData", NONE);
        assert(!response);
        std::cout << std::endl;

        std::cout << "Test7 - invalid inputs: Negative image size." << std::endl;
        response = test(US, -1, "sensorData", NONE);
        assert(!response);
        std::cout << std::endl;

        std::cout << "Test8: Empty return." << std::endl;
        response = test(US, 5, "sensorData", RETURN_EMPTY);
        assert(!response);
        std::cout << std::endl;

        std::cout << "Test9: Timeout return." << std::endl;
        response = test(US, 5, "sensorData", RETURN_TIMEOUT);
        assert(!response);
        std::cout << std::endl;

        std::cout << "Test10: Short delay (US)." << std::endl;
        response = test(US, 5, "sensorData", DELAY500);
        assert(response);
        std::cout << std::endl;

        std::cout << "Test11: Long delay (US)." << std::endl;
        response = test(US, 5, "sensorData", DELAY1100);
        assert(!response);
        std::cout << std::endl;

        std::cout << "Test12: Short delay (CT)." << std::endl;
        response = test(CT, 5, "sensorData", DELAY1100);
        assert(response);
        std::cout << std::endl;

        std::cout << "Test13: Long delay (CT)." << std::endl;
        response = test(CT, 5, "sensorData", DELAY2000);
        assert(!response);
        std::cout << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "All tests passed successfully." << std::endl;

    return 0;
}