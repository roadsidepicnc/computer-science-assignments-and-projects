#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//									PART 2
/////////////////////////////////////////////////////////////////////////////////////////////////

int mb, kb;
int blockSize;
int numberOfBlocks, numberOfDirectoryBlocks, numberOfDirectories;
int numberOfINodes, numberOfINodeBlocks, numberOfFreeINodes;
int iNodeEntrySize, directoryEntrySize, rootDirectoryEntrySize;

char nullFileName[15];
char nullNumber[9];
char nullString[21];

FILE * fp;

/////////////////////////////////////////////////////////////////////////////////////////////////

void makeNull();
void writeSuperBlock();
void writeRoot();
void writeINodeBlocks();
void writeDirectories();
void writeFreeSpace();

/////////////////////////////////////////////////////////////////////////////////////////////////

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

void writeSuperBlock()
{
	char temp[100];
	fseek(fp, 0, SEEK_SET);

	sprintf(temp, "superblock:\n");
	fwrite(temp , 1, strlen(temp), fp);

	sprintf(temp, "number_of_blocks:%d\n", numberOfBlocks);
	fwrite(temp , 1, strlen(temp), fp);

	sprintf(temp, "block_size:%d\n", blockSize);
	fwrite(temp , 1, strlen(temp), fp);

	sprintf(temp, "number_of_directory_blocks:%d\n", numberOfDirectoryBlocks);
	fwrite(temp , 1, strlen(temp), fp);

	sprintf(temp, "number_of_directories:%d\n", (numberOfDirectoryBlocks * blockSize) / directoryEntrySize);
	fwrite(temp , 1, strlen(temp), fp);

	sprintf(temp, "number_of_inodes:%d\n", numberOfINodes);
	fwrite(temp , 1, strlen(temp), fp);

	sprintf(temp, "inode_block_starts_at:%d\n", blockSize);
	fwrite(temp , 1, strlen(temp), fp);

	sprintf(temp, "free_space_management_block_starts_at:%d\n", blockSize * (numberOfINodeBlocks + 1));
	fwrite(temp , 1, strlen(temp), fp);

	sprintf(temp, "root_directory_blocks_starts_at:%d\n", blockSize * (numberOfINodeBlocks + 2));
	fwrite(temp , 1, strlen(temp), fp);

	sprintf(temp, "directory_block_starts_at:%d\n", blockSize * (numberOfINodeBlocks + 3));
	fwrite(temp , 1, strlen(temp), fp);

	sprintf(temp, "data_block_starts_at:%d\n", blockSize * (numberOfINodeBlocks + 3 + numberOfDirectoryBlocks));
	fwrite(temp , 1, strlen(temp), fp);

	sprintf(temp, "number_of_root_directory:%d\n", blockSize / rootDirectoryEntrySize);
	fwrite(temp , 1, strlen(temp), fp);

}

void writeINodeBlocks()
{
	char temp[100];
    fseek(fp, blockSize, SEEK_SET);

    sprintf(temp, "\ni-node Block\n");
	fwrite(temp , 1, strlen(temp), fp);

    for(int i = 0; i < numberOfINodes; i++)
    {
    	sprintf(temp, "inode%d\n", i);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "file_name:(%s)\n", nullFileName);
		fwrite(temp , 1, strlen(temp), fp);
 
		sprintf(temp, "data1:(%s)\n", nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "data2:(%s)\n", nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "data3:(%s)\n", nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "data4:(%s)\n", nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "data5:(%s)\n", nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "data6:(%s)\n", nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "data7:(%s)\n", nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "data8:(%s)\n", nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "single_indirect:(%s)\n", nullFileName);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "double_indirect:(%s)\n", nullFileName);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "triple_indirect:(%s)\n", nullFileName);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "size:(%s)\n", nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "creation_date:(%s)\n", nullString);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "last_modification_date:(%s)\n", nullString);
		fwrite(temp , 1, strlen(temp), fp);
    } 

}

void writeRoot()
{
	char temp[100];
    fseek(fp, blockSize * (numberOfINodeBlocks + 2), SEEK_SET);

    sprintf(temp, "\nroot_directory\n");
	fwrite(temp , 1, strlen(temp), fp);

    for(int i = 0; i < blockSize / rootDirectoryEntrySize; i++)
    {	
    	sprintf(temp, "d_name%d:(%s)\n", i, nullFileName);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "f_name%d:(%s)\n", i, nullFileName);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "d_number%d:(%s)\n", i, nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "f_number%d:(%s)\n", i, nullNumber);
		fwrite(temp , 1, strlen(temp), fp);
    }
}

