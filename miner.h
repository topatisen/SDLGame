//Miner header class-thingy-file
#ifndef MINER_H
#define MINER_H


class cPoint /*The points that will be used as an array for the miner, easier to make as a class to be more flexible osv.*/
{
	public:
		int x,y;
		bool exists;
		SDL_Texture *sPoint;
	void create(int mouse_x, int mouse_y)
	{
		x = mouse_x;
		y = mouse_y;
	}

};

class cMiner /*THE ALMIGHTY MINER*/
{
	public:
		int  gravity, mouse_x,mouse_y, goalx, goaly;
		float x, y,hspeed, vspeed;
		SDL_Texture *sMiner;
		SDL_Texture *sGoal;//just for testing, will be in other class later
		bool moveToGoal;
		void create(SDL_Renderer *ren)
		{
			sMiner = loadTexture("sMiner.bmp",ren);
			sGoal = loadTexture("sGoal.bmp",ren);
			x = 1024/2;
			y = 768/2;
			goalx = x;
			goaly = y;
			vspeed = 0;
			hspeed = 0;
			gravity = 0;
			mouse_x, mouse_y = 0;
			moveToGoal = false;
		};
		void run(SDL_Event e)
		{
			//physics-shizzle
			x += hspeed;
			y += vspeed;
			vspeed += gravity;
			///
			//limit hspeed
			if(hspeed > 2) hspeed = 2;
			if(hspeed < -2) hspeed = -2;
			
			
			//mousey mousey, you are so lousey
			SDL_GetMouseState(&mouse_x,&mouse_y);/*This has to be inside the class to update properly, for some strange reason :S*/
			SDL_PumpEvents();
			
			while(SDL_PollEvent(&e))//poll events, this also needs to be here to update properly
			{
				if(e.type == SDL_MOUSEBUTTONDOWN)//when mousebutton is pressed, set goal to mouse position
				{
					goalx = mouse_x;
					goaly = mouse_y;
					//round them up!
					goalx = goalx-goalx%64;
					goaly = goaly-goaly%64;
				}
			}
			if(x < goalx)//move towards goal
			{
				hspeed +=0.2;
			}
			else// --||--
			{
				hspeed -=0.2;
			}
			//Jump up and down!
			if(y < 768-64)
			{
				gravity = 1;
			}
			else
			{
				gravity = 0;
				
				if(goaly < y-16)
				{
					vspeed = -12;
				}
				else
				{
					vspeed = -0.1;
				}
			}
			////
		};
		void draw(SDL_Renderer *ren)
		{
			renderTexture(sMiner, ren, x, y);
			renderTexture(sGoal, ren, goalx, goaly);
		};

};

#endif