#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Laby.hpp"
#include "../mathematics.hpp"
#include "../primitives.hpp"

#define LABYRINTH_GLOW
#define NOT_VISITED 0
#define VISITED		1
#define LEFT_WALL	2
#define RIGHT_WALL	4
#define UP_WALL		8
#define DOWN_WALL	16

void Laby::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.98f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos < fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos > fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

#ifndef LABYRINTH_GLOW
	renderScene(pos, alpha);
#else
	glViewport(0, 0, 512, 512);
	renderScene(pos, alpha);
	myEnable(MY_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, dmsGetTexture("render0")->getID());
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 512, 512, 0);
	glBindTexture(GL_TEXTURE_2D, dmsGetTexture("render2")->getID()); //alkup. talteen
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 512, 512, 0);

	myDisable(MY_DEPTH_TEST);
//	myDisable(MY_DEPTH_MASK); //<--- miksi tästä tulee kummallinen bugitus?
	dmsPerspective2D(1,1);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	Texture *render1;
	Texture *render2;
	
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
//	float blendalpha = 0.45f;
//	const float radiusmod = 0.001f;
//	const int count = 10;

	float blendalpha = 0.45f;
	const float radiusmod = 0.001f;
	const int count = 10;
	for (int i=0;i<count;i++)
	{
	    const float kohta = pos + i / (float)count;
        const float radius = i * radiusmod;
        //piirrettävän tekstuurin paikan muutokset
        const float xdisplace = radius*(float)sin(kohta*4*3.141592f);
        const float ydisplace = radius*(float)cos(kohta*4*3.141592f);
        
        //zoomataan hieman, muuten näyttää rumalta
        const float zoom = 0.005f;
        
        if (!(i%2))
        {
                render1 = dmsGetTexture("render0");
                render2 = dmsGetTexture("render1");
        }
        else
        {
                render1 = dmsGetTexture("render1");
                render2 = dmsGetTexture("render0");
        }

        glBindTexture(GL_TEXTURE_2D, render1->getID());
        glBegin(GL_QUADS);
        glColor4f(1, 1, 1, blendalpha);
        glTexCoord2f(zoom, zoom);
        glVertex2f(xdisplace, ydisplace);
        glTexCoord2f(1-zoom, zoom);
        glVertex2f(1+xdisplace, ydisplace);
        glTexCoord2f(1-zoom, 1-zoom);
        glVertex2f(1+xdisplace, 1+ydisplace); //, 1);
        glTexCoord2f(zoom, 1-zoom);
        glVertex2f(xdisplace, 1+ydisplace);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, render2->getID());
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 512, 512, 0);
	}
	myDisable(MY_BLEND);
	glViewport(0, 0, dmsGetWindowWidth(), dmsGetWindowHeight());
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D, dmsGetTexture("render2")->getID());
    glBegin(GL_QUADS);
    glColor4f(1,1,1,1);
    glTexCoord2f(0, 0);
    glVertex2f(0, 0);
    glTexCoord2f(1, 0);
    glVertex2f(1, 0);
    glTexCoord2f(1, 1);
    glVertex2f(1, 1);
    glTexCoord2f(0, 1);
    glVertex2f(0, 1);
    glEnd();
	myEnable(MY_BLEND);

    glColor4f(1,1,1,1);
    glBindTexture(GL_TEXTURE_2D, render2->getID());
    glBegin(GL_QUADS);
    glColor4f(1,1,1,1);
    glTexCoord2f(0, 0);
    glVertex2f(0, 0);
    glTexCoord2f(1, 0);
    glVertex2f(1, 0);
    glTexCoord2f(1, 1);
    glVertex2f(1, 1);
    glTexCoord2f(0, 1);
    glVertex2f(0, 1);
    glEnd();
	dmsPerspective3D();
#endif 
}

void Laby::renderScene(float pos, float alpha)
{
	int i;
	myDisable(MY_TEXTURE_2D);
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	myEnable(MY_LINE_SMOOTH);
	myEnable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK);

//	glLoadIdentity();
//	glTranslatef(0, 0, -5.0f+pos*3);
//	glRotatef(pos*450, 1, -0.5f, 0.3f);

	Vector campos = kamera->getValue(pos*0.96f);
	Vector viewpos = viewpoint->getValue(pos);
	Vector rot = rotationtrack->getValue(pos);

	const float rl = pos*18;
	const float a = (float)sin(rl)*0.2f;
	const float b = (float)cos(rl)*0.2f;
	glLoadIdentity();
