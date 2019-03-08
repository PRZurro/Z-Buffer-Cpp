
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Started by Ángel on october of 2013                                        *
 *                                                                             *
 *  This is free software released into the public domain.                     *
 *                                                                             *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "declarations/Internal.hpp"

#include "Scene.hpp"

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

using namespace sf;
using namespace przurro;

static const size_t window_width  = 800;
static const size_t window_height = 600;


int main ()
{
    // Create the window and the view that will be shown within the window:

    Window window(VideoMode(window_width, window_height), "Z-Buffer", Style::Titlebar | Style::Close, ContextSettings(32));

	std::string assetsFolderPath = "..\\..\\assets\\scenes\\scene01.xml";

    Scene scene(assetsFolderPath, window_width, window_height);

    // Initialization:

    window.setVerticalSyncEnabled (true);

    glDisable (GL_BLEND);
    glDisable (GL_DITHER);
    glDisable (GL_CULL_FACE);
    glDisable (GL_DEPTH_TEST);
    glDisable (GL_TEXTURE_2D);

    glViewport     (0, 0, window_width, window_height);
    glMatrixMode   (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho        (0, GLdouble(window_width), 0, GLdouble(window_height), -1, 1);

    // Run the main loop:

    bool running = true;

    do
    {
        // Attend the window events:

        Event event;

        while (window.pollEvent (event))
        {
            if (event.type == Event::Closed)
            {
                running = false;
            }
        }

        // Update the view:

        scene.update ();

        // Repaint the view:

        scene.draw ();

        // Swap the OpenGL buffers:

        window.display ();
    }
    while (running);

    // Close the application:

    return (EXIT_SUCCESS);
}
