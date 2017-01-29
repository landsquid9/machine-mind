/*
-----------------------------------------------------------------------------
Filename:    main.cpp
-----------------------------------------------------------------------------

This is where the program starts. The bulk of the program is controlled from 
a Machine Mind instance, which is in charge of the 3D world.
Network communications are carried out in a separate thread using a Network
instance, because the websocket functions are blocking.

*/

/* Built in includes */
#include <thread> // To create a new thread
#include <mutex>  // To ensure mutually exclusive data acces between threads

/* My includes */
#include "machine-mind.h"  // The 3D world
#include "network.h"       // Network comms
#include "shared-data.h"   // Holds data to be shared between threads

/* Ogre Includes */
#include <OgreException.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object
		Mind::MachineMind app;

		// Create Network object
		Mind::Network net;

		// Create Shared Data object
		Mind::SharedData sharedData;

		// Create Mutex
		std::mutex sharedDataMutex;

		// Setup Network
		net.setup(&sharedData, &sharedDataMutex);

		// Create a new thread.
		std::thread thread1 = net.wsThread();

		try
		{
			app.run(&sharedData, &sharedDataMutex);
		}
		catch (Ogre::Exception& e)
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occurred: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		/* Threads must be joined before quitting */
		thread1.join();

		return 0;
	}

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
