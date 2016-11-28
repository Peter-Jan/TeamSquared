#ifndef MATERIAL_BLOCK
#define MATERIAL_BLOCK

#include "Block.h"
#include <string>

class materialBlock:public Block
{
public:
	materialBlock();
	materialBlock(int roomsize, int x, int y, int z);
	~materialBlock();
	void initialize();
	void setImage(int imageX,int imageY);
	void setColor(int matR, int matG, int matB);
	void setParam(int health, int quantity);
	void setName(std::string inStr);
	void setXYZ(double x, double y, double z);
	int getImx() const;
	int getImy() const;
	std::string getName() const;
protected:
	int imageX, imageY;
	int healthParam, quantParam,health;
	std::string name;
};





#endif // !MATERIAL_BLOCK

