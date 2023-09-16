#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

struct flock lock;

int pickStrings(char * mainStr, char str[100][100], int lenStr[], int * lenStrLen, int caseSens[]);
int checkValidity(char * str1, char * str2);
int checkIfValidCharacter(char c);
char checkCaseSensitivity(char c);
void compare(int * fd, char * str1, char * str2, int caseSens, char * filePath);

/*
	This function picks strings between '/' char's and stores them in a 2 dimensional char array.
	Each string holds the operations to operate on it.
	Also, holds the data about which line has case sensitivity and which doesn't.
*/
int pickStrings(char * mainStr, char str[100][100], int lenStr[], int * lenStrLen, int caseSens[])
{
	int curStrLen = 0;
	int stringNum = 0;
	int mainStrLen;
	int resetIndex = 0;
	int flag = 0;
	int counter = 0;

	for(mainStrLen = 0; mainStr[mainStrLen] != '\0'; ++mainStrLen);

	if(mainStr[0] != '/' || ((mainStr[mainStrLen - 1] != '/' && mainStr[mainStrLen - 1] != 'i')))
	{
		perror("Error occured\n");
		return -1;
	}

	while(1)
	{

		if(resetIndex == mainStrLen - 1)
		{
			int tempLen;
			for(tempLen = 0; str[stringNum - 1][tempLen] != '\0'; ++tempLen);

			if((stringNum -1 ) % 2 == 0)
			{
				perror("Error occured\n");
				return -1;
			}
			else if(tempLen > 0)
			{
				*lenStrLen = stringNum;
				return 1;
			}
		}
		if(stringNum % 2 == 0 && stringNum != 0)
		{
			if(mainStr[resetIndex + 1] == ';' && mainStr[resetIndex + 2] == '/')
			{
				resetIndex += 2;
			}
			else 
			{
				perror("Error occured\n");
				return -1;
			}
		}

		for(int i = resetIndex + 1; i < mainStrLen; i++)
		{	
			if(mainStr[i] == '/')
			{
				
				lenStr[stringNum] = i - resetIndex - 1;
				stringNum++;
				resetIndex = i;

				if(i != mainStrLen - 1 && mainStr[i + 1] == 'i')
				{
					caseSens[counter] = 1;
					resetIndex++;
				}
				else
				{
					caseSens[counter] = 0;
				}

				counter++;
				break;
			}

			str[stringNum][i - resetIndex - 1] = mainStr[i];
			
		}
	}
}

/*
	This function takes a char c.
	Returns 1 if c is a digit or a letter. Else returns 0.
*/
int checkIfValidCharacter(char c)
{
	int result = 0;

	if(c >= '0' && c <= '9')	result = 1;
	else if(c >= 'a' && c <= 'z') result = 1;
	else if(c >= 'A' && c <= 'Z') result = 1;
	else result = 0;

	return result;
}

/*
	This function takes 2 strings str1 and str2.
	Returns 1 if both of the strings are valid for operations at the next step.
	(Basically, it looks string lengths, some of the character's locations etc.)
	Else returns 0.
*/
int checkValidity(char * str1, char * str2)
{
	int result = 1, letterFound = 0, str1Len, str2Len;

	for(str1Len = 0; str1[str1Len] != '\0'; ++str1Len);
	for(str2Len = 0; str2[str2Len] != '\0'; ++str2Len);

	if(str2Len == 0 || str1Len == 0)
	{
		return 0;
	}	

	for(int i = 0; i < str2Len; i++)
	{
		int temp = checkIfValidCharacter(str2[i]);
		if(temp == 0)
		{
			return 0;
		}
	}

	if(result == 1)
	{
		for(int i = 0; i < str1Len; i++)
		{
			if(str1[i] == '^' && i != 0)
			{
				result = 0;
				break;
			}
			else if(str1[i] == '$' && i != str1Len - 1)
			{
				result = 0;
				break;
			}
			else if((str1[i] == '*' && i == 0 )|| (str1[i] == '*' && i > 0 && str1[i - 1] == '^'))
			{
				result = 0;
				break;
			}
			else if(str1[i] == '[' && str1Len < i + 2)
			{
				result = 0;
				break;
			}
			else if(str1[i] == '[' && str1Len >= i + 2)
			{

				int j;
				for(j = i + 1; j < str1Len; j++)
				{
					if(j == i + 1 && str1[j] == ']')
					{
						return 0;
					}
					else if(str1[j] == ']')
					{
						break;
					}
					else
					{
						int temp = checkIfValidCharacter(str1[j]);
						if(temp == 0)
						{
							return 0;
						}
						else letterFound = 1;
					}

				}
				i = j;
				
			}
			else if((str1[i] >= '0' && str1[i] <= '9')	
					||(str1[i] >= 'a' && str1[i] <= 'z')
					||(str1[i] >= 'A' && str1[i] <= 'Z') )
			{
				int temp = checkIfValidCharacter(str1[i]);
				if(temp == 0)
				{
					return 0;
				}
				else letterFound = 1;
			}
			else if(str1[i] == '*' || str1[i] == '^' || str1[i] == '$')
			{
				result = 1;
			}
			else return 0;

		}

		if(letterFound == 0)
		{
			result = 0;
		}
	}

	return result;
}

