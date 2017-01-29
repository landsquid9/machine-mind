#include "landscape.h"

Mind::Landscape::Landscape()
{

}

Mind::Landscape::~Landscape()
{

}

bool Mind::Landscape::setup(Ogre::SceneManager& sm)
{
	groundEntity = sm.createEntity("landscape.mesh");

	groundNode = sm.getRootSceneNode()->createChildSceneNode();

	groundNode->attachObject(groundEntity);
	groundNode->setPosition(0, -30, 0);
	groundNode->setScale(50, 50, 50);

	/* Add basic Material */
	Ogre::MaterialManager& materialManager = Ogre::MaterialManager::getSingleton();
	Ogre::MaterialPtr basicMaterial = materialManager.create("Basic-Material", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::Technique* firstTechnique = basicMaterial->getTechnique(0);
	Ogre::Pass* firstPass = firstTechnique->getPass(0);
	//firstPass->setLightingEnabled(false);

	//groundEntity->setMaterialName("Examples/Rockwall");
	//groundEntity->setMaterialName("Basic-Material");


	return true;
}

