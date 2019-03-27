//////////////////////////////////////////
//   @file   luminance_adjustment.cpp
//   @author Tomomasa Uchida
//   @date   2019/03/15
//////////////////////////////////////////

#include <kvs/glut/Application>
#include <kvs/Version> //KVS2

#include <kvs/PointObject>

#if KVS_VERSION_MAJOR == 1
    #include <kvs/glew/ParticleBasedRenderer> //KVS1
#elif KVS_VERSION_MAJOR == 2
    #include <kvs/ParticleBasedRenderer> //KVS2
#endif

#include <kvs/glut/Screen>
#include <kvs/Camera>
#include <kvs/RotationMatrix33>

#include <cstring>
#include <iostream>

#include "single_inputfile.h"
#include "file_format.h"
#include "spbr.h"
#include "mainfn_utility.h"
#include "shuffle.h"

#include "luminance_adjustment.h"
#include <kvs/ColorImage>
#include <kvs/GrayImage>

#include "version.h"
#include <sstream>
#include <iomanip>

LuminanceAdjustment::LuminanceAdjustment()
{
    // Message
    std::cout << "\n\n** LuminanceAdjustment constructor is called.\n"  << std::endl;
} // End constructor

LuminanceAdjustment::LuminanceAdjustment(FILE_FORMAT file_format):
    m_file_format(file_format)
{
    // Message
    std::cout << "\n\n** LuminanceAdjustment constructor is called."  << std::endl;

    if ( m_file_format == SPBR_ASCII )
        std::cout << "**  FILE_FORMAT : SPBR_ASCII"  << "\n" << std::endl;
    else if ( m_file_format == SPBR_BINARY )
        std::cout << "**  FILE_FORMAT : SPBR_BINARY" << "\n" << std::endl;
    else if ( m_file_format == PLY_ASCII )
        std::cout << "**  FILE_FORMAT : PLY_ASCII"   << "\n" << std::endl;
    else if ( m_file_format == PLY_BINARY )
        std::cout << "**  FILE_FORMAT : PLY_BINARY"  << "\n" << std::endl;
} // End constructor

void LuminanceAdjustment::RegisterObject( kvs::Scene* scene, int argc, char** argv, SPBR* spbr_engine, const size_t LR )
{
    // scene->registerObject( LuminanceAdjustment::CreateObject(argc, argv), LuminanceAdjustment::CreateRenderer(spbr_engine, LR) );
    scene->registerObject( CreateObject(argc, argv), CreateRenderer(spbr_engine, LR) );
} // End RegisterObject

kvs::PointObject* LuminanceAdjustment::CreateObject(int argc, char** argv) {
    // Read the first data file (argv[1])
    SPBR* spbr_engine          = new SPBR( argv[1] , SPBR_ASCII_FORMAT::SPBR_ASCII );
    kvs::PointObject* object   = spbr_engine;

    // Read and append the remaining files:  
    //  argv[2], argv[3], ..., argv[argc-1]
    for (int i = 3; i <= argc; i++) {
        if ( isASCII_PLY_File(argv[i - 1]) ) {
            SPBR* spbr_tmp = new SPBR(argv[i - 1], PLY_ASCII_FORMAT::PLY_ASCII);
            object->add(*kvs::PointObject::DownCast(spbr_tmp));

        } else if ( isBINARY_PLY_File(argv[i - 1]) ) {
            SPBR* spbr_tmp = new SPBR(argv[i - 1], PLY_BINARY_FORMAT::PLY_BINARY);
            object->add(*kvs::PointObject::DownCast(spbr_tmp));
        
        } else if ( isBinarySPBR_File(argv[i - 1]) ) {
            SPBR* spbr_tmp = new SPBR(argv[i - 1], SPBR_BINARY_FORMAT::SPBR_BINARY);
            object->add(*kvs::PointObject::DownCast(spbr_tmp));
        
        } else {
            SPBR* spbr_tmp = new SPBR(argv[i - 1], SPBR_ASCII_FORMAT::SPBR_ASCII);
            object->add(*kvs::PointObject::DownCast(spbr_tmp));
        }
    } // end for

    addBoundingBoxToScene( spbr_engine );

    // Forced shuffle
    if ( spbr_engine->isForcedShuffleOn() ) {
        Shuffle shuffle_engine( spbr_engine );
    }

    // Set name
    object->setName("Object");

    // Object rotation (Z==>X) if required
    if ( spbr_engine->isZXRotation() ) {
        double zrot_deg = spbr_engine->objectZXRotAngle (0) ; 
        double xrot_deg = spbr_engine->objectZXRotAngle (1) ; 
        ToolXform::rotateZX( object, zrot_deg, xrot_deg, kvs::Vector3f( 0, 0, 0 ) );
    }

    return object;
} // End CreateObject()

