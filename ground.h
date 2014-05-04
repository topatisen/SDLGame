//ground classes-header thingy
#ifndef GROUND_H
#define GROUND_H

class cStepladder//simple ladder, "jumpthroughable"
{
	public:
		int x,y, mouse_x, mouse_y;
		SDL_Texture *sStepladder;
	
		void create(SDL_Renderer *ren, int startx, int starty)
		{
			sStepladder = loadTexture("sStepladder.bmp",ren);
			x = startx;
			y = starty;
		};
		
		void run()
		{
			SDL_GetMouseState(&mouse_x,&mouse_y);
			if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(3))//removed when clicking right button
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
			renderTexture(sStepladder, ren, x, y);
		};
};

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
		
		void run()//removed with right-click
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
	cStepladder oStepladder[1000];
	int groundnum, laddernum, groundx, groundy, mouse_x, mouse_y;
	
	
	void create(SDL_Renderer *ren)
	{
		groundx = 0;
		groundy = 384;
		groundnum = 0;
		laddernum = 0;
		oGround[groundnum].create(ren,groundx, groundy);
	}
	void makeGround(SDL_Renderer *ren)//change name later
	{
		SDL_GetMouseState(&mouse_x,&mouse_y);
		if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(2))//mittenmusknappen/musmittenknappen/middlemousebutton places stepladder
		{
			oStepladder[laddernum].create(ren,mouse_x, mouse_y);
			oStepladder[laddernum].x = oStepladder[laddernum].x-oStepladder[laddernum].x%64;//snap
			oStepladder[laddernum].y = oStepladder[laddernum].y-oStepladder[laddernum].y%64;//snap
			laddernum ++;
		}
		
		for(int i = 0;i<laddernum;i++)//make ground removable
		{
			oStepladder[i].run();
		}
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
		for(int i = 0;i<laddernum;i++)
		{
			oStepladder[i].draw(ren);
		}
	}
};
#endif