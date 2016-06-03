#include <stdio.h>
#include <stdlib.h>


struct User{
	char id[10];
	char name[20];
	struct User* next;
};

struct User* alloc_user(){
	struct User* user = (struct User*)malloc(sizeof(struct User));
	user->next = NULL;
	return user;
}

struct User* insert_user(struct User* list, char id[], char name[] ){

	struct User* user = alloc_user();
	if(list==NULL)
	{
		sscanf(id, "%s", user->id);
		sscanf(name,"%s",user->name);
		list = user;
		
	}
	else
	{
		sscanf(id, "%s", user->id);
		sscanf(name,"%s",user->name);
		user->next = list;
	}
	
	return user;
}


void print_user(struct User* list) 
{	
	struct User* p = list;
	int i;
	
	for(  ; p != NULL ; p=p->next)
	{
		printf("%s\n",p->id);
		printf("%s\n",p->name);
	}


}

void main()
{
	FILE *fa;

	char tstr[50];
	char name[50];
	char id[50];

	fa = fopen("user.txt","r");
	
	int num=0;
	
	struct User* user_list = NULL;

	while( fgets(tstr,50,fa) != NULL )
	{
		
		if(num%4 == 0)
		{
			
			sscanf(tstr,"%s",id);
			
		}
		else if(num%4 == 2)
		{
			
			sscanf(tstr,"%s",name);
			
			user_list = insert_user(user_list, id, name);
		}
		
		num = num + 1;
			
		
	}
	fclose(fa);
	
	print_user(user_list);
	
}