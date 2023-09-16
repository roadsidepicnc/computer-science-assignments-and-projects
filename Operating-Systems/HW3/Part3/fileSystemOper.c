#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

//									PART 3
/////////////////////////////////////////////////////////////////////////////////////////////////

FILE * fp;

int kb, mb;
int blockSize;
int numberOfBlocks, numberOfDirectoryBlocks, numberOfDirectories, numberOfRootDirectory;
int numberOfINodes, numberOfINodeBlocks, numberOfFreeINodes;
int iNodeEntrySize, directoryEntrySize, rootDirectoryEntrySize;
int iNodeBlocksStartAt, freeSpaceManagementBlockStartsAt, rootDirectoryStartsAt, directoryBlockStartsAt, dataBlockStartsAt;

int numberOfFreeInodes, numberOfFreeBlocks, freeDirectories, firstFreeInode, firstFreeDirectory, firstFreeDataBlock, rootFiles, rootDirectories;
int usedDataBlocks, usedINodes, usedDirectories;

char s[4] = ":()";
char nullFileName[15];
char nullNumber[9];
char nullString[21];

int writeArray[8];
int writeArrayLength;
int stringSize;

/////////////////////////////////////////////////////////////////////////////////////////////////

void resetDataBlocksOfINode(int nodeNum);
void writeDataBlocksOfINode(int num);
void writeNewDataBlock(char * fileName2);
void removeDirectoryFromDirectory(int source, int goal);
void readDir(int location, char * fileName2);
int getReadLocation(int nodeNum);
int getRootDirectoryNum(char * path);
int checkIfEmpty(int num);
void resetDirectory(int num);
void resetRootDirectory(int num);
int checkMaxSizeOfFile(char * fileName2);
void printRootDirectory();
void printNameOfDirectory(int num);
void checkContentOfDirectory(int num);

/////////////////////////////////////////////////////////////////////////////////////////////////

char * makeNumberString(int num)
{
	char * str = (char *) malloc(8 * sizeof(char));
	char x[8];

	sprintf(x, "%d", num);

	int n = num, count = 0;

    if(n == 0) count = 1;

    while(n != 0)  
   	{  
       	n = n / 10;  
       	count++;  
   	}

	for(int i = 0; i < 8; i++)	str[i] = '0';
	
	for(int i = 0; i < count; i++)
	{
		str[7 - i] = x[count - 1 - i];
	}	

	return str;
}

char * makeNullString(char * input)
{
	char * str = (char *) malloc(14 * sizeof(char));
	strcpy(str, input);
	for(int i = strlen(input); i < 14; i++)
	{
		str[i] = ' ';
	}
	return str;
}

void makeNull()
{
	int x = sizeof(nullFileName);
	int y = sizeof(nullString);
	int z = sizeof(nullNumber);

	for(int i = 0; i < x; i++)
	{
        nullFileName[i] = ' ';        
    }
    nullFileName[sizeof(nullFileName) - 1] = '\0'; 

	for(int i = 0; i < y; i++)
	{
        nullString[i] = ' ';          
    }
    nullString[sizeof(nullString) - 1] = '\0'; 

    for(int i = 0; i < z; i++)
	{
        nullNumber[i] = ' ';          
    }
    nullNumber[sizeof(nullNumber) - 1] = '\0'; 
}

int countCharOccurences(char * str, char c)	
{
    int counter = 0;
    int x = strlen(str);

    for(int i = 0; i < x; i++)
    {
        if(str[i] == c) counter++;
    }

    return counter;
}

void pickFromSuperblock()
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp[50];

	fseek(fp, 0, SEEK_SET);
	
	for(int i = 0; i < 12; i++)
	{

		getline(&line, &length, fp);
		token = strtok(line, s);

		while(token != NULL)
		{  			
			strcpy(temp, token);
      		token = strtok(NULL, s);
    	}

    	if(i == 1) numberOfBlocks = atoi(temp);
    	else if(i == 2) blockSize = atoi(temp);
    	else if(i == 3) numberOfDirectoryBlocks = atoi(temp);
    	else if(i == 4) numberOfDirectories = atoi(temp);
    	else if(i == 5) numberOfINodes = atoi(temp);
    	else if(i == 6) iNodeBlocksStartAt = atoi(temp);
    	else if(i == 7) freeSpaceManagementBlockStartsAt = atoi(temp);
    	else if(i == 8) rootDirectoryStartsAt = atoi(temp);
    	else if(i == 9) directoryBlockStartsAt = atoi(temp);
    	else if(i == 10) dataBlockStartsAt = atoi(temp);    	
    	else if(i == 11) numberOfRootDirectory = atoi(temp);    	
	}		 
}

int findEmptyRootDirectory()
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];
	char temp2[50];

	fseek(fp, rootDirectoryStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < numberOfRootDirectory; i++)
	{
		int counter = 0;

		getline(&line, &length, fp);
		getline(&line, &length, fp);
		getline(&line, &length, fp);
		

		token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			if(counter != 1) strcpy(temp2, token);
			token = strtok(NULL, s);
			counter++;
		}

		if(strcmp(temp1, nullNumber) == 0)
    	{
    		return i;
    	}
    	getline(&line, &length, fp);
	}

	return -1;

}

int findEmptyRootINode()
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];
	char temp2[50];

	fseek(fp, rootDirectoryStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < numberOfRootDirectory; i++)
	{
		int counter = 0;

		getline(&line, &length, fp);
		getline(&line, &length, fp);
		getline(&line, &length, fp);
		getline(&line, &length, fp);

		token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			if(counter != 1) strcpy(temp2, token);
			token = strtok(NULL, s);
			counter++;
		}

		if(strcmp(temp1, nullNumber) == 0)
    	{
    		return i;
    	}

    	
	}

	return -1;

}

void addRootDirectory(char * name, int num, int location)
{	
	size_t length;
	char * line = NULL;

	int n = location, count = 0;

    if(n == 0) count = 1;

    while(n != 0)  
   	{  
       	n = n / 10;  
       	count++;  
   	}

	fseek(fp, rootDirectoryStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < location * 4; i++)
	{
		getline(&line, &length, fp);
	}

	fseek(fp, strlen("d_name:(") + count, SEEK_CUR);
	fprintf(fp, "%s", name);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	fseek(fp, strlen("d_number:(") + count, SEEK_CUR);
	char * c = makeNumberString(num);
	for(int i = 0; i < 8; i++) fprintf(fp, "%c", c[i]);
}

