class Block
{
public:
	Block();
	int getX() const;
	int getY() const;
	int getZ() const;
	void setCoordinate(const int xin, const int yin, const int zin);
	void setDimension(const int win, const int hin, const int lin);
	void setColor(const int cin);
	void draw();

private:
	int x, y, z;
	int color;
	int w, h, l;

};

Block::Block()
{
	setDimension(10, 10, 10);
}

void Block::draw()
{

}

void Block::setDimension(const int win, const int hin, const int lin)
{
	w = win;
	h = hin;
	l = lin;
}
int Block::getX() const
{
	return x;
}
int Block::getY() const
{
	return y;
}
int Block::getZ() const
{
	return z;
}
void Block::setCoordinate(const int xin, const int yin,const int zin)
{
	x = xin;
	y = yin;
	z = zin;
}
void Block::setColor(const int cin)
{
	color = cin;
}

}
