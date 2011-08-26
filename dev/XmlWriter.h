#ifndef XMLWRITER_H_INCLUDED
#define XMLWRITER_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include "MayaNCache.h"

static FILE *file;
	
void printXmlHeader(
		int in, int out, int timePerFrame,
		char* sceneFile, char* mayaVersion, 
		char* userName, char* particleShapeName, char* fileName
		);

void closeXmlFile();

void writeXmlChannel(Channel *channel,int in, int out,int fps, CACHETYPE type);

#endif