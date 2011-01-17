#include "perspective_test.h"

int main(int argc, char **argv)
{
	//Create a lesson instance
	NeHe::Lesson* lesson = new perspective_test();

	//If we can initialize the lesson
	if(lesson->init())
	{
		//Then we run it
		lesson->run();
	}
	else
	{
		std::cout << "Lesson initializiation failed!" << std::endl;
	}

	//If we can't initialize, or the lesson has quit we delete the instance
	delete lesson;

	return 0;
};
