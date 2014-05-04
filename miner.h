//Miner header class-thingy-file
#ifndef MINER_H
#define MINER_H

class cPoint /*The points that will be used as an array for the miner, easier to make as a class to be more flexible osv.*/
{
	public:
		int x,y;
		bool exists, taken;
		char text[40];
		
		//SDL_Surface* surfaceMessage;
		//SDL_Texture* Message;
		SDL_Color black;
		TTF_Font *font;
	void create(int mouse_x, int mouse_y, SDL_Renderer *ren)
	{	
		
		x = mouse_x;
		y = mouse_y;
		//number test
		/*black = {0, 0, 0};
		font = TTF_OpenFont("font.ttf", 32);
		surfaceMessage = TTF_RenderText_Solid(font, "Testing testing", black);
		Message = SDL_CreateTextureFromSurface(ren, surfaceMessage);
		SDL_FreeSurface(surfaceMessage);*/
		
		taken = false;
		exists = true;//to be drawn or not to be drawn, that is the question
	}
	void draw(SDL_Renderer *ren, int idNumber, SDL_Texture *sPoint,SDL_Texture *sPointTaken)
	{
		//numbertest
		/*sprintf(text,"%d",idNumber);
		surfaceMessage = TTF_RenderText_Solid(font,text, black);
		Message = SDL_CreateTextureFromSurface(ren, surfaceMessage);
		SDL_FreeSurface(surfaceMessage);*/
	
		if(taken == false)
			renderTexture(sPoint, ren, x, y);
		else
			renderTexture(sPointTaken, ren, x, y);
			
		//renderTexture(Message, ren, x+24, y+8);
	};
};

class cMiner /*THE ALMIGHTY MINER*/
{
	public:
		int  gravity, mouse_x,mouse_y, goalx, goaly;
		int pointnum, currentPoint;
		float x, y,hspeed, vspeed, dx, dy;
		SDL_Texture *sMiner;
		SDL_Texture *sSelected;
		
		//points
		SDL_Texture *sPoint;
		SDL_Texture *sPointTaken;
		
		cPoint oPoint[20];
		bool collision, goalAbove, goalBelow, moveToGoal, selected, clicked;
		TTF_Font *fFont;
		void create(SDL_Renderer *ren)
		{
			pointnum = 0;
			currentPoint = 1;
			
			//text test
			fFont = TTF_OpenFont("font.ttf", 24);
			
			sMiner = loadTexture("sMiner.bmp",ren);
			sSelected = loadTexture("sSelected.bmp",ren);
			
			//points
			sPoint = loadTexture("sGoal.bmp", ren);
			sPointTaken = loadTexture("sGoalTaken.bmp", ren);
			
			x = 1024/2;
			y = 0;
			goalx = x;
			goaly = y;
			vspeed = 0;
			hspeed = 0;
			gravity = 0;
			mouse_x, mouse_y = 0;
			selected = false;
			collision = false;
			moveToGoal = false;
			clicked = false;
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
			
			
				//when left mousebutton is pressed
				if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
				{
					if(tool == 0&&clicked == false)//if the tool is 0, select the miner if the mouse is upon him! Huvaligen!
					{
						if(mouse_x > x&&mouse_x < x+64&&mouse_y > y&&mouse_y < y+64)
						{
							selected = true;
							clicked = true;
						}
						else
						{
							selected = false;
							clicked = true;
						}
					}
					if(tool == 1&&selected == true&&clicked == false)//if the tool is "goalmaker(1)", make goals
					{
						oPoint[pointnum].create(mouse_x,mouse_y,ren);
						oPoint[pointnum].x = mouse_x;
						oPoint[pointnum].y = mouse_y;
						//round them up!
						oPoint[pointnum].x = oPoint[pointnum].x-oPoint[pointnum].x%64;
						oPoint[pointnum].y = oPoint[pointnum].y-oPoint[pointnum].y%64;
						if(pointnum < 20)
							pointnum ++;
						else
						{
							pointnum = 0;
						}
						clicked = true;
					}
				}
				else
				{
					clicked = false;
				}
			
			
			//move to next goal if "inside" the goal/point (circle-collision, faster and doesn't need to be that precise)
			dx = (oPoint[currentPoint].x+32)-(x+32);
			dy = (oPoint[currentPoint].y+32)-(y+32);
			if(((dx*dx)+(dy*dy))<(16*32))
			{
				//resets both of the goal-bools, will make workaround later
				goalBelow = false;
				goalAbove = false;
				
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
			if(pointnum > 1&&moveToGoal == true)
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
			{
				hspeed = 0;
			}
			
			////
			
		};
		
		void checkCollision(int otherx, int othery)
		{
			//Top collision
			if((y+64>othery&&x+46>otherx&&x<otherx+46&&y<othery))//
			{
				if((oPoint[currentPoint].y+64 < y)&&(pointnum > 1))//if the goal is above;jump, otherwise; float smoooooooooooothly towards the goal
				{
					goalBelow = false;
					goalAbove = true;
					vspeed = -12;
				}
				else
				{
					goalAbove = false;
					goalBelow = true;
					y = othery-64;
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
		
		void checkLadder(int otherx, int othery)
		{
			//if goal is above, jump on them, otherwise jump through them
			if((y>(othery+vspeed)&&x+46>otherx&&x<otherx+46&&y-50<(othery+vspeed))&&goalAbove == true)//
			{
				vspeed = -12;
			}
			else
			{
				//vspeed = 4;
				gravity = 1;
			}
		};
		void draw(SDL_Renderer *ren)
		{
			for(int i = 0;i<pointnum;i++)
			{
				//draw lines between points
				/*if(pointnum > 1&&(oPoint[i+1].exists == true))
				{
					//between points
					SDL_RenderDrawLine(ren,oPoint[i].x+32, oPoint[i].y+32,oPoint[i+1].x+32,oPoint[i+1].y+32);
					//betwwen last point and first point
					SDL_RenderDrawLine(ren,oPoint[0].x+32, oPoint[0].y+32,oPoint[pointnum-1].x+32,oPoint[pointnum-1].y+32);
				}
				
				*/
				if(oPoint[i].exists == true)
					oPoint[i].draw(ren,i,sPoint,sPointTaken);
			}
			if(selected == true)
			{
				renderTexture(sSelected, ren, x, (y-64));
			}
			renderTexture(sMiner, ren, x, y);
		};

};

#endif