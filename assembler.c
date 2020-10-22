#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX_LEN 512
#define LABEL_MAX_LEN 32
#define OPERAND_MAX_LEN 32
#define OPERATOR_MAX_LEN 32

char* source_path;
char* object_path;

char line_to_parse[LINE_MAX_LEN];

char label[LABEL_MAX_LEN];
char operator[OPERATOR_MAX_LEN];
char operand[OPERAND_MAX_LEN];

int line_idx = 0;

FILE* input_file;
FILE* output_file;

void read_token(char* line_to_parse, char* place_to_store){
	int token_idx = 0;
	while(!(line_to_parse[line_idx] == ' ' || line_to_parse[line_idx] == '\t' || line_to_parse[line_idx] == '\n')){
		place_to_store[token_idx] = line_to_parse[line_idx];
		token_idx++;
		line_idx++;
	}
	place_to_store[token_idx] = '\0';
	return;
}

void read_blank(char* line_to_parse){
	while(line_to_parse[line_idx] == ' ' || line_to_parse[line_idx] == '\t')
		line_idx++;
}

void parse_line(char* line_to_parse, char* label, char* operator, char* operand){
	line_idx = 0;
	read_token(line_to_parse,label);
	read_blank(line_to_parse);
	read_token(line_to_parse,operator);
	read_blank(line_to_parse);
	read_token(line_to_parse,operand);
	return;
}

void do_path1(){
	while(fgets(line_to_parse,LINE_MAX_LEN,input_file)){
		if(line_to_parse[0] == '.')
			continue;

		parse_line(line_to_parse,label,operator,operand);
		if(strlen(label) + strlen(operator) + strlen(operand) == 0)
			continue;

		printf("%s %s %s\n",label, operator, operand);
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
