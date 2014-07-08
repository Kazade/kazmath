#include "perspective_test.h"

perspective_test::perspective_test()
{
}

bool perspective_test::init()
{
	if (!m_Window.createWindow(800, 600, 32, false, "Testing Kazmath's perspective matrix"))
	{
		return false;
	}

	glShadeModel(GL_SMOOTH);                // Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);   // Black Background
	glClearDepth(1.0f);                     // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);                // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                 // The Type Of Depth Testing To Do

	// Now we set up the vertex and color data we want to draw to the screen

	//The first three Vertices represent a triangle
	m_Vertices.push_back(Vertex( -1.5f, 1.0f, -6.0f));    // Top
	m_Vertices.push_back(Vertex(-2.5f, -1.0f, -6.0f));   // Bottom Left
	m_Vertices.push_back(Vertex( -0.5f, -1.0f, -6.0f));   // Bottom Right

	// We specify a color for every vertex, colors are float values for
	// Red Green Blue each ranging from 0.0 (nothing) to 1.0 (full intensity)
	m_Colors.push_back(Vertex(1.0f, 0.0f, 0.0f));       // Red
	m_Colors.push_back(Vertex(0.0f, 1.0f, 0.0f));        // Green
	m_Colors.push_back(Vertex(0.0f, 0.0f, 1.0f));		// Blue

	// Next thing we want to draw is a quad:
	m_Vertices.push_back(Vertex( 0.5f, 1.0f, -6.0f));	// Top Left
	m_Vertices.push_back(Vertex( 2.5f, 1.0f, -6.0f));	// Top Right
	m_Vertices.push_back(Vertex( 2.5f, -1.0f, -6.0f));	// Bottom Right
	m_Vertices.push_back(Vertex( 0.5f, -1.0f, -6.0f));	// Bottom Left

	m_Colors.push_back(Vertex(1.0f, 1.0f, 1.0f));       // White
	m_Colors.push_back(Vertex(1.0f, 0.0f, 1.0f));        // Lila
	m_Colors.push_back(Vertex(0.0f, 1.0f, 1.0f));		// Turquoise
	m_Colors.push_back(Vertex(1.0f, 1.0f, 0.0f));		// Yellow

	return true;
}

void perspective_test::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();					// Reset The Current Modelview Matrix

    kmMat4 rotation;
    kmVec3 axis;
    axis.x = 0.0f;
    axis.y = 0.0f;
    axis.z = 1.0f;

    kmMat4RotationAxisAngle(&rotation, &axis , kmDegreesToRadians(35.0f));
    std::cout << "Rotation Matrix with Kazmath is: " << std::endl;
	output_matrix(&rotation);

    //glMultMatrixf(&rotation.mat[0]);

    glRotatef(35.0f, 0.0f, 0.0f, 1.0f);

    kmMat4 modelview_matrix;
	glGetFloatv(GL_MODELVIEW_MATRIX, &modelview_matrix.mat[0]);
	std::cout << "Rotation Matrix with Fixed Function pipeline is: " << std::endl;
	output_matrix(&modelview_matrix);

	//Now we need to tell OpenGL to use the arrays we filled with data
	glEnableClientState(GL_VERTEX_ARRAY);   //We want a vertex array
	glEnableClientState(GL_COLOR_ARRAY);    //and a color array

	glVertexPointer(3, GL_FLOAT, 0, &m_Vertices[0]);   //All values are grouped to three Floats, we start at the beginning of the array (offset=0) and want to use m_vertices as VertexArray
	glColorPointer(3, GL_FLOAT, 0, &m_Colors[0]);      //Same here, but use m_colors

	glDrawArrays(GL_TRIANGLES, 0, 3); //We draw the first three vertices in the array as a triangle
	glDrawArrays(GL_QUADS, 3, 4);     //next 4 vertices are a quad (starting at the 3rd one)

	//Disable using the Vertex and Color array
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void perspective_test::output_matrix(const kmMat4* matrix)
{
	for (int i = 0; i < 4; i++)
	{
		std::cout << " | ";
		for (int j = 0; j < 4; j++)
		{
			std::cout << std::setw(8) << matrix->mat[i + 4 * j] << " ";
		}
		std::cout << " | " << std::endl;
	}
}

void perspective_test::resize(int x, int y)
{
	std::cout << "Resizing Window to " << x << "x" << y << std::endl;

	if (y <= 0)
	{
		y = 1;
	}

	glViewport(0, 0, x, y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)x / (GLfloat)y, 1.0f, 100.0f);

	kmMat4 perspective_matrix;
	glGetFloatv(GL_PROJECTION_MATRIX, &perspective_matrix.mat[0]);
	std::cout << "Projection Matrix with gluPerspective is: " << std::endl;
	output_matrix(&perspective_matrix);

	kmMat4 kazmath_matrix;
	kmMat4PerspectiveProjection(&kazmath_matrix, 45.0f, (GLfloat)x / (GLfloat)y, 1.0f, 100.0f);
	std::cout << "Projection Matrix with Kazmath is: " << std::endl;
	output_matrix(&kazmath_matrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
