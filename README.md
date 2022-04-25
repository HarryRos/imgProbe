# imgProbe
Interface that queries an image sensor and writes the returned data to disk as tiff images.

# Description
This project implements an interface that facilitates the use of a hardware sensor that reads ultrasound or computed tomography images. It provides a wrapper class that calls the hardware asynchronously and allows the main routine to continue without blocking. It also handles errors and hardware freeze.

For the intended use, refer to `imgProbeMain.cpp` (see below for compilation and execution instructions).

For the testing suite, refer to `imgProbeTesting.cpp` (see below for compilation and execution instructions).

# Dependencies

CImg libary (https://cimg.eu/): Used to write tiff images to disk

# imgProbeMain
This executable is an example of the expected use of the imgProbe interface within a typical workflow.

## Compile and execute

    g++ -I. -g .\src\**.c* .\imgProbeMain.cpp -ltiff -lgdi32 -static-libstdc++ -std=c++2a -o .\imgProbe.exe
    .\imgProbe.exe

## Expected output
The programme reads images of a predefined size (5x5) from the sensor, which render an off-centred cross shape with a pseudo-random grey value, and saves them with incremental numbers as filenames in .\sensorData.

    Main thread: Calling sensor for new image.
    Main thread: Proceessing alternative task 1.
    Main thread: Proceessing alternative task 2.
    Main thread: Finished alternative tasks and waiting for sensor.
    Main thread: Image captured successfully.

    Main thread: Calling sensor for new image.
    Main thread: Proceessing alternative task 1.
    Main thread: Proceessing alternative task 2.
    Main thread: Finished alternative tasks and waiting for sensor.
    Main thread: Image captured successfully.

    ...

# imgProbeTesting
This basic testing suite induces sensor faults using a mock sensor driver that was implemented here. It then performs unit testing on the imgProbe interface to ensure it is functioning properly.

## Compile and execute

    g++ -I. -g .\src\**.c* .\Testing\imgProbeTesting.cpp -ltiff -lgdi32 -static-libstdc++ -std=c++2a -o .\imgProbeTesting.exe
    .\imgProbeTesting.exe  

## Expected output

    Test1 - valid inputs: US

    Test2 - valid inputs: CT

    Test3 - valid inputs: Large image size.

    Test4 - valid inputs: Non-existent output dir.

    Test5 - invalid inputs: Invalid output dir name.
    TIFFOpen: ./sensor<data\00000.tiff: Cannot open.
    Failed to write tiff image to disk: [instance(5,5,1,1,0000019c39062d70,shared)] CImg<double>::save_tiff(): Failed to open file './sensor<data\00000.tiff' for writing.

    Test6 - invalid inputs: Huge image size.
    Failed to initialise image probe: Invalid image dimension.
    ERROR: The probe is not initialised. Image capture failed.

    Test7 - invalid inputs: Negative image size.
    Failed to initialise image probe: Invalid image dimension.
    ERROR: The probe is not initialised. Image capture failed.

    Test8: Empty return.
    Retrieved empty image. Skipping image 0.tiff

    Test9: Timeout return.
    Failed to retrieve image. TIMEOUT error.

    Test10: Short delay (US).

    Test11: Long delay (US).
    ERROR: The probe did not respond. Image capture failed.

    Test12: Short delay (CT).

    Test13: Long delay (CT).
    ERROR: The probe did not respond. Image capture failed.

    All tests passed successfully.

# Author
Charalambos (Harry) Rossides Ph.D.

https://www.harryrossides.info/

rossides.ac@gmail.com