void addRootINode(char * name, int num, int location)
{	
	size_t length;
	char * line = NULL;

	int n = location, count = 0;

    if(n == 0) count = 1;

    while(n != 0)  
   	{  
       	n = n / 10;  
       	count++;  
   	}

	fseek(fp, rootDirectoryStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < location * 4; i++)
	{
		getline(&line, &length, fp);
	}

	getline(&line, &length, fp);
	fseek(fp, strlen("f_name:(") + count, SEEK_CUR);
	fprintf(fp, "%s", name);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	fseek(fp, strlen("f_number:(") + count, SEEK_CUR);
	char * c = makeNumberString(num);
	for(int i = 0; i < 8; i++) fprintf(fp, "%c", c[i]);
}

int searchRootDirectory(char * name)	// Searches directory names in root directory. Returns the correct directory's num
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];

	fseek(fp, rootDirectoryStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < numberOfRootDirectory; i++)
	{
		int counter = 0;
		getline(&line, &length, fp);
		token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			token = strtok(NULL, s);
			counter++;
		}

		getline(&line, &length, fp);
    	getline(&line, &length, fp);

    	int v = 1;
    	int ll = strlen(name);

    	for(int i = 0; i < ll; i++)
    	{
    		if(temp1[i] != name[i])
    		{
    			v = 0;
    			break;
    		}
    	}

    	if(v == 1)
    	{

    		counter = 0;
    		token = strtok(line, s);

			while(counter < 2)
			{
				strcpy(temp1, token);
				token = strtok(NULL, s);
				counter++;
			}

			return atoi(temp1);
    	}
    	getline(&line, &length, fp);

	}

	return -1;
}

int searchRootINode(char * name)
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];

	fseek(fp, rootDirectoryStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < numberOfRootDirectory; i++)
	{
		int counter = 0;
		getline(&line, &length, fp);
		getline(&line, &length, fp);

		token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			token = strtok(NULL, s);
			counter++;
		}
		getline(&line, &length, fp);
    	getline(&line, &length, fp);

    	int v = 1;
    	int ll = strlen(name);

    	for(int i = 0; i < ll; i++)
    	{
    		if(temp1[i] != name[i])
    		{
    			v = 0;
    			break;
    		}
    	}

    	if(v == 1)
    	{
    		counter = 0;
    		token = strtok(line, s);

			while(counter < 2)
			{
				strcpy(temp1, token);
				token = strtok(NULL, s);
				counter++;
			}

			return atoi(temp1);
    	}
    	

	}

	return -1;
}

int searchRootFile(char * name)
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];

	fseek(fp, rootDirectoryStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < numberOfRootDirectory; i++)
	{
		int counter = 0;
		getline(&line, &length, fp);		
		getline(&line, &length, fp);

		token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			token = strtok(NULL, s);
			counter++;
		}

		int v = 1;

		int ll = strlen(name);
    	for(int i = 0; i < ll; i++)
    	{
    		if(temp1[i] != name[i])
    		{
    			v = 0;
    			break;
    		}
    	}
    	
    	if(v == 1)
    	{
    		getline(&line, &length, fp);
    		getline(&line, &length, fp);

    		counter = 0;
    		token = strtok(line, s);

			while(counter < 2)
			{
				strcpy(temp1, token);
				token = strtok(NULL, s);
				counter++;
			}

			return atoi(temp1);
    	}
    	else
    	{
    		getline(&line, &length, fp);
    		getline(&line, &length, fp);
    	}

	}

	return -1;
}

void addDirectory(char * name, int num)
{
	size_t length;
	char * line = NULL;

	fseek(fp, directoryBlockStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 18 * num; i++)
	{
		getline(&line, &length, fp);
	}

	getline(&line, &length, fp);
	fseek(fp, strlen("directory_name:("), SEEK_CUR);
	fprintf(fp, "%s", name);
}

void addINodeToDirectory(int num, int nodeNum)
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];

	fseek(fp, directoryBlockStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 18 * num + 2; i++)
	{
		getline(&line, &length, fp);
	}

	for(int i = 0; i < 8; i++)
	{
		int counter = 0;
		getline(&line, &length, fp);
		token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			token = strtok(NULL, s);
			counter++;
		}

		if(strcmp(temp1, nullNumber) == 0)
		{			
			fseek(fp, -1 * strlen("        ) "), SEEK_CUR);
			char * c = makeNumberString(nodeNum);
			for(int j = 0; j < 8; j++) fprintf(fp, "%c", c[j]);
			break;
		}
	}

}

void addINode(char * name, int num)
{
	size_t length;
	char * line = NULL;

	fseek(fp, iNodeBlocksStartAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 16 * num; i++)
	{
		getline(&line, &length, fp);
	}

	getline(&line, &length, fp);
	fseek(fp, strlen("file_name:("), SEEK_CUR);
	fprintf(fp, "%s", name);
}

void addSizeToINode(int nodeNum)
{
	size_t length;
	char * line = NULL;

	fseek(fp, iNodeBlocksStartAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 16 * nodeNum + 13; i++)
	{
		getline(&line, &length, fp);
	}

	int n = stringSize, count = 0;

    if(n == 0) count = 1;

    while(n != 0)  
    {  
       	n = n / 10;  
       	count++;  
   	}

   	char * c = makeNumberString(stringSize);

	fseek(fp, strlen("size:("), SEEK_CUR);
	for(int i = 0; i < 8; i++) fprintf(fp, "%c", c[i]);
}

void addCreationTimeToINode(int nodeNum)
{
	size_t length;
	char * line = NULL;
	char time_string[50];
	struct tm tm;
	time_t current_time;

	fseek(fp, iNodeBlocksStartAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 16 * nodeNum + 14; i++)
	{
		getline(&line, &length, fp);
	}

   	current_time = time(NULL);
    tm=*localtime(&current_time);
    sprintf(time_string,"%02d/%02d/%04d-%02d.%02d.%02d",tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900,tm.tm_hour, tm.tm_min, tm.tm_sec);
	fseek(fp, strlen("creation_date:("), SEEK_CUR);
	fprintf(fp, "%s", time_string);
}

