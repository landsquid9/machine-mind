#include "shared-data.h"

Mind::SharedData::SharedData() :
	quit(false),
	newData(false),
	error("")
{

}

void Mind::SharedData::setMessage(char* c, unsigned int sz)
{
	
	/* Transfer message */
	if (sz > 1024)
	{
		sz = 1024;
	}
	for (int i = 0; i < 1024; i++)
	{
		if (i < sz)
		{
			message[i] = c[i];
		}
		else
		{
			message[i] = ' ';
		}
	}

	/* Parse message into coordinates */
	/* Max length is 10 (p, 4 digit x pos, comma, 4 digit y pos) */

	bool valid = true;

	/* For temporarily holding positions */
	char x[5] = { '\0', '\0', '\0', '\0', '\0' };
	char y[5] = { '\0', '\0', '\0', '\0', '\0' };

	/* keep track of our pos in char array */
	unsigned int cPos = 0;

	if (sz <= 10 && sz >= 4)
	{


		/* Check First char*/
		if (message[cPos] == 'p')
		{
			cPos++;

			/* Check chars 1, 2, 3, 4 */
			for (; cPos < 5; cPos++)
			{
				/* Is digit? */
				if (isDigit(message[cPos]))
				{
					/* add to x char array */
					x[cPos - 1] = message[cPos];
				}
				/* Is comma */
				else if (message[cPos] == ',')
				{
					break;
				}
				else 
				{
					valid = false; // not number or comma, invalid
					throw std::string("First number contains non-digit/comma char");
					break;
				}
			}

			/* are we still valid? */
			if (valid)
			{
				/* If cPos got to "5" we need to check if next char is comma */
				if (cPos == 5)
				{

					if (message[cPos] != ',')
					{
						valid = false; // First number is not followed by comma. Invalid.
						throw std::string("First number is not followed by comma");
					}
				}
				else
				{
					/* Else if it is not 5, we already received a comma. Everything from now on
					 * should be a number. We store the next four chars in y if they are numbers, otherwise
					 * invalid */
					cPos++;
					for (int i = 0; i < 4; i++)
					{
						if (cPos >= sz)
						{
							break; // no morechars left
						}

						if (isDigit(message[cPos]))
						{
							y[i] = message[cPos];
						}
						else
						{
							valid = false; // non-digit after comma
							throw std::string("Second number contains non-digit");
							break;
						}

						cPos++;
					}
				}
			}
		}
		else
		{
			valid = false; // Does not begin with p
			throw std::string("does not begin with p");
		}
		
	}
	else if (sz < 4)
	{
		valid = false; // too few characters
		throw std::string("too few characters");
	}
	else
	{
		valid = false;//too many characters
		throw std::string("too many characters");
	}

	/* if we are still valid */
	if (valid)
	{
		/* we need to store x and y as numbers */
		/* Stringstreams for converting textual x and y coords to numbers */
		std::stringstream ssX;
		std::stringstream ssY;
		ssX << x;
		ssX >> position.x;
		ssY << y;
		ssY >> position.y;

		newData = true;
	}
	else
	{
		position.x = 0;
		position.y = 0;
		newData = false;
	}

}

char* Mind::SharedData::getMessage()
{
	newData = false;
	return message;
}

Mind::Coords Mind::SharedData::getPosition()
{
	newData = false;
	return position;
}

bool Mind::SharedData::isDigit(char c)
{
	return ('0' <= c && c <= '9');
}