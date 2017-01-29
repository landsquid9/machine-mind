/*
-----------------------------------------------------------------------------
Filename:    machine-mind.cpp
-----------------------------------------------------------------------------
*/

#include "machine-mind.h"

Mind::MachineMind::MachineMind()
	:mRoot(0),
	mResourcesCfg(""),
	mPluginsCfg(""),
	mWindow(0),
	mSceneMgr(0),
	mInputMgr(0),
	mMouse(0),
	mKeyboard(0),
	landscape(),
	mShutdown(false)
{

}

Mind::MachineMind::~MachineMind()
{
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);

	windowClosed(mWindow);

	delete mRoot;
}

bool Mind::MachineMind::run(SharedData *s, std::mutex *m)
{
	sd = s;
	mtx = m;

	// Load the right files 
	#ifdef _DEBUG
		mResourcesCfg = "resources_d.cfg";
		mPluginsCfg = "plugins_d.cfg";
	#else
		mResourcesCfg = "resources.cfg";
		mPluginsCfg = "plugins.cfg";
	#endif

	// Initialise ogre
	mRoot = new Ogre::Root(mPluginsCfg);

	// Process configuration file
	if(!setupConfiguration()) return false;

	// Setup window
	if(!setupWindow()) return false;

	// Setup resources
	if(!setupResources()) return false;
	
	// Setup scene
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
	
	if (!setupScene()) return false;

	user.setup(mSceneMgr, mWindow);

	// OIS
	if(!setupOIS()) return false;
	if (!setupEvents()) return false;


	Ogre::LogManager::getSingletonPtr()->logMessage("*** Setup Completed ***");

	/* and begin */
	mRoot->startRendering();

	return true;
}

//---------------------------------------------------------------------------

bool Mind::MachineMind::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	if (mWindow->isClosed()) return false;
	if (mShutdown) return false;
	mKeyboard->capture();
	mMouse->capture();
	
	user.update(fe.timeSinceLastFrame);	

	/* Plants */
	for (iPlants = vPlants.begin(); iPlants != vPlants.end(); ++iPlants)
	{
		iPlants->update(fe.timeSinceLastFrame);
		if (iPlants->deleteNow)
		{
			iPlants->remove();
			vPlants.erase(iPlants);
			break;
		}
	}

	mtx->lock();
	if (sd->error != "")
	{
		Ogre::LogManager::getSingletonPtr()->logMessage(sd->error);
		sd->error = "";
	}
	if (sd->newData)
	{
		plantPos = sd->getPosition();
		std::stringstream sX;
		sX << (plantPos.x);
		std::stringstream sY;
		sY << (plantPos.y);
		Ogre::LogManager::getSingletonPtr()->logMessage(sX.str());
		Ogre::LogManager::getSingletonPtr()->logMessage(sY.str());
		vPlants.push_back(PlantCluster());
		vPlants.back().setup(mSceneMgr, plantPos.x, plantPos.y);
	}

	mtx->unlock();

	return true;
}

void Mind::MachineMind::windowResized(Ogre::RenderWindow* rw)
{
	int left, top;
	unsigned int width, height, depth;

	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState& ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void Mind::MachineMind::windowClosed(Ogre::RenderWindow* rw)
{
	if (rw == mWindow)
	{
		if (mInputMgr)
		{
			mInputMgr->destroyInputObject(mMouse);
			mInputMgr->destroyInputObject(mKeyboard);

			OIS::InputManager::destroyInputSystem(mInputMgr);
			mInputMgr = 0;
		}
	}
}

bool Mind::MachineMind::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	/* normal mouse processing here... */
	return true;
}

bool Mind::MachineMind::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	/* normal mouse processing here... */
	return true;
}

bool Mind::MachineMind::mouseMoved(const OIS::MouseEvent& evt)
{
	user.moveCamera(evt.state.X.rel, evt.state.Y.rel);

	return true;
}

bool Mind::MachineMind::keyPressed(const OIS::KeyEvent& evt)
{
	switch (evt.key)
	{
	case OIS::KC_ESCAPE:
		mtx->lock();
		sd->quit = true;
		mtx->unlock();
		mShutdown = true;
		break;
	case OIS::KC_UP:
	case OIS::KC_W:
		
		user.forward();
		break;

	case OIS::KC_DOWN:
	case OIS::KC_S:
		user.backward();
		break;

	case OIS::KC_LEFT:
	case OIS::KC_A:
		user.left();
		break;

	case OIS::KC_RIGHT:
	case OIS::KC_D:
		user.right();
		break;

	default:
		break;
	}

	return true;
}

bool Mind::MachineMind::keyReleased(const OIS::KeyEvent& evt)
{
	switch (evt.key)
	{
	case OIS::KC_UP:
	case OIS::KC_W:

		user.stopForward();
		break;

	case OIS::KC_DOWN:
	case OIS::KC_S:
		user.stopBackward();
		break;

	case OIS::KC_LEFT:
	case OIS::KC_A:
		user.stopLeft();
		break;

	case OIS::KC_RIGHT:
	case OIS::KC_D:
		user.stopRight();
		break;

	default:
		break;
	}
	return true;
}

bool Mind::MachineMind::setupConfiguration()
{
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);
	Ogre::String name, locType;
	Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();
	while (secIt.hasMoreElements())
	{
		Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator it;

		for (it = settings->begin(); it != settings->end(); ++it)
		{
			locType = it->first;
			name = it->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
		}
	}

	// Show the options screen
	if (!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
	{
		return false;
	}

	return true;
}
bool Mind::MachineMind::setupWindow()
{
	mWindow = mRoot->initialise(true, "MachineMind Render Window");

	return true;
}
bool Mind::MachineMind::setupResources()
{
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	return true;
}

bool Mind::MachineMind::setupScene()
{
	landscape.setup(*mSceneMgr);

	mSceneMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	return true;
}
bool Mind::MachineMind::setupOIS()
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");

	OIS::ParamList pl;
	size_t windowHandle = 0;
	std::ostringstream windowHandleStr;

	mWindow->getCustomAttribute("WINDOW", &windowHandle);
	windowHandleStr << windowHandle;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHandleStr.str()));

	mInputMgr = OIS::InputManager::createInputSystem(pl);

	mKeyboard = static_cast<OIS::Keyboard*>(
		mInputMgr->createInputObject(OIS::OISKeyboard, true));
	mMouse = static_cast<OIS::Mouse*>(
		mInputMgr->createInputObject(OIS::OISMouse, true));

	return true;
}
bool Mind::MachineMind::setupEvents()
{
	windowResized(mWindow);
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mRoot->addFrameListener(this);

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);
	return true;
}