void LuminanceAdjustment::SetObject( SPBR* spbr_engine, kvs::PointObject* object )
{
    object->setName("Object");

    // Object rotation (Z==>X) if required
    if ( spbr_engine->isZXRotation() ) {
        double zrot_deg = spbr_engine->objectZXRotAngle (0) ; 
        double xrot_deg = spbr_engine->objectZXRotAngle (1) ; 
        ToolXform::rotateZX( object, zrot_deg, xrot_deg, kvs::Vector3f( 0, 0, 0 ) );
    }
} // End SetObject()

kvs::glsl::ParticleBasedRenderer* LuminanceAdjustment::CreateRenderer( SPBR* spbr_engine, const size_t LR)
{
    kvs::glsl::ParticleBasedRenderer* renderer = new kvs::glsl::ParticleBasedRenderer();
    renderer->setName( "Renderer" );
    renderer->setRepetitionLevel( LR );
    
    // Set Lambert shading or keep Phong shading
    setShadingType( spbr_engine, renderer );

    // Shading control (ON/OFF)
    if ( spbr_engine->isShading() == false ) {
        std::cout << "** Shading is off" << std::endl;
        renderer->disableShading();
    }

    // LOD control (ON/OFF)
    if ( spbr_engine->isLOD() )
        renderer->enableLODControl();

    // Particle zoom control (ON/OFF)
    if ( spbr_engine->isParticleZoomOn() == false )
        renderer->disableZooming();
    else
        renderer->enableZooming();

    // Shuffle control (ON/OFF)
    if ( spbr_engine->isParticleShuffleOn() )
        renderer->enableShuffle();
    else
        renderer->disableShuffle();

    return renderer;
} // End CreateRenderer()

void LuminanceAdjustment::ReplaceObject( kvs::Scene* scene, int argc, char** argv, SPBR* spbr_engine, const size_t LR )
{
    scene->replaceObject( "Object", CreateObject(argc, argv) );
    scene->replaceRenderer( "Renderer", CreateRenderer(spbr_engine, LR) );
} // End ReplaceObject()

void LuminanceAdjustment::SnapshotImage( kvs::Scene* scene, const std::string filename, const int repeat_level )
{
    // Snapshot
    scene->screen()->redraw();
    kvs::ColorImage color_image_tmp = scene->camera()->snapshot();

    // Save color image
    if ( m_snapshot_counter == 0 ) m_img_Color       = color_image_tmp;
    if ( m_snapshot_counter == 1 ) m_img_Color_LR1   = color_image_tmp;

    // Write color image
    // color_image_tmp.write( filename + "_LR" + kvs::String::ToString(repeat_level) + ".bmp" ); // kvs2.7
    std::ostringstream oss;
    oss << repeat_level;
    color_image_tmp.write( filename + "_LR" + oss.str() + ".bmp" );
    std::cout << "** Snapshot repeat level \"" << repeat_level << "\" image (BMP)" << std::endl;

    m_snapshot_counter++;
} // End snapshotTwoImages()

