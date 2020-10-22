#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LABEL_MAX_LEN 32
#define OPERATOR_MAX_LEN 32

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

label_node* setLabel(label_node** label_table, char* label, int address){
	int idx = hash(label, sizeof(label_table)/sizeof(label_node*));
	label_node* new_node = make_label_node(label, address);

	if(label_table != NULL){
		label_node* starting_node = label_table[idx];
		if(starting_node == NULL){
			label_table[idx] = new_node;
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


label_node* getLabel(label_node** label_table, char* label){
	int idx = hash(label, sizeof(label_table)/sizeof(label_node*));
	if(label_table != NULL){
		label_node* starting_node = label_table[idx];
		while(strcmp(starting_node->label, label) != 0){
			starting_node = starting_node->next;
		}
		return starting_node;
	}
	return NULL;
}	


int main(void){
	return 0;
}
