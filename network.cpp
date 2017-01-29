#include "network.h"

Mind::Network::Network()
	: connected(false)
{


}

Mind::Network::~Network()
{
	/* Free up memory */
	delete session;
	delete request;
	delete response;
	delete ws;
}

bool Mind::Network::setup(SharedData *s, std::mutex *m)
{
	sd = s;
	mtx = m;

	/* Server Address */
	Poco::URI uri("http://aether.mod.bz/");

	try
	{
		/* Create client session */
		session = new Poco::Net::HTTPClientSession(uri.getHost(), uri.getPort());
		/* Send websocket request */
		request = new Poco::Net::HTTPRequest(Poco::Net::HTTPRequest::HTTP_GET, "/ws", Poco::Net::HTTPRequest::HTTP_1_1);
		response = new Poco::Net::HTTPResponse();
		/* Create Websocket */

		ws = new Poco::Net::WebSocket(*session, *request, *response);
	}
	catch(Poco::Exception)
	{
		return false;
	}




	/* We need to set a receive time out */
	Poco::Timespan ts(5000);
	ws->setReceiveTimeout(ts);

	/* config message. TODO: externalise to txt file */
	payload = "{\"messageType\":\"config\","
						 "\"messageContent\":"
							"{"
								"\"device\":\"nodeMCU\","
								"\"name\":\"OGRE\","
								"\"mode\":\"receive\","
								"\"dataType\":\"Text\""
							"}}";
	/* Send config */
	ws->sendFrame(payload.data(), (int)payload.size());
	//int n = ws->receiveFrame(buffer, sizeof(buffer), flags);
	//Ogre::LogManager::getSingletonPtr()->logMessage(buffer);

	connected = true;
	return true;

}

bool Mind::Network::run()
{
	if (connected)
	{
		int n = 0;
		bool exit = false;
		while (!exit)
		{
			/* Receive */
			try
			{
				n = ws->receiveFrame(buffer, sizeof(buffer), flags);
				mtx->lock();
				try
				{
					sd->setMessage(buffer, n);
				}
				catch (std::string err)
				{
					sd->error = err;
				}
				mtx->unlock();
			}
			catch (Poco::TimeoutException)
			{

			}
			mtx->lock();
			exit = sd->quit;
			mtx->unlock();
		}
	}


	return true;
}

std::thread Mind::Network::wsThread()
{
	return std::thread([=] { run(); });
}
