#include "textfile.h"

char* textFileRead(const char * filename){
	FILE * fp;
	char * content = NULL;
	int count = 0;
	if(filename != NULL){
		fp = fopen(filename, "rt");

		if(fp != NULL){
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if(count > 0){
				content = (char *)malloc(sizeof(char) * (count + 1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

int textFileWrite(char * filename, char * s){
	FILE * fp;
	int status = 0;
	if(filename != NULL){
		fp = fopen(filename, "w");
		if(fp != NULL){
			if(fwrite(s, sizeof(char), strlen(s), fp)== strlen(s)){
				status = 1;
			}
			fclose(fp);
		}
	}
	return status;
}
