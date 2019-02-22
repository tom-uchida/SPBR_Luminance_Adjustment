////////////////////////////////////////
//   @file   create_two_input_images.h
//   @author Tomomasa Uchida
//   @date   2019/02/09
////////////////////////////////////////

#ifndef CREATE_TWO_INPUT_IMAGES_HH
#define CREATE_TWO_INPUT_IMAGES_HH

#include <kvs/glut/Application>
#include <kvs/Version> //KVS2

#if KVS_VERSION_MAJOR == 2
  #include <kvs/Scene> //KVS2
#endif

#include <kvs/glut/Screen>
#include <kvs/ColorImage>

#include "single_inputfile.h"

class createTwoInputImages : public kvs::glut::Screen
{
private:

    const int num_of_loop = 2;
    int     m_original_LR;
    char    m_filename_LR1[256];
    char    m_filename[256];
    bool    m_is_init = true;
    kvs::ColorImage m_snapshot_image;
    SingleInputFile* m_sif = SingleInputFile::GetInstance();

public:

    createTwoInputImages( kvs::glut::Application* app, int original_LR ):
        kvs::glut::Screen( app ),
        m_original_LR( original_LR )
    {} // End createTwoInputImages()
    
    void paintEvent(void) {
        if ( m_is_init ) {
            unsigned int now_LR;
            kvs::ObjectBase*    ob   = scene()->objectManager()->object();
            kvs::RendererBase*  rb   = scene()->rendererManager()->renderer();

            for (int i = 0; i < num_of_loop; i++) {
                
                // Snapshot repeat level "1" image
                if ( i == 0 ) {
                    m_sif->GetNameBody( m_filename_LR1 );
                    strcat( m_filename_LR1, "_LR1.bmp" );
                    m_snapshot_image = scene()->camera()->snapshot();
                    m_snapshot_image.write( m_filename_LR1 );
                    std::cerr << "\n** Snapshot repeat level \"1\" image (BMP)" << std::endl;

                    registerObject(static_cast<kvs::PointObject*>(ob), static_cast<kvs::glsl::ParticleBasedRenderer*>(rb));
                    rb = scene()->rendererManager()->renderer();
                    static_cast<kvs::glsl::ParticleBasedRenderer*>(rb)->setRepetitionLevel( m_original_LR );

                    scene()->objectManager()->change(1, static_cast<kvs::PointObject*>(ob), true);
                    scene()->rendererManager()->change(1, static_cast<kvs::glsl::ParticleBasedRenderer*>(rb), true);

                    redraw();

                    kvs::glut::Screen::paintEvent();
                } // end if

                // Snapshot original repeat level image
                if ( i == 1 )  {
                    // setBackgroundColor( kvs::RGBColor(255, 255, 255) );
                    // kvs::RendererBase* rb = scene()->rendererManager()->renderer();
                    now_LR = static_cast<kvs::glsl::ParticleBasedRenderer*>(rb)->repetitionLevel();
                    std::cout << "now_LR =" << now_LR << std::endl;
                    
                    // static_cast<kvs::glsl::ParticleBasedRenderer*>(rb)->setRepetitionLevel( m_original_LR );
                    // now_LR = static_cast<kvs::glsl::ParticleBasedRenderer*>(rb)->repetitionLevel();
                    // std::cout << "now_LR =" << now_LR << std::endl;
                    
                    // static_cast<kvs::glsl::ParticleBasedRenderer*>(rb)->setRepetitionLevel( m_original_LR );
                    //scene()->replaceRenderer(1, static_cast<kvs::glsl::ParticleBasedRenderer*>(rb), true);
                    //reset();
                    // redraw();
                    // scene()->camera()->update();

                    //kvs::glut::Screen::paintEvent();

                    m_sif->GetBMPName( m_filename );
                    m_snapshot_image = scene()->camera()->snapshot();
                    m_snapshot_image.write( m_filename );
                    std::cerr << "** Snapshot repeat level \"" << m_original_LR << "\" image (BMP)" << std::endl;

                    m_is_init = false;
                    //exit(0);
                } // end if
            } // end for
        } // end if
    } // end paintEvent()

    // void paintEvent(void) {
    //     unsigned int now_LR;
    //     kvs::RendererBase* rb = scene()->rendererManager()->renderer();

    //     for(int i=0; i<10; i++) {
            
    //         // now_LR = static_cast<kvs::glsl::ParticleBasedRenderer*>(rb)->repetitionLevel();
    //         // std::cout << "now_LR =" << now_LR << std::endl;
                    
    //         static_cast<kvs::glsl::ParticleBasedRenderer*>(rb)->setRepetitionLevel( m_original_LR );
    //         // now_LR = static_cast<kvs::glsl::ParticleBasedRenderer*>(rb)->repetitionLevel();
    //         // std::cout << "now_LR =" << now_LR << std::endl;

    //         kvs::glut::Screen::paintEvent();
    //         kvs::ColorImage snapshot_image;
    //         snapshot_image = scene()->camera()->snapshot();
    //         std::string o_filename( "SPBR_DATA/image" );
    //         char s[5];
    //         sprintf(s, "%03d", i);
    //         o_filename += s;
    //         o_filename += ".bmp";
    //         snapshot_image.write(o_filename);

    //         m_original_LR += 10;
    //     }
        
    //     exit(0);
    // }
    //----
};

#endif // end of create_two_input_images.h