void LuminanceAdjustment::adjustLuminance( const std::string filename )
{
    // Display opening message
    displayMessage();

    // Calc number of pixels of the image
    size_t N_all = m_img_Color.numberOfPixels();
    size_t N_all_non_bgcolor = calcNumOfPixelsNonBGColor( m_img_Color );
    std::cout << "** Num. of pixels               : " << N_all             << " (pixels)" << std::endl;
    std::cout << "** Num. of pixels non BGColor   : " << N_all_non_bgcolor << " (pixels)" << std::endl;

    // Convert color to gray
    kvs::GrayImage img_Gray( m_img_Color );
    kvs::GrayImage img_Gray_LR1( m_img_Color_LR1 );

    // ====================================
    //  STEP1 : Get max pixel value (LR=1)
    // ====================================
    kvs::UInt8 max_pixel_value_LR1 = calcMaxPixelValue( img_Gray_LR1 );
    std::cout << "** Max pixel value (LR=1)       : " << +max_pixel_value_LR1 << " (pixel value)" << std::endl;

    // =================================================
    //  STEP2 : Search for reference pixel value (LR=1) 
    // =================================================
    size_t N_all_non_bgcolor_LR1 = calcNumOfPixelsNonBGColor( m_img_Color_LR1 );
    kvs::UInt8 reference_pixel_value_LR1 = searchReferencePixelValue( img_Gray_LR1, N_all_non_bgcolor_LR1, max_pixel_value_LR1 );
    std::cout << "** Reference pixel value (LR=1) : " << +reference_pixel_value_LR1 << " (pixel value)" << std::endl;

    // ==========================
    //  STEP3 : Adjust luminance
    // ==========================
    float p = calcAdjustmentParameter( m_img_Color, reference_pixel_value_LR1, N_all_non_bgcolor );
    doLuminanceAdjustment( m_img_Color, p );
    std::cout << "** Adjustment parameter         : " << p << std::endl;

    // Write adjusted image
    std::ostringstream oss;
    oss << p;
    std::string adjusted_filename(filename + "_adjusted" + oss.str() + ".bmp");
    m_img_Color.write( adjusted_filename );
    std::cout << "** Saved adjusted image."                 << std::endl;
    std::cout << "   (PATH : " << adjusted_filename << ")"  << std::endl;
    std::cout << "================================\n"       << std::endl;
    
    // Exec. open command
    std::string EXEC("open ");
    EXEC += adjusted_filename;
    system( EXEC.c_str() );

} // End adjustLuminance()

inline void LuminanceAdjustment::displayMessage()
{
    std::cout << "\n" << std::endl;
    std::cout << LA_TITLE                   << std::endl;
    std::cout << "          " << LA_VERSION << std::endl;
    std::cout << "           " << LA_DATE   << std::endl;
    std::cout << "        " << LA_AUTHOR    << "\n" << std::endl;
} // End displayMessage()

inline int LuminanceAdjustment::calcNumOfPixelsNonBGColor( const kvs::ColorImage& color_image )
{
    size_t counter = 0;
    kvs::RGBColor pixel;

    for ( size_t j = 0; j < color_image.height(); j++ ) {
        for ( size_t i = 0; i < color_image.width(); i++ ) {
            if ( color_image.pixel( i, j ) == m_bgcolor ) {
            } else { counter++; }
        }
    }

    return counter;
} // End calcNumOfPixelsNonBGColor()

inline kvs::UInt8 LuminanceAdjustment::calcMaxPixelValue( const kvs::GrayImage& gray_image )
{
    kvs::UInt8 max_pixel_value = 0;

    for ( size_t j = 0; j < gray_image.height(); j++ ) {
        for ( size_t i = 0; i < gray_image.width(); i++ ) {
            if ( gray_image.pixel( i, j ) > max_pixel_value ) 
                max_pixel_value = gray_image.pixel( i, j );
        }
    }

    return max_pixel_value;
} // End calcMaxPixelValue()