void addLastModificationTimeToINode(int nodeNum)
{
	size_t length;
	char * line = NULL;
	char time_string[50];
	struct tm tm;
	time_t current_time;

	fseek(fp, iNodeBlocksStartAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 16 * nodeNum + 15; i++)
	{
		getline(&line, &length, fp);
	}

   	current_time = time(NULL);
    tm=*localtime(&current_time);
    sprintf(time_string,"%02d/%02d/%04d-%02d.%02d.%02d",tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900,tm.tm_hour, tm.tm_min, tm.tm_sec);
	fseek(fp, strlen("last_modification_date:("), SEEK_CUR);
	fprintf(fp, "%s", time_string);
}

void addDirectoryOfDirectory(int num, int index)
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];

	fseek(fp, directoryBlockStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 18 * num + 10; i++)
	{
		getline(&line, &length, fp);
	}

	for(int i = 0; i < 8; i++)
	{	
		getline(&line, &length, fp);
		int counter = 0;	
		token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			token = strtok(NULL, s);
			counter++;
		}

		

		if(strcmp(temp1, nullNumber) == 0)
		{
			fseek(fp, -1 * strlen("00000000") - 2, SEEK_CUR);	
			fprintf(fp, "%s", makeNumberString(index));
			break;
		}
	}

}

int checkDirectoryNumber(char * name, int num)
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];
	int counter = 0;

	fseek(fp, directoryBlockStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 18 * num; i++)
	{
		getline(&line, &length, fp);
	}

	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	token = strtok(line, s);

	while(counter < 2)
	{
		strcpy(temp1, token);
		token = strtok(NULL, s);
		counter++;
	}

	int v = 1;
	int ll = strlen(name);

    	for(int i = 0; i < ll; i++)
    	{
    		if(temp1[i] != name[i])
    		{
    			v = 0;
    			break;
    		}
    	}

	if(v == 1)
	{
		return 1;
	}
	else return 0;
}

int checkINodeNumber(char * name, int num)
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];
	int counter = 0;

	fseek(fp, iNodeBlocksStartAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 16 * num; i++)
	{
		getline(&line, &length, fp);
	}

	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	token = strtok(line, s);

	while(counter < 2)
	{
		strcpy(temp1, token);
		token = strtok(NULL, s);
		counter++;
	}

	int v = 1;
	int ll = strlen(name);

    for(int i = 0; i < ll; i++)
    {
    	if(temp1[i] != name[i])
    	{
    		v = 0;
    		break;
    	}
    }

	if(v == 1)
	{
		return 1;
	}
	else return 0;
}

int findEmptyDirectory()
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];

	fseek(fp, directoryBlockStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < numberOfDirectories; i++)
	{
		int counter = 0;
		getline(&line, &length, fp);
		getline(&line, &length, fp);
		
		token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			token = strtok(NULL, s);
			counter++;
		}

		if(strcmp(temp1, nullFileName) == 0) return i;
		
		for(int i = 0; i < 16; i++)
		{
			getline(&line, &length, fp);
		}
	}

	return -1;
} 

int findEmptyINode()
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];

	fseek(fp, iNodeBlocksStartAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < numberOfINodes; i++)
	{
		int counter = 0;
		getline(&line, &length, fp);
		getline(&line, &length, fp);
		
		token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			token = strtok(NULL, s);
			counter++;
		}

		if(strcmp(temp1, nullFileName) == 0) return i;
		
		for(int i = 0; i < 14; i++)
		{
			getline(&line, &length, fp);
		}
	}

	return -1;
}

int checkDirectoriesOfDirectory(char * name, int num)
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];
	int counter = 0;


	fseek(fp, directoryBlockStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 18 * num + 10; i++)
	{
		getline(&line, &length, fp);
	}

	for(int i = 0; i < 8; i++)
	{	
		counter = 0;

		token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			token = strtok(NULL, s);
			counter++;
		}

		getline(&line, &length, fp);

		if(strcmp(temp1, nullNumber) != 0)
		{
			if(checkDirectoryNumber(name, atoi(temp1)) == 1) return atoi(temp1);	
		}

	}

	return -1;	
}

int checkINodesOfDirectory(char * name, int num)
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];
	int counter = 0;

	fseek(fp, directoryBlockStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 18 * num + 2; i++)
	{
		getline(&line, &length, fp);
	}

	for(int i = 0; i < 8; i++)
	{	
		counter = 0;
		getline(&line, &length, fp);
		token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			token = strtok(NULL, s);
			counter++;
		}
	

		if(strcmp(temp1, nullNumber) != 0)
		{
			if(checkINodeNumber(name, atoi(temp1)) == 1) return atoi(temp1);	
		}

	}

	return -1;	
}

