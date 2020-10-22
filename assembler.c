#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define LINE_MAX_LEN 512

char* source_path;
char* object_path;

char line_to_parse[LINE_MAX_LEN];

FILE* input_file;
FILE* output_file;

void do_path1(){
	while(fgets(line_to_parse,LINE_MAX_LEN,input_file)){
		printf("%s\n",line_to_parse);
	}
}

int main (int argc, char* argv[]){
	if(argc < 3){
		printf("you need source.asm and object.asm");
		return 0;
	}else{
		source_path = argv[1];
		object_path = argv[2];
	}

	input_file = fopen(source_path, "r");
	output_file = fopen(object_path, "w");

	if(input_file == NULL || output_file == NULL){
		printf("you need source.asm and object.obj");
		return 0;
	}

	do_path1();

	return 0;
}
