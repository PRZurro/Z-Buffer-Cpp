
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Started by Ángel on december of 2013                                       *
 *                                                                             *
 *  This is free software released into the public domain.                     *
 *                                                                             *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "View.hpp"

#include <cmath>
#include <cassert>


using namespace toolkit;

namespace przurro
{

	View::View(float nearPlaneD = 0.4f, float farPlaneD = 800.f, float fovDegrees = 90.0f, size_t width = 800, size_t height = 600)
    :
        width       (width ),
        height      (height),
        colorBuffer(width, height),
        rasterizer  (colorBuffer),
		projectionM(nearPlaneD, farPlaneD, fovDegrees, float(width) / float(height))
    {   
    }


    void View::paint ()
    {
		rasterizer.clear;

		// Se copia el framebúffer oculto en el framebúffer de la ventana:

		rasterizer.get_color_buffer().gl_draw_pixels(0, 0);
    }
}
