
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

#include "declarations/Internal.hpp"
#include "declarations/Math.hpp"
	
#include "Rasterizer.hpp"	

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

			Camera_sptr * activeCamera;

        public:

            View();

			~View()
			{
				delete activeCamera;
			}

		public:

            void paint  ();

		public:

			Rasterizer<Color_Buff> & get_rasterizer()
			{
				return rasterizer;
			}
        };
    }

#endif
