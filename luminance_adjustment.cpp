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

#include "luminance_adjustment.h"

LuminanceAdjustment::LuminanceAdjustment()
{
    // Message
    std::cout << "\n\n===== Luminance Adjustment ====="             << std::endl;
    std::cout << "        Tomomasa Uchida"                          << std::endl;
    std::cout << "          2019/03/16"                             << std::endl;
    std::cout << "\n** LuminanceAdjustment constructor is called."  << std::endl;
} // End constructor

LuminanceAdjustment::LuminanceAdjustment(FILE_FORMAT file_format):
    m_file_format(file_format)
{
    // Message
    std::cout << "\n\n===== Luminance Adjustment ====="             << std::endl;
    std::cout << "        Tomomasa Uchida"                          << std::endl;
    std::cout << "          2019/03/14"                             << std::endl;
    std::cout << "\n** LuminanceAdjustment constructor is called."  << std::endl;

    // if ( m_file_format == SPBR_ASCII )
    //     std::cout << "** FILE_FORMAT : SPBR_ASCII"  << std::endl;
    // else if ( m_file_format == SPBR_BINARY )
    //     std::cout << "** FILE_FORMAT : SPBR_BINARY" << std::endl;
    // else if ( m_file_format == PLY_ASCII )
    //     std::cout << "** FILE_FORMAT : PLY_ASCII"   << std::endl;
    // else if ( m_file_format == PLY_BINARY )
    //     std::cout << "** FILE_FORMAT : PLY_BINARY"  << std::endl;
} // End constructor

void LuminanceAdjustment::RegisterObject( kvs::Scene* scene, int argc, char** argv, SPBR* spbr_engine, const size_t LR )
{
    scene->registerObject( LuminanceAdjustment::CreateObject(argc, argv), LuminanceAdjustment::CreateRenderer(spbr_engine, LR) );
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
} // End SetObject

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
} // End ReplaceObject

void LuminanceAdjustment::SnapshotImage( kvs::Scene* scene, const std::string filename )
{
    scene->screen()->redraw();
    scene->camera()->snapshot().write( filename );
} // End SnapshotImage