inline kvs::UInt8 LuminanceAdjustment::searchReferencePixelValue(const kvs::GrayImage& gray_image, const size_t N_all_non_bgcolor, const kvs::UInt8 max_pixel_value_LR1)
{
    kvs::UInt8 reference_pixel_value = max_pixel_value_LR1;
    float ratio_tmp = 0.0;

    // Search for reference pixel value
    while ( ratio_tmp < m_ratio_of_reference_section ) {
        int counter = 0;
        for ( size_t j = 0; j < gray_image.height(); j++ ) {
            for ( size_t i = 0; i < gray_image.width(); i++ ) {
                if ( gray_image.pixel( i, j ) >= reference_pixel_value ) 
                    counter++;
            }
        }

        ratio_tmp = float(counter) / float(N_all_non_bgcolor);
        //std::cout << "ratio_tmp : " << ratio_tmp << std::endl;

        // Next pixel value
        reference_pixel_value--;
    } // end while

    return reference_pixel_value;
} // End searchReferencePixelValue()

float LuminanceAdjustment::calcAdjustmentParameter( const kvs::ColorImage& color_image, const kvs::UInt8 reference_pixel_value_LR1, const size_t N_all_non_bgcolor )
{
    float adjustment_parameter = 1.0;
    float tmp_ratio_of_reference_section = 0.0;
    
    std::cout << std::endl;
    while ( tmp_ratio_of_reference_section < m_ratio_of_reference_section ) {
        // Update adjustment parameter
        adjustment_parameter += 0.1;

        tmp_ratio_of_reference_section = tempolarilyAdjustLuminance(
            /* kvs::ColorImage  */ color_image, 
            /* const float      */ adjustment_parameter, 
            /* const kvs::UInt8 */ reference_pixel_value_LR1, 
            /* const size_t     */ N_all_non_bgcolor );
    } // end while
    std::cout << std::endl;

    return adjustment_parameter -= 0.1;
} // End doLuminanceAdjustment()

inline float LuminanceAdjustment::tempolarilyAdjustLuminance( const kvs::ColorImage& color_image, const float p, const kvs::UInt8 reference_pixel_value_LR1, const size_t N_all_non_bgcolor )
{
    kvs::ColorImage color_image_tmp = deepCopyColorImage( color_image );
    doLuminanceAdjustment( color_image_tmp, p );

    // Convert color to gray
    kvs::GrayImage gray_image_tmp( color_image_tmp );

    int counter = 0;
    for ( size_t j = 0; j < gray_image_tmp.height(); j++ )
        for ( size_t i = 0; i < gray_image_tmp.width(); i++ )
            if ( gray_image_tmp.pixel( i, j ) >= reference_pixel_value_LR1 ) 
                counter++;

    float tmp_ratio = float(counter) / float(N_all_non_bgcolor);
    std::cout   << "           ( parameter, ratio ) : ( " 
                << p << ", " << std::setprecision(2) << 100*tmp_ratio << "(%) )" << std::endl;

    return tmp_ratio;
} // End 

inline kvs::ColorImage LuminanceAdjustment::deepCopyColorImage( const kvs::ColorImage& other )
{
    kvs::ColorImage duplicated_color_image( other.width(), other.height() );

    for ( size_t j = 0; j < other.height(); j++ )
        for ( size_t i = 0; i < other.width(); i++ )
                duplicated_color_image.setPixel( i, j, other.pixel( i, j ) );

    return duplicated_color_image;
} // End deepCopyImage()

inline void LuminanceAdjustment::doLuminanceAdjustment( kvs::ColorImage& color_image, const float p )
{
    kvs::RGBColor   pixel;
    kvs::UInt8      r, g, b;

    for ( size_t j = 0; j < color_image.height(); j++ ) {
        for ( size_t i = 0; i < color_image.width(); i++ ) {
            pixel = color_image.pixel( i, j );
            
            // Check whether the pixel value exceeds 255 after adjusting.
            if ( p*pixel.r() > 255 ) r = 255;
            else                     r = p*pixel.r();
            
            if ( p*pixel.g() > 255 ) g = 255;
            else                     g = p*pixel.g();

            if ( p*pixel.b() > 255 ) b = 255;
            else                     b = p*pixel.b();

            pixel.set( r, g, b );

            // Apply adjustment
            color_image.setPixel( i, j, pixel );
        } // end for
    } // end for
} // End doLuminanceAdjustment()