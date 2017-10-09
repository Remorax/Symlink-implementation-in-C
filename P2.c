#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>

long countCharacters (int f) // A function to count the total number of characters in a file given the file descriptor
{
	return lseek(f, 0, SEEK_END); //Moves the file descriptor pointer to the end of the file and returns the position, thus returning the size of the file
}

int main(int argc, char* argv[])
{
	if(argc!=3){
		write(1, "The instruction should be of the form : ./a.out <PATH-TO-INPUTFILE> <PATH-TO-OUTPUTFILE>\n", 89);
		return 0;
	}
	// Initialise all variables to "No" first
	char* directoryCreation = "No";
	char* fileCreation = "No";
	char* symlinkCreation = "No";
	
	char* inputTransformation = "No";
	
	char* userReadFile = "No";
	char* userWriteFile = "No";
	char* userExecuteFile = "No";

	char* groupReadFile = "No";
	char* groupWriteFile = "No";
	char* groupExecuteFile = "No";

	char* othersReadFile = "No";
	char* othersWriteFile = "No";
	char* othersExecuteFile = "No";

	char* userReadDirectory = "No";
	char* userWriteDirectory = "No";
	char* userExecuteDirectory = "No";

	char* groupReadDirectory = "No";
	char* groupWriteDirectory = "No";
	char* groupExecuteDirectory = "No";

	char* othersReadDirectory = "No";
	char* othersWriteDirectory = "No";
	char* othersExecuteDirectory = "No";

	struct stat st = {0};

	// Check for existence of Assignment directory
	if (stat("Assignment/", &st) != -1) {
		// Directory exists
		directoryCreation = "Yes";

		struct stat retDir = {0};
		stat( "./Assignment", &retDir);	//Save the retDir as a struct, which contains permission info, for later use

		chmod("./Assignment", 0700); //Modifies the permissions of the directory 'Assignment' to read/write/execute permissions for the user

		char *src = argv[2]; // The source of the symlink
		
		struct stat st2 = {0};
		if(stat(src, &st2) !=-1){
			fileCreation = "Yes"; // Output file exists
		}
		else{
			write(1, "Output file does not exist\n", 27);
			return 0;
		}
		struct stat ret;
		stat( src, &ret);
		
		char* dest = "./symlink"; //The dest of the symlink
		char* inputFile = argv[1];
		int fd1 = open(src, O_RDONLY);	//Read the output file
		int fd2 = open(inputFile, O_RDONLY);	//Read the input file
		long totalNumberOfChars = countCharacters(fd2); //Counts the number of characters in the file
		lseek(fd2, -1L, 2); //Moves the file descriptor pointer to the last position
		bool correctOutput = true;
		while(totalNumberOfChars){
			char inputCharacterArray[1];
			read(fd2, inputCharacterArray, 1);
			char inputCharacter = inputCharacterArray[0];
			//Switch case
			if(inputCharacter>='a'&&inputCharacter<='z'){
				inputCharacter -= 32;
			}
			else if(inputCharacter>='A'&&inputCharacter<='Z'){
				inputCharacter += 32;
			}
			char outputCharacterArray[1];
			read(fd1, outputCharacterArray, 1);
			char outputCharacter = outputCharacterArray[0];
			if(inputCharacter != outputCharacter){
				printf("%c %c\n", inputCharacter, outputCharacter);
				correctOutput = false; //The output is not correct
				break;
			}
			lseek(fd2, -2L, SEEK_CUR); // moves the file descriptor pointer to the previous character
			totalNumberOfChars--;
		}
		if(correctOutput){
			inputTransformation = "Yes";
		}
		else{
			write(1, "Output is not correct\n", 22);
		}
		close(fd1);
		close(fd2);
		int symlinkRet = symlink(src, dest);  //Create a symlink from 'src' to 'dest'
		if(symlinkRet==-1){
			write(1, "Symlink creation failed.\n", 25);
		}
		else{
			symlinkCreation = "Yes";
		}

		//Set permission variables for the file
		if(ret.st_mode & S_IRUSR){
			userReadFile = "Yes";
		}
		if(ret.st_mode & S_IWUSR){
			userWriteFile = "Yes";	
		}
		if(ret.st_mode & S_IXUSR){
			userExecuteFile = "Yes";
		}
		if(ret.st_mode & S_IRGRP){
			groupReadFile = "Yes";
		}
		if(ret.st_mode & S_IWGRP){
			groupWriteFile = "Yes";
		}
		if(ret.st_mode & S_IXGRP){
			groupExecuteFile = "Yes";
		}
		if(ret.st_mode & S_IROTH){
			othersReadFile = "Yes";
		}
		if(ret.st_mode & S_IWOTH){
			othersWriteFile = "Yes";
		}
		if(ret.st_mode & S_IXOTH){
			othersExecuteFile = "Yes";
		}
    	
		
		//Set permission variables for the directory
		if(retDir.st_mode & S_IRUSR){
			userReadDirectory = "Yes";
		}
		if(retDir.st_mode & S_IWUSR){
			userWriteDirectory = "Yes";	
		}
		if(retDir.st_mode & S_IXUSR){
			userExecuteDirectory = "Yes";
		}
		if(retDir.st_mode & S_IRGRP){
			groupReadDirectory = "Yes";
		}
		if(retDir.st_mode & S_IWGRP){
			groupWriteDirectory = "Yes";
		}
		if(retDir.st_mode & S_IXGRP){
			groupExecuteDirectory = "Yes";
		}
		if(retDir.st_mode & S_IROTH){
			othersReadDirectory = "Yes";
		}
		if(retDir.st_mode & S_IWOTH){
			othersWriteDirectory = "Yes";
		}
		if(retDir.st_mode & S_IXOTH){
			othersExecuteDirectory = "Yes";
		}

		// The final output statements

		write(1, "Checking whether the directory has been created: ", 49); write(1, directoryCreation, 3);
		write(1, "\nChecking whether the file has been created: ", 45);	write(1, fileCreation, 3);
		write(1, "\nChecking whether the symlink has been created: ", 48); write(1, symlinkCreation, 3);	

		write(1, "\n\nChecking whether file contents have been reversed and case-inverted: ", 71); write(1, inputTransformation, 3);

		write(1, "\n\nUser has read permission on file: ", 36); write(1, userReadFile, 3);
		write(1, "\nUser has write permission on file: ", 36); write(1, userWriteFile, 3);	
		write(1, "\nUser has execute permission on file: ", 38); write(1, userExecuteFile, 2);

		write(1, "\n\nGroup has read permission on file: ", 37); write(1, groupReadFile, 2);
		write(1, "\nGroup has write permission on file: ", 37); write(1, groupWriteFile, 2);
		write(1, "\nGroup has execute permission on file: ", 39); write(1, groupExecuteFile, 2);

		write(1, "\n\nOthers has read permission on file: ", 38); write(1, othersReadFile, 2);
		write(1, "\nOthers has write permission on file: ", 38); write(1, othersWriteFile, 2);
		write(1, "\nOthers has execute permission on file: ", 40); write(1, othersExecuteFile, 2);

		write(1, "\n\nUser has read permission on directory: ", 41); write(1, userReadDirectory, 3);
		write(1, "\nUser has write permission on directory: ", 41); write(1, userWriteDirectory, 3);
		write(1, "\nUser has execute permission on directory: ", 43); write(1, userExecuteDirectory, 2);

		write(1, "\n\nGroup has read permission on directory: ", 42); write(1, groupReadDirectory, 2);
		write(1, "\nGroup has write permission on directory: ", 42); write(1, groupWriteDirectory, 2);
		write(1, "\nGroup has execute permission on directory: ", 44); write(1, groupExecuteDirectory, 2);

		write(1, "\n\nOthers has read permission on directory: ", 44); write(1, othersReadDirectory, 2);
		write(1, "\nOthers has write permission on directory: ", 43); write(1, othersWriteDirectory, 2);
		write(1, "\nOthers has execute permission on directory: ", 45); write(1, othersExecuteDirectory, 2);
		write(1, "\n", 1);

		chmod(src, 0600); //Modifies the permissions of the output file (the source of the symlink) in the directory 'Assignment' to read/write permissions for the user (the original state)

    	chmod("./Assignment", 0600); //Modifies the permissions of the directory 'Assignment' to read/write permissions for the user (the original state)

	}
	else{
		write(1, "Assignment directory has not been created.\n", 43);
		return 0;
	}
	return 0;
}