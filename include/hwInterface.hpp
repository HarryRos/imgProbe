#pragma once

#include <filesystem>
#include <mutex>
#include "include\imgSensor.h"
#include <future>

namespace imgSensor
{

    /**
     * This class handles asynchronous communication with an image probe hardware, capable of returning ultrasound or CT images.
     * */
    class ImgProbe
    {
    private:
        std::mutex m;           ///< A mutex lock to be used for serialisation of asynchronous tasks. 
        size_t imgDim;          ///< The probe's image dimensions (width and height).
        size_t counter;         ///< An incremental value to differentiate retrieved image filenames.
        IMAGE_TYPE probeType;   ///< The image type that the probe reads, ultrasound (US) or computed tomography (CT).
        std::ostringstream fileNameFmt;  ///< Used for filename string formatting.
        std::filesystem::path outPath;   ///< The directory where the read images will be saved.
        bool state;             ///< True if the probe was initialised properly.

        double *imgData;        ///< Pointer to the image data being read from the hardware (one image at a time).
        /**
         * Calls the probe driver and requests a new image of type \b ImgProbe::probeType. 
         * This is supposed to be run asynchronously, using a future promise.
         * On success, it writes the data on an array that starts at \b ImgProbe::imgData, which has a length of \b ImgProbe::imgDim x \b ImgProbe::imgDim and sets the value of \b promise to true.
         * 
         * */
        void queryProbe(std::promise<bool> *promise);

    public:
        /**
         * The probe's constructor. Creates a new probe object that can be used to read images of type \b probeType, with size \b imgDim x \b imgDim and stored using incremental filenames in \b outDir.
         * 
         * @attention Sets \b ImgProbe::state on success. If it fails to aquire the required resources, \b ImgProbe::captureImg will subsequently error.
         * */
        ImgProbe(IMAGE_TYPE probeType, size_t imgDim, std::string outDir);
        /**
         * Queries the hardware by running \b ImgProbe::queryProbe. Aborts if the probe does not respond by the maximum wait time defined accordingly for each probe type (see \b MAX_WAIT_TIME_US, \b MAX_WAIT_TIME_CT ).
         * 
         * @returns True on success.
         * */
        bool captureImg();
    };
}