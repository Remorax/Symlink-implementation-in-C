#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

long countCharacters (int filedes)
{
	return lseek(filedes, 0, SEEK_END); //Moves the file descriptor pointer to the end of the file and returns the position, thus returning the size of the file
}

int main(int argc, char *argv[])
{
	if (argc!=3)
	{
		write(1, "The instruction should be of the form : ./a.out <PATH-TO-INPUTFILE> <PATH-TO-OUTPUTFILE>\n", 89);
		return 0;	
	}
	char* inputFile = argv[1];
	struct stat st = {0};
	// Check whether Assignment directory exists or not		
	if (stat("Assignment/", &st) == -1) {
		mkdir("Assignment", 0700); 
		char* outputFile = argv[2];
		int fd1 = open(inputFile, O_RDONLY);
		int fd2 = open(outputFile, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
		long totalNumberOfChars = countCharacters(fd1); //Counts the number of characters in the file
		lseek(fd1, -1L, 2); //Moves the pointer of file descriptor to the last position
		while(totalNumberOfChars){
			char characterArray[1];
			int x = read(fd1, characterArray, 1);
			char currentCharacter = characterArray[0];
			//Inverts case
			if(currentCharacter>='a'&&currentCharacter<='z'){
				currentCharacter -= 32;
			}
			else if(currentCharacter>='A'&&currentCharacter<='Z'){
				currentCharacter += 32;
			}
			characterArray[0] = currentCharacter;
			write(fd2, characterArray, 1);
			lseek(fd1, -2L, SEEK_CUR); // moves the pointer of file descriptor to the previous character
			totalNumberOfChars--;
		}
		chmod(outputFile, 0600); //Changes the permissions of 'pathFile' to read/write permissions for the user
		chmod("./Assignment", 0600); //Creates the directory 'Assignment' with read/write permissions for the user
		close(fd1);
		close(fd2);
	}

	return 0;
}