void mkdir(char * path)
{		
	char str[100];
	char * token;
	char tokens[100][100];
	char p[3] = "\\";

	int counter = 0;
	int slashCount = countCharOccurences(path, '\\');
	strcpy(str, path);
	token = strtok(str, p);

	while(token != NULL)
	{  	
		strcpy(tokens[counter], token);
      	token = strtok(NULL, p);
      	counter++;
    }

    if(slashCount == 0)
    {
    	printf("Input Is Wrong:\n");
		exit(EXIT_FAILURE);
    }

	if(slashCount == 1) // Root Directory
	{
		if(searchRootDirectory(tokens[0]) != -1) 
		{
			printf("Directory Already Exists1:\n");
			exit(EXIT_FAILURE);
		}

		int available = findEmptyDirectory();

		if(available == -1)
		{
			printf("There Is No Available Directory Position On the System");
			exit(EXIT_FAILURE);
		}

		addDirectory(tokens[0], available);
		int tt = findEmptyRootDirectory();

		if(tt == -1)
		{
			printf("Root Directory Is Full For Directory");
			exit(EXIT_FAILURE);
		}

		freeDirectories--;
		usedDirectories++;

		addRootDirectory(tokens[0], available, tt);
	}
	else
	{	
		int index;

		for(int i = 0; i < counter; i++)
		{
			if(i == 0)
			{
				index = searchRootDirectory(tokens[0]);
				if(index == - 1) 
				{
					printf("No Such Directory1:\n");
					exit(EXIT_FAILURE);
				}
			}
			else if(i != counter - 1)
			{
				index = checkDirectoriesOfDirectory(tokens[i], index);

				if(index == - 1) 
				{
					printf("No Such Directory2:\n");
					exit(EXIT_FAILURE);
				}
			}
			else if(i == counter -1)
			{			
				if(checkDirectoriesOfDirectory(tokens[i], index) != -1)
				{
					printf("Directory Already Exists2:\n");
					exit(EXIT_FAILURE);
				}

				int available = findEmptyDirectory();

				if(available == -1)
				{
					printf("There Is No Available Directory Position On the System");
					exit(EXIT_FAILURE);
				}

				addDirectory(tokens[i], available);
				addDirectoryOfDirectory(index, available);

				freeDirectories--;
				usedDirectories++;
			}
		}
	}
}

void rmdir(char * path)
{
	char str[100];
	char * token;
	char tokens[100][100];
	char p[3] = "\\";

	int counter = 0;
	int slashCount = countCharOccurences(path, '\\');
	strcpy(str, path);
	token = strtok(str, p);

	while(token != NULL)
	{  	
		strcpy(tokens[counter], token);
      	token = strtok(NULL, p);
      	counter++;
    }

    if(slashCount == 0)
    {
    	printf("Input Is Wrong:\n");
		exit(EXIT_FAILURE);
    }

	if(slashCount == 1) // Root Directory
	{
		int kk = getRootDirectoryNum(tokens[0]);
		if(kk != -1) 
		{
			int hh = searchRootDirectory(tokens[0]);

			if(checkIfEmpty(hh))
			{			
				resetRootDirectory(kk);
				resetDirectory(hh);

				freeDirectories++;
				usedDirectories--;

				return;
			}
			else
			{
				printf("Directory Is Not Empty:\n");
				exit(EXIT_FAILURE);
			}
			
		}
		else
		{
			printf("Directory Doesn't Exist1:\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{	
		int index;

		for(int i = 0; i < counter; i++)
		{
			if(i == 0)
			{
				index = searchRootDirectory(tokens[0]);
				if(index == - 1) 
				{
					printf("No Such Directory1:\n");
					exit(EXIT_FAILURE);
				}
			}
			else if(i != counter - 1)
			{
				index = checkDirectoriesOfDirectory(tokens[i], index);

				if(index == - 1) 
				{
					printf("No Such Directory2:\n");
					exit(EXIT_FAILURE);
				}
			}
			else if(i == counter -1)
			{		
				int check = checkDirectoriesOfDirectory(tokens[i], index);	

				if(check != -1)
				{
					if(checkIfEmpty(check) == 1)
					{
						removeDirectoryFromDirectory(index, check);
						resetDirectory(check);

						freeDirectories++;
						usedDirectories--;

						return;
					}
					else
					{
						printf("Directory Is Not Empty:\n");
						exit(EXIT_FAILURE);
					}
					
				}
				else
				{
					printf("Directory Doesn't Exist2:\n");
					exit(EXIT_FAILURE);
				}
			}
		}
	}
}

void removeDirectoryFromDirectory(int source, int goal)
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];
	int fin;

	fseek(fp, directoryBlockStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 18 * source + 10; i++)
	{
		getline(&line, &length, fp);
	}

	for(int i = 0; i < 8; i++)
	{
		getline(&line, &length, fp);

		int counter = 0;
		token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			token = strtok(NULL, s);
			counter++;
		}

		if(strcmp(temp1, makeNumberString(goal)) == 0) 
		{
			fin = i;
			break;
		}
	
	}

	fseek(fp, directoryBlockStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < 18 * source + 10 + fin; i++)
	{
		getline(&line, &length, fp);
	}

	fseek(fp, strlen("directory1:("), SEEK_CUR);
	fprintf(fp, "%s", nullNumber);
}

void removeINodeFromDirectory(int source, int goal)
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];
	int fin;

	fseek(fp, directoryBlockStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 18 * source + 2; i++)
	{
		getline(&line, &length, fp);
	}

	for(int i = 0; i < 8; i++)
	{
		getline(&line, &length, fp);

		int counter = 0;
		token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			token = strtok(NULL, s);
			counter++;
		}

		if(strcmp(temp1, makeNumberString(goal)) == 0) 
		{
			fin = i;
			break;
		}	
	}

	fseek(fp, directoryBlockStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < 18 * source + 2 + fin; i++)
	{
		getline(&line, &length, fp);
	}

	fseek(fp, strlen("file1:("), SEEK_CUR);
	fprintf(fp, "%s", nullNumber);

}

int checkIfEmpty(int num)
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];

	fseek(fp, directoryBlockStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < num * 18; i++)
	{
		getline(&line, &length, fp);
	}

	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < 16; i++)
	{
		getline(&line, &length, fp);
		int counter = 0;
		token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			token = strtok(NULL, s);
			counter++;
		}

		if(strcmp(temp1, nullNumber) != 0) return 0;
	}

	return 1;
}

void resetDirectory(int num)
{
	size_t length;
	char * line = NULL;

	int n = num, count = 0;

    if(n == 0) count = 1;

    while(n != 0)  
   	{  
       	n = n / 10;  
       	count++;  
   	} 

	fseek(fp, directoryBlockStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < num * 18 + 1; i++)
	{
		getline(&line, &length, fp);
	}

	fseek(fp, strlen("directory_name:("), SEEK_CUR);
	fprintf(fp, "%s", nullFileName);

	getline(&line, &length, fp);

	for(int i = 0; i < 8; i++)
	{
		fseek(fp, strlen("file1:("), SEEK_CUR);
		fprintf(fp, "%s", nullNumber);
		getline(&line, &length, fp);
	}

	for(int i = 0; i < 8; i++)
	{
		fseek(fp, strlen("directory2:("), SEEK_CUR);
		fprintf(fp, "%s", nullNumber);
		getline(&line, &length, fp);
	}
}

