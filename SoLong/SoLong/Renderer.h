#pragma once

class CRenderer
{
public:
	int Render(int[80][50]);
	int ChangeTrapRenderMask(void);
	CRenderer(void);
	~CRenderer(void);
private:
	bool RenderingMode;
	int LastRendered[80][50];
	int RenderTrapsNearSnake(int [80][50], int x, int y);
};
