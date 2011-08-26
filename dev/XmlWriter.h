#ifndef XMLWRITER_H_INCLUDED
#define XMLWRITER_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include "MayaNCache.h"

//channels [array of channels], sceneFile[], mayaVersion[], userName[]
void printXml(Channel *channel,char* sceneFile, char* mayaVersion, char* userName);

//channels [array of channels]
void writeXmlChannel(Channel *channel);

void closeXmlFile();

#endif