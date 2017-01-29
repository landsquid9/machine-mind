/*
-----------------------------------------------------------------------------
Filename:    landscape.h
-----------------------------------------------------------------------------

This represents the landscape of the world. It consists of an imported 3D model 

*/

#pragma once

/* Built in includes */
#include <sstream>

/* Ogre includes */
#include <OgreEntity.h>
#include <OgreMeshManager.h>
#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreSceneNode.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreLogManager.h>
#include <OgreSceneNode.h>


namespace Mind
{
	class Landscape
	{

	public:

		Landscape(void);
		~Landscape(void);

		/* Run first to setup */
		bool setup(Ogre::SceneManager& sm);

	protected:

	private:

		/* The mesh */
		Ogre::Entity* groundEntity;
		Ogre::SceneNode* groundNode;

	};
}

