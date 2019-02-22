//////////////////////////////////////
//   @file   correct_pixel_value.h
//   @author Tomomasa Uchida
//   @date   2019/02/11
//////////////////////////////////////

#if !defined  CORRECT_PIXEL_VALUE_H
#define       CORRECT_PIXEL_VALUE_H

#include "spbr.h"

class CorrectPixelValue {

public:
    CorrectPixelValue();

    int mainsub_spbr(
        kvs::glut::Application* app,
        int                     argc,
        char**                  argv,
        SPBR*                   spbr_engine,
        kvs::PointObject*       object );

private:
    void runPython();
};

#endif // end of correct_pixel_value.h