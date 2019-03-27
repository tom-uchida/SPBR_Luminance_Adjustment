////////////////////////////////////////
//   @file   luminance_adjustment.h
//   @author Tomomasa Uchida
//   @date   2019/03/15
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

    // main
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
    void        SetObject( SPBR* spbr_engine, kvs::PointObject* object );
    void        ReplaceObject( kvs::Scene* scene, int argc, char** argv, SPBR* spbr_engine, const size_t LR );
    void        SnapshotImage( kvs::Scene* scene, const std::string filename, const int repeat_level );
    size_t      getSnapshotCounter() const { return m_snapshot_counter; };

    // Functions to adjust luminance of the image
    void        adjustLuminance();
    void        setBackgroundColor( kvs::RGBColor bgcolor ) { m_bgcolor = bgcolor; };
    int         calcNumOfPixelsNonBGColor( const kvs::ColorImage& image );
    kvs::UInt8  calcMaxPixelValue( const kvs::GrayImage& image );
    kvs::UInt8  searchReferencePixelValue(const kvs::GrayImage& gray_image, const size_t N_all_non_bgcolor, const kvs::UInt8 max_pixel_value_LR1);
    float       calcAdjustmentParameter(const kvs::ColorImage& color_image, const kvs::UInt8 reference_pixel_value_LR1, const size_t N_all_non_bgcolor );
    float       tempolarilyAdjustLuminance( const kvs::ColorImage& color_image, const float p, const kvs::UInt8 reference_pixel_value_LR1, const size_t N_all_non_bgcolor );
    kvs::ColorImage deepCopyColorImage( const kvs::ColorImage& other );
    void        doLuminanceAdjustment( kvs::ColorImage& color_image, const float p );

private:
    FILE_FORMAT     m_file_format;
    size_t          m_snapshot_counter = 0;
    kvs::RGBColor   m_bgcolor;
    kvs::ColorImage m_img_Color, m_img_Color_LR1;
    const float     m_ratio_of_reference_section = 0.01; // 1(%)
};

#endif // end of luminance_adjustment.h