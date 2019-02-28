//////////////////////////////////////
//   @file   luminance_adjustment.cpp
//   @author Tomomasa Uchida
//   @date   2019/02/11
//////////////////////////////////////

#include <kvs/glut/Application>
#include <kvs/Version> //KVS2

#if KVS_VERSION_MAJOR == 1
  #include <kvs/glew/ParticleBasedRenderer> //KVS1
#elif KVS_VERSION_MAJOR == 2
  #include <kvs/ParticleBasedRenderer> //KVS2
#endif

#include <kvs/glut/Screen>

#include <cstring>
#include <iostream>

#include "event_control.h"

#include "spbr.h"
#include "version.h"
#include "display_opbr_usage.h"

#include "toolxform.h"
#include "shuffle.h"

#include "mainfn_utility.h"

#include "luminance_adjustment.h"
#include "create_two_input_images.h"
#include <kvs/glut/Timer>
#include "control_object_renderer.h"

LuminanceAdjustment::LuminanceAdjustment()
{
    //---- Message
    std::cout << "\n\n===== Luminance Adjustment ====="           << std::endl;
    std::cout << "        Tomomasa Uchida"                    << std::endl;
    std::cout << "           2019/02/28"                      << std::endl;
    std::cout << "\n** LuminanceAdjustment constructor is called."<< std::endl;
}

int LuminanceAdjustment::mainsub_spbr(
    kvs::glut::Application* app,
    int                     argc,
    char**                  argv,
    SPBR*                   spbr_engine,
    kvs::PointObject*       object )
{
    // Set the total bounding box
    //   Note: This updates the total bounding box of the 
    //         read point objects.
    addBoundingBoxToScene ( spbr_engine ) ;

    // Forced shuffle
    if( spbr_engine->isForcedShuffleOn () ) {
        Shuffle shuffle_engine ( spbr_engine );
    }

    //===== END OF CREATING THE POINT OBJECT =====//

    // Create screen
    kvs::glut::Screen screen( app );

    // Register object and renderer
    unsigned int original_LR = spbr_engine->repeatLevel();
    RegisterObject( screen.scene(), argc, argv, spbr_engine, original_LR);

    // Set camera type (orthogonal/perspective) and 
    //   other camera parameters: 
    //   camera position, look-at position, and view angle
    setCameraParameters( spbr_engine, &screen ); 

    // Set image resolution to the screen
    unsigned int img_resoln = spbr_engine->imageResolution();
    screen.setGeometry( 0, 0, img_resoln, img_resoln );

    // Forcibly set background color
    screen.setBackgroundColor( kvs::RGBColor(0, 0, 0) );
    std::cout << "** Forcibly, background color is set to \"black\"." << std::endl;

    // Set window title
    setWindowTitle( SPBR_WINDOW_TITLE, &screen );

    // Add events to the screen
    InitializeEvent init;
    screen.addEvent( &init );

    KeyPressEvent   key;
    screen.addEvent( &key );

    // Add timer event
    int sec = 1;
    kvs::glut::Timer timer( sec*1000 ); // msec
    TimerEvent timer_event( argc, argv, &timer, screen.scene(), spbr_engine, original_LR);
    screen.addTimerEvent( &timer_event, &timer );
    screen.addEvent( &timer_event );

    // Display menu in console
    std::cout << "** Executing particle-based rendering...\n" << std::endl;

    // Create and show the window
    screen.show();

    return ( app->run() );
} // End mainsub()

void LuminanceAdjustment::runPython() {
    // Set name of two input images
    char input_image[256];
    char input_image_LR1[256];
    SingleInputFile* p = SingleInputFile::GetInstance();
    p->GetNameBody( input_image );
    p->GetNameBody( input_image_LR1 );
    strcat( input_image, ".bmp" );
    strcat( input_image_LR1, "_LR1.bmp" );

    // Run python
    std::string EXEC_PYTHON("python correct_pixel_value.py ");
    EXEC_PYTHON += input_image;
    EXEC_PYTHON += " ";
    EXEC_PYTHON += input_image_LR1;
    system( EXEC_PYTHON.c_str() );
} // End runPython()