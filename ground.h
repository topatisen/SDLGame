//ground class-header thingy
#ifndef GROUND_H
#define GROUND_H
class cGround
{
	public:
		int x,y, mouse_x, mouse_y;
		SDL_Texture *sGround;
	
		void create(SDL_Renderer *ren, int startx, int starty)
		{
			sGround = loadTexture("sGround.bmp",ren);
			x = startx;
			y = starty;
		};
		
		void run()//only exists so that it can be removed when clicking right button
		{
			SDL_GetMouseState(&mouse_x,&mouse_y);
			if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(3))
			{
				if(mouse_x > x&&mouse_x < x+64&&mouse_y > y&&mouse_y < y+64)
				{
					x = 1000;
					y = 1000;
				}
			}
		};
		void draw(SDL_Renderer *ren)
		{
			renderTexture(sGround, ren, x, y);
		};
};

class cCreateGround
{
	public:
	cGround oGround[1000];
	int groundnum, groundx, groundy;
	
	
	void create(SDL_Renderer *ren)
	{
		groundx = 0;
		groundy = 384;
		groundnum = 0;
		oGround[groundnum].create(ren,groundx, groundy);
	}
	void makeGround(SDL_Renderer *ren)
	{
		
		for(int i = 0;i<groundnum;i++)//make ground removable
		{
			oGround[i].run();
		}
		if(groundnum < 999)
		{
			oGround[groundnum].create(ren,groundx, groundy);
			if(groundx < 1024)
			{
				groundx +=64;
			}
			else
			{
				groundy +=64;
				groundx = 0;
			}
			groundnum ++;
		}
	};
	void draw(SDL_Renderer *ren)
	{
		for(int i = 0;i<groundnum;i++)
			{
				oGround[i].draw(ren);
			}
	}
};
#endif