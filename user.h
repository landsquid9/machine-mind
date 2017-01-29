/*
-----------------------------------------------------------------------------
Filename:    user.h
-----------------------------------------------------------------------------

This controls the camera and controls. Actual input is captured in the main application,
but it calls functions from this object when the controls should move camera. 

*/

#pragma once

/* built in */

/* Ogre */
#include <OgreSceneManager.h>
#include <OgreNode.h>
#include <OgreSceneNode.h>
#include <OgreCamera.h>
#include <OgreRay.h>
#include <OgreLogManager.h>
#include <OgreEntity.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>

/* My Includes */

namespace Mind
{
	class User
	{

	public:

		User();
		~User();

		/* Run setup first */
		bool setup(Ogre::SceneManager* sm, Ogre::RenderWindow *rw);
		/* Then run this every frame */
		bool update(Ogre::Real time);

		/* For keyboard movement of camera */
		void forward();
		void backward();
		void left();
		void right();
		/* when key released */
		void stopForward();
		void stopBackward();
		void stopLeft();
		void stopRight();
		/* For mouse movement of camera */
		void moveCamera(Ogre::Real x, Ogre::Real y);

	protected:

	private:

		/* Raycasting for gravity */
		bool raycastFromPoint(const Ogre::Vector3 &point,
			const Ogre::Vector3 &normal,
			Ogre::Vector3 &result);
		/* This is required for above function */
		void getMeshInformation(const Ogre::MeshPtr mesh,
			size_t &vertex_count,
			Ogre::Vector3* &vertices,
			size_t &index_count,
			unsigned long* &indices,
			const Ogre::Vector3 &position,
			const Ogre::Quaternion &orient,
			const Ogre::Vector3 &scale);

		/* ptr to scene manager passed in */
		Ogre::SceneManager *sceneMgr;

		/* Camera */
		Ogre::Camera* camera;        
		Ogre::SceneNode* cameraNode;  

		/* Camera Movement */
		Ogre::Vector3 direction;
		Ogre::Real move;
		Ogre::Real rotate;

		/* Gravity */
		Ogre::RaySceneQuery* raySceneQuery;
		Ogre::Vector3 closestPoint;
		Ogre::Real gravVelocity;
		Ogre::Real gravAcceleration;
		Ogre::Real gravVelocityMax;
		Ogre::Real gravVelocityMin;

	};

}
