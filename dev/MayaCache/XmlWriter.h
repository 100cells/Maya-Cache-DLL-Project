#ifndef XMLWRITER_H_INCLUDED
#define XMLWRITER_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include "MayaNCache.h"

BOOL isXmlFinalized;

void printXml(Channel *channel);

void writeXmlChannel(Channel *channel);

void closeXmlFile();

#endif