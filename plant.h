/*
-----------------------------------------------------------------------------
Filename:    plant.h
-----------------------------------------------------------------------------

This is represents a plant in the 3D world.

*/

#pragma once

/* Built in includes */

/* Ogre */
#include <OgreEntity.h>
#include <OgreNode.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreLogManager.h>
#include <OgreRay.h>
#include <OgreSceneQuery.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>


/* My includes */

namespace Mind
{
	class Plant
	{

	public:

		Plant();
		~Plant();

		/* call once to setup */
		bool setup(Ogre::SceneManager * sm, Ogre::Real x, Ogre::Real z, Ogre::Real sc);

		/* call every frame to update */
		void update(Ogre::Real timeInterval);

		/* Call before deleting to correctly remove meshes */
		void remove();

	protected:

	private:

		/* Used for polygon level raycasting in order to place plant
		 * on the landscape 
		 */
		bool raycastFromPoint(const Ogre::Vector3 &point,
			const Ogre::Vector3 &normal,
			Ogre::Vector3 &result);
		/* Required for raycasting fn */
		void getMeshInformation(const Ogre::MeshPtr mesh,
			size_t &vertex_count,
			Ogre::Vector3* &vertices,
			size_t &index_count,
			unsigned long* &indices,
			const Ogre::Vector3 &position,
			const Ogre::Quaternion &orient,
			const Ogre::Vector3 &scale);

		/* The mesh */
		Ogre::Entity *plantEntity;
		Ogre::SceneNode   *plantNode;

		/* Pointer to scene manager */
		Ogre::SceneManager *sceneManager;

		/* To control scale of plants */
		Ogre::Real scale;
		Ogre::Real xScale;
		Ogre::Real yScale;
		Ogre::Real zScale;

		/* to control position of plants */
		Ogre::Real posX;
		Ogre::Real posY;
		Ogre::Real posZ;

		/* For raycasting */
		Ogre::RaySceneQuery* raySceneQuery;
		Ogre::Vector3 closestPoint;

		/* How quickly to scale */
		Ogre::Real scaleSpeed;

	};
}
