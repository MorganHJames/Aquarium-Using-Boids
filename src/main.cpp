#include "aquarium.h"
#include "Constants.h"

// main that controls the creation/destruction of an application
int main(int argc, char* argv[])
{
	// explicitly control the creation of our application
	aquarium* app = new aquarium();
	app->run("Aquarium", g_uiDEFAULT_SCREENWIDTH, g_uiDEFAULT_SCREENHEIGHT, false);

	// explicitly control the destruction of our application
	delete app;

	return 0;
}