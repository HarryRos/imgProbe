#ifndef IMG_SENSOR_H
#define IMG_SENSOR_H

/**
 * Mock enum and variable used to manually induce different kinds of erroneous behaviour for the getImage interface. These are added here for testing purposes.
 * */
enum INDUCED_ERROR
{
    NONE,
    RETURN_EMPTY,
    RETURN_TIMEOUT,
    DELAY500,
    DELAY1100,
    DELAY2000
};

extern INDUCED_ERROR errState;
extern unsigned long imgSize; // This should be set by the hw initialisation procedure.

// A routine to manually induce errors to the sensor.
void induceError(INDUCED_ERROR err);

// The setup routine was not provider, an ad-hoc routine was written for the initialisation.
void initialiseSensor(unsigned long imgSize);

// Actual hw library interface from here onwards.
enum ERROR_TYPE
{
    OK,
    EMPTY,
    TIMEOUT
};

enum IMAGE_TYPE
{
    US,
    CT
};

/**
 * Mock hardware driver that respects the actual interface but can be used to induce errors via the global variable errState.
 * 
 * Retrieves an image of type \b type and stores the data in the memory space that begins at \b image.
 * 
 * It is assumed that the probe has been initialised beforehand with the appropriate state, including the image dimension (interface not provided). 
 * */
ERROR_TYPE getImage(IMAGE_TYPE type, double *image);

#endif // IMG_SENSOR_H