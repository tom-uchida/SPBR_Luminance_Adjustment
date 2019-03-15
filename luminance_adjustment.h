////////////////////////////////////////
//   @file   luminance_adjustment.h
//   @author Tomomasa Uchida
//   @date   2019/03/16
////////////////////////////////////////

#if !defined  LUMINANCE_ADJUSTMENT_H
#define       LUMINANCE_ADJUSTMENT_H

#include "spbr.h"

class LuminanceAdjustment {
public:
    enum FILE_FORMAT
    {
        SPBR_ASCII  = 0,
        SPBR_BINARY = 1,
        PLY_ASCII   = 2,
        PLY_BINARY  = 3,
    };

    // Constructor
    LuminanceAdjustment();
    LuminanceAdjustment(FILE_FORMAT file_format);

    int mainsub_spbr(
        kvs::glut::Application* app,
        int                     argc,
        char**                  argv,
        SPBR*                   spbr_engine,
        kvs::PointObject*       object,
        FILE_FORMAT             file_format
    );

    // Functions to control Object and Renderer
    static void RegisterObject( kvs::Scene* scene, int argc, char** argv, SPBR* spbr_engine, const size_t LR );
    static kvs::PointObject* CreateObject(int argc, char** argv);
    static kvs::glsl::ParticleBasedRenderer* CreateRenderer( SPBR* spbr_engine, const size_t LR);
    void SetObject( SPBR* spbr_engine, kvs::PointObject* object );
    void ReplaceObject( kvs::Scene* scene, int argc, char** argv, SPBR* spbr_engine, const size_t LR );
    void SnapshotImage( kvs::Scene* scene, const std::string filename );

private:
    FILE_FORMAT m_file_format;

    void adjustLuminance(std::string img_name, std::string img_name_LR1);
};

#endif // end of luminance_adjustment.h