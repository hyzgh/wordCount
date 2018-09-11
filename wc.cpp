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
		// 构造函数，设置各初始值为0
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

		/* 假如需要统计字符数或词数或行数，调用countBasic函数
		 * 假如需要统计空白行、代码行和注释行，调用countBasic函数
		 */
		void count(map<char, bool> mode) {
			if(mode['c'] || mode['w'] || mode['l'])
				countBasic(pFile);
			if(mode['a'])
				countSpecialLines();
		}

		// 统计字符、词、行
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

		/* 统计空白行、代码行、注释行
		 * 要确定一行是不是空白行，只要通过判断该行的可打印字符个数是否不超过1个即可。另外还要注意，该行不能在块注释中。
		 * 要确定一行是不是代码行，只要判断该行的可打印字符个数是否超过1个即可。另外还要注意，改行不在块注释中。
		 * 假如一行是注释行，那么该行首先必须不是代码行，其次，需要有注释标志。
		 * 根据以上信息，可写出正则表达式进行匹配。
		 */
		void countSpecialLines() {
			string line;
			ifstream file(fileName);
			bool blockCommentFlag = false;
			string blankLineRegex = "(\\s*)([{};]?)(\\s*)";
			string lineCommentRegex = "(\\s*)([{};]?)(\\s*)(//)(.*)";
			string blockCommentStartFlagRegex = "(\\s*)([{};]?)(\\s*)(/{1})(\\*{1})(.*)";
			string blockCommentStartFlag1Regex = "(.*)(/{1})(\\*{1})(.*)";
			string blockCommentCloseFlagRegex = "(.*)(\\*{1})(/{1})(\\s*)";
			while (getline(file, line)) {
				if(blockCommentFlag) {
					commentLines++;
					if(regex_match(line, regex(blockCommentCloseFlagRegex))) {
						blockCommentFlag = false;
					}
				}
				else if(regex_match(line, regex(blankLineRegex))) {
					blankLines++;
				}
				else if(regex_match(line, regex(lineCommentRegex))) {
					commentLines++;
				}
				else if(regex_match(line, regex(blockCommentStartFlagRegex))) {
					commentLines++;
					blockCommentFlag = true;
				}
				else if(regex_match(line, regex(blockCommentStartFlag1Regex))) {
					codeLines++;
					blockCommentFlag = true;
				}
				else {
					codeLines++;
				}
			}
			file.close();
		}

		// 输出
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

// 设置参数模式，假如是非法参数，返回0，否则返回1
bool setMode(char *s, map<char, bool> &mode) {
	for(int i = 1; i < strlen(s); i++)  {
		char c = s[i];
		if(c == 'c' || c == 'w' || c == 'l' || c == 's' || c == 'a')
			mode[c] = true;
		else
			return false;
	}
	return true;
}

// 统计某一文件
void readFile(char *s, map<char, bool> mode) {
	File *file = new File(s);
	file->count(mode);
	file->print(mode);
}

// 递归统计某一目录下的所有文件
void recursiveReadFiles(DIR *dir, map<char, bool> mode) {
	DIR *ndir;
	struct dirent *ent;
	while((ent = readdir(dir)) != NULL) {
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
	// 处理未输出参数的情况
	if(argc == 1) {
		printf("Please specify parameters!\n");
		return -1;
	}
	// 设置参数
	for(int i = 1; i < argc; i++) {
		if(argv[i][0] == '-') {
			if(!setMode(argv[i], mode)) {
				printf("Invalid option!\n");
				return -1;
			}
		}
	}
	/* 假如是文件，调用readFile函数
	 * 假如是目录，调用recursiveReadFiles函数
	 */
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
