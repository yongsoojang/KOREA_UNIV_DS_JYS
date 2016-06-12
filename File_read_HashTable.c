#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//=======================================================================================

struct User{
	char id[10];
	struct User_Profile* profile;
	//struct friend* friend;
	struct User* next;
	//struct Tweet* tweet;
};

struct User_Profile{
	char sign_up_date[50];
	char screen_name[20];
};

struct User* alloc_user(){

	struct User* user = (struct User*)malloc(sizeof(struct User));
	struct User_Profile* user_profile = (struct User_Profile*)malloc(sizeof(struct User_Profile));
	user->profile = user_profile;
	user->next = NULL;
	//user->friend = NULL;
	//user->tweet = NULL;
	return user;
}

struct User* create_user( char id[], char sign_up_date[], char screen_name[] ){

	struct User* user = alloc_user();
	
	sscanf(id, "%s", user->id);
	sscanf(screen_name,"%s",user->profile->screen_name);
	strcpy(user->profile->sign_up_date,sign_up_date);
	
	return user;
}




//====================================================================================================================

struct HashTable{
	int key;
	struct HashTable* next;
	struct User* user;
};

struct HashTable* alloc_HashTable()
{
	struct HashTable* hashtable = (struct HashTable*)malloc(sizeof(struct HashTable));
	hashtable->next = NULL;
	hashtable->user = NULL;
	return hashtable;
}

struct HashTable* insert_HashTable(struct HashTable* table, int key)
{
	struct HashTable* hashtable = alloc_HashTable();
	hashtable->key = key;
	hashtable->next = table;
	return hashtable;
}

int key_of_HastTable(char id[])
{
	int id_for_hash;
	int i;
	for(i=0;i<strlen(id);i++)
	{
		if(i%2==0)
		{
			id_for_hash  = id_for_hash + (id[i]-48)*10;
		}
		else
		{
			id_for_hash  = id_for_hash + id[i]-48;
		}
	}
	return id_for_hash%100;
}

struct HashTable* find_key(struct HashTable* table, int key)
{
	if(key>99)
	{
		printf("범위를 넘어섰습니다\n");
		exit(1);
	}
	
	struct HashTable* p = table;
	for( ; p != NULL ; p=p->next)
	{
		if( p->key == key )
			return p;
	}	
}

void add_user_in_HashTable(struct HashTable* key_place,struct User* user)
{
	if( key_place->user == NULL)
	{
		key_place->user = user;
	}
	else
	{
		user->next = key_place->user;
		key_place->user = user;
	}
}

//===================================================================================


void print_user(struct HashTable* hashtable) 
{	
	struct HashTable *p = hashtable;
	
	for( ; p != NULL; p= p->next)
	{
		printf("Hash Key = %d \n",p->key);
		struct User* q = p->user;
		for( ; q!= NULL; q=q->next)
		{
			printf("%s\n",q->id);	
			printf("%s",q->profile->sign_up_date);
			printf("%s\n",q->profile->screen_name);	
			printf("\n\n");
		}
		printf("==================================\n");
	}
}





void main()
{


//====================HashTable=========================
	int i;
	struct HashTable* hashtable = NULL;
	for(i=0;i<100;i++) // 100개 생성
	{
		hashtable = insert_HashTable(hashtable,i);
	}
// =========================================================
	FILE *fa;

	char tstr[1000];
	char screen_name[50];
	char sign_up_date[50];
	char id[10];
	int id_for_hash;

	fa = fopen("user.txt","r");
	
	int num=0;
	

	while( fgets(tstr,50,fa) != NULL )
	{
		
		if(num%4 == 0)
		{
			id_for_hash = 0;
			sscanf(tstr,"%s",id);
			id_for_hash = key_of_HastTable(id);
		}
		else if(num%4 == 1)
		{
			strcpy(sign_up_date,tstr);
		}
			
		else if(num%4 == 2)
		{
			
			sscanf(tstr,"%s",screen_name);
			struct HashTable* p = find_key(hashtable,id_for_hash);
			
			struct User* user = create_user(id,sign_up_date,screen_name);

			add_user_in_HashTable(p,user);
			
		} 
		
		
		num = num + 1;
			
		
	}
	fclose(fa);
	print_user(hashtable);
}