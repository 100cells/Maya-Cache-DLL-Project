#include "XmlWriter.h"

void printXml(Channel *channel)
{
	int i;
	info.mayaXMLFile=fopen(info.xmlFileName, "wb");
	setvbuf(info.mayaXMLFile, info.xmlChannelBuffer, _IOFBF, BUFFERLENGTH);

	if(info.mayaXMLFile!=NULL)
	{
		fprintf(info.mayaXMLFile,"<?xml version=\"1.0\"?>\n");
		switch (info.cacheFormat)
		{
		case ONEFILE:
			fprintf(info.mayaXMLFile,"<Autodesk_Cache_File>\n\t<cacheType Type=\"OneFile\" Format=\"mcc\"/>\n");
			break;
		case ONEFILEPERFRAME:
			fprintf(info.mayaXMLFile,"<Autodesk_Cache_File>\n\t<cacheType Type=\"OneFilePerFrame\" Format=\"mcc\"/>\n");
			break;
		}
		fprintf(info.mayaXMLFile,"\t<time Range=\"%d-%d\"/>\n\t<cacheTimePerFrame TimePerFrame=\"%d\"/>\n", info.start, info.end, info.mayaTPF);
		fprintf(info.mayaXMLFile,"\t<cacheVersion Version=\"2.0\"/>\n");

		for (i=0;i<info.nExtras;i++)
			fprintf(info.mayaXMLFile,"\t<extra>%s</extra>\n", info.extras[i]);
		
		fprintf(info.mayaXMLFile,"\t<Channels>\n");
		writeXmlChannel(channel);
	}
	else
		printf("Unable to open %s file in write mode\n",info.xmlFileName);
}

void writeXmlChannel(Channel *channel)
{
	char *channelType;
	int i,number = 0;
	int j=0;
    if(info.mayaXMLFile!=NULL)
		for(i=0;i<CACHENUMBEROFCHANNELS;i++)
		{
			if(channel[i].type==DBLA)
				channelType = DOUBLEARRAY;
			else
				channelType = FLOATVECTORYARRAY;
			if (channel[i].enabled)
				fprintf(info.mayaXMLFile,"\t\t<channel%d ChannelName=\"%s\" ChannelType=\"%s\" ChannelInterpretation=\"%s\" SamplingType=\"Regular\" SamplingRate=\"%d\" StartTime=\"%d\" EndTime=\"%d\"/>\n",j++,channel[i].name, channelType, names[i], info.mayaTPF, info.start, info.end);
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
