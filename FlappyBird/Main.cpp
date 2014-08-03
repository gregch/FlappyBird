#include "MainMenu.h"
#include "App.h"
 
int main(int argc, char** argv)
{
	App app;
	MainMenu mainMenu(app);
	app.setScene(&mainMenu);
	return app.run(argc, argv);
}