//	gluLookAt(campos.x, campos.y, campos.z, viewpos.x, viewpos.y, viewpos.z, b, 1-b, 0);	
	gluLookAt(campos.x, campos.y, campos.z, viewpos.x, viewpos.y, viewpos.z, rot.x, rot.y, rot.z);	

//	Primitives::flatCube(0.995f);	

	const int edgesdrawn = (int)edgecount;

	float fogcol[] = {0, 0, 0, 1};

	glFogi(GL_FOG_MODE, GL_LINEAR); //EXP, EXP2, LINEAR
	glFogfv(GL_FOG_COLOR, fogcol);
	glFogf(GL_FOG_DENSITY, 0.4f+0.4f*(float)cos(pos*10));
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 0);
	glFogf(GL_FOG_END, 5);
	myEnable(MY_FOG);

	const float pacfade = Mathematics::calcSaturate(pos, 0, 0.98f, 16);
	for (i=0;i<pacmancount;i++)
	{
		float t1 = pos*pacmen[i].pointcount;
		int index = (int)t1;
		float frac = t1 - (int)t1;

		Vector v1, v2;
		v1 = pacmen[i].points[index];
		if (i<pacmancount-1)
			v2 = pacmen[i].points[index+1];
		else
			v2 = pacmen[i].points[index];

		Vector pacpos = v1*(1-frac) + v2*frac;

		float paccol[] = {1, 1, 0.0f, alpha*pacfade};
		glColor4fv((float *)&paccol);
		const float pacradius = 0.1f;
		const int pacsteps = 24;

		const float suu = 1.2f+1.2f*(float)cos(pos*350);
		const float suusuunta = pacmen[i].suu[index];
		const float pacstep = (2*3.141592f-suu) / pacsteps;

		float pacalpha = 0;
		glBegin(GL_TRIANGLE_STRIP);
		for (int j=0;j<pacsteps;j++)
		{
			glVertex3fv((float *)&pacpos);
			Vector p = pacpos + Vector((float)sin(pacalpha+suusuunta), 0, (float)cos(pacalpha+suusuunta))*pacradius;
			glVertex3fv((float *)&p);
			pacalpha += pacstep;
		}
		glEnd();
	}

	float col1[] = { 0.3f, 0.35f, 0.9f, alpha*0.5f};
	float col2[] = { 0, 0, 0, alpha*0.0f};
//	float col2[] = { 0.3f, 0.35f, 0.9f, alpha*0.5f*Mathematics::calcPosFloat(pos, 0.4f, 0.5f)};
	glLineWidth(2);

	glBegin(GL_LINES);
	for (i=0;i<edgesdrawn;i++)
	{
		//normaali kohta on x = edges[i].v.x1, y = edges[i].v1.y
		int index = edgeorder[i];

		Vector y1 = Vector(edges[index].v1.x, 0, edges[index].v1.y);
		Vector y2 = Vector(edges[index].v2.x, 0, edges[index].v2.y);

		glColor4fv((float *)&col1);
		glVertex3fv((float *)&y1);
		glVertex3fv((float *)&y2);

		glColor4fv((float *)&col2);
//		glVertex3fv((float *)&a1);
//		glVertex3fv((float *)&a2);
	}
	glEnd();
	myEnable(MY_DEPTH_TEST);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float col3[] = { 0.5f, 0.55f, 0.9f, alpha*0.2f};
	float col4[] = { 0, 0, 0, alpha*0.0f};
	glBegin(GL_LINES);
	for (i=0;i<edgesdrawn;i++)
	{
		//normaali kohta on x = edges[i].v.x1, y = edges[i].v1.y
		int index = edgeorder[i];

		const float h = 0.05f;
		Vector y1 = Vector(edges[index].v1.x, 0, edges[index].v1.y);
		Vector y2 = Vector(edges[index].v2.x, 0, edges[index].v2.y);
		Vector y3 = Vector(edges[index].v1.x, h, edges[index].v1.y);
		Vector y4 = Vector(edges[index].v2.x, h, edges[index].v2.y);

		glColor4fv((float *)&col3);
//		glVertex3fv((float *)&y1);
//		glVertex3fv((float *)&y2);
//		glColor4fv((float *)&col4);
		glVertex3fv((float *)&y4);
		glVertex3fv((float *)&y3);

		Vector a1 = y1 + Vector(0, 0.8f, 0);
		Vector a2 = y2 + Vector(0, 0.8f, 0);

		glColor4fv((float *)&col2);
//		glVertex3fv((float *)&a1);
//		glVertex3fv((float *)&a2);



	}
	glEnd();

	const float gh = 20;
	const float gs = 1.5f;
	glLineWidth(1.0f);
	Vector g1 = Vector(-gh, gs, -gh);
	Vector g2 = Vector(gh, gs, -gh);
	Vector g3 = Vector(gh, gs, gh);
	Vector g4 = Vector(-gh, gs, gh);

	glColor4f(0.5f, 0.55f, 0.9f, alpha*1.2f);
	Primitives::renderGrid(g1, g2, g3, g4, 50);


	myEnable(MY_FOG);
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor4f(1,1,1,alpha*1.0f);

	const float len = 0.005f;
	for (i=0;i<pointcount;i++)
	{
		if (pointflags[i])
		{
			glVertex3fv((float *)&points[i]);
			glVertex3fv((float *)&(points[i]+Vector(0, len, 0)));
		}
	}
	glEnd();

	myDisable(MY_FOG);

	glLineWidth(1.0f);
}

