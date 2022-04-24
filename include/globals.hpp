#pragma once

#include <cstddef>

namespace imgSensor
{
    extern size_t MAX_IMG_DIM;      ///< Maximum allowed image dimension (including width and height).
    extern size_t MAX_WAIT_TIME_US; ///< Maximum wait time to query ultrasound probes.
    extern size_t MAX_WAIT_TIME_CT; ///< Maximum wait time to query computed tomography probes.
}