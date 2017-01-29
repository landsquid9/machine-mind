/*
-----------------------------------------------------------------------------
Filename:    machine-mind.h
-----------------------------------------------------------------------------

This is the base file for Machine Mind. This is a 3D visualisation of the mind 
of a machine. Robots mounted with ultrasonic sensors feed data to this app. 
Upon encountering an object, that object's position is logged, and is used
to generate objects within this world. 

*/

#pragma once

/* Built in includes*/
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <sstream>
#include <vector>

/* Ogre includes for 3D rendering */
#include <OgreRoot.h>                      // Base Ogre object
#include <OgreString.h>                    // Ogre version of a string type
#include <OgreConfigFile.h>                // For parsing configuration file
#include <OgreRenderWindow.h>              // What the scene is rendered to
#include <OgreTextureManager.h>            // Textures
#include <OgreSceneManager.h>              // Scene
#include <OgreCamera.h>                    // Camera
#include <OgreWindowEventUtilities.h>      // TO control the window
#include <OgreFrameListener.h>             // For m+k input
#include <OgreViewport.h>                  // The on screen view of a scene
#include <OgreSceneNode.h>                 // Base object for everything within a scene
#include <OgreEntity.h>                    // The 3D objects
#include <OgreLogManager.h>				   // for logging
#include <OgreRay.h>					   // For gravity

/* OIS includes for processing input from mouse and keyboard*/
#include <OISEvents.h>                     // Events
#include <OISInputManager.h>               // Manager
#include <OISKeyboard.h>                   // Keyboard
#include <OISMouse.h>                      // Mouse



/* My Includes */
#include "landscape.h"        // The landscape
#include "shared-data.h"      // Holds data from network
#include "plant-cluster.h"    // A group of 3D plants
#include "user.h"             // Camera and controls


//---------------------------------------------------------------------------

namespace Mind
{
	/* Typedefs to making thigns easier to read */
	typedef std::vector<Mind::PlantCluster> PlantVector;
	typedef std::vector<Mind::PlantCluster>::iterator PlantItr;


	class MachineMind
		: public Ogre::WindowEventListener, // To control window
		public Ogre::FrameListener,         // for rendering
		public OIS::MouseListener,          // To capture mouse input
		public OIS::KeyListener             // To capture keyboard input
	{
	public:
		MachineMind(void);
		virtual ~MachineMind(void);

		/* Runs the program */
		bool run(SharedData *s, std::mutex *m);


	protected:


	private:
		/* called every frame */
		virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

		/* called when the window is resized or closed */
		virtual void windowResized(Ogre::RenderWindow* rw);
		virtual void windowClosed(Ogre::RenderWindow* rw);

		/* Input */
		virtual bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
		virtual bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
		virtual bool mouseMoved(const OIS::MouseEvent& evt);
		virtual bool keyPressed(const OIS::KeyEvent& evt);
		virtual bool keyReleased(const OIS::KeyEvent& evt);

		/* Setup is split up into these sections to aid reading*/
		bool setupConfiguration();
		bool setupWindow();
		bool setupResources();
		bool setupScene();
		bool setupOIS();
		bool setupEvents();

		Ogre::Root* mRoot;             // Root object
		Ogre::String mResourcesCfg;    // The name of the resource file
		Ogre::String mPluginsCfg;      // The name of the plugins file
		Ogre::RenderWindow* mWindow;   // The window
		Ogre::SceneManager* mSceneMgr; // Scene manager

		OIS::InputManager* mInputMgr;  // Manager
		OIS::Keyboard* mKeyboard;      // Keyboard
		OIS::Mouse* mMouse;            // Mouse

		Mind::Landscape landscape;     // The landscape
		Mind::PlantVector vPlants;     // A vector of plant clusters
		Mind::PlantItr iPlants;        // Iterator for the vector
		Mind::User user;               // Camera and controls
		Mind::SharedData *sd;          // Data from network

		Coords plantPos;               // xy coordinates of where the next plant cluster should go

		std::mutex *mtx;               // Mutual exclusion for threads

		bool mShutdown;                // Should we close the program?

	};

}

