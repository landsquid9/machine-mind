/*
-----------------------------------------------------------------------------
Filename:    shared-data.h
-----------------------------------------------------------------------------

This holds data required by both threads. It should only be accessed using a 
mutex.

*/

#pragma once

/* BUilt in includes */
#include <sstream>
#include <string>

namespace Mind
{
	/* A struct repesenting a 2D position */
	struct Coords
	{
		int x = 0;
		int y = 0;
	};

	class SharedData
	{

	public:

		SharedData();

		/* Set message. Format should be as follows, for it
		   to be interpreted as coordinates:
		   "p10,40"
		   the numbers can be anywhere between 0 and 9999. p indicates positonal data. Everything up until
		   the first comma will determine x coord. Next four digits determine 
		   y. TODO: find out if keep data to 8 characters is efficient. If so, reduce range to
		   0 - 999. */
		void setMessage(char* c, unsigned int sz);
		char* getMessage();
		Coords getPosition();

		/* flags */
		bool quit;   
		bool newData;
		
		/* Error message to be set by poco but to be displayed by ogre */
		std::string error;

	protected:

	private:
		/* Check if char is digit */
		bool isDigit(char c);

		/* message from server */
		char message[1024] = { 0 };

		/* position received from server */
		Coords position;



	};
}