void resetINode(int num)
{
	size_t length;
	char * line = NULL;

	int n = num, count = 0;

    if(n == 0) count = 1;

    while(n != 0)  
   	{  
       	n = n / 10;  
       	count++;  
   	} 

	fseek(fp, iNodeBlocksStartAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < num * 16; i++)
	{
		getline(&line, &length, fp);
	}
	
	fseek(fp, strlen("file_name:("), SEEK_CUR);
	fprintf(fp, "%s", nullFileName);

	getline(&line, &length, fp);

	for(int i = 0; i < 8; i++)
	{
		fseek(fp, strlen("file1:("), SEEK_CUR);
		fprintf(fp, "%s", nullNumber);
		getline(&line, &length, fp);
	}

	getline(&line, &length, fp);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	fseek(fp, strlen("size:("), SEEK_CUR);
	fprintf(fp, "%s", nullNumber);
	getline(&line, &length, fp);

	fseek(fp, strlen("creation_date:("), SEEK_CUR);
	fprintf(fp, "%s", nullString);
	getline(&line, &length, fp);

	fseek(fp, strlen("last_modification_date:"), SEEK_CUR);
	fprintf(fp, "%s", nullString);
	getline(&line, &length, fp);
}

int getRootDirectoryNum(char * path)
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];

	fseek(fp, rootDirectoryStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < numberOfRootDirectory; i++)
	{
		int counter = 0;

    	getline(&line, &length, fp); 

    	token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			token = strtok(NULL, s);
			counter++;
		}

		int v = 1;
		int ll = strlen(path);

    	for(int i = 0; i < ll; i++)
    	{
    		if(temp1[i] != path[i])
    		{
    			v = 0;
    			break;
    		}
    	}

    	if(v == 1)
    	{	
    		return i;
    	}

    	getline(&line, &length, fp);
    	getline(&line, &length, fp);
    	getline(&line, &length, fp);
    	
	}

	return -1;
}

int getRootINodeNum(char * path)
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];

	fseek(fp, rootDirectoryStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < numberOfRootDirectory; i++)
	{
		int counter = 0;

    	getline(&line, &length, fp);
    	getline(&line, &length, fp); 

    	token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			token = strtok(NULL, s);
			counter++;
		}

		int v = 1;

		int ll = strlen(path);
    	for(int i = 0; i < ll; i++)
    	{
    		if(temp1[i] != path[i])
    		{
    			v = 0;
    			break;
    		}
    	}

    	if(v == 1)
    	{	
    		return i;
    	}
    	
    	getline(&line, &length, fp);
    	getline(&line, &length, fp);
    	
	}

	return -1;
}

void resetRootDirectory(int num)
{
	size_t length;
	char * line = NULL;

	int n = num, count = 0;

    if(n == 0) count = 1;

    while(n != 0)  
   	{  
       	n = n / 10;  
       	count++;  
   	}

	fseek(fp, rootDirectoryStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < 4 * num; i++)
	{
		getline(&line, &length, fp);
	}

	fseek(fp, count + strlen("d_name:("), SEEK_CUR);
	fprintf(fp, "%s", nullFileName);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	fseek(fp, count + strlen("d_number:("), SEEK_CUR);
	fprintf(fp, "%s", nullNumber);
}

void resetRootINode(int num)
{
	size_t length;
	char * line = NULL;

	int n = num, count = 0;

    if(n == 0) count = 1;

    while(n != 0)  
   	{  
       	n = n / 10;  
       	count++;  
   	}

	fseek(fp, rootDirectoryStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < 4 * num; i++)
	{
		getline(&line, &length, fp);
	}

	getline(&line, &length, fp);
	fseek(fp, count + strlen("f_name:("), SEEK_CUR);
	fprintf(fp, "%s", nullFileName);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	fseek(fp, count + strlen("f_number:("), SEEK_CUR);
	fprintf(fp, "%s", nullNumber);
}

void readFreeSpaceManagement()
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp[50];

	fseek(fp, freeSpaceManagementBlockStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 10; i++)
	{

		getline(&line, &length, fp);
		token = strtok(line, s);

		while(token != NULL)
		{  			
			strcpy(temp, token);
      		token = strtok(NULL, s);
    	}

    	if(i == 1) numberOfFreeINodes = atoi(temp);
    	else if(i == 2) numberOfFreeBlocks = atoi(temp);
    	else if(i == 3) freeDirectories = atoi(temp);
    	else if(i == 4) firstFreeDataBlock = atoi(temp); 	
    	else if(i == 5) usedDataBlocks = atoi(temp);  	
    	else if(i == 6) usedINodes = atoi(temp);  	
    	else if(i == 7) usedDirectories = atoi(temp);  	
	}

}

