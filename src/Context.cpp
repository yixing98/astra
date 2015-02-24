#include "Context.h"
#include <cstdlib>
#include "openni/OpenNIAdapter.h"

namespace sensekit {

    sensekit_status_t Context::initialize()
    {
        m_deviceManager.initialize();
        m_initialized = true;

        return SENSEKIT_STATUS_SUCCESS;
    }

    void Context::ensure_initialized()
    {
        if (!m_initialized)
        {
            initialize();
        }
    }

    sensekit_status_t Context::open_sensor(char* uri, sensekit_sensor_t** sensor)
    {
        if (NULL == uri)
        {
            return SENSEKIT_STATUS_INVALID_PARAMETER;
        }

        ensure_initialized();

        Device* device = NULL;
        m_deviceManager.query_for_device(uri, &device);

        *sensor  = (sensekit_sensor_t*)malloc(sizeof(sensekit_sensor_t));
        (*sensor)->p_deviceHandle = device;

        return SENSEKIT_STATUS_SUCCESS;
    }

    sensekit_status_t Context::close_sensor(sensekit_sensor_t** sensor)
    {
        ensure_initialized();

        Device* device = (*sensor)->p_deviceHandle;
        
        if (NULL != device)
        {
            delete device;
        }

        free(*sensor);

        *sensor = NULL;

        return SENSEKIT_STATUS_SUCCESS;
    }
}