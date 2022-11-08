#include "Room.h"



Room::Room()
{
}

Room::Room(int r, int c, int h, int w)
{
	row = r;
	col = c;
	height = h;
	width = w;
	centerX = col + width / 2;		//refers to column
	centerY = row + height / 2;	//refers to row


}


Room::~Room()
{
}
