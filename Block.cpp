class Block
{
public:
	Block();
	int getX() const;
	int getY() const;
	int getZ() const;
	void setCoordinate(const int xin, const int yin, const int zin);
	void setDimension(const int win, const int hin, const int lin);
	void setColor(const int rin,const int gin, const int bin);
	void draw();

private:
	int x, y, z;
	int r,g,b;
	int w, h, l;

};

Block::Block()
{
	setDimension(10, 10, 10);
}

void Block::draw()
{
	void DrawUnitCube(void)
	{
		glBegin(GL_QUADS);

		glColor3ub(0, 0, 255);

		glVertex3i(x, y, z);
		glVertex3i(x + w, y, z);
		glVertex3i(x+w, y, z+l);
		glVertex3i(x, y, z+l);

		glVertex3i(x, y+h, z);
		glVertex3i(x + w, y+h, z);
		glVertex3i(x + w, y+h, z + l);
		glVertex3i(x, y+h, z + l);

		glVertex3i(x, y+h, z+l);
		glVertex3i(x+w, y+h, z + l);
		glVertex3i(x+w, y, z + l);
		glVertex3i(x, y, z + l);

		glVertex3i(x, y + h, z);
		glVertex3i(x + w, y + h, z);
		glVertex3i(x + w, y, z);
		glVertex3i(x, y, z);

		glVertex3i(x, y, z);
		glVertex3i(x, y, z+l);
		glVertex3i(x, y+h, z+l);
		glVertex3i(x, y+h, z);

		glVertex3i(x+w, y, z);
		glVertex3i(x+w, y, z + l);
		glVertex3i(x+w, y + h, z + l);
		glVertex3i(x+w, y + h, z);

		glEnd();
	}
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
void Block::setColor(const int rin, const int gin, const int bin)
{
	r = rin;
	g = gin;
	b = bin;
}

}