void writeFreeSpaceManagament()
{
	size_t length;
	char * line = NULL;
	char * c;

	fseek(fp, freeSpaceManagementBlockStartsAt, SEEK_SET);	
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	fseek(fp, strlen("number_of_free_inodes:"), SEEK_CUR);
	fprintf(fp, "%d", numberOfFreeINodes);
	getline(&line, &length, fp);

	fseek(fp, strlen("number_of_free_blocks:"), SEEK_CUR);
	fprintf(fp, "%d", numberOfFreeBlocks);
	getline(&line, &length, fp);

	fseek(fp, strlen("free_directories:"), SEEK_CUR);
	fprintf(fp, "%d", freeDirectories);
	getline(&line, &length, fp);

	fseek(fp, strlen("first_free_data_block:"), SEEK_CUR);
	c = makeNumberString(firstFreeDataBlock);
	for(int i = 0; i < 8; i++) fprintf(fp, "%c", c[i]);
	getline(&line, &length, fp);

	fseek(fp, strlen("number_of_used_data_blocks:"), SEEK_CUR);
	fprintf(fp, "%d", usedDataBlocks);
	getline(&line, &length, fp);

	fseek(fp, strlen("number_of_used_inodes:"), SEEK_CUR);
	fprintf(fp, "%d", usedINodes);
	getline(&line, &length, fp);

	fseek(fp, strlen("number_of_used_directories:"), SEEK_CUR);
	fprintf(fp, "%d", usedDirectories);
	getline(&line, &length, fp);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void write(char * path, char * fileName2)
{
	char str[100];
	char * token;
	char tokens[100][100];
	char p[3] = "\\";

	int file2Size = checkMaxSizeOfFile(fileName2);

	if(file2Size > 8 * blockSize)
	{
		printf("FILE SIZE EXTENDS MAX FILE SIZE\n");
		exit(EXIT_FAILURE);
	}

	int counter = 0;
	int slashCount = countCharOccurences(path, '\\');
	strcpy(str, path);
	token = strtok(str, p);

	while(token != NULL)
	{  	
		strcpy(tokens[counter], token);
      	token = strtok(NULL, p);
      	counter++;
    }

    if(slashCount == 0)
    {
    	printf("Input Is Wrong:\n");
		exit(EXIT_FAILURE);
    }

	if(slashCount == 1) // Root Directory
	{
		if(searchRootINode(tokens[0]) != -1) 
		{
			int n = searchRootINode(tokens[0]);
			resetDataBlocksOfINode(n);
			writeNewDataBlock(fileName2);
			writeDataBlocksOfINode(n);
			addSizeToINode(n);
			addLastModificationTimeToINode(n);
			return;
		}

		int available = findEmptyINode();

		if(available == -1)
		{
			printf("There Is No Available i-node Position On the System");
			exit(EXIT_FAILURE);
		}

		int tt = findEmptyRootINode();

		if(tt == -1)
		{
			printf("Root Directory Is Full For File");
			exit(EXIT_FAILURE);
		}

		usedINodes++;
		numberOfFreeINodes--;

		addRootINode(tokens[0], available, tt);
	
		addINode(tokens[0], available);
		writeNewDataBlock(fileName2);
		writeDataBlocksOfINode(available);
		addSizeToINode(available);
		addCreationTimeToINode(available);
		addLastModificationTimeToINode(available);
		return;
	}
	else
	{	
		int index;

		for(int i = 0; i < counter; i++)
		{
			if(i == 0)
			{
				index = searchRootDirectory(tokens[0]);
				if(index == - 1) 
				{
					printf("No Such Directory1:\n");
					exit(EXIT_FAILURE);
				}
			}
			else if(i != counter - 1)
			{
				index = checkDirectoriesOfDirectory(tokens[i], index);

				if(index == - 1) 
				{
					printf("No Such Directory2:\n");
					exit(EXIT_FAILURE);
				}
			}
			else if(i == counter -1)
			{	
				int n = checkINodesOfDirectory(tokens[i], index);

				if(n != -1)
				{
					resetDataBlocksOfINode(n);
					writeNewDataBlock(fileName2);
					writeDataBlocksOfINode(n);
					addSizeToINode(n);
					addLastModificationTimeToINode(n);
					return;
				}
				else
				{

					int available = findEmptyINode();	

					if(available == -1)
					{
						printf("There Is No Available i-node Position On the System");
						exit(EXIT_FAILURE);
					}

					addINode(tokens[i], available);
					addINodeToDirectory(index, available);
					writeNewDataBlock(fileName2);
					writeDataBlocksOfINode(available);
					addSizeToINode(available);
					addCreationTimeToINode(available);
					addLastModificationTimeToINode(available);

					usedINodes++;
					numberOfFreeINodes--;

					return;
				}
				
			}
		}
	}
}

void read(char * path, char * fileName2)
{
	char str[100];
	char * token;
	char tokens[100][100];
	char p[3] = "\\";

	int counter = 0;
	int slashCount = countCharOccurences(path, '\\');
	strcpy(str, path);
	token = strtok(str, p);

	while(token != NULL)
	{  	
		strcpy(tokens[counter], token);
      	token = strtok(NULL, p);
      	counter++;
    }

    if(slashCount == 0)
    {
    	printf("Input Is Wrong:\n");
		exit(EXIT_FAILURE);
    }

	if(slashCount == 1) // Root Directory
	{
		if(searchRootINode(tokens[0]) != -1) 
		{
			int n = searchRootINode(tokens[0]);
			int vv = getReadLocation(n);
			readDir(vv, fileName2);
			return;
		}
		else
		{
			printf("There is No File1:\n");
			exit(EXIT_FAILURE);
		}		
		
	}
	else
	{	
		int index;

		for(int i = 0; i < counter; i++)
		{
			if(i == 0)
			{
				index = searchRootDirectory(tokens[0]);
				if(index == - 1) 
				{
					printf("No Such Directory1:\n");
					exit(EXIT_FAILURE);
				}
			}
			else if(i != counter - 1)
			{
				index = checkDirectoriesOfDirectory(tokens[i], index);

				if(index == - 1) 
				{
					printf("No Such Directory2:\n");
					exit(EXIT_FAILURE);
				}
			}
			else if(i == counter -1)
			{	
				int n = checkINodesOfDirectory(tokens[i], index);

				if(n != -1)
				{
					int vv = getReadLocation(n);
					readDir(vv, fileName2);
					return;
				}
				else
				{
					printf("There is No File2:\n");
					exit(EXIT_FAILURE);
				}
				
			}
		}
	}
}

int getReadLocation(int nodeNum)
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];
	int counter = 0;
	int res;

	fseek(fp, iNodeBlocksStartAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 16 * nodeNum + 2; i++)
	{
		getline(&line, &length, fp);
	}

	getline(&line, &length, fp);

	token = strtok(line, s);

	while(counter < 2)
	{
		strcpy(temp1, token);
		token = strtok(NULL, s);
		counter++;
	}
	
    res = atoi(temp1);

    for(int i = 0; i < 11; i++)
	{
		getline(&line, &length, fp);
	}

	counter = 0;
	token = strtok(line, s);

	while(counter < 2)
	{
		strcpy(temp1, token);
		token = strtok(NULL, s);
		counter++;
	}

    stringSize = atoi(temp1);

    return res;
}