/*
	It takes a char c.
	If c is a upper case letter, it returns c's lower case letter version.
	If c is not a upper case letter just returns c without any modification. 
*/
char checkCaseSensitivity(char c)
{
	if(c >= 'A' && c <= 'Z')
	{
		return c + 32;
	}
	else
	{
		return c;
	}
}

/*
	This function is the most important function of this homework.
	It reads file char by char and detects if str1 is found on the string (with required operations of course).
	When detected it puts str2 at the previous str1's position at the file and rest of the file stays the same.
	These operations repeat for each detected str1.
*/
void compare(int * fd, char * str1, char * str2, int caseSens, char * filePath)
{
	char buffer[1], bufferRest[10000];
	size_t bytes_read1 = sizeof(buffer);
	int counter1 = 0, counter2 = 0, counter3 = 0, str1Len, str2Len;
	int succes = 0, letterFound = 0, goingMatched = 0;
	char prevChar = '\0', checkChar;
	int fatal = 0, checkedAlready = 0, checkFurther = 0;
	int backup = -1, tempCounter = 0, both = 0;
	char tempStr[1000];
	int checkedMult = 0, multFound = 0;

	for(str1Len = 0; str1[str1Len] != '\0'; ++str1Len);
	for(str2Len = 0; str2[str2Len] != '\0'; ++str2Len);

	lseek(*fd, 0, SEEK_SET);

	while(bytes_read1 == sizeof(buffer))
	{
		lseek(*fd, counter2, SEEK_SET);
		bytes_read1 = read(*fd, buffer, sizeof(buffer));

		char c1;
		char c2; 	
		int expFlag = 0;	
		backup = -1;
		checkedMult = 0;

		if(bytes_read1 == sizeof(buffer))
		{

			while(str1[counter1] == '^' || str1[counter1] == '*' || str1[counter1] == '[')
			{
				expFlag = 0;
				if(str1[counter1] == '^')
				{
					if(counter1 == 0 && (counter2 == 0 || prevChar == 10 || prevChar == 13))
					{
						expFlag = 1;
						counter1++;
					}
					else
					{
						expFlag = 1;
						fatal = 1;
						break;

					}
				}
				
				else if(str1[counter1] == '*' && counter1 != 0)
				{
					multFound = 1;
					checkedMult = 1;
					char char1, char2;
					if(caseSens == 1)	// If case insensitive
					{
						char1 = checkCaseSensitivity(buffer[0]);
						char2 = checkCaseSensitivity(str1[counter1 - 1]);
					}
					else
					{
						char1 = buffer[0];
						char2 = str1[counter1 - 1];
					}
					counter1++;

					if(counter1 - 1 == backup)
					{
						;
						both = 1;
					}
					else if(char1 == char2)
					{
						checkChar = char1;
						checkFurther = 1;
						goingMatched = 1;
						counter3++;
					}
					else
					{
						goingMatched = 1;
						checkFurther = 0;
					}

				}
				else if(str1[counter1] == '[')
				{					
					checkedAlready = 1;

					for(int i = counter1 + 1; i < str1Len; i++)
					{
						if(str1[i] != ']')
						{
							char c;
							if(caseSens == 1) c = checkCaseSensitivity(str1[i]);
							else c = str1[i];
							tempStr[tempCounter] = c;
							tempCounter++;
							counter1++;
							backup = counter1;
						}
						else
						{
							counter1++;
							break;
						}
					}

					counter1++;
					
				}

				if(both == 1 && expFlag == 0)
				{				
					int done = 0;
					char c;
					if(caseSens == 1) c = checkCaseSensitivity(buffer[0]);
					else c = buffer[0];
					for(int i = 0; i < tempCounter; i++)
					{
						if(c == tempStr[i])
						{
							goingMatched = 1;
							checkedAlready = 1;
							done = 1;
							checkChar = c;
							checkFurther = 1;
							goingMatched = 1;
							break;
						}
					}
					if(done == 0)
					{
						goingMatched = 0;
						fatal = 1;
						counter1 = 0;
					}
				}
				else if(((str1[counter1 + 1] != '\0' && str1[counter1 + 1] != '*') || str1[counter1 + 1] == '\0') && expFlag == 0 && backup != -1)
				{
					int done = 0;
					char c;
					if(caseSens == 1) c = checkCaseSensitivity(buffer[0]);
					else c = buffer[0];

					for(int i = 0; i < tempCounter; i++)
					{
						if(c == tempStr[i])
						{
							counter3++;
							goingMatched = 1;
							checkedAlready = 1;
							done = 1;
							break;
						}
					}
					if(done == 0)
					{
						goingMatched = 0;
						fatal = 1;
						counter1 = 0;
					}
				}
		}

		if(caseSens == 1 )	// If case insensitive
		{
			c1 = checkCaseSensitivity(buffer[0]);
			c2 = checkCaseSensitivity(str1[counter1]);
		}
		else
		{
			c1 = buffer[0];
			c2 = str1[counter1];
		}

			if(checkFurther == 1)
			{
				if(checkChar == c1)
				{
					checkFurther = 1;
					goingMatched = 1;
					counter3++;
				}
				else
				{
					goingMatched = 1;
					checkFurther = 0;
				}
			}
			else if(str1[counter1] != '\0' && c1 == c2 && checkedAlready == 0 && checkFurther == 0 && succes == 0)
			{
				goingMatched = 1;
				letterFound = 1;
				counter1++;
				counter3++;
			}
			else if(c2 != '\0' && checkedAlready == 0 && checkFurther == 0 && succes == 0)
			{
				goingMatched = 0;
				letterFound = 1;
				counter1 = 0;
				counter2 = counter2 - counter3;
				counter3 = 0;
				succes = 0;
			}

			if((str1[counter1] == '\0' && goingMatched == 1) && fatal == 0 && succes == 0)	
			{		
				succes = 1;
			}

			if(succes == 1)
			{	
				char begFile[10000];

				succes = 0;
				letterFound = 0;

				size_t zz = read(*fd, bufferRest, sizeof(bufferRest));

				lseek(*fd, 0, SEEK_SET);

				size_t yy = read(*fd, begFile, counter2 + 1 - counter3);

				lseek(*fd, counter2 + 1 - counter3 + str2Len, SEEK_SET);

				*fd = open(filePath, O_RDWR | O_TRUNC, 0666);
				write(*fd, begFile, yy);
				write(*fd, str2, str2Len);
				write(*fd, bufferRest, zz);

				fcntl(*fd, F_SETLKW, &lock);

				lseek(*fd, counter2 + 1 - counter3 + str2Len, SEEK_SET);
	
				goingMatched = 0;
				counter2 = counter2 - (counter3 - str2Len);
				counter1 = 0;
				counter3 = 0;
				letterFound = 0;
				//close(*fd);
			}
			else if(str1[counter1] == '$')
			{
				//counter2++;
				//lseek(fd, counter2, SEEK_SET);
				char nextChar[1];
				size_t bytes_read_temp = read(*fd, nextChar, sizeof(nextChar));
				lseek(*fd, counter2 + 1, SEEK_SET);

				if(counter1 == str1Len - 1 && (nextChar[0] == 10 || nextChar[0] == 13 || bytes_read_temp != sizeof(nextChar)) && goingMatched == 1)
				{
					char begFile[10000];

					succes = 0;
					letterFound = 0;

					size_t zz = read(*fd, bufferRest, sizeof(bufferRest));

					lseek(*fd, 0, SEEK_SET);

					size_t yy = read(*fd, begFile, counter2 + 1 - counter3);

					lseek(*fd, counter2 + 1 - counter3 + str2Len, SEEK_SET);

					*fd = open(filePath, O_RDWR | O_TRUNC, 0666);
					write(*fd, begFile, yy);
					write(*fd, str2, str2Len);
					write(*fd, bufferRest, zz);

					fcntl(*fd, F_SETLKW, &lock);

					lseek(*fd, counter2 + 1 - counter3 + str2Len, SEEK_SET);
	
					goingMatched = 0;
					counter2 = counter2 - (counter3 - str2Len);
					counter1 = 0;
					counter3 = 0;
					letterFound = 0;
					//close(*fd);
				}
				else
				{
					counter1 = 0;
					fatal = 1;
					perror("Error occured\n");
				}
			}	

		}

		checkedAlready = 0;
		fatal = 0;
		prevChar = buffer[0];
		counter2++;
	}

}

//////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{	
	int fd;
	char str [100][100];
	int lenStr[10000], caseSens[1000];
	int mainCheck, lenStrLen;
	memset(&lock, 0, sizeof(lock));

	fd = open(argv[2], O_RDWR);
	lock.l_type = F_WRLCK;

	if(fd == -1)
	{
		perror("File couldn't be opened\n");
		return 0;
	}

	if(fcntl(fd, F_SETLKW, &lock) == -1)
	{
		perror("Some other descriptor is using this file\n");
		return 0;
	}

	close(fd);
	int pick = pickStrings(argv[1], str, lenStr, &lenStrLen, caseSens);

	if(pick == 1)
	{	
		for(int i = 0; i < lenStrLen; i += 2)
		{
			int vadlidity = checkValidity(str[i], str[i + 1]);
			if(vadlidity == 1)
			{
				fd = open(argv[2], O_RDWR);

				compare(&fd, str[i], str[i + 1], caseSens[i + 1], argv[2]);
			} 
			else 
			{
				perror("Input strings are not valid\n");
				break;
			}
		}
	}
	else
	{
		perror("Input strings are not valid\n");
	}

	close(fd);

	return 0;
}