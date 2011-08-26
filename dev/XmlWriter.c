#include "XmlWriter.h"

void printXmlHeader(
		int in, int out, int timePerFrame,
		char* sceneFile, char* mayaVersion, char* userName,
		char* particleShapeName, char* fileName
		)
{
	// adding extension ".xml"
	char *name;

	name = (char *)malloc(sizeof(char)*(strlen(fileName)+4));
	strcpy(name, fileName);
	strcat(name, ".xml");
	file = fopen(name,"w");

    //apro il file in scrittura
    if(file!=NULL)
    {
		fprintf(file,"<?xml version=\"1.0\"?>\n");
		fprintf(file,"<Autodesk_Cache_File>\n\t<cacheType Type=\"OneFile\" Format=\"mcc\"/>\n");
		fprintf(file,"\t<time Range=\"%d-%d\"/>\n\t<cacheTimePerFrame TimePerFrame=\"%d\"/>\n", in, out, timePerFrame);
		fprintf(file,"\t<cacheVersion Version=\"2.0\"/>\n");
		fprintf(file,"\t<extra>%s</extra>\n\t<extra>%s</extra>\n\t<extra>%s</extra>\n", sceneFile, mayaVersion, userName);
		fprintf(file,"\t<extra>NCloth Info for %s:</extra>\n", particleShapeName);
		// fprintf(file,"\t<extra>Passive Colliders: none</extra>\n\t<extra>Constraints: none</extra>\n");
		fprintf(file,"\t<Channels>\n");
    }
    else
        printf("Data could not be saved.\n");
}

void closeXmlFile()
{
    if(file!=NULL)
    {
        fprintf(file,"\t</Channels>\n</Autodesk_Cache_File>\n");
        fclose(file);
    }
}
void writeXmlChannel(Channel *channel, int in, int out,int fps,CACHETYPE type)
{
	char *channelType;
	int i,number = 0;

    if(file!=NULL)
		for(i=0;i<type;i++)
		{
			if(channel[i].type==DBLA)
				channelType = "DoubleArray";
			else
				channelType = "FloatVectorArray";

			fprintf(file,"\t\t<channel%d ChannelName=\"%s\" ChannelType=\"%s\" ChannelInterpretation=\"%s\" SamplingType=\"Regular\" SamplingRate=\"%d\" StartTime=\"%d\" EndTime=\"%d\"/>\n",i,channel[i].name, channelType, channel[i].attribute, fps, in, out);
		}
        
}