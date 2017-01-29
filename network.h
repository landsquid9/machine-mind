/*
-----------------------------------------------------------------------------
Filename:    network.h
-----------------------------------------------------------------------------

This handles the web connection. It connects to the Aether IOT platform
and provides an appropriate config message. It then listens for messages and stores 
xy coordinates it receives in the shared data object for the Ogre application to
access.

*/

#pragma once

/* My Includes */
#include "shared-data.h"

/* Built in includes */
#include <thread>
#include <mutex>

/* Poco includes */
#include <Poco/Net/Websocket.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/NullStream.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <Poco/Net/NetException.h>

namespace Mind
{

	class Network 
	{

	public:

		Network();
		~Network();

		/* Runs repeatedly */
		bool run();

		/* call setup first. Give it a pointer to a shared data obj
		 * and a mutex
		 */
		bool setup(SharedData *s, std::mutex *m);

		/* Returns a thread to run */
		std::thread wsThread();


	private:


		/* Various object needed to setup websockets and send and receive data with */
		Poco::Net::HTTPClientSession *session; 
		Poco::Net::HTTPRequest *request; 
		Poco::Net::HTTPResponse *response;
		Poco::Net::WebSocket *ws;

		/* A string to be sent via websockets */
		std::string payload;
		
		/* A buffer to hold a message from the server */
		char buffer[1024];
		/* Flags to be returned from server. It's required,
		 * but we don't do anything with it
		 */
		int flags;

		/* Pont to an object that stores data needed between threads */
		SharedData *sd;

		/* Pointer to mutex for threads */
		std::mutex *mtx;

		/* Did we manage to connect? */
		bool connected;

	};
}