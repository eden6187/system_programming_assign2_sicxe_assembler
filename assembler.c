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

op op_set[64] = {
	{"ADD",3,0x18},
	{"ADDF", 3,0x58},
	{"ADDR",2,0x90},
	{"AND",3,0x40},
	{"CLEAR",2,0x4},
	{"COMP",3,0x28},
	{"COMPF",3,0x88},
	{"DIV",3,0x24},
	{"DIVF",0x64},
	{"DIVR",2,0x9c},
	{"FIX",1,0xc4},
	{"FLOAT",1,0xc0},
	{"HIO",1,0xf4},
	{"J",3,0x3c},
	{"JEQ",3,0X30},
	{"JGT",3,0X34},
	{"JLT",3,0X38},
	{"JSUB",3,0X48},
	{"LDA",3,0X00},
	{"LDB",3,0X68},
	{"LDCH",3,0X50},
	{"LDF",3,0X70},
	{"LDL",3,0X08},
	{"LDS",3,0X6C},
	{"LDT",3,0X74},
	{"LDX",3,0X04},
	{"LPS",3,0XD0},
	{"MUL",3,0x20},
	{"MULF",3,0x60},
	{"MULR",2,0X98},
	{"NORM",1,0XC8},
	{"OR",3,0X44},
	{"RD",3,0XD8},
	{"RMO",2,0XAC},
	{"RSUB",3,0X4C},
	{"SHIFTL",2,0XA4},
	{"SHITFR",2,0XA8},
	{"SIO",1,0XF0},
	{"SSK",3,"0XEC"},
	{"STA",3,"0X0C"},
	{"STB",3,0X78},
	{"STCH",3,0X54},
	{"STF",3,80},
	{"STI",3,0XD4},
	{"STL",3,0X14},
	{"STS",3,0X7C},
	{"STSW",3,0XE8},
	{"STT",3,0X84},
	{"STX",3,0X10},
	{"SUB",3,0X1C},
	{"SUBF",3,0X5C},
	{"SUBR",2,0X94},
	{"SVC",2,0XB0},
	{"TD",3,0XE0},
	{"TIO",1,0XF8},
	{"TIX",3,0X2C},
	{"TIXR",2,0XB8},
	{"WD",3,0XDC}
};



typedef struct op_t{
	char *mnemoic;
	int format;
	int op_code;	
}op;

typedef struct op_node_t{
	struct op_node_t* next;
	char* mnemonic;
	int op_code;
	int format;
}opnode;


typedef struct label_node_t{
	struct label_node_t* next;
	char* label;
	int address;
}label_node;

opnode* make_op_node(char* mnemonic, int op_code, int format){
	opnode* new_node = (opnode*)malloc(sizeof(opnode));
	char new_string[LABEL_MAX_LEN];
	strcpy(new_string, mnemonic);
	new_node->mnemonic = new_string;

	new_node->format = format;

	new_node->op_code = op_code;

	new_node->next = NULL;

	return new_node;
}

label_node* make_label_node(char* label, int address){
	label_node* new_node = (label_node*)malloc(sizeof(label_node));
	char new_string[OPERATOR_MAX_LEN];
	strcpy(new_string, label);
	new_node->label = new_string;

	new_node->address = address;

	new_node->next = NULL;

	return new_node;
}

opnode** make_op_table(int table_size){
	opnode** op_table = (opnode**)malloc(table_size * sizeof(opnode*));
	return op_table;
}


label_node** make_label_table(int table_size){
	label_node** label_table = (label_node**)malloc(table_size * sizeof(label_node*));
	return label_table;
}

int hash(char* key, int table_size){
	int sum = 0;
	for(int i = 0 ; i < strlen(key); i++){
		sum += key[i];
	}
	return sum % table_size;
}

opnode* getOp(opnode** optable, char* mnemonic){
	int idx = hash(mnemonic, sizeof(optable)/sizeof(opnode*));
	if(optable != NULL){
		opnode* starting_node = optable[idx];
		while(strcmp(starting_node->mnemonic, mnemonic) != 0){
			starting_node = starting_node->next;
		}
		return starting_node;
	}
	return NULL;
}


opnode* setOp(opnode** optable, char* mnemonic, int opcode, int format){

	int idx = hash(mnemonic, sizeof(optable)/sizeof(opnode*));
	opnode* new_node = (opnode*)malloc(sizeof(opnode));
	char new_mnemoic[LABEL_MAX_LEN];
	strcpy(new_mnemoic, mnemonic);

	new_node->mnemonic = new_mnemoic;
	new_node-> op_code = opcode;
	new_node->format = format;
	new_node->next = NULL;

	if(optable != NULL){
		opnode* starting_node = optable[idx];
		if(starting_node == NULL){
			optable[idx] = new_node;
			return new_node;
		}else{
			while(starting_node->next != NULL){
				starting_node = starting_node -> next;
			}
			starting_node->next = new_node;
			return new_node;
		}

	}

	return NULL;
}

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
