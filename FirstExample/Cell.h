#pragma once
#include <vector>
class Cell
{
private:
	int row;
	int col;
	Cell* parent;
	//vector<Cell*> outgoing;
	int g;
	int h;
	int color;
public:
	Cell();
	Cell(int r, int c);
	Cell(int r, int c, Cell * p);
	~Cell();
	int getRow() { return row; }
	int getCol() { return col; }
	int getG() { return g; }
	int getH() { return h; }
	int getF() 
	{ 
		//printf("g:%d , h:%d\n",g,h);	//for slow run , allow this print
		return g + h; }

	void setG(int newG) { g=newG; }
	int getColor() { return color;}
	void setColor(int newColor) { color = newColor; }
	void setH(int newH) {  h=newH; }
	Cell* getParent() { return parent; }

};