void readDir(int location, char * fileName2)
{
	char * data = (char *) malloc(2 * stringSize * sizeof(char));
	FILE * ptr = fopen(fileName2, "w");

	fseek(fp, dataBlockStartsAt + (location * blockSize), SEEK_SET);
	fgets(data, 2 * stringSize, fp);
	fprintf(ptr, "%s", data);
	free(data);
	fclose(ptr);
}

void writeNewDataBlock(char * fileName2)
{
	char * data = (char *) malloc(blockSize * sizeof(char) + 1);
	int parts = 0;
	stringSize = 0;

	FILE * ptr = fopen(fileName2, "a+");

	while (fgets(data, blockSize + 1, ptr) != NULL) 
	{
		fseek(fp, dataBlockStartsAt + (firstFreeDataBlock + parts) * blockSize, SEEK_SET);
		fprintf(fp, "%s", data);
		writeArray[parts] = firstFreeDataBlock + parts;
		stringSize += strlen(data);
        parts++;
    }	

    free(data);
    writeArrayLength = parts;
    firstFreeDataBlock += parts;
    numberOfFreeBlocks -= parts; // VVVVV 
    usedDataBlocks += parts;

	fclose(ptr);	
}

int checkMaxSizeOfFile(char * fileName2)
{
	FILE * ptr = fopen(fileName2, "r");

	fseek(ptr, 0L, SEEK_END);
	int sz = ftell(ptr);

	fclose(ptr);	
	return sz;
}

void writeDataBlocksOfINode(int num)
{
	size_t length;
	char * line = NULL;

	fseek(fp, iNodeBlocksStartAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 16 * num; i++)
	{
		getline(&line, &length, fp);
	}

	getline(&line, &length, fp);
	getline(&line, &length, fp);

	

	for(int i = 0; i < writeArrayLength; i++)
	{
		char * c = makeNumberString(writeArray[i]);
		fseek(fp, strlen("data1:("), SEEK_CUR);	

		for(int j = 0; j < 8; j++) 
		{				
			fprintf(fp, "%c", c[j]);
		}
			
		getline(&line, &length, fp);
	}
}

void resetDataBlocksOfINode(int nodeNum)
{
	size_t length;
	char * line = NULL;

	fseek(fp, iNodeBlocksStartAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < 16 * nodeNum + 2; i++)
	{
		getline(&line, &length, fp);
	}

	for(int i = 0; i < 8; i++)
	{
		fseek(fp, strlen("data1:("), SEEK_CUR);	
		fprintf(fp, "        ");
		getline(&line, &length, fp);
	}	
}

void del(char * path)
{
	char str[100];
	char * token;
	char tokens[100][100];
	char p[3] = "\\";

	int counter = 0;
	int slashCount = countCharOccurences(path, '\\');
	strcpy(str, path);
	token = strtok(str, p);

	while(token != NULL)
	{  	
		strcpy(tokens[counter], token);
      	token = strtok(NULL, p);
      	counter++;
    }

    if(slashCount == 0)
    {
    	printf("Input Is Wrong:\n");
		exit(EXIT_FAILURE);
    }

	if(slashCount == 1) // Root Directory
	{
		int kk = getRootINodeNum(tokens[0]);
		if(kk != -1) 
		{
			int hh = searchRootINode(tokens[0]);

			if(hh != -1)
			{	
				resetRootINode(kk);
				resetINode(hh);

				usedINodes--;
				numberOfFreeINodes++;

				return;
			}
			else
			{
				printf("There Is No File:\n");
				exit(EXIT_FAILURE);
			}
			
		}
		else
		{
			printf("There Is No File:\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{	
		int index;

		for(int i = 0; i < counter; i++)
		{
			if(i == 0)
			{
				index = searchRootDirectory(tokens[0]);
				if(index == - 1) 
				{
					printf("No Such Directory1:\n");
					exit(EXIT_FAILURE);
				}
			}
			else if(i != counter - 1)
			{
				index = checkDirectoriesOfDirectory(tokens[i], index);

				if(index == - 1) 
				{
					printf("No Such Directory2:\n");
					exit(EXIT_FAILURE);
				}
			}
			else if(i == counter -1)
			{		
				int check = checkINodesOfDirectory(tokens[i], index);	

				if(check != -1)
				{
					removeINodeFromDirectory(index, check);
					resetINode(check);

					usedINodes--;
					numberOfFreeINodes++;

					return;										
				}
				else
				{
					printf("File Doesn't Exist2:\n");
					exit(EXIT_FAILURE);
				}
			}
		}
	}
}

void dumpe2fs()
{
	printf("Number of Blocks:%d\n", numberOfBlocks);
	printf("Block Size:%d\n", blockSize);
	printf("Number of Directories:%d\n", numberOfDirectories);
	printf("Number of i-nodes:%d\n", numberOfINodes);
	printf("Number of Root Directory:%d\n", numberOfRootDirectory);
	printf("Number of Free Data Blocks:%d\n", numberOfFreeBlocks);
	printf("Number of Free i-node Blocks:%d\n", numberOfFreeINodes);
	printf("Number of Free Directory Blocks:%d\n", freeDirectories);
	printf("Number of Used Data Blocks:%d\n", usedDataBlocks);
	printf("Number of Used i-node Blocks:%d\n", usedINodes);
	printf("Number of Used Directory Blocks:%d\n", usedDirectories);
}

void dir(char * path)
{
	char str[100];
	char * token;
	char tokens[100][100];
	char p[3] = "\\";

	int counter = 0;
	int slashCount = countCharOccurences(path, '\\');
	strcpy(str, path);
	token = strtok(str, p);

	while(token != NULL)
	{  	
		strcpy(tokens[counter], token);
      	token = strtok(NULL, p);
      	counter++;
    }

    if(slashCount == 0)
    {
    	printf("Input Is Wrong:\n");
		exit(EXIT_FAILURE);
    }

	if(slashCount == 1 && strlen(tokens[0]) == 0) // Root Directory
	{
		printf("Root Directory:\n");
		printRootDirectory();	
	}
	else
	{	
		int index;

		for(int i = 0; i < counter; i++)
		{
			if(i == 0)
			{
				index = searchRootDirectory(tokens[0]);
				if(index == - 1) 
				{
					printf("No Such Directory1:\n");
					exit(EXIT_FAILURE);
				}
			}
			if(i != counter - 1 && i != 0)
			{
				index = checkDirectoriesOfDirectory(tokens[i], index);
				if(index == -1) 
				{
					printf("No Such Directory2:\n");
					exit(EXIT_FAILURE);
				}
			}
			else if(i == counter -1)
			{	
				if(counter > 2) index = checkDirectoriesOfDirectory(tokens[i], index);
	

				if(index != -1)
				{
					printf("%s Directory:\n", tokens[i]);
					checkContentOfDirectory(index);		
					return;						
				}
				else
				{
					printf("Directory Doesn't Exist2:\n");
					exit(EXIT_FAILURE);
				}
			}
		}
	}

}

void printRootDirectory()
{
	size_t length;
	char * line = NULL;
	char * token;
	char temp1[50];

	fseek(fp, rootDirectoryStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);

	for(int i = 0; i < numberOfRootDirectory; i++)
	{
		int counter = 0;
		getline(&line, &length, fp);
		token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			token = strtok(NULL, s);
			counter++;
		}

		if(strcmp(temp1, nullFileName) != 0)
		{
			printf("(Directory) %s\n", temp1);
		}

		counter = 0;

		getline(&line, &length, fp);

		token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			token = strtok(NULL, s);
			counter++;
		}

		if(strcmp(temp1, nullFileName) != 0)
		{
			printf("(File) %s\n", temp1);
		}
    	
    	getline(&line, &length, fp);
    	getline(&line, &length, fp);

	}

}