bool Laby::safeGet(int x, int y)
{
	//ei mennä reunojen ylitse
	if (x < 0)
		return false;
	if (x >= gridx)
		return false;
	if (y < 0)
		return false;
	if (y >= gridy)
		return false;

	if (grid[x + y*gridx] & VISITED)
		return false;
	else
		return true;
}


Laby::Laby()
{	
	int i;
	int x, y;
	gridx = 70;
	gridy = 70;
	edgecount = 0;
	int visitedcells = 1;
	int totalcells = gridx*gridy;

	srand(686);
	
	grid = new int [totalcells];
	int *stack_x = new int [totalcells];
	int *stack_y = new int [totalcells];
	int stackpointer = 0;

	pointcount = gridx*gridy;
	points = new Vector[pointcount];
	pointflags = new bool[pointcount];

	for (i=0;i<pointcount;i++)
	{
		pointflags[i] = true;
	}

	kamera = new Spline();
	viewpoint = new Spline();
	rotationtrack = new Spline();

	kamera->addpoint(Vector(0, 0.0f, -5));
	kamera->addpoint(Vector(3, 0.7f, -1));
	kamera->addpoint(Vector(1, 0.4f, 1.5));
	kamera->addpoint(Vector(5, 0.6f, 3));
	kamera->addpoint(Vector(2, 0.8f, 5));
	kamera->addpoint(Vector(-2, 0.1f, 2));
	kamera->addpoint(Vector(-4.5f, 0.5f, -3));

	viewpoint->addpoint(Vector(0, 0, 0));
	viewpoint->addpoint(Vector(0, 0, 0));
	viewpoint->addpoint(Vector(0, 0, 0));
	viewpoint->addpoint(Vector(0, 0, 0));
	viewpoint->addpoint(Vector(0, 0, 0));

	rotationtrack->addpoint(Vector(0, 1, 0));
	rotationtrack->addpoint(Vector(0.1f, 1, 0));
	rotationtrack->addpoint(Vector(0, 1, 0));
	rotationtrack->addpoint(Vector(-1, 1, 0));
	rotationtrack->addpoint(Vector(-0.2f, 1, 1));
	rotationtrack->addpoint(Vector(0, 1, 0));
	rotationtrack->addpoint(Vector(0, 1, 1.5f));
	rotationtrack->addpoint(Vector(0, 1, 0));

	Matrix rot, rot2, rot3, rot4, rot5;
	rot.makeRotation(0, 1.8f, 0);
	rot2.makeRotation(0, 1.0f, 0);
	rot3.makeRotation(1, 0.2f, 0);
	rot4.makeRotation(1.7f, -0.7f, 1.2f);
	rot5.makeRotation(2.2f, -1.4f, 1.9f);

	for (y=0;y<gridy;y++)
	{
		for (x=0;x<gridx;x++)
		{
			//ei käyty, kaikki seinät pystyssä
			grid[x+y*gridx] = NOT_VISITED | LEFT_WALL | RIGHT_WALL | UP_WALL | DOWN_WALL; 
		}
	}

	//valitaan randomilla solu
	srand(1650919);
	int currentcell_x = (rand()%gridx);
	int currentcell_y = (rand()%gridy);

	grid[currentcell_x + currentcell_y*gridx] = VISITED;
	i = 0;

	while (visitedcells < totalcells)
	{
		i++;
		//etsitään
		int ok_neighbours = 0;
		int neighbour_x[9];
		int neighbour_y[9];
		//etsitään currentcellin kaikki naapurit, joissa ei olla käyty

		if (safeGet(currentcell_x, currentcell_y-1)==true)
		{
			neighbour_x[ok_neighbours] = currentcell_x;
			neighbour_y[ok_neighbours] = currentcell_y-1;
			ok_neighbours++;
		}

		if (safeGet(currentcell_x-1, currentcell_y)==true)
		{
			neighbour_x[ok_neighbours] = currentcell_x-1;
			neighbour_y[ok_neighbours] = currentcell_y;
			ok_neighbours++;
		}
		if (safeGet(currentcell_x+1, currentcell_y)==true)
		{
			neighbour_x[ok_neighbours] = currentcell_x+1;
			neighbour_y[ok_neighbours] = currentcell_y;
			ok_neighbours++;
		}
 
		if (safeGet(currentcell_x, currentcell_y+1)==true)
		{
			neighbour_x[ok_neighbours] = currentcell_x;
			neighbour_y[ok_neighbours] = currentcell_y+1;
			ok_neighbours++;
		}

		if (ok_neighbours > 0)
		{
			//löytyi solu, jossa ei olla käyty vielä, mennään siis sinne
			int random_cell;
			if (ok_neighbours == 1)
				random_cell = 0;
			else
				random_cell = rand()%ok_neighbours;

			//dmsMsg("ok_neighbours = %d, valittu %d\n", ok_neighbours, random_cell);
			stack_x[stackpointer] = currentcell_x;
			stack_y[stackpointer] = currentcell_y;

			stackpointer++;

			//kaadetaan seinä

			//mentiin oikealle
			if (currentcell_x < neighbour_x[random_cell])
			{
				grid[currentcell_x+currentcell_y*gridx] &= (0xFFFFFFFF-RIGHT_WALL);
				grid[neighbour_x[random_cell]+neighbour_y[random_cell]*gridx] &= (0xFFFFFFFF-LEFT_WALL);
			}
			//mentiin vasemmalle
			if (currentcell_x > neighbour_x[random_cell])
			{
				grid[currentcell_x+currentcell_y*gridx] &= (0xFFFFFFFF-LEFT_WALL);

				grid[neighbour_x[random_cell]+neighbour_y[random_cell]*gridx] &= (0xFFFFFFFF-RIGHT_WALL);
			}
			//mentiin alas
			if (currentcell_y < neighbour_y[random_cell])
			{
				grid[currentcell_x+currentcell_y*gridx] &= (0xFFFFFFFF-DOWN_WALL);
				grid[neighbour_x[random_cell]+neighbour_y[random_cell]*gridx] &= (0xFFFFFFFF-UP_WALL);
			}
			//mentiin ylös
			if (currentcell_y > neighbour_y[random_cell])
			{
				grid[currentcell_x+currentcell_y*gridx] &= (0xFFFFFFFF-UP_WALL);
				grid[neighbour_x[random_cell]+neighbour_y[random_cell]*gridx] &= (0xFFFFFFFF-DOWN_WALL);
			}

			currentcell_x = neighbour_x[random_cell];
			currentcell_y = neighbour_y[random_cell];
			grid[currentcell_x + currentcell_y*gridx] |= VISITED;

//			dmsMsg("push, x = %d, y = %d\n", currentcell_x, currentcell_y);
			visitedcells++;
		}
		else
		{
			//popataan vanha solu pinosta
			stackpointer--;
			currentcell_x = stack_x[stackpointer];
			currentcell_y = stack_y[stackpointer];
//			dmsMsg("pop, x = %d, y = %d\n", currentcell_x, currentcell_y);
		}
	}

	const int maxedges = 100000;
	edges = new LabyEdge[maxedges];
	edgeorder = new int [maxedges];
	int edgeoffset = 0;
	
	for (y=0;y<gridy;y++)
	{
		for (x=0;x<gridx;x++)
		{
			const float xpos = (x-gridx*0.5f) / gridx;
			const float ypos = (y-gridy*0.5f) / gridy;
			const float xstep = 1.0f/gridx;
			const float ystep = 1.0f/gridy;

			scale = 30;
			//menevät ykkösen alueelle kaikki

			if ((grid[x+y*gridx]&(LEFT_WALL))!=0) //vasen seinä on olemassa -> tehdään pystysuora seinä
			{
				edges[edgeoffset].v1 = Vector(xpos, ypos, 0)*scale;
				edges[edgeoffset++].v2 = Vector(xpos, ypos+ystep, 0)*scale;
				edgecount++;
			}

			if ((grid[x+y*gridx]&(UP_WALL))!=0) //yläseinä on olemassa -> tehdään vaakasuora seinä
			{
				edges[edgeoffset].v1 = Vector(xpos, ypos, 0)*scale;
				edges[edgeoffset++].v2 = Vector(xpos+xstep, ypos, 0)*scale;
				edgecount++;
			}

			points[x+y*gridx] = (Vector(xpos, 0, ypos)+Vector(xstep, 0, ystep)*0.5f)*scale;

			if (edgecount >= maxedges)
				goto done;	//MWHAHAHA ! 
		}
	}
done:

	//arvotaan paikat
	for (i=0;i<edgecount;i++)
	{
		edgeorder[i] = i;
	}

	for (i=0;i<5000;i++)
	{
		int ekakohta = rand()%edgecount;
		int tokakohta = rand()%edgecount;
		int temp = edgeorder[ekakohta];
		edgeorder[ekakohta]=edgeorder[tokakohta];
		edgeorder[tokakohta]=temp;
	}
	
	pacmancount = 200;
	pacmen = new Pacman[pacmancount];

	for (i=0;i<pacmancount;i++)
	{
		pacmen[i].pointcount = 70;
		pacmen[i].points = new Vector[pacmen[i].pointcount];
		pacmen[i].suu = new float[pacmen[i].pointcount];
		pacmen[i].posx = rand()%gridx;
		pacmen[i].posy = rand()%gridy;

		pacmen[i].dx = 1;
		pacmen[i].dy = -1;

		for (int j=0;j<pacmen[i].pointcount;j++)
		{
			int x = pacmen[i].posx;
			int y = pacmen[i].posy;
			Vector pacpos = (Vector((x-gridx*0.5f)/gridx, 0, (y-gridy*0.5f)/gridy) + 
							 Vector(1.0f/gridx, 0, 1.0f/gridy)*0.5f)*scale;

			pacmen[i].points[j] = pacpos;

			int nx = pacmen[i].posx + pacmen[i].dx;
			int ny = pacmen[i].posy + pacmen[i].dy;

			if (nx < 0)
				nx = -nx;
			if (ny < 0)
				ny = -ny;

			int oi = pacmen[i].posx + pacmen[i].posy * gridx;
			int ni = nx+ny*gridx;

			int value = grid[oi];
			int suunta = rand()%2;

			const float suuadd = 0.2f;
			const float oikeasuu = 0.0f + suuadd;
			const float vasensuu = 0.5f + suuadd;
			const float alasuu = 0.25f + suuadd;
			const float ylasuu = 0.75f + suuadd;

			if (suunta==0)
			{
				//menossa oikealle
				if (pacmen[i].dx == 1)
				{
					if ((value&RIGHT_WALL)==0)
					{
						//päästään oikealle
						pacmen[i].posx = nx;
						pacmen[i].suu[j] = oikeasuu;
					}
					else
					{
						//ei päästä
						pacmen[i].dx = -1;
					}
				}
				//menossa vasemmalle
				else if (pacmen[i].dx == -1)
				{
					if ((value&LEFT_WALL)==0)
					{
						//päästään vasemmalle
						pacmen[i].posx = nx;
						pacmen[i].suu[j] = vasensuu;
					}
					else
					{
						//ei päästä
						pacmen[i].dx = 1;
					}
				}
			}
			else
			{
				if (pacmen[i].dy == -1)
				{
					if ((value&UP_WALL)==0)
					{
						pacmen[i].posy = ny;
						pacmen[i].suu[j] = ylasuu;
					}
					else
					{
						pacmen[i].dy = 1;
					}
				}
				else if (pacmen[i].dy == 1)
				{
					if ((value&DOWN_WALL)==0)
					{
						pacmen[i].posy = ny;
						pacmen[i].suu[j] = alasuu;
					}
					else
					{
						pacmen[i].dy = -1;
					}
				}
			}
		}
	}
	
	delete [] stack_x;
	delete [] stack_y;
}

Laby::~Laby()
{
	int i;

	delete [] edges;
	delete [] edgeorder;
	delete [] points;
	delete [] pointflags;
	delete [] grid;

	for (i=0;i<pacmancount;i++)
	{
		delete [] pacmen[i].points;
		delete [] pacmen[i].suu;
	}

	delete [] pacmen;
	delete kamera;
	delete viewpoint;
	delete rotationtrack;
}

bool Laby::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

