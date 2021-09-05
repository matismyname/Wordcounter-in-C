#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct words {
	char word[20];
	struct words *raddr;
	struct words *laddr;
	int count;
};

#define NUMOFWORDS 20	//Number of most occuring words

struct words *makestruct(char temparr[]);
struct words *comparestruct(struct words *currentptr, char temparr[]);
int cmpstr(char s[], char t[]);
int cmparrvals(int ind);
void wordcount(int wcr, char temparr[]);
void freenode(struct words *ptr);

char wchar[NUMOFWORDS][20];	//Array to save the words, max word length is 20
int wc[NUMOFWORDS];		//Array to save the amount of occurences
int min = 1;

int main(int argc, char **argv) {

	struct words *rootptr = NULL;
	char temparr[20];
	FILE *fptr;
	fptr = fopen(argv[1], "r");
	
	char ch;
	int index = 0;
	for(int i = 0; i<NUMOFWORDS; i++) { wc[i] = 1; }

	while((ch = fgetc(fptr)) != EOF) {
		if(ch != '\n') {
			temparr[index] = tolower(ch);
			index++;
		}
		else {
			temparr[index] = '\0';	
			rootptr = comparestruct(rootptr, temparr);
			
			for(int i = 0; i <= index; i++) { temparr[i] = ' '; }
			index = 0;
		}
	}
	fclose(fptr);
	for(int i = 0; i<NUMOFWORDS; i++) {printf("Word: %s \t \t Count: %d \n", wchar[i], wc[i]);}
	freenode(rootptr);

	return 0;
}

//Delete the binary search tree
//Most modern OS do this automatically
void freenode(struct words *rootptr) {
	if(rootptr == NULL) {
		;
	}
	else {
		freenode(rootptr->laddr);
		freenode(rootptr->raddr);
		free(rootptr);
		rootptr = NULL;
	}

}

//Generates nodes for the tree data structure
struct words *makestruct(char temparr[]) {
	struct words *ptr = (struct words*)malloc(sizeof(struct words));
	int index = 0;

	while(temparr[index] != '\0') {
		ptr->word[index] = temparr[index];
		index++;
	}
	ptr->word[index] = temparr[index];
	ptr->count = 1;
	ptr->raddr = NULL;
	ptr->laddr = NULL;

	return ptr;
}

//Compares structures recursively
struct words *comparestruct(struct words *currentptr, char temparr[]) {
	if(currentptr == NULL)
		currentptr = makestruct(temparr);
	else if(cmpstr( currentptr->word, temparr ) < 0) 
		currentptr->laddr = comparestruct(currentptr->laddr, temparr);
	else if(cmpstr( currentptr->word, temparr ) > 0)
		currentptr->raddr = comparestruct(currentptr->raddr, temparr);
	else {
		currentptr->count++;
		wordcount(currentptr->count, temparr);
	}

	return currentptr;
}

// a < b < c < ... < Z
// Returns 0 when all the chars are the same, see strcmp docs
int cmpstr(char s[], char t[]) {
	int cmp = strcmp(s, t);
	return cmp;
}

//Fills the array wchar with the words and the array wc with the number of occurences of the word
void wordcount(int count, char temparr[]) {
	int ind;
	if(count <= min)
		;
	else
		for(int i = 0; i < NUMOFWORDS; i++) 
			if(wc[i] < count) { 
				if(wc[i] != 1 && (cmpstr(wchar[i], temparr) != 0) ) {
					ind = cmparrvals(i);
					if(ind != NUMOFWORDS + 1)	
						for(int k = 0; k < 20; k++) { wchar[ind][k] = wchar[i][k]; }
				}
				wc[i] = count;
				for(int k = 0; k < 20; k++) {wchar[i][k] = temparr[k];}
				break;
			}
	for(int i = 0; i<NUMOFWORDS; i++)
		if(wc[i] < min)
			min = wc[i];
}

//Returns the index the smallest occurence in wchar
int cmparrvals(int i) {
	int ret = NUMOFWORDS+1;
	for(int k = i+1; k<NUMOFWORDS; k++) {
		if(wc[i] < wc[k])
			;
		else {
			ret = k;
	       		break;
		}
	}
	return ret;
}

