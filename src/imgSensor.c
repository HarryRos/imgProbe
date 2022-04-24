#include "include\imgSensor.h"
#include <time.h>

INDUCED_ERROR errState = NONE;
unsigned long imgSize = 5;

ERROR_TYPE getImage(IMAGE_TYPE type, double *image)
{

    switch (errState)
    {
    case NONE:
    {
        for (unsigned long i = 0; i < imgSize; i++)
            for (unsigned long j = 0; j < imgSize; j++)
                if (i == 1 || j == 1)
                    image[i * 5 + j] = 1;
                else
                    image[i * 5 + j] = 0;

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
        struct timespec ts;
        ts.tv_sec = 500 / 1000;
        ts.tv_nsec = (500 % 1000) * 1000000;
        nanosleep(&ts, NULL);

        for (unsigned long i = 0; i < imgSize; i++)
            for (unsigned long j = 0; j < imgSize; j++)
                if (i == 2 || j == 2)
                    image[i * 5 + j] = 1;
                else
                    image[i * 5 + j] = 0;
        return OK;
        break;
    }
    case DELAY1100:
    {
        struct timespec ts;
        ts.tv_sec = 1100 / 1000;
        ts.tv_nsec = (1100 % 1000) * 1000000;
        nanosleep(&ts, NULL);

        for (unsigned long i = 0; i < imgSize; i++)
            for (unsigned long j = 0; j < imgSize; j++)
                if (i == 3 || j == 3)
                    image[i * 5 + j] = 1;
                else
                    image[i * 5 + j] = 0;
        return OK;
        break;
    }
    default:
        return EMPTY;
    }
};