void printNameOfDirectory(int num)
{
	size_t length;
	char * line = NULL;
	int counter = 0;
	char * token;
	char temp1[50];

	fseek(fp, directoryBlockStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 18 * num + 1; i++)
	{
		getline(&line, &length, fp);
	}

	getline(&line, &length, fp);

	token = strtok(line, s);

	while(counter < 2)
	{
		strcpy(temp1, token);
		if(counter != 1) strcpy(temp1, token);
		token = strtok(NULL, s);
		counter++;
	}

	printf("(Directory) %s\n", temp1);
	
}

void printNameOfINode(int num)
{
	size_t length;
	char * line = NULL;
	int counter = 0;
	char * token;
	char temp1[50];

	fseek(fp, iNodeBlocksStartAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 16 * num + 1; i++)
	{
		getline(&line, &length, fp);
	}

	getline(&line, &length, fp);
	token = strtok(line, s);

	while(counter < 2)
	{
		strcpy(temp1, token);
		if(counter != 1) strcpy(temp1, token);
		token = strtok(NULL, s);
		counter++;
	}

	printf("(File) %s\n", temp1);
	
}

void checkContentOfDirectory(int num)
{
	size_t length;
	char * line = NULL;
	int counter = 0;
	char * token;
	char temp1[50];

	fseek(fp, directoryBlockStartsAt, SEEK_SET);
	getline(&line, &length, fp);
	getline(&line, &length, fp);
	
	for(int i = 0; i < 18 * num + 2; i++)
	{
		getline(&line, &length, fp);
	}

	for(int i = 0; i < 8; i++)
	{
		counter = 0;

		fseek(fp, directoryBlockStartsAt, SEEK_SET);
		getline(&line, &length, fp);
		getline(&line, &length, fp);

	
		for(int j = 0; j < 18 * num + 2 + i; j++)
		{
			getline(&line, &length, fp);		
		}

		getline(&line, &length, fp);

		token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			if(counter != 1) strcpy(temp1, token);
			token = strtok(NULL, s);
			counter++;
		}

		if(strcmp(temp1, nullNumber) != 0)
		{
			int fileNum = atoi(temp1);
			
			printNameOfINode(fileNum);
		}
	}	

	for(int i = 0; i < 8; i++)
	{
		counter = 0;

		fseek(fp, directoryBlockStartsAt, SEEK_SET);
		getline(&line, &length, fp);
		getline(&line, &length, fp);
	
		for(int j = 0; j < 18 * num + 10 + i; j++)
		{
			getline(&line, &length, fp);		
		}

		getline(&line, &length, fp);
			
		token = strtok(line, s);

		while(counter < 2)
		{
			strcpy(temp1, token);
			if(counter != 1) strcpy(temp1, token);
			token = strtok(NULL, s);
			counter++;
		}

		if(strcmp(temp1, nullNumber) != 0)
		{
			int dirNum = atoi(temp1);
			printNameOfDirectory(dirNum);
		}
	}
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[])
{
	fp = fopen(argv[1], "r+");

	if(fp == NULL)
	{
		printf("File Couldn't be Opened\n");
		exit(EXIT_FAILURE);
	}

	kb = 1000;
	mb = 1000 * kb;

	makeNull();
	pickFromSuperblock();
	readFreeSpaceManagement();

	if(strcmp(argv[2], "mkdir") == 0 && argc == 4)
	{
		printf("Executing mkdir operation\n");
		mkdir(argv[3]);
	}
	else if(strcmp(argv[2], "rmdir") == 0 && argc == 4)
	{
		printf("Executing rmdir operation\n");
		rmdir(argv[3]);	
	}
	else if(strcmp(argv[2], "write") == 0 && argc == 5)
	{
		printf("Executing write operation\n");
		write(argv[3], argv[4]);		
	}
	else if(strcmp(argv[2], "del") == 0 && argc == 4)
	{
		printf("Executing del operation\n");
		del(argv[3]);		
	}
	else if(strcmp(argv[2], "read") == 0 && argc == 5)
	{
		printf("Executing read operation\n");
		read(argv[3], argv[4]);	
	}
	else if(strcmp(argv[2], "dumpe2fs") == 0 && argc == 3)
	{
		printf("Executing dumpe2fs operation\n");
		dumpe2fs();	
	}
	else if(strcmp(argv[2], "dir") == 0 && argc == 4)
	{
		printf("Executing dir operation\n");
		dir(argv[3]);		
	}
	else
	{
		printf("Unknown Command is Entered\n");
		exit(EXIT_FAILURE);
	}

	writeFreeSpaceManagament();
   	fclose(fp);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////