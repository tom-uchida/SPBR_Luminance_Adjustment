//////////////////////////////////////////////////
//   @file   mainsub_luminance_adjustment.cpp
//   @author Tomomasa Uchida
//   @date   2019/03/16
//////////////////////////////////////////////////

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
#include "file_format.h"
#include "spbr.h"
#include "version.h"
#include "display_opbr_usage.h"

#include "toolxform.h"
#include "shuffle.h"

#include "mainfn_utility.h"

#include "luminance_adjustment.h"
#include <kvs/glut/Timer>

int mainsub_luminance_adjustment(
    kvs::glut::Application*             app,
    int                                 argc,
    char**                              argv,
    SPBR*                               spbr_engine,
    kvs::PointObject*                   object,
    LuminanceAdjustment::FILE_FORMAT    file_format)
{
    // Instantiate class LuminanceAdjustment
    LuminanceAdjustment* la = new LuminanceAdjustment(file_format); 

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
    unsigned int original_repeat_level = spbr_engine->repeatLevel();
    la->RegisterObject( screen.scene(), argc, argv, spbr_engine, original_repeat_level);

    // Set camera type (orthogonal/perspective) and 
    //   other camera parameters: 
    //   camera position, look-at position, and view angle
    setCameraParameters( spbr_engine, &screen ); 

    // Set image resolution to the screen
    unsigned int img_resoln = spbr_engine->imageResolution();
    screen.setGeometry( 0, 0, img_resoln, img_resoln );

    // Forcibly, set background color "Black"
    screen.setBackgroundColor( kvs::RGBColor(0, 0, 0) );
    std::cout << "** Forcibly, background color is set to \"black\"." << std::endl;

    // Set window title
    setWindowTitle( SPBR_WINDOW_TITLE, &screen );

    // Add initialize event
    InitializeEvent init;
    screen.addEvent( &init );

    // Add keypress event
    KeyPressEvent   key;
    screen.addEvent( &key );

    // Add timer event
    const int sec = 1; // sec
    kvs::glut::Timer timer( sec*1000 ); // msec
    TimerEvent timer_event(
        /* LuminanceAdjustment* */  la, 
        /* int                  */  argc, 
        /* char**               */  argv, 
        /* kvs::glut::Timer*    */  &timer, 
        /* kvs::Scene*          */  screen.scene(), 
        /* SPBR*                */  spbr_engine, 
        /* const int            */  original_repeat_level);
    screen.addTimerEvent( &timer_event, &timer );
    screen.addEvent( &timer_event );

    std::cout << "** Executing particle-based rendering...\n" << std::endl;

    // Create and show the window
    screen.show();

    return ( app->run() );
} // End mainsub_spbr()

// end of mainsub_luminance_adjustment.cpp