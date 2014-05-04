//Miner header class-thingy-file
#ifndef MINER_H
#define MINER_H

class cPoint /*The points that will be used as an array for the miner, easier to make as a class to be more flexible osv.*/
{
	public:
		int x,y;
		bool exists, taken;
		char text[40];
		SDL_Texture *sPoint;
		SDL_Texture *sPointTaken;
		SDL_Surface* surfaceMessage;
		SDL_Texture* Message;
		SDL_Color black;
		TTF_Font *font;
	void create(int mouse_x, int mouse_y, SDL_Renderer *ren)
	{	
		sPoint = loadTexture("sGoal.bmp", ren);
		sPointTaken = loadTexture("sGoalTaken.bmp", ren);
		x = mouse_x;
		y = mouse_y;
		//number test
		black = {0, 0, 0};
		font = TTF_OpenFont("font.ttf", 32);
		surfaceMessage = TTF_RenderText_Solid(font, "Testing testing", black);
		Message = SDL_CreateTextureFromSurface(ren, surfaceMessage);
		SDL_FreeSurface(surfaceMessage);
		
		taken = false;
		exists = true;//to be drawn or not to be drawn, that is the question
	}
	void draw(SDL_Renderer *ren, int idNumber)
	{
		//numbertest
		sprintf(text,"%d",idNumber);
		surfaceMessage = TTF_RenderText_Solid(font,text, black);
		Message = SDL_CreateTextureFromSurface(ren, surfaceMessage);
		SDL_FreeSurface(surfaceMessage);
	
		if(taken == false)
			renderTexture(sPoint, ren, x, y);
		else
			renderTexture(sPointTaken, ren, x, y);
			
		renderTexture(Message, ren, x+24, y+8);
	};
};

class cMiner /*THE ALMIGHTY MINER*/
{
	public:
		int  gravity, mouse_x,mouse_y, goalx, goaly;
		int pointnum, currentPoint;
		float x, y,hspeed, vspeed;
		SDL_Texture *sMiner;
		cPoint oPoint[10];
		bool collision;
		TTF_Font *fFont;
		void create(SDL_Renderer *ren)
		{
			pointnum = 0;
			currentPoint = 1;
			
			//text test
			fFont = TTF_OpenFont("font.ttf", 24);
			
			sMiner = loadTexture("sMiner.bmp",ren);
			x = 1024/2;
			y = 0;
			goalx = x;
			goaly = y;
			vspeed = 0;
			hspeed = 0;
			gravity = 0;
			mouse_x, mouse_y = 0;
			collision = false;
		};
		
		void run(SDL_Event e, SDL_Renderer *ren)
		{
			//physics-shizzle
			x += hspeed;
			y += vspeed;
			vspeed += gravity;
			///
			//limit hspeed
			if(hspeed > 3) hspeed = 3;
			if(hspeed < -3) hspeed = -3;
			
			
			//mousey mousey, you are so lousey
			SDL_GetMouseState(&mouse_x,&mouse_y);/*This has to be inside the class to update properly, for some strange reason :S*/
			SDL_PumpEvents();
			
			while(SDL_PollEvent(&e))//poll events, this also needs to be here to update properly
			{
				if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))//when left mousebutton is pressed, set goal to mouse position
				{
					oPoint[pointnum].create(mouse_x,mouse_y,ren);
					oPoint[pointnum].x = mouse_x;
					oPoint[pointnum].y = mouse_y;
					//round them up!
					oPoint[pointnum].x = oPoint[pointnum].x-oPoint[pointnum].x%64;
					oPoint[pointnum].y = oPoint[pointnum].y-oPoint[pointnum].y%64;
					if(pointnum < 10)
						pointnum ++;
					else
					{
						pointnum = 0;
					}
				}
			}
			//move to next goal if "inside" the goal/point
			if(x>oPoint[currentPoint].x&&x < oPoint[currentPoint].x+32)
			{
				oPoint[currentPoint].taken = true;
				if(currentPoint < pointnum-1)
				{
					currentPoint ++;
				}
				else
				{
					for(int i = 0;i<pointnum;i++)
					{
						oPoint[i].taken = false;
					}
					currentPoint = 0;
				}
			}
			if(pointnum > 1)
			{
			if(x < oPoint[currentPoint].x)//move towards goal
			{
				hspeed +=0.2;
			}
			else// --||--
			{
				hspeed -=0.2;
			}
			}
			else
			hspeed = 0;
			
			////
			
		};
		
		void checkCollision(int otherx, int othery)
		{
			//Top collision
			if((y+64>othery&&x+46>otherx&&x<otherx+46&&y<othery))//
			{
				if(oPoint[currentPoint].y < y-16)//if the goal is above;jump, otherwise; float smoooooooooooothly towards the goal
				{
					vspeed = -12;
				}
				else
				{
					y = othery-68;
					vspeed = 0;
				}
			}
			else
			{
				gravity = 1;
			}
			//right 
			if((x+48+hspeed > otherx&&x+44<otherx&&y+60>othery&&y<othery+48))	
			{	
				x = otherx-48;
				hspeed = 0;
			}
			//left
			if((x+hspeed<otherx+48&&x>otherx+44&&y+60>othery&&y<othery+48))
			{		
				x = otherx+48;
				hspeed = 0;
			}
			//bottom (working) 
			if((y<othery+50&&y>othery&&x+46>otherx&&x<otherx+46))
			{	
				vspeed = 0;
				gravity = 0;
				y = othery+50;
			}
		};
		
		void draw(SDL_Renderer *ren)
		{
			for(int i = 0;i<pointnum;i++)
			{
				if(oPoint[i].exists == true)
					oPoint[i].draw(ren,i);
			}
			renderTexture(sMiner, ren, x, y);
		};

};

#endif