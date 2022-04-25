
#include "include\hwInterface.hpp"
#include "include\globals.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <future>
#include <string>
#include <thread>
#include <float.h>

#define cimg_use_tiff
#include <CImg.h>

namespace imgSensor
{
    ImgProbe::ImgProbe(IMAGE_TYPE probeType, size_t imgDim, std::string outDir) : probeType(probeType), imgDim(imgDim), outPath(std::filesystem::path(outDir)), counter(0)
    {
        // Begin in an uninitialised state.
        state = false;

        // Make sure that the requested image dimensions are valid.
        if (imgDim <= 0 || imgDim > MAX_IMG_DIM)
        {
            std::cerr << "Failed to initialise image probe: Invalid image dimension." << std::endl;
            state = false;
            return;
        }
        else
        {
            initialiseSensor(imgDim); // Size of the image returned by the sensor.
        }

        // Check if the requested output path is indeed a directory.
        if (std::filesystem::exists(outPath))
        {
            if (!std::filesystem::is_directory(outPath))
            {
                std::cerr << "Failed to initialise image probe: Invalid output path." << std::endl;
                state = false;
                return;
            }
        }
        else
        {
            // Create the output directory if it does not exist.
            try
            {
                if (!std::filesystem::create_directory(outPath))
                {
                    std::cerr << "Failed to initialise image probe: Cannot create directory." << std::endl;
                    state = false;
                    return;
                }
            }
            catch (const std::filesystem::filesystem_error &e)
            {
                std::cerr << "Failed to initialise image probe: Cannot create directory." << e.what() << std::endl;
                state = false;
                return;
            }
            catch (...)
            {
                std::cerr << "Failed to initialise image probe: Unknown filesystem error." << std::endl;
                state = false;
                return;
            }
        }

        // Set the output file names format (5 spaces with leading zeros).
        fileNameFmt << std::setw(5) << std::setfill('0');

        // Allocate the memory space to store incoming data.
        imgData = new double[imgDim * imgDim];
        if (imgData == nullptr)
        {
            std::cerr << "Failed to initialise image probe: Memory allocation error." << std::endl;
            state = false;
            return;
        }

        // Quiet exception mode for the CImg library (no popup menu)
        cimg_library::cimg::exception_mode(0);

        // Initialisation succeeded.
        state = true;
        return;
    };

    void ImgProbe::queryProbe(std::promise<bool> *promise)
    {
        std::lock_guard<std::mutex> lk(m);
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));

        switch (getImage(probeType, imgData))
        {
        case ERROR_TYPE::OK:
        {
            // Scale the range down from 64bit to 32bit so that it can be saved to a tiff file
            auto scale_ratio = FLT_MAX/DBL_MAX;
            for(size_t i=0 ; i<imgDim*imgDim ; i++)
                imgData[i] *= scale_ratio;

            // Structure the imgData into a CImg object.
            // For efficiency, the data is not copied but the imgData array is shared by reference.
            cimg_library::CImg img(imgData, imgDim, imgDim, 1, 1, true);
            try
            {
                // Create a new filename by incrementing the probe's image counter.
                std::ostringstream fileName;
                fileName.copyfmt(fileNameFmt);
                fileName << counter++ << ".tiff";

                /**
                 * Something weird is going on here: Although CImg seems to support doubles (64bit depth), save_tiff outputs 32bit tiffs. Thus, the data was scaled beforehand to 32 bit.
                 *
                 * TODO: This needs to be investigated further.
                 * */
                img.save_tiff((outPath / fileName.str()).string().c_str());
                promise->set_value(true);
                return;
            }
            catch (const cimg_library::CImgIOException &e)
                {
                std::cerr << "Failed to write tiff image to disk: " << e.what() << std::endl;
                promise->set_value(false);
                return;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Failed to write tiff image to disk: " << e.what() << std::endl;
                promise->set_value(false);
                return;
            }
            catch (...)
            {
                std::cerr << "Failed to write tiff image to disk. Unknown error." << std::endl;
                promise->set_value(false);
                return;
            }
        }
        case ERROR_TYPE::EMPTY:
            std::cerr << "Retrieved empty image. Skipping image " << counter++ << ".tiff" << std::endl;
            promise->set_value(false);
            return;
        case ERROR_TYPE::TIMEOUT:
            std::cerr << "Failed to retrieve image. TIMEOUT error." << std::endl;
            promise->set_value(false);
            return;
        default:
            std::cerr << "Failed to retrieve image. Unknown error." << std::endl;
            promise->set_value(false);
            return;
        }
    }

    bool ImgProbe::captureImg()
    {
        if (!state)
        {
            std::cerr << "ERROR: The probe is not initialised. Image capture failed." << std::endl;
            return false;
        }

        // Depending on the probe type, retrieve the maximum time that the function will wait for a responce before aborting.
        size_t maxWaitTime;
        switch (probeType)
        {
        case US:
            maxWaitTime = MAX_WAIT_TIME_US;
            break;
        case CT:
            maxWaitTime = MAX_WAIT_TIME_CT;
            break;
        default:
            maxWaitTime = MAX_WAIT_TIME_US;
            break;
        };

        try
        {

            // Create a promise object to retrieve the result of an asynchronous call to queryProbe
            std::promise<bool> promise;
            auto promise_future = promise.get_future();

            // Run the queryProbe operation asynchronously
            auto f = std::async(&ImgProbe::queryProbe, this, &promise);

            // Capture the current time and set the timepoint when to stop waiting
            auto maxWait_passed = std::chrono::system_clock::now() + std::chrono::milliseconds(maxWaitTime);

            // Wait until queryProbe returns or maxWaitTime time has passed
            if (std::future_status::ready == promise_future.wait_until(maxWait_passed))
            {
                return promise_future.get();
            }
            else
            {
                std::cerr << "ERROR: The probe did not respond. Image capture failed." << std::endl;
                return false;
            }
        }
        catch (const std::system_error &e)
        {
            std::cerr << "Could not initialise new probe query thread: " << e.what() << std::endl;
            return false;
        }
        catch (const std::bad_alloc &e)
        {
            std::cerr << "Could not allocate memory for new probe query thread: " << e.what() << std::endl;
            return false;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "ERROR: Image capture failed. Unknown exception thrown." << std::endl;
            return false;
        }

        std::cerr << "ERROR: Image capture failed. Unknown error occured." << std::endl;
        return false;
    }
}