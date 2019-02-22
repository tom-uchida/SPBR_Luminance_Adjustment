//////////////////////////////////////////
//   @file   control_object_renderer.h
//   @author Tomomasa Uchida
//   @date   2019/02/16
//////////////////////////////////////////

#if !defined  CONTROL_OBJECT_RENDERER_HH
#define       CONTROL_OBJECT_RENDERER_HH

#include <iostream>
#include <kvs/glut/Application>
#include <kvs/Version> //KVS2
#include <kvs/glut/Screen>
#include <kvs/PointObject>
#include <kvs/ParticleBasedRenderer>
#include <kvs/Camera>
#include <kvs/RotationMatrix33>

#include "event_control.h"
#include "single_inputfile.h"
#include "spbr.h"
#include "file_format.h"

// class ControlObjectRenderer() {
// private:
//     int     m_argc;
//     char**  m_argv;

//     enum
//     {
//         SPBR_ASCII,
//         SPBR_BINARY,
//         PLY_ASCII,
//         PLY_BINARY
//     }

// public:
//     ControlObjectRenderer(int argc, char** argv) :
//         m_argc( argc ),
//         m_argv( argv )
//     {};
// };

static kvs::PointObject* CreateObject(int argc, char** argv) {
    // Read the first data file (argv[1])
    SPBR* spbr_engine          = new SPBR( argv[1] , SPBR_ASCII );
    kvs::PointObject* object   = spbr_engine;

    // Read and append the remaining files:  
    //  argv[2], argv[3], ..., argv[argc-1]
    for (int i = 3; i <= argc; i++) {
        if ( isASCII_PLY_File(argv[i - 1]) ) {
            SPBR* spbr_tmp = new SPBR(argv[i - 1], PLY_ASCII);
            object->add(*kvs::PointObject::DownCast(spbr_tmp));

        } else if ( isBINARY_PLY_File(argv[i - 1]) ) {
            SPBR* spbr_tmp = new SPBR(argv[i - 1], PLY_BINARY);
            object->add(*kvs::PointObject::DownCast(spbr_tmp));
        
        } else if ( isBinarySPBR_File(argv[i - 1]) ) {
            SPBR* spbr_tmp = new SPBR(argv[i - 1], SPBR_BINARY);
            object->add(*kvs::PointObject::DownCast(spbr_tmp));
        
        } else {
            SPBR* spbr_tmp = new SPBR(argv[i - 1], SPBR_ASCII);
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
}

static void SetObject( SPBR* spbr_engine, kvs::PointObject* object )
{
    object->setName("Object");

    // Object rotation (Z==>X) if required
    if ( spbr_engine->isZXRotation() ) {
        double zrot_deg = spbr_engine->objectZXRotAngle (0) ; 
        double xrot_deg = spbr_engine->objectZXRotAngle (1) ; 
        ToolXform::rotateZX( object, zrot_deg, xrot_deg, kvs::Vector3f( 0, 0, 0 ) );
    }
}

static kvs::glsl::ParticleBasedRenderer* CreateRenderer( SPBR* spbr_engine, const size_t LR)
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
}

static void RegisterObject( kvs::Scene* scene, int argc, char** argv, SPBR* spbr_engine, const size_t LR )
{
    scene->registerObject( CreateObject(argc, argv), CreateRenderer(spbr_engine, LR) );
}

static void ReplaceObject( kvs::Scene* scene, int argc, char** argv, SPBR* spbr_engine, const size_t LR )
{
    scene->replaceObject( "Object", CreateObject(argc, argv) );
    scene->replaceRenderer( "Renderer", CreateRenderer(spbr_engine, LR) );
}

static void CaptureImage( kvs::Scene* scene, const std::string filename )
{
    scene->screen()->redraw();
    scene->camera()->snapshot().write( filename );
}

#endif // end of control_object_renderer.h