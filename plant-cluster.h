/*
-----------------------------------------------------------------------------
Filename:    plant-cluster.h
-----------------------------------------------------------------------------

Plants are spawned as clusters with random scales

*/
#pragma once

/* Built in includes */
#include <vector>

/* Ogre includes */
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreMath.h>
#include <OgreLogManager.h>

/* my includes */
#include "plant.h"

/* Make things simpler */
typedef std::vector<Mind::Plant> vPlants;

namespace Mind
{
	class PlantCluster
	{

	public:

		PlantCluster();
		~PlantCluster();

		/* RUn once to setup. Needs ptr to scene manager, and the position
		 * to place it 
		 */
		void setup(Ogre::SceneManager *sm, int x, int y);
		/* Call every frame to update, with time since last frame */
		void update(Ogre::Real timeInterval);
		/* call before destruction to properly remove meshes */
		void remove();

		/* Flag for destruction this object  */
		bool deleteNow;

	protected:

	private:

		/* Number of plants in cluster*/
		unsigned int numPlants;

		/* Vector of plants */
		vPlants plantVector;

		/* For position */
		Ogre::SceneNode *plantClusterNode;
		Ogre::Real posX;
		Ogre::Real posY;
		Ogre::Real posZ;

		/* Radius of cluster */
		Ogre::Real radius;

		/* Milliseconds */
		Ogre::Real timeAlive; // How long has this been around?
		Ogre::Real lifeSpan; // How long should it exist for?

	};
}