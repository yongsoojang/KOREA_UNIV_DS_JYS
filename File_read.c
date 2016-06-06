#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User* find_user(struct User* list, char id[]);

struct User{
	char id[10];
	struct User_Profile* profile;
	struct friend* friend;
	struct User* next;
};

struct User_Profile{
	char sign_up_date[40];
	char screen_name[20];
};

struct friend{
	char friend_id[10];
	struct friend* next;
};

void friend_init(struct friend* self)
{
	self->next = NULL;
}

void add_friend(struct User* list, char self_id[], char friend_id[])
{
	
	struct User* user = find_user(list,self_id);
	if(user == NULL)
	{
		return;
	}
	
	struct friend * friend = (struct friend*)malloc(sizeof(struct friend));
	friend_init(friend);
	sscanf(friend_id, "%s", friend->friend_id);
	
	
	if(user->friend == NULL)
	{	
		user->friend = friend;
	}
	
	else
	{
		friend->next = user->friend;
		user->friend = friend;
	}	
	
}

struct User* find_user(struct User* list, char id[])
{
	struct User* p = list;
	
	for(  ; p != NULL ; p=p->next)
	{
		if( strcmp( p->id , id ) ==0 )
		{
			return p;
		}
	}
}

struct User* alloc_user(){

	struct User* user = (struct User*)malloc(sizeof(struct User));
	struct User_Profile* user_profile = (struct User_Profile*)malloc(sizeof(struct User_Profile));
	user->profile = user_profile;
	user->next = NULL;
	user->friend = NULL;
	return user;
}

struct User* insert_user(struct User* list, char id[], char sign_up_date[], char screen_name[] ){

	struct User* user = alloc_user();
	if(list==NULL)
	{
		sscanf(id, "%s", user->id);
		sscanf(screen_name,"%s",user->profile->screen_name);
		strcpy(user->profile->sign_up_date,sign_up_date);
		list = user;
		
	}
	else
	{
		sscanf(id, "%s", user->id);
		sscanf(screen_name,"%s",user->profile->screen_name);
		strcpy(user->profile->sign_up_date,sign_up_date);
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
		printf("%s",p->profile->sign_up_date);
		printf("%s\n",p->profile->screen_name);
		printf("========friend_list========\n");
		for( ;p->friend != NULL ; p->friend = p->friend->next)
		{
			printf("%s\n",p->friend);
		}
		printf("\n");
		printf("\n");
	}


}

void main()
{
	FILE *fa;

	char tstr[50];
	char screen_name[50];
	char sign_up_date[50];
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
		else if(num%4 == 1)
		{
			strcpy(sign_up_date,tstr);
		}
			
		else if(num%4 == 2)
		{
			
			sscanf(tstr,"%s",screen_name);
			
			user_list = insert_user(user_list, id, sign_up_date ,screen_name);
		}
		
		num = num + 1;
			
		
	}
	fclose(fa);
	
	fa = fopen("friend.txt","r");
	
	num = 0;
	char my_id[10];
	char friend_id[10];
	add_friend(user_list, "491963952", "341312698");
	
	while( fgets(tstr,10,fa) != NULL )
	{
		if(num%3 == 0)
		{
			sscanf(tstr,"%s",my_id);
		}
		else if(num%3 == 1)
		{
			sscanf(tstr,"%s",friend_id);
			add_friend(user_list, my_id, friend_id);
		}
		num = num + 1;
	
	}
	
	print_user(user_list);
	fclose(fa);
	
}