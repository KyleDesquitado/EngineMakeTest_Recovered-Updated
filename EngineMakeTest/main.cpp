#include "InputSystem.h"
#include "AppWindow.h"

int main()
{
	AppWindow app;
	InputSystem input;
	if (app.init())
	{
		while (app.isRun())
		{
			
			app.broadcast();
			
		}
	}


	return 0;
}