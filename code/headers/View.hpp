
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Started by Ángel on december of 2013                                       *
 *                                                                             *
 *  This is free software released into the public domain.                     *
 *                                                                             *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef VIEW_HEADER
#define VIEW_HEADER

	#include "Declarations.hpp"
	#include "Rasterizer.hpp"	
	#include "Vector.hpp"

    namespace przurro
    {
        class View
        {
        private:
            size_t width;
            size_t height;

		private:

			Rasterizer<Color_Buff>	rasterizer;

			Color_Buff	colorBuffer;
			Projection_Matrix3f  projectionM; 

        public:

            View(float nearPlaneD, float farPlaneD, float fovDegrees, size_t width, size_t height);

		public:

            void paint  ();

		public:

			Rasterizer<Color_Buff> & get_rasterizer()
			{
				return rasterizer;
			}

			Projection_Matrix3f & get_projection_matrix()
			{
				return projectionM;
			}
        };
    }

#endif
