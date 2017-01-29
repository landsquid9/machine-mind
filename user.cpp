#include "user.h"

Mind::User::User()
	:move(100),
	rotate(0.07),
	gravVelocity(0.0),
	gravVelocityMax(20.0),
	gravVelocityMin(-20.0),
	gravAcceleration(0.1)
{

}

Mind::User::~User()
{

}

bool Mind::User::setup(Ogre::SceneManager *sm, Ogre::RenderWindow * rw)
{
	sceneMgr = sm;
	camera = sceneMgr->createCamera("MainCam");
	cameraNode = sceneMgr->getRootSceneNode()->createChildSceneNode(
		"CameraNode1", Ogre::Vector3(0, 100, 800));
	cameraNode->attachObject(camera);

	camera->setNearClipDistance(5);

	Ogre::Viewport* vp = rw->addViewport(camera);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

	camera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) /
		Ogre::Real(vp->getActualHeight()));

	Ogre::Light* light = sceneMgr->createLight("MainLight");
	cameraNode->attachObject(light);

	raySceneQuery = sceneMgr->createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
	if (NULL == raySceneQuery)
	{
		Ogre::LogManager::getSingletonPtr()->logMessage("Failed to create Ogre::RaySceneQuery instance");
		return (false);
	}
	raySceneQuery->setSortByDistance(true);

	return true;
}

bool Mind::User::update(Ogre::Real time)
{
	cameraNode->translate(direction * time, Ogre::Node::TS_LOCAL);
	if (cameraNode->getPosition().x > 1500)
	{
		cameraNode->setPosition(1500,
			cameraNode->getPosition().y,
			cameraNode->getPosition().z);
	}
	else if (cameraNode->getPosition().x < -1500)
	{
		cameraNode->setPosition(-1500,
			cameraNode->getPosition().y,
			cameraNode->getPosition().z);
	}

	if (cameraNode->getPosition().z > 1500)
	{
		cameraNode->setPosition(cameraNode->getPosition().x,
			cameraNode->getPosition().y,
			1500);
	}
	else if (cameraNode->getPosition().z < -1500)
	{
		cameraNode->setPosition(cameraNode->getPosition().x,
			cameraNode->getPosition().y,
			-1500);
	}

	/* gravity */
	// create the ray scene query object

	raySceneQuery = sceneMgr->createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
	if (NULL == raySceneQuery)
	{
		Ogre::LogManager::getSingletonPtr()->logMessage("Failed to create Ogre::RaySceneQuery instance");
		return (false);
	}
	raySceneQuery->setSortByDistance(true);

	if (raycastFromPoint(cameraNode->getPosition(), Ogre::Vector3(0, -1, 0), closestPoint))
	{
		if (closestPoint.y < cameraNode->getPosition().y - 50)
		{
			if (gravVelocity > gravVelocityMin)
			{
				gravVelocity -= gravAcceleration;
			}

		}
		else if (closestPoint.y > cameraNode->getPosition().y - 20)
		{
			if (gravVelocity < gravVelocityMax)
			{
				gravVelocity += gravAcceleration * 2;
			}

		}
		else
		{
			gravVelocity = 0.0;
		}
	}
	else
	{
		if (gravVelocity < gravVelocityMax)
		{
			gravVelocity += gravAcceleration * 2;
		}
	}

	cameraNode->translate(Ogre::Vector3(0, gravVelocity, 0));

	return true;
}

void Mind::User::moveCamera(Ogre::Real x, Ogre::Real y)
{
	cameraNode->yaw(Ogre::Degree(-rotate * x), Ogre::Node::TS_WORLD);
	camera->pitch(Ogre::Degree(-rotate * y));

}


void Mind::User::forward()
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** f1 ***");
	direction.z = -move;

}

void Mind::User::backward()
{
	direction.z = move;
}

void Mind::User::left()
{
	direction.x = -move;
}

void Mind::User::right()
{
	direction.x = move;
}

void Mind::User::stopForward()
{
	direction.z = 0;
}

void Mind::User::stopBackward()
{
	direction.z = 0;
}

void Mind::User::stopLeft()
{
	direction.x = 0;
}

void Mind::User::stopRight()
{
	direction.x = 0;
}

