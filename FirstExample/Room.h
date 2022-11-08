#pragma once
class Room
{
private:
	int row;
	int col;
	int height;
	int width;
	int centerX;
	int centerY;
public:
	Room();
	Room(int r, int c, int h, int w);
	~Room();
	int getRow() { return row; }
	int getCol() { return col; }
	int getHeight() { return height; }
	int getWidth() { return width; }
	int getCenterX() { return centerX; }
	int getCenterY() { return centerY; }
};

