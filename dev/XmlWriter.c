#include "XmlWriter.h"

void printXml(Channel *channel,char* sceneFile, char* mayaVersion, char* userName)
{
	if(info.mayaXMLFile!=NULL)
	{
		fprintf(info.mayaXMLFile,"<?xml version=\"1.0\"?>\n");
		fprintf(info.mayaXMLFile,"<Autodesk_Cache_File>\n\t<cacheType Type=\"OneFile\" Format=\"mcc\"/>\n");
		fprintf(info.mayaXMLFile,"\t<time Range=\"%d-%d\"/>\n\t<cacheTimePerFrame TimePerFrame=\"%d\"/>\n", info.start, info.end, info.mayaFPS);
		fprintf(info.mayaXMLFile,"\t<cacheVersion Version=\"2.0\"/>\n");
		fprintf(info.mayaXMLFile,"\t<extra>%s</extra>\n\t<extra>%s</extra>\n\t<extra>%s</extra>\n", sceneFile, mayaVersion, userName);
		fprintf(info.mayaXMLFile,"\t<extra>NParticles Info for %s:</extra>\n", info.particleSysName);
		fprintf(info.mayaXMLFile,"\t<Channels>\n");
		writeXmlChannel(channel);
	}
	else
		printf("Impossibile aprire il file %s in scrittura\n",info.xmlFileName);
}

void writeXmlChannel(Channel *channel)
{
	char *channelType;
	int i,number = 0;

    if(info.mayaXMLFile!=NULL)
		for(i=0;i<info.option;i++)
		{
			if(channel[i].type==DBLA)
				channelType = DOUBLEARRAY;
			else
				channelType = FLOATVECTORYARRAY;
			
			fprintf(info.mayaXMLFile,"\t\t<channel%d ChannelName=\"%s\" ChannelType=\"%s\" ChannelInterpretation=\"%s\" SamplingType=\"Regular\" SamplingRate=\"%d\" StartTime=\"%d\" EndTime=\"%d\"/>\n",i,channel[i].name, channelType, channel[i].attribute, info.mayaFPS, info.start, info.end);
		}
	else
		printf("info.mayaXMLFile=NULL\n");
}

void closeXmlFile()
{
	if(info.mayaXMLFile!=NULL)
    {
		fflush(info.mayaXMLFile);
        fprintf(info.mayaXMLFile,"\t</Channels>\n</Autodesk_Cache_File>\n");
        fclose(info.mayaXMLFile);
	}
	isXmlFinalized=FALSE;
}
