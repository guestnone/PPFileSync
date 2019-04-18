#include "PPFSPrerequisites.h"

void handler(int signum)
{
	
}

int main(void)
{
	char *source=NULL;
    char *destination=NULL;
	int sleepTime = 5 * 60;
	int recurency=0;
	int threshold = 100;
	int argument=0;
	while((argument=getopt(argc,argv,"s:d:w:t:R"))!=-1)
	{
		//source destination sleep(wait) recurency threshold
		switch(argument)
		{
        	case 's':
			source = optarg;
//printf("source: %s\n",optarg);

		break;

		case 'd':
			destination=optarg;
//printf("destination: %s\n",optarg);
		break;

		case 'w':
			sleepTime = atoi(optarg);
//printf("sleep time: %s\n",optarg);
		break;

		case 'R':
			recurency=1;
			printf("recurency\n");
		break;

		case 't':
			threshold = atoi(optarg);
//printf("threshold: %s\n",optarg);
		break;

		}

	}

		//return 0;
	printf("     -----BEGIN OF PROGRAM-----\n");
	while(1)
	{
        deleteAdditionalItems(source, destination,recurency);
		sleep(sleepTime);
	}

}
