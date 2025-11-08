
#include "pch.h"
#include "application.h"

int main()
{

	if( auto app = std::make_unique<Application>() )
	{
		app->Initialize();
		app->Run();
	}

	return 0;
}