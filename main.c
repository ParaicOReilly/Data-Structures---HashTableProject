#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

//Set string size and array size
#define max_name 256
#define table_size 99991

//entry struct:contains a name and it's frequency in the list

typedef struct entry entry;
struct entry
{
char Person_ID[max_name];
char Deposition_ID[max_name];
char Surname[max_name];
char Forename[max_name];
char Age[max_name];
char Person_Type[max_name];
char Gender[max_name];
char Nationality[max_name];
char Religion[max_name];
char Occupation[max_name];
entry * next;
};

//hash function
int hash(char* s)
{
int hash = 0;
while(*s){
hash = (hash + *s);
s++;
}
return hash % table_size;
}

//Global Variables
entry * hash_table[table_size];
int collisions=0;
int terms=0;

//Function Definitions
void create_table();
entry* create_entry(char *input);
entry *search(char * input);
void addorincrement(char* name);
void next_token ( char *buf, FILE *f, int string_max );
int load_file ( char *fname );
void print();
bool insert(entry * n);
void fill_entry(FILE *csv, entry *n);
void print_list(entry*n);


int main()
{
create_table();
load_file("people.csv");
double term = terms*1.0;
double load = term/99999991.0;
printf(" Capacity: %i\n",table_size);
printf(" Num Terms: %i\n",terms);
printf(" Collisions: %i\n",collisions);
printf(" Load: %f\n",load);


char PersonID[10] = "Person_ID";
char Depositions[11] = "Deposition_ID";
char Surname[10] = "Surname";
char Forename[10] = "Forename";
char Age[10] = "Age       ";
char PersonType[10] = "PersonType";
char Gender[10] = "Gender";
char Nationality[11] = "Nationality";
char Religion[10] = "Relgiion";
char Occupation[10] = "Occupation";
    
char name[20];
char quit[20] = "quit";
entry* tmp;
printf("Enter term to get frequency or type \"quit\" to escape\n");
//Loop to check freqeuncies
while(strcmp(name,"quit")!=0)
{

    
    scanf("%s",name);
    tmp = search(name);
    if(strcmp(name,quit)==0)
    {
      printf(">>> ");
      break;
    }
    if(tmp==NULL)
    {
     printf("%s not in table \n",name);
     
    }
    if(tmp!=NULL )
    {
      //printf("PersonID    DepositionID  Surname    Forename   Age       PersonType  Gender     Nationality Religion   Occupation\n");
      printf("\nThe entries that match the forename entered are:\n\n");
      printf("%10s", PersonID);
      printf("%13s",Depositions);
      printf("%10s",Surname);
      printf("%10s",Forename);
      printf("%10s",Age);
      printf("%12s",PersonType);
      printf("%10s",Gender);
      printf("%12s",Nationality);
      printf("%10s",Religion);
      printf("%12s\n",Occupation);
      print_list(tmp);
    }
}
return 0;
}

bool insert(entry * n)
{
    int index = hash(n->Surname);
    entry*tmp=hash_table[index];
    //PUTS INTO LINKED LIST IF EMPTY
    if(tmp==NULL)
    {
    n->next = hash_table[index];
    hash_table[index]=n;
    return(true);
    }
    if(strcmp(n->Surname,hash_table[index]->Surname)==0)
    {
     n->next = hash_table[index];
     hash_table[index]=n;
     return(true);
    }
    while(tmp!=NULL)
    {
        index++;
        collisions++;
        tmp=hash_table[index];
    }
    n->next = hash_table[index];
    hash_table[index]=n;
    return(true);

}

//Takes a char value returns the entry if present ,NULL otherwise

entry *search(char *name)
{
 int index = hash(name);
 if(hash_table[index]!=NULL && strncmp(hash_table[index]->Surname,name,table_size)==0 && strcmp(name,"quit")!=0)
 {return hash_table[index];}

 for(int i=0;i<table_size-index;i++)
 {
 int probe=i+index;
 if(hash_table[probe]!=NULL && strncmp(hash_table[probe]->Surname,name,table_size)==0 &&strcmp(name,"quit")!=0)
 {
  return hash_table[probe];
 }
 }
 /*left of index
 for(int i=0;i<index;i++)
 {
 int probe=i;
 if(hash_table[probe]!=NULL && strncmp(hash_table[probe]->Surname,name,table_size)==0 && strcmp(name,"quit")!=0)
 {
  return hash_table[probe];
 }*/
  return NULL;
  }


//Parser crap
int next_field( FILE *f, char *buf, int max ) {
	int i=0, end=0, quoted=0;
	
	for(;;) {
		// fetch the next character from file		
		buf[i] = fgetc(f);
		// if we encounter quotes then flip our state and immediately fetch next char
		if(buf[i]=='"') { quoted=!quoted; buf[i] = fgetc(f); }
		// end of field on comma if we're not inside quotes
		if(buf[i]==',' && !quoted) { break; }
		// end record on newline or end of file
		if(feof(f) || buf[i]=='\n') { end=1; break; } 
		// truncate fields that would overflow the buffer
		if( i<max-1 ) { ++i; } 
	}

	buf[i] = 0; // null terminate the string
	return end; // flag stating whether or not this is end of the line
}


int load_file ( char *fname ) 
{
  FILE *f;
  char buf[max_name];

  // boiler plate code to ensure we can open the file
  f = fopen(fname, "r");
  if (!f) 
  { 
	 printf("Unable to open %s\n", fname);
	 return 0; 
  }
  else
  {
    printf("File %s loaded\n",fname);
  }
	
	// read until the end of the file
	while ( !feof(f) ) 
    {
     entry *new_entry = (entry*) malloc(sizeof(entry));
     fill_entry(f,new_entry); 
    }
    
	// always remember to close your file stream
    fclose(f);
   
	return 1;
}

//Initializes table
void create_table()
{
    for(int i=0;i<table_size;i++)
    {hash_table[i]=NULL;}
}



void fill_entry(FILE *csv, entry *n)
{
    char buf[max_name];
    next_field( csv, n->Person_ID, max_name);
    next_field( csv, n->Deposition_ID, max_name );
    next_field( csv, n->Surname, max_name );
    next_field( csv, n->Forename, max_name ); 
    next_field( csv, n->Age, max_name );
    next_field( csv, n->Person_Type, max_name);
    next_field( csv, n->Gender, max_name);
    next_field( csv, n->Nationality, max_name);
    next_field( csv, n->Religion, max_name);
    next_field( csv, n->Occupation, max_name);
    insert(n);
    terms++;
}

void print_entry(entry* n)
{
printf("%10s",n->Person_ID);
printf("%14s",n->Deposition_ID);
printf("%10s",n->Surname);
printf("%10s",n->Forename);
printf("%10s",n->Age);
printf("%10s",n->Person_Type);
printf("%10s",n->Gender);
printf("%12s",n->Nationality);
printf("%12s",n->Religion);
printf("%12s\n",n->Occupation);
}

void print_list(entry*n)
{

entry*tmp=n;
print_entry(tmp);
while(tmp->next!=NULL)
{
tmp=tmp->next;
print_entry(tmp);
}
}