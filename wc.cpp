#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <cstddef>
#include <string>
#include <cstdio>
#include <regex>
#include <map>
using namespace std;

class File {
	private:
		FILE *pFile;
		string fileName;
		int characters, words, lines, blankLines, codeLines, commentLines;

	public:
		File(char *s) {
			this->pFile = fopen(s, "r");
			this->fileName = s;
			characters = 0;
			words = 0;
			lines = 0;
			blankLines = 0;
			codeLines = 0;
			commentLines = 0;
		}

		void count(map<char, bool> mode) {
			if(mode['c'] || mode['w'] || mode['l'])
				countBasic(pFile);
			if(mode['a'])
				countSpecialLines();
		}

		void countBasic(FILE *pFile) {
			char c;
			bool spaceFlag = true;
			while((c = fgetc(pFile)) != EOF) {
				characters++;

				words += (spaceFlag == true && isgraph(c));
				spaceFlag = isspace(c);

				lines += c == '\n';
			}
		}

		void countSpecialLines() {
			string line;
			ifstream file(fileName);
			bool blockCommentFlag = false;
			string blankLineRegex = "(\\s*)([{};]?)(\\s*)";
			string lineCommentRegex = "(\\s*)([{};]?)(\\s*)(//)(.*)";
			string blockCommentRegex = "(\\s*)([{};]?)(\\s*)(/{1})(\\*{1})(.*)";
			string blockComment1Regex = "(.*)(/{1})(\\*{1})(.*)";
			string blockCommentCloseRegex = "(.*)(\\*{1})(/{1})(\\s*)";
			while (getline(file, line)) {
				if(blockCommentFlag) {
					commentLines++;
					if(regex_match(line, regex(blockCommentCloseRegex))) {
						blockCommentFlag = false;
					}
				}
				else if(regex_match(line, regex(blankLineRegex))) {
					blankLines++;
				}
				else if(regex_match(line, regex(lineCommentRegex))) {
					commentLines++;
				}
				else if(regex_match(line, regex(blockCommentRegex))) {
					commentLines++;
					blockCommentFlag = true;
				}
				else if(regex_match(line, regex(blockComment1Regex))) {
					codeLines++;
					blockCommentFlag = true;
				}
				else {
					codeLines++;
				}
			}
			file.close();
		}

		void print(map<char, bool> mode) {
			if(mode['c'])
				printf("%6d", characters);
			if(mode['w'])
				printf("%6d", words);
			if(mode['l'])
				printf("%6d", lines);
			if(mode['a'])
				printf("%6d%6d%6d", blankLines, codeLines, commentLines);
			putchar('\n');
		}
};

bool setMode(char *s, map<char, bool> &mode) {
	for(int i = 1; i < strlen(s); i++)  {
		char c = s[i];
		if(c == 'c' || c == 'w' || c == 'l' || c == 's' || c == 'a')
			mode[s[i]] = true;
		else
			return false;
	}
	return true;
}

void readFile(char *s, map<char, bool> mode) {
	File *file = new File(s);
	file->count(mode);
	file->print(mode);
}

void recursiveReadFiles(DIR *dir, map<char, bool> mode) {
	DIR *ndir;
	struct dirent *ent;
	while((ent = readdir (dir)) != NULL) {
		if(!strcmp(ent->d_name, "..") || !strcmp(ent->d_name, ".")) {
			continue;
		}
		if((ndir = opendir(ent->d_name)) != NULL) {
			recursiveReadFiles(ndir, mode);
		}
		else { 
			readFile(ent->d_name, mode);
		}
	}
	closedir(dir);
}

int main(int argc, char **argv) {
	map<char, bool> mode;
	if(argc == 1) {
		printf("Please specify parameters!\n");
		return -1;
	}
	for(int i = 1; i < argc; i++) {
		if(argv[i][0] == '-') {
			if(!setMode(argv[i], mode)) {
				printf("Invalid option!\n");
				return -1;
			}
		}
	}
	for(int i = 1; i < argc; i++) {
		if(argv[i][0] != '-') {
			DIR *dir;
			if((dir = opendir(argv[i])) != NULL && mode['s']) {
				recursiveReadFiles(dir, mode);
			}
			else if(dir) {
				printf("Cannot access '%s': This is a directory!\n", argv[i]);
				return -1;
			}
			else if(fopen(argv[i], "r") != NULL) {
				readFile(argv[i], mode);
			}
			else {
				printf("Cannot access '%s': No such file or directory!\n", argv[i]);
				return -1;
			}
		}
	}
	return 0;
}
