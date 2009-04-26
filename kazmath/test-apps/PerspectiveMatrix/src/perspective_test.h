#ifndef _NEHE_LESSON_1
#define _NEHE_LESSON_1

#include "basecode/lesson.h"
#include <kazmath/mat4.h>
#include <kazmath/vec3.h>
#include <iostream>
#include <iomanip>
#include <vector>

class perspective_test : public NeHe::Lesson
{
    /*
		 * We need a structure to store our vertices in, otherwise we
		 * just had a huge bunch of floats in the end
		 */
		struct Vertex
		{
			float x, y, z;

			Vertex(float x, float y, float z)
			{
				this->x = x;
				this->y = y;
				this->z = z;
			}
		};

		std::vector<Vertex> m_Vertices; //We have a vector of vertices, each vertex with its 3 coordinates
		std::vector<Vertex> m_Colors; //also 3 floats for colors, so we just use the Vertex-struct

	public:
		perspective_test();
		virtual bool init();


    private:
		virtual void draw();
		virtual void resize(int x, int y);
		void output_matrix(const kmMat4* matrix);
};

#endif