// raycast from a point in to the scene.
// returns success or failure.
// on success the point is returned in the result.
bool Mind::User::raycastFromPoint(const Ogre::Vector3 &point,
	const Ogre::Vector3 &normal,
	Ogre::Vector3 &result)
{
	// create the ray to test
	Ogre::Ray ray(Ogre::Vector3(point.x, point.y, point.z),
		Ogre::Vector3(normal.x, normal.y, normal.z));

	// check we are initialised
	if (raySceneQuery != NULL)
	{
		// create a query object
		raySceneQuery->setRay(ray);

		// execute the query, returns a vector of hits
		if (raySceneQuery->execute().size() <= 0)
		{
			// raycast did not hit an objects bounding box
			return (false);
		}
	}
	else
	{
		Ogre::LogManager::getSingletonPtr()->logMessage("Cannot raycast without RaySceneQuery instance");
		return (false);
	}

	// at this point we have raycast to a series of different objects bounding boxes.
	// we need to test these different objects to see which is the first polygon hit.
	// there are some minor optimizations (distance based) that mean we wont have to
	// check all of the objects most of the time, but the worst case scenario is that
	// we need to test every triangle of every object.
	Ogre::Real closest_distance = -1.0f;
	Ogre::Vector3 closest_result;
	Ogre::RaySceneQueryResult &query_result = raySceneQuery->getLastResults();
	for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
	{
		// stop checking if we have found a raycast hit that is closer
		// than all remaining entities
		if ((closest_distance >= 0.0f) &&
			(closest_distance < query_result[qr_idx].distance))
		{
			break;
		}

		// only check this result if its a hit against an entity
		if ((query_result[qr_idx].movable != NULL) &&
			(query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
		{
			// get the entity to check
			Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);

			// mesh data to retrieve         
			size_t vertex_count;
			size_t index_count;
			Ogre::Vector3 *vertices;
			unsigned long *indices;

			// get the mesh information
			getMeshInformation(pentity->getMesh(), vertex_count, vertices, index_count, indices,
				pentity->getParentNode()->getPosition(),
				pentity->getParentNode()->getOrientation(),
				pentity->getParentNode()->_getDerivedScale());

			// test for hitting individual triangles on the mesh
			bool new_closest_found = false;
			for (int i = 0; i < static_cast<int>(index_count); i += 3)
			{
				// check for a hit against this triangle
				std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, vertices[indices[i]],
					vertices[indices[i + 1]], vertices[indices[i + 2]], true, false);

				// if it was a hit check if its the closest
				if (hit.first)
				{
					if ((closest_distance < 0.0f) ||
						(hit.second < closest_distance))
					{
						// this is the closest so far, save it off
						closest_distance = hit.second;
						new_closest_found = true;
					}
				}
			}

			// free the verticies and indicies memory
			delete[] vertices;
			delete[] indices;

			// if we found a new closest raycast for this object, update the
			// closest_result before moving on to the next object.
			if (new_closest_found)
			{
				closest_result = ray.getPoint(closest_distance);
			}
		}
	}

	// return the result
	if (closest_distance >= 0.0f)
	{
		// raycast success
		result = closest_result;
		return (true);
	}
	else
	{
		// raycast failed
		return (false);
	}
}

void Mind::User::getMeshInformation(const Ogre::MeshPtr mesh, 
						size_t & vertex_count, 
						Ogre::Vector3 *& vertices, 
						size_t & index_count, 
						unsigned long *& indices, 
						const Ogre::Vector3 & position, 
						const Ogre::Quaternion & orient, 
						const Ogre::Vector3 & scale)
{

	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	vertex_count = index_count = 0;

	// Calculate how many vertices and indices we're going to need
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		// We only need to add the shared vertices once
		if (submesh->useSharedVertices)
		{
			if (!added_shared)
			{
				vertex_count += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else
		{
			vertex_count += submesh->vertexData->vertexCount;
		}

		// Add the indices
		index_count += submesh->indexData->indexCount;
	}


	// Allocate space for the vertices and indices
	vertices = new Ogre::Vector3[vertex_count];
	indices = new unsigned long[index_count];

	added_shared = false;

	// Run through the submeshes again, adding the data into the arrays
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

		if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
		{
			if (submesh->useSharedVertices)
			{
				added_shared = true;
				shared_offset = current_offset;
			}

			const Ogre::VertexElement* posElem =
				vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

			Ogre::HardwareVertexBufferSharedPtr vbuf =
				vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

			unsigned char* vertex =
				static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

			// There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
			//  as second argument. So make it float, to avoid trouble when Ogre::Real will
			//  be comiled/typedefed as double:
			//      Ogre::Real* pReal;
			float* pReal;

			for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
			{
				posElem->baseVertexPointerToElement(vertex, &pReal);

				Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

				vertices[current_offset + j] = (orient * (pt * scale)) + position;
			}

			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}


		Ogre::IndexData* index_data = submesh->indexData;
		size_t numTris = index_data->indexCount / 3;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
		if (ibuf.isNull()) continue; // need to check if index buffer is valid (which will be not if the mesh doesn't have triangles like a pointcloud)

		bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

		unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


		size_t offset = (submesh->useSharedVertices) ? shared_offset : current_offset;
		size_t index_start = index_data->indexStart;
		size_t last_index = numTris * 3 + index_start;

		if (use32bitindexes)
			for (size_t k = index_start; k < last_index; ++k)
			{
				indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
			}

		else
			for (size_t k = index_start; k < last_index; ++k)
			{
				indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
					static_cast<unsigned long>(offset);
			}

		ibuf->unlock();
		current_offset = next_offset;
	}
}