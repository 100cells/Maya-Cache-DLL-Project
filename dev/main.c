#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "MayaNCache.h"
#include "XmlWriter.h" 

//example how to use the MayaNCache library
int main()
{
	// 2 particles moving in a circular path
	// first one, rotates around X axis clockwise
	// second one, rotates around Y axis clockwise
	// the example will save only the particles' positions
	Channel *channels;
	double  *id, *birthtime,*lifespanPP,*finalLifespanPP;
	float *position,*velocity;
	double count,start,end;
	unsigned long numberOfElements=2;		// 2 particles system
	float r1,r2,angle1,angle2,delta;
	int j,totalLength;
	unsigned int fps;

	fps=24;
	start=5;		// start at time "0"
	end=15;	// simulation lenght "10" (in seconds) 
	r1=5;					
	r2=7;
	angle1=0;
	angle2=90;
	
	// initialization
	init("test","d://temp//2ParticlesExample",ONEFILE,POSITIONVELOCITY,fps,start,end);
	writeMayaNCacheHeader();

	totalLength=(int)(info.fps*info.duration);
	delta=(float)(360.0/totalLength);

	channels=(Channel*)malloc(sizeof(Channel)*POSITIONVELOCITY);
	// for the entire simulation's length
	for(j=0;j<totalLength;j++)
	{
		count = (double)numberOfElements;
		id = (double*)malloc(sizeof(double)*numberOfElements);
		position = (float*)malloc(3*sizeof(float)*numberOfElements);
		velocity = (float*)malloc(3*sizeof(float)*numberOfElements);
		birthtime = (double*)calloc(numberOfElements, sizeof(double));
		lifespanPP = (double*)calloc(numberOfElements, sizeof(double));
		finalLifespanPP = (double*)calloc(numberOfElements, sizeof(double));
		
		// initializating cache channels

		// COUNT
		channels[_COUNT].type = DBLA;
		channels[_COUNT].numberOfElements = 1;
		channels[_COUNT].name=cName.names[_COUNT];
		channels[_COUNT].attribute=aName.names[_COUNT];

		// ID
		channels[_ID].type = DBLA;
		channels[_ID].numberOfElements = numberOfElements;
		channels[_ID].name=cName.names[_ID];
		channels[_ID].attribute=aName.names[_ID];

		// POSITION
		channels[_POSITION].type = FVCA;
		channels[_POSITION].numberOfElements = numberOfElements;
		channels[_POSITION].name=cName.names[_POSITION];
		channels[_POSITION].attribute=aName.names[_POSITION];

		// VELOCITY
		channels[_VELOCITY].type = FVCA;
		channels[_VELOCITY].numberOfElements = numberOfElements;
		channels[_VELOCITY].name=cName.names[_VELOCITY];
		channels[_VELOCITY].attribute=aName.names[_VELOCITY];

		// BIRTHTHIME
		channels[_BIRTHTIME].type = DBLA;
		channels[_BIRTHTIME].numberOfElements = numberOfElements;
		channels[_BIRTHTIME].name=cName.names[_BIRTHTIME];
		channels[_BIRTHTIME].attribute=aName.names[_BIRTHTIME];

		// LIFESPANPP
		channels[_LIFESPANPP].type = DBLA;
		channels[_LIFESPANPP].numberOfElements = numberOfElements;
		channels[_LIFESPANPP].name=cName.names[_LIFESPANPP];
		channels[_LIFESPANPP].attribute=aName.names[_LIFESPANPP];

		// FINALLIFESPANPP
		channels[_FINALLIFESPANPP].type = DBLA;
		channels[_FINALLIFESPANPP].numberOfElements = numberOfElements;
		channels[_FINALLIFESPANPP].name=cName.names[_FINALLIFESPANPP];
		channels[_FINALLIFESPANPP].attribute=aName.names[_FINALLIFESPANPP];

		// for each particle in the particle system 
		// compute the position and save the datas
		
		// position, velocity and id for the first particle (x axis)
		id[0]=10;
		position[0]=0;
		position[1]=(float)(r1*sin(3.14*angle1/180.0));
		position[2]=(float)(r1*cos(3.14*angle1/180.0));
		velocity[0]=0;
		velocity[1]=(float)(r1*3.14/180.0*cos(3.14*angle1/180.0));
		velocity[2]=(float)(-r1*3.14/180.0*sin(3.14*angle1/180.0));
		
		
		// position, velocity and id for the second particle (y axis)
		id[1]=100;
		position[3]=(float)(r2*sin(3.14*angle2/180.0));
		position[4]=0;
		position[5]=(float)(r2*cos(3.14*angle2/180.0));
		velocity[3]=(float)(r2*3.14/180.0*cos(3.14*angle2/180.0));
		velocity[4]=0;
		velocity[5]=(float)(-r2*3.14/180.0*sin(3.14*angle2/180.0));

		angle1+=delta;
		angle2-=delta;

		// i don't need to compute the birthtime because the particles are already presents at time 0
		// and the "calloc" fill the memory area with zeros. Same appends to lifespanPP and finalLifeSpanPP

		channels[_COUNT].elements = &count;
		channels[_ID].elements = id;
		channels[_POSITION].elements = position;
		channels[_VELOCITY].elements = velocity;
		channels[_BIRTHTIME].elements = birthtime;
		channels[_LIFESPANPP].elements = lifespanPP;
		channels[_FINALLIFESPANPP].elements = finalLifespanPP;
		
		// the first frame is the #1, NOT #0
		writeMayaNCacheBlock(j+1, channels);
		//writeMayaNCacheBlock(j+1+(int)(start*fps), channels);

		if(id!=NULL)
			free(id);
		if(position!=NULL)
			free(position);
		if(velocity!=NULL)
			free(velocity);
		if(birthtime!=NULL)
			free(birthtime);
		if(lifespanPP!=NULL)
			free(lifespanPP);
		if(finalLifespanPP!=NULL)
			free(finalLifespanPP);
	}

	if(channels!=NULL)
		free(channels);

	// closing the maya ncache file and exit
	closeMayaNCacheFile();
	getchar();
	return 0;
}
