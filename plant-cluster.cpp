#include "plant-cluster.h"

Mind::PlantCluster::PlantCluster() :
	numPlants(10),
	radius(50),
	lifeSpan(10000),
	timeAlive(0),
	deleteNow(false)
{

}

Mind::PlantCluster::~PlantCluster()
{
	Ogre::LogManager::getSingletonPtr()->logMessage("deconstructing plant cluster");
}

void Mind::PlantCluster::setup(Ogre::SceneManager *sm, int x, int y)
{
	posX = x;
	posY = y;
	posZ = 0;

	//plantClusterNode = sm->getRootSceneNode()->createChildSceneNode();
	//plantClusterNode->setPosition(posX, posY, posZ);

	/* Setup the cluster. First populate vector */
	for (int i = 0; i < numPlants; i++)
	{
		plantVector.push_back(Plant());
	}

	/* Create position variables to place the plants.
	 * The first one will be placed at the centre of the cluster 
	 */

	Ogre::Real fX = posX;
	Ogre::Real fZ = posY;

	/* The first one, in the centre */
	plantVector[0].setup(sm, fX, fZ, 5.0);

	int edgePlants = numPlants - 1;

	Ogre::Real degrees = 360 / edgePlants;
	Ogre::Real radians = Ogre::Math::DegreesToRadians(degrees);

	/* Now setup each plant arund the edge */
	for (int i = 1; i < edgePlants + 1; i++)
	{
		Ogre::Real thisRadians = radians * i;

		fX = posX + radius * Ogre::Math::Cos(thisRadians);
		fZ = posZ + radius * Ogre::Math::Sin(thisRadians);

		Ogre::Real ran = Ogre::Math::RangeRandom(1.0, 5.0);

		plantVector[i].setup(sm, fX, fZ, ran);
		
	}

}

void Mind::PlantCluster::update(Ogre::Real timeInterval)
{
	timeAlive += timeInterval * 1000;
	if (timeAlive > lifeSpan)
	{
		deleteNow = true;
	}

	for (int i = 0; i < numPlants; i++)
	{
		plantVector[i].update(timeInterval);
	}
}

void Mind::PlantCluster::remove()
{
	Ogre::LogManager::getSingletonPtr()->logMessage("removing plants");
	for (int i = 0; i < numPlants; i++)
	{
		plantVector[i].remove();
	}
}