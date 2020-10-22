#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX_LEN 512
#define LABEL_MAX_LEN 32
#define OPERAND_MAX_LEN 32
#define OPERATOR_MAX_LEN 32

char* source_path;
char* object_path;
char* intermediate_path = "./interi.txt";

char line_to_parse[LINE_MAX_LEN];

char label[LABEL_MAX_LEN];
char operator[OPERATOR_MAX_LEN];
char operand[OPERAND_MAX_LEN];

int line_idx = 0;
int location_counter = 0;
int starting_address = 0;

FILE* input_file;
FILE* output_file;
FILE* intermediate_file;

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
	intermediate_file = fopen(intermediate_path,"w");
	
	/* read first line */
	fgets(line_to_parse,LINE_MAX_LEN,input_file);
	if(strcpy(operator,"START")==0){
		int opd = (int)strtol(operand, NULL, 10);
		starting_address = opd;
		location_counter = starting_address;
		fprintf(intermediate_file, "%-32s, %-32d\n", operator, opd);
		fgets(line_to_parse,LINE_MAX_LEN,input_file);
	}else{
		location_counter = 0;
	}
	while(strcmp(operator, "END") != 0){
		if(line_to_parse[0] == '.')
			continue;
		if( strlen(label) != 0 ){
			// searching symbol tab for label
		}

		//search optab
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
