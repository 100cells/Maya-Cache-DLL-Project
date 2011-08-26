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
	// first one, rotates around X axis
	// second one, rotates around Y axis
	// the example will save only the particles' positions
	Channel *channels;
	char    *idName,*countName,*birthTimeName, *positionName,*lifespanPPName,*finalLifespanPPName,*velocityName;
	double  *id, *count,*birthtime,*lifespanPP,*finalLifespanPP;
	float *position,*velocity;
	double temp;
	unsigned long numberOfElements=2;		// 2 particles system
	float r1,r2,angle1,angle2,delta;
	int j,fps,simulationLengthSec;
	// initialization
	setMayaNCacheFileName("2ParticlesExample");

	fps=25;	// frame per second
	simulationLengthSec=10; // value in second
	r1=5;					
	r2=7;
	angle1=0;
	angle2=90;
	delta=(float)(360.0/(fps*simulationLengthSec));
	
	// initializating the maya ncache file header (one file only)
	writeMayaNCacheHeader(ONEFILE);	

	// for the entire simulation's length
	for(j=0;j<fps*simulationLengthSec;j++)
	{
		id = (double*)malloc(sizeof(double)*numberOfElements);
		position = (float*)malloc(3*sizeof(float)*numberOfElements);
		velocity = (float*)calloc(3*numberOfElements,sizeof(float));
		birthtime = (double*)calloc(numberOfElements, sizeof(double));
		lifespanPP = (double*)calloc(numberOfElements, sizeof(double));
		finalLifespanPP = (double*)calloc(numberOfElements, sizeof(double));
		temp = (double)numberOfElements;
		count = &temp;
		channels=(Channel*)malloc(sizeof(Channel)*POSITIONVELOCITY);

		// initializating cache channels

		// COUNT
		channels[_COUNT].type = DBLA;
		channels[_COUNT].numberOfElements = 1;
		countName=(char*)calloc(sizeof(PTSYS)+7, sizeof(char));
		strcpy(countName, PTSYS);
		strcat(countName, "_count");
		channels[_COUNT].name=countName;
		channels[_COUNT].attribute="count";

		// ID
		channels[_ID].type = DBLA;
		channels[_ID].numberOfElements = numberOfElements;
		idName=(char*)calloc(sizeof(PTSYS)+4, sizeof(char));
		strcpy(idName, PTSYS);
		strcat(idName, "_id");
		channels[_ID].name=idName;
		channels[_ID].attribute="id";

		// POSITION
		channels[_POSITION].type = FVCA;
		channels[_POSITION].numberOfElements = numberOfElements;
		positionName=(char*)calloc(sizeof(PTSYS)+10, sizeof(char));
		strcpy(positionName, PTSYS);
		strcat(positionName, "_position");
		channels[_POSITION].name=positionName;
		channels[_POSITION].attribute="position";

		// VELOCITY
		channels[_VELOCITY].type = FVCA;
		channels[_VELOCITY].numberOfElements = numberOfElements;
		velocityName=(char*)calloc(sizeof(PTSYS)+10, sizeof(char));
		strcpy(velocityName, PTSYS);
		strcat(velocityName, "_velocity");
		channels[_VELOCITY].name=velocityName;
		channels[_VELOCITY].attribute="velocity";

		// BIRTHTHIME
		channels[_BIRTHTIME].type = DBLA;
		channels[_BIRTHTIME].numberOfElements = numberOfElements;
		birthTimeName=(char*)calloc(sizeof(PTSYS)+11, sizeof(char));
		strcpy(birthTimeName, PTSYS);
		strcat(birthTimeName, "_birthTime");
		channels[_BIRTHTIME].name=birthTimeName;
		channels[_BIRTHTIME].attribute="birthTime";

		// LIFESPANPP
		channels[_LIFESPANPP].type = DBLA;
		channels[_LIFESPANPP].numberOfElements = numberOfElements;
		lifespanPPName=(char*)calloc(sizeof(PTSYS)+12, sizeof(char));
		strcpy(lifespanPPName, PTSYS);
		strcat(lifespanPPName, "_lifespanPP");
		channels[_LIFESPANPP].name=lifespanPPName;
		channels[_LIFESPANPP].attribute="lifespanPP";

		// FINALLIFESPANPP
		channels[_FINALLIFESPANPP].type = DBLA;
		channels[_FINALLIFESPANPP].numberOfElements = numberOfElements;
		finalLifespanPPName=(char*)calloc(sizeof(PTSYS)+17, sizeof(char));
		strcpy(finalLifespanPPName, PTSYS);
		strcat(finalLifespanPPName, "_finalLifespanPP");
		channels[_FINALLIFESPANPP].name=finalLifespanPPName;
		channels[_FINALLIFESPANPP].attribute="finalLifespanPP";

		// for each particle in the particle system 
		// compute the position and save the datas
		
		// position and id for the first particle (x axis)
		id[0]=10;
		position[0]=0;
		position[1]=(float)(r1*sin(3.14*angle1/180.0));
		position[2]=(float)(r1*cos(3.14*angle1/180.0));
		
		// position and id for the second particle (y axis)
		id[1]=100;
		position[3]=(float)(r2*sin(3.14*angle2/180.0));
		position[4]=0;
		position[5]=(float)(r2*cos(3.14*angle2/180.0));

		angle1+=delta;
		angle2-=delta;
		// i don't need to compute the birthtime because the particles are already presents at time 0
		// and the "calloc" fill the memory area with zeros. Same append to lifespanPP and finalLifeSpanPP and velocity

		channels[_COUNT].elements = count;
		channels[_ID].elements = id;
		channels[_POSITION].elements = position;
		channels[_VELOCITY].elements = velocity;
		channels[_BIRTHTIME].elements = birthtime;
		channels[_LIFESPANPP].elements = lifespanPP;
		channels[_FINALLIFESPANPP].elements = finalLifespanPP;
		
		writeMayaNCacheBlock(j+1, fps,channels,POSITIONVELOCITY);
		if(j==0)
		{
			// writing the xml description file
			printXmlHeader(6000/fps, fps*simulationLengthSec*6000/fps, 6000/fps,"none","maya 2011 x64","me",PTSYS, "2ParticlesExample");
			writeXmlChannel(channels,6000/fps, fps*simulationLengthSec*6000/fps,6000/fps,POSITIONVELOCITY);
			closeXmlFile();
		}
		
		free(id);
		free(position);
		free(velocity);
		free(birthtime);
		free(lifespanPP);
		free(finalLifespanPP);
		
	}

	// closing the maya ncache file
	closeMayaNCacheFile();
	getchar();

	return 0;
}