void writeDirectories()
{
	char temp[100];
    fseek(fp, blockSize * (numberOfINodeBlocks + 3), SEEK_SET);

    sprintf(temp, "\ndirectories:\n");
	fwrite(temp , 1, strlen(temp), fp);

    for(int i = 0; i < (numberOfDirectoryBlocks * blockSize) / directoryEntrySize; i++)
    {
    	sprintf(temp, "directory%d\n", i);
		fwrite(temp , 1, strlen(temp), fp);

  		sprintf(temp, "directory_name:(%s)\n", nullFileName);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "file1:(%s)\n",nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "file2:(%s)\n",nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "file3:(%s)\n",nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "file4:(%s)\n",nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "file5:(%s)\n",nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "file6:(%s)\n",nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "file7:(%s)\n",nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "file8:(%s)\n",nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "directory1:(%s)\n",nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "directory2:(%s)\n",nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "directory3:(%s)\n",nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "directory4:(%s)\n",nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "directory5:(%s)\n",nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "directory6:(%s)\n",nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "directory7:(%s)\n",nullNumber);
		fwrite(temp , 1, strlen(temp), fp);

		sprintf(temp, "directory8:(%s)\n",nullNumber);
		fwrite(temp , 1, strlen(temp), fp);
    }
}

void writeFreeSpace()
{
	char temp[100];
    fseek(fp, blockSize * (numberOfINodeBlocks + 1), SEEK_SET);

    sprintf(temp, "\nfree_space_management:\n");
	fwrite(temp , 1, strlen(temp), fp);

	sprintf(temp, "number_of_free_inodes:%d\n", numberOfINodes);
	fwrite(temp , 1, strlen(temp), fp);

	sprintf(temp, "number_of_free_blocks:%d\n", numberOfBlocks - (numberOfINodeBlocks + numberOfDirectoryBlocks + 3));
	fwrite(temp , 1, strlen(temp), fp);

	sprintf(temp, "free_directories:%d\n", numberOfDirectories);
	fwrite(temp , 1, strlen(temp), fp);

	sprintf(temp, "first_free_data_block:00000000\n");
	fwrite(temp , 1, strlen(temp), fp);

	sprintf(temp, "number_of_used_data_blocks:0\n");
	fwrite(temp , 1, strlen(temp), fp);

	sprintf(temp, "number_of_used_inodes:0\n");
	fwrite(temp , 1, strlen(temp), fp);

	sprintf(temp, "number_of_used_directories:0\n");
	fwrite(temp , 1, strlen(temp), fp);

}

/////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc,char * argv[])
{
	if(argc != 3)
	{
		printf("Error: Wrong Number of Inputs!\n");
	}

	int blockInput;

	iNodeEntrySize = 375;
	directoryEntrySize = 370;
	rootDirectoryEntrySize = 100;
	mb = 1000 * 1000;
	kb = 1000;
	
	int totalBytes = 16 * mb;

	int offset = totalBytes - 1;
    fp = fopen(argv[2], "w+");

    if(fp == NULL)
    {
    	printf("Input File Couldn't be Opened\n");
    	exit(EXIT_FAILURE);
    }

    fseek(fp, offset, SEEK_SET);
    fprintf(fp, "%c",'\0');

	blockInput = atoi(argv[1]); 	

	if(blockInput == 0) 
	{
		printf("Block Size is Not Sufficient\n");
		exit(EXIT_FAILURE);
	}

	blockSize = blockInput * kb;

	numberOfBlocks = totalBytes / blockSize;
	numberOfDirectoryBlocks = numberOfBlocks / 20; // %5 of the system
	numberOfINodeBlocks = numberOfBlocks / 10;	   // %10 of the system
	numberOfINodes = numberOfINodeBlocks * blockSize / iNodeEntrySize;
	numberOfDirectories = numberOfDirectoryBlocks * blockSize / directoryEntrySize;
	//numberOfFreeINodes = numberOfINodes;

    printf("%s is created with block size %d\n", argv[2], blockSize);

	makeNull();
	writeSuperBlock();
	writeINodeBlocks();
	writeFreeSpace();
	writeRoot();
	writeDirectories();

    fclose(fp);
    return 0;
}