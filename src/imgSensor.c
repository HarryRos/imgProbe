#include "include\imgSensor.h"
#include <time.h>
#include <stdlib.h>
#include <float.h>

// Default values, these should be set externally by the caller.
INDUCED_ERROR errState = NONE; // Manually induced error. Default: NONE
unsigned long imgSize = 5;     // This should be set by the hw initialisation procedure.


void induceError(INDUCED_ERROR err)
{
    errState = err;
}

void initialiseSensor(unsigned long sz)
{
    // No validation happens here as this function is supposedly provided by a third party.
    imgSize = sz;  
    // Seed the random engine to create dissimilar images 
    srand(time(NULL));
}

ERROR_TYPE getImage(IMAGE_TYPE type, double *image)
{
    switch (errState)
    {
    case NONE:
    {
        // Create an image with a recognisable shape
        double v = ((double)rand()/RAND_MAX*2.0-1.0) * DBL_MAX;
        for (unsigned long i = 0; i < imgSize; i++)
            for (unsigned long j = 0; j < imgSize; j++)
                if (i == 1 || j == 1)
                    image[i * imgSize + j] = v;
                else
                    image[i * imgSize + j] = 0;

        return OK;
        break;
    }

    case RETURN_EMPTY:
        return EMPTY;
        break;

    case RETURN_TIMEOUT:
        return TIMEOUT;
        break;

    case DELAY500:
    {
        // Restrict size for convenience in these test cases
        if (imgSize < 5)
            imgSize = 5;

        // Sleep for 500 ms
        struct timespec ts;
        ts.tv_sec = 500 / 1000;
        ts.tv_nsec = (500 % 1000) * 1000000;
        nanosleep(&ts, NULL);

        // Create an image with a recognisable shape
        double v = ((double)rand()/RAND_MAX*2.0-1.0) * DBL_MAX;
        for (unsigned long i = 0; i < imgSize; i++)
            for (unsigned long j = 0; j < imgSize; j++)
                if (i == 2 || j == 2)
                    image[i * imgSize + j] = v;
                else
                    image[i * imgSize + j] = 0;
        return OK;
        break;
    }
    case DELAY1100:
    {
        // Restrict size for convenience in these test cases
        if (imgSize < 5)
            imgSize = 5;

        // Sleep for 1100 ms
        struct timespec ts;
        ts.tv_sec = 1100 / 1000;
        ts.tv_nsec = (1100 % 1000) * 1000000;
        nanosleep(&ts, NULL);

        // Create an image with a recognisable shape
        double v = ((double)rand()/RAND_MAX*2.0-1.0) * DBL_MAX;
        for (unsigned long i = 0; i < imgSize; i++)
            for (unsigned long j = 0; j < imgSize; j++)
                if (i == 3 || j == 3)
                    image[i * imgSize + j] = v;
                else
                    image[i * imgSize + j] = 0;
        return OK;
        break;
    }
    case DELAY2000:
    {
        // Restrict size for convenience in these test cases
        if (imgSize < 5)
            imgSize = 5;

        // Sleep for 2000 ms
        struct timespec ts;
        ts.tv_sec = 2000 / 1000;
        ts.tv_nsec = (2000 % 1000) * 1000000;
        nanosleep(&ts, NULL);

        // Create an image with a recognisable shape
        double v = ((double)rand()/RAND_MAX*2.0-1.0) * DBL_MAX;
        for (unsigned long i = 0; i < imgSize; i++)
            for (unsigned long j = 0; j < imgSize; j++)
                if (i == 4 || j == 4)
                    image[i * imgSize + j] = v;
                else
                    image[i * imgSize + j] = 0;
        return OK;
        break;
    }
    default:
        return EMPTY;
    }
};
