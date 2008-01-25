/*
 * Copyright (C) 2008 Leandro A. F. Fernandes and Manuel M. Oliveira
 *
 * author   : Fernandes, Leandro A. F.
 * e-mail   : laffernandes@gmail.com
 * home page: http://www.inf.ufrgs.br/~laffernandes
 *
 *
 * The complete description of the implemented techinique can be found at
 *
 *      Leandro A. F. Fernandes, Manuel M. Oliveira
 *      Real-time line detection through an improved Hough transform voting scheme
 *      Pattern Recognition (PR), Elsevier, 41:1, 2008, 299-314.
 *      DOI: http://dx.doi.org/10.1016/j.patcog.2007.04.003
 *      Project Page: http://www.inf.ufrgs.br/~laffernandes/kht.html
 *
 * If you use this implementation, please reference the above paper.
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

#include <ctime>
#include <iostream>

#include <IL/il.h>
#include <IL/ilu.h>

#include <GL/glew.h>

#ifdef _WIN32
	#define NOMINMAX
	#include <GL/wglew.h>
#else
	#define GLX_GLXEXT_PROTOTYPES
	#include <GL/glx.h>
#endif

#include <GL/glut.h>

#include "../kht_source/kht.h"

// Image files.
struct binary_image_t
{
	// Pixels buffer size (in bytes).
	size_t buffer_size;
	
	// DevIL identification.
	ILuint devil_id;

	// Image file path.
	const char *filename;
	
	// Image pixels.
	unsigned char *pixels;

	// Image pixels copy.
	unsigned char *pixels_copy;

	// Image dimentions.
	size_t width;
	size_t height;

	// Number of relevant lines to be displayed.
	size_t relevant_lines;

	// Class constructor.
	binary_image_t() :
		buffer_size(0),
		devil_id(0),
		filename(0),
		height(0),
		pixels(0),
		pixels_copy(0),
		relevant_lines(0),
		width(0)
	{
	}

	// Class destructor.
	~binary_image_t()
	{
		ilDeleteImage( devil_id );
		free( pixels );
		free( pixels_copy );
	}
};

// Specifies a list of binary images.
static list<binary_image_t,1> images_list;

// Specifies the image to be processed.
static size_t current_image_index = 0;

// Frame rate.
static double frame_rate = 0.0;

// pi value.
static const double pi = 3.14159265358979323846;

// Updates window title.
inline
void
update_window_title()
{
	if (!images_list.empty())
	{
		binary_image_t &image = images_list[current_image_index];

		static char title[1024];
		sprintf( title, "KHT Sample Application - Image '%s' - %d most relevant lines - %1.2f fps", image.filename, image.relevant_lines, frame_rate );
		glutSetWindowTitle( title );
	}
	else
	{
		glutSetWindowTitle( "KHT Sample Application - Error: unknown image files" );
	}
}

// This callback function is called by GLUT when the scene must be rendered.
void
glut_display_func()
{
	binary_image_t *image = (!images_list.empty()) ? &images_list[current_image_index] : 0;
	
	// Clear back buffer.
	glClear( GL_COLOR_BUFFER_BIT );

	if (image)
	{
		// Execute the proposed Hough transform voting scheme and the peak detection procedure.
		static lines_list_t lines;

		memcpy( image->pixels_copy, image->pixels, image->buffer_size );
		kht( lines, image->pixels_copy, image->width, image->height, 10, 2.0, 0.5, 0.002, 2 );

		// Draw current image.
		double window_width = glutGet( GLUT_WINDOW_WIDTH );
		double window_height = glutGet( GLUT_WINDOW_HEIGHT );
		double half_window_width = 0.5 * window_width;
		double half_window_height = 0.5 * window_height;

		double xfactor = std::min( window_width, static_cast<double>( image->width ) ) / image->width;
		double yfactor = std::min( window_height, static_cast<double>( image->height ) ) / image->height;
		double factor = std::min( xfactor, yfactor );
		double half_factor = 0.5 * factor;

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho( -half_window_width, half_window_width, half_window_height, -half_window_height, -1.0, 1.0 );
		
		glPixelZoom( factor, -factor );
		glRasterPos2d( -half_factor * image->width, -half_factor * image->height );
		glDrawPixels( image->width, image->height, GL_LUMINANCE, GL_UNSIGNED_BYTE, image->pixels );

		// Draw detected line.
		glColor3d( 1.0, 1.0, 0.0 );
		glScaled( factor, factor, 1.0 );

		glBegin( GL_LINES );
		for (size_t i=0, end=std::min( image->relevant_lines, lines.size() ); i!=end; ++i)
		{
			line_t &line = lines[i];

			if (line.theta != 0.0)
			{
				static const double deg_to_rad = pi / 180.0;
				double theta = line.theta * deg_to_rad;
				double one_div_sin_theta = 1.0 / sin( theta );
				double cos_theta = cos( theta );
				double aux = half_window_width / factor;
				
				glVertex2d( -aux, (line.rho + aux * cos_theta) * one_div_sin_theta );
				glVertex2d( aux, (line.rho - aux * cos_theta) * one_div_sin_theta );
			}
			else
			{
				double aux = half_window_height / factor;

				glVertex2d( line.rho, -aux );
				glVertex2d( line.rho, aux );
			}
		}
		glEnd();
	}

	// Display back buffer.
	glutSwapBuffers();

	// Update frame rate.
	static int frames = 0;
	static clock_t last_time = 0;

	clock_t time_now = clock();
	++frames;
	if ((time_now - last_time) > (CLOCKS_PER_SEC * 2))
	{
		frame_rate = static_cast<double>( frames * CLOCKS_PER_SEC ) / (time_now - last_time);
		last_time = time_now;
		frames = 0;

		update_window_title();
	}
}

// This callback function is called by GLUT when the window is idle.
void
glut_idle_func()
{
	glutPostRedisplay();
}

// This callback function is called by GLUT when a key is pressured.
void
glut_keyboard_func(unsigned char key, int x, int y)
{
	if (!images_list.empty())
	{
		bool update_title = false;
		
		switch (key)
		{
			// Increase the number of relevant lines.
			case '+':
			case '=':
				images_list[current_image_index].relevant_lines++;
				update_title = true;
				break;

			// Decrease the number of relevant lines.
			case '-':
			case '_':
				if (images_list[current_image_index].relevant_lines != 0)
				{
					images_list[current_image_index].relevant_lines--;
					update_title = true;
				}
				break;
			
			// Set prior image as current.
			case '<':
			case ',':
				current_image_index = ((current_image_index != 0) ? current_image_index : images_list.size()) - 1;
				update_title = true;
				break;

			// Set next image as current.
			case '>':
			case '.':
				current_image_index =  (current_image_index + 1) % images_list.size();
				update_title = true;
				break;
		}

		if (update_title)
		{
			update_window_title();
		}
	}
}

// This callback function is called by GLUT when the window is resized.
void
glut_reshape_func(int width, int height)
{
	glViewport( 0, 0, width, height );
}

// The main function.
int
main(int argc, char *argv[])
{
	// Print some help.
	std::cout << "Kernel-Based Hough Transform Sample Application" << std::endl;
	std::cout << std::endl;
	std::cout << "  Control Keys:" << std::endl;
	std::cout << std::endl;
	std::cout << "     < and > : Prior/Next image" << std::endl;
	std::cout << "     - and + : Decrease/Increase the number of relevant lines" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	
	// DevIL initialization.
	ilInit();
	iluInit();
	ilOriginFunc( IL_ORIGIN_UPPER_LEFT );
	ilEnable( IL_ORIGIN_SET );

	// Load input binary images.
	static const size_t images_count = 8;
	static const char *filenames[images_count]       = { "./images/chess_binary.bmp", "./images/road_binary.bmp", "./images/wall_binary.bmp", "./images/board_binary.bmp", "./images/church_binary.bmp", "./images/building_binary.bmp", "./images/beach_binary.bmp", "./images/simple_binary.bmp" };
	static const size_t relevant_lines[images_count] = { 25,                          15,                         36,                         38,                          40,                           19,                             19,                          8                            };

	for (size_t i=0; i!=images_count; ++i)
	{
		ILuint devil_id = ilGenImage();
		ilBindImage( devil_id );

		std::cout << "Loading image '" << filenames[i] << "'... ";

		if (ilLoadImage( filenames[i] ))
		{
			binary_image_t &binary_image = images_list.push_back();

			// Convert image to single channel.
			ilConvertImage( IL_LUMINANCE, IL_BYTE );
			
			// Get image data.
			binary_image.devil_id = devil_id;

			binary_image.filename = filenames[i];
			binary_image.relevant_lines = relevant_lines[i];

			binary_image.width = ilGetInteger( IL_IMAGE_WIDTH );
			binary_image.height = ilGetInteger( IL_IMAGE_HEIGHT );
			
			binary_image.buffer_size = binary_image.height * binary_image.width * sizeof( unsigned char );
			binary_image.pixels = static_cast<unsigned char*>( malloc( binary_image.buffer_size ) );
			binary_image.pixels_copy = static_cast<unsigned char*>( malloc( binary_image.buffer_size ) );
			
			memcpy( binary_image.pixels, ilGetData(), binary_image.buffer_size );
			memset( binary_image.pixels_copy, 0, binary_image.buffer_size );

			std::cout << "done." << std::endl;
		}
		else
		{
			ilDeleteImage( devil_id );

			std::cout << "error." << std::endl;
		}
	}

	// GLUT initialization.
	static const int window_width = 700, window_height = 700;

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE );
	glutInitWindowPosition( (glutGet( GLUT_SCREEN_WIDTH ) - window_height) / 2, (glutGet( GLUT_SCREEN_HEIGHT ) - window_height) / 2 );
	glutInitWindowSize( window_width, window_height );

	// Create a GLUT window.
	glutCreateWindow( "" );

	glutDisplayFunc( glut_display_func );
	glutIdleFunc( glut_idle_func );
	glutKeyboardFunc( glut_keyboard_func );
	glutReshapeFunc( glut_reshape_func );

	update_window_title();

	// GLEW initialization.
	glewInit();

	// Disable v-sync.
#ifdef _WIN32
	wglSwapIntervalEXT( 0 );
#else
	glXSwapIntervalSGI( 0 );
#endif

	// Application main loop.
	glutMainLoop();
	
	// Shut down DevIL.
	ilShutDown();

	return EXIT_SUCCESS;
}
