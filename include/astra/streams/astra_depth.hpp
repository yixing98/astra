#ifndef ASTRA_DEPTH_HPP
#define ASTRA_DEPTH_HPP

#include <astra_core/astra_core.hpp>
#include <astra/capi/astra_ctypes.h>
#include <astra/capi/streams/depth_capi.h>
#include <astra/streams/astra_image.hpp>
#include <astra/vector.hpp>

namespace astra {

    class coordinate_mapper
    {
    public:
        coordinate_mapper(astra_depthstream_t depthStream)
            : depthStream_(depthStream)
        { }

        coordinate_mapper(const coordinate_mapper& rhs)
        {
            this->depthStream_ = rhs.depthStream_;
        }

        coordinate_mapper operator=(const coordinate_mapper& rhs)
        {
            this->depthStream_ = rhs.depthStream_;
            return *this;
        }

        vector3f convert_depth_to_world(astra::vector3f depthPosition) const
        {
            float worldX, worldY, worldZ;
            astra_convert_depth_to_world(depthStream_,
                                         depthPosition.x,
                                         depthPosition.y,
                                         depthPosition.z,
                                         &worldX, &worldY, &worldZ);

            return vector3f(worldX, worldY, worldZ);
        }

        vector3f convert_world_to_depth(vector3f worldPosition) const
        {
            float depthX, depthY, depthZ;
            astra_convert_world_to_depth(depthStream_,
                                         worldPosition.x,
                                         worldPosition.y,
                                         worldPosition.z,
                                         &depthX, &depthY, &depthZ);

            return vector3f(depthX, depthY, depthZ);
        }

        void convert_depth_to_world(float  depthX, float  depthY, float  depthZ,
                                    float* worldX, float* worldY, float* worldZ) const
        {
            astra_convert_depth_to_world(depthStream_, depthX, depthY, depthZ,
                                         worldX, worldY, worldZ);
        }

        void convert_world_to_depth(float  worldX, float  worldY, float  worldZ,
                                    float* depthX, float* depthY, float* depthZ) const
        {
            astra_convert_world_to_depth(depthStream_,
                                         worldX, worldY, worldZ,
                                         depthX, depthY, depthZ);
        }

    private:
        astra_depthstream_t depthStream_;
    };

    class depthstream : public imagestream
    {
    public:
        explicit depthstream(astra_streamconnection_t connection)
            : imagestream(connection),
              depthStream_(reinterpret_cast<astra_depthstream_t>(connection)),
              coordinateMapper_(reinterpret_cast<astra_depthstream_t>(connection))
        { }

        static const astra_stream_type_t id = ASTRA_STREAM_DEPTH;

        conversion_cache_t depth_to_world_data()
        {
            conversion_cache_t data;
            astra_depthstream_get_depth_to_world_data(depthStream_, &data);

            return data;
        }

        bool registration_enabled()
        {
            bool enabled = false;
            astra_depthstream_get_registration(depthStream_, &enabled);

            return enabled;
        }

        void enable_registration(bool enable)
        {
            astra_depthstream_set_registration(depthStream_, enable);
        }

        const coordinate_mapper& coordinateMapper() { return coordinateMapper_; };

    private:
        astra_depthstream_t depthStream_;
        coordinate_mapper coordinateMapper_;
    };

    class depthframe : public imageframe<int16_t, ASTRA_STREAM_DEPTH>
    {
    public:
        depthframe(astra_imageframe_t frame)
            : imageframe(frame, ASTRA_PIXEL_FORMAT_DEPTH_MM)
        {}

    };
}

#endif /* ASTRA_DEPTH_HPP */