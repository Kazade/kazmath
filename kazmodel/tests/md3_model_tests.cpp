 #define BOOST_TEST_DYN_LINK
 #define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE md3_model_tests
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <kazmodel/model.h>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

SDL_WindowID window;

BOOST_AUTO_TEST_CASE(md3_test_load) {
    unsigned model_id;
    kmGenModels(1, &model_id);
    kmBindModel(model_id);

    //Loading a valid file should return true
    BOOST_ASSERT(kmLoadMD3ModelFromFile((const KMbyte*)"models/models/players/sonic/head.md3"));
    //And there should be no errors
    BOOST_CHECK(kmGetError() == KM_NO_ERROR);

    //Should have 3 parts
    BOOST_CHECK(kmGetModelAttri(KM_NUM_MODEL_PARTS) == 3);

    //The 3rd part (index 2) should be the head and so should have a single frame
    BOOST_CHECK(kmGetModelPartAttri(2, KM_NUM_MODEL_PART_FRAMES) == 1);
    //The head should have a single mesh
    BOOST_CHECK(kmGetModelPartAttri(2, KM_NUM_MODEL_PART_MESHES) == 1);
    //The head should have a single tag (to join to the body)
    BOOST_CHECK(kmGetModelPartAttri(2, KM_NUM_MODEL_PART_TAGS) == 1);

    //Loading an invalid file should return false
    BOOST_CHECK(!kmLoadMD3ModelFromFile((const KMbyte*)"/some/invalid/file.md3"));

    //An error should be recorded
    BOOST_CHECK(kmGetError() != KM_NO_ERROR);
}

KMboolean setupSDL() {
	if (SDL_VideoInit(NULL, 0) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return KM_FALSE;
	}
	//Create an opengl 3.0 window
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, context);

	SDL_Event resize;
	resize.type = SDL_WINDOWEVENT;
	resize.window.event = SDL_WINDOWEVENT_RESIZED;
	resize.window.data1 = 640;
	resize.window.data2 = 480;
	SDL_PushEvent(&resize);

	return KM_TRUE;
}

BOOST_AUTO_TEST_CASE(md3_test_render) {
    KMuint model;
    int i = 10000;
    static float angle = 0.0f;

    kmGenModels(1, &model);
    kmBindModel(model);
    kmLoadMD3ModelFromFile((const KMbyte*)"models/models/players/sonic/head.md3");
    kmSetPartAnimation(0, "LEGS_RUN");
    kmSetPartAnimation(1, "TORSO_STAND2");

    //kmSetPartFrame(0, 0);
    //kmSetPartFrame(1, 149);
  //  kmSetPartFrame(2, 0);


    BOOST_ASSERT(setupSDL());

    while(i--) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
                case SDL_WINDOWEVENT:
                    switch(event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED: {
                            int width = event.window.data1;
                            int height = event.window.data2;

                            if (height == 0) {
                                height = 1;
                            }

                            glViewport(0, 0, width, height);

                            glMatrixMode(GL_PROJECTION);
                            glLoadIdentity();
                            gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
                            glMatrixMode(GL_MODELVIEW);
                            glLoadIdentity();
                        }
                        break;
                        default:
                            break;
                    }
                break;
				default:
					break;
			}
		}

        (angle > 360.0f)? angle -=360.0f : angle +=0.02f;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -80.0f);
        glRotatef(angle, 0, 1, 0);
        kmUpdateModel(0.001f);
        kmRenderModel();
        SDL_GL_SwapWindow(window);
    }

    SDL_Quit();

}
