//////////////////////////////////////
//   @file   luminance_adjustment.h
//   @author Tomomasa Uchida
//   @date   2019/02/11
//////////////////////////////////////

#if !defined  LUMINANCE_ADJUSTMENT_H
#define       LUMINANCE_ADJUSTMENT_H

#include "spbr.h"

class LuminanceAdjustment {
private:
    void adjustLuminance(std::string img_name, std::string img_name_LR1);

public:
    LuminanceAdjustment();

    static int mainsub_spbr(
        kvs::glut::Application* app,
        int                     argc,
        char**                  argv,
        SPBR*                   spbr_engine,
        kvs::PointObject*       object );

    
};

#endif // end of luminance_adjustment.h