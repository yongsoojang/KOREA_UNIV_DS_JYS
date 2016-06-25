#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define RED 0
#define BLACK 1
#define HASH_SIZE 300
#define WHITE 2
#define GRAY 3


struct HashTable{
	int key;
	struct HashTable* next;
	struct User* RBTree;
};

struct word{
	char id[20];
	char tweet_date[50];
	char word[500];
	struct word* next;
};

struct User{
	int color;
	char id[10];
	int id_integer;
	struct User_Profile* profile;
	struct word* tweet;
	struct following* following;
	struct follower* follower;
	
	struct User* parent;
	struct User* left;
	struct User* right;
		
	
};



struct following{
	char following_id[20];
	struct following* next;
};

struct follower{
	char follower_id[20];
	struct follower* next;
};



struct User_Profile{
	char sign_up_date[50];
	char screen_name[20];
};


struct HashTable* alloc_HashTable()
{
	struct HashTable* hashtable = (struct HashTable*)malloc(sizeof(struct HashTable));
	hashtable->next = NULL;
	hashtable->RBTree = NULL;
	return hashtable;
}

struct HashTable* insert_HashTable(struct HashTable* table, int key)
{
	struct HashTable* hashtable = alloc_HashTable();
	hashtable->key = key;
	hashtable->next = table;
	return hashtable;
}

//===================================================================================

int find_key(char id[])
{	
	int key_for_hash;
	int i;
	key_for_hash = 0;
	for(i=0;i<strlen(id);i++)
	{
		if(i%2==0)
		{
			key_for_hash = key_for_hash + (int)(id[i]-48)*10;
		}
		else
		{
			key_for_hash = key_for_hash + (int)id[i]-48;
		}
	}
	return ((key_for_hash)%HASH_SIZE);
}



int find_key2(char id[])
{	
	int key_for_hash;
	int i;
	key_for_hash = 0;
	
	for(i=0;i<strlen(id);i++)
	{
		if(i%2==0)
		{
			key_for_hash = key_for_hash + (int)(id[i]-48)*10;
		}
		else
		{
			key_for_hash = key_for_hash + (int)id[i]-48;
		}
	}
	
	return ((key_for_hash)%HASH_SIZE);
}



int char_to_int(char id[])
{
	int value=0;
	int i;
	
	for(i=0;i<strlen(id);i++)
	{
		value = value + (id[i]-48)*pow(10.0,strlen(id)-i-1);
	}
	return value;
}
struct User* create_T_nil();

struct User* create_user(char id[], char sign_up_date[], char screen_name[])
{
	struct User_Profile* profile = (struct User_Profile*)malloc(sizeof(struct User_Profile));
	struct User* user = (struct User*)malloc(sizeof(struct User));
	strcpy(profile->sign_up_date, sign_up_date);
	sscanf(screen_name,"%s",profile->screen_name);
	user->profile = profile;
	sscanf(id,"%s",user->id);
	user->id_integer = char_to_int(id);
	user->color = RED;
	user->parent = NULL;
	user->right = create_T_nil();
	user->right->parent = user;
	user->left = create_T_nil();
	user->left->parent = user;
	user->tweet = NULL;
	user->following = NULL;
	user->follower = NULL;
	return user;
}

struct User* create_T_nil()
{
	struct User* user = (struct User*)malloc(sizeof(struct User));
	user->color = BLACK;
	user->id[0]=0;
	user->id_integer = 0;
	user->profile = NULL;
	user->following = NULL;
	user->follower = NULL;
	user->parent = NULL;
	user->left = NULL;
	user->right = NULL;
	user->tweet = NULL;
	return user;
}
struct HashTable* find_HashTable(struct HashTable* table, int key)
{
	if(key>=HASH_SIZE)
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

struct User *grandparent(struct User *n)
{
 	if ((n != NULL) && (n->parent != NULL))
  		return n->parent->parent;
 	else
  		return NULL;
}

struct User *uncle(struct User *n)
{
 	struct User *g = grandparent(n);
 	if (g == NULL)
  		return NULL; // No grandparent means no uncle
 	if (n->parent == g->left)
 		 return g->right;
 	else
  		return g->left;
}

void rotate_right(struct HashTable* Table, struct User *x)
{
	struct User* y = x->left;
	x->left = y->right;
	if(y->right != NULL)
	{
		(y->right)->parent = x;
	}
	y->parent = x->parent;
	if(x->parent == NULL)
	{
		(Table->RBTree) = y; 
	}
	else if(x == (x->parent)->right)
	{
		(x->parent)->right = y;
	}
	else
	{
		(x->parent)->left = y;
	}
	y->right = x;
	x->parent = y;	
}
void rotate_left(struct HashTable* Table, struct User *x)
{
	
	struct User* y = x->right;
	x->right = y->left;
	if(y->left != NULL)
	{
		(y->left)->parent = x;
	}
	y->parent = x->parent;
	if(x->parent == NULL)
	{
		(Table->RBTree) = y;
	}
	else if(x == (x->parent)->left)
	{
		(x->parent)->left = y;
	}
	else
	{
		(x->parent)->right = y;
	}
	y->left = x;
	x->parent = y;	
}

void RB_Insert_Fixup(struct HashTable* T, struct User* z)
{	
	while( (z->parent)->color == RED)
	{	
		if(z->parent == ((z->parent)->parent)->left)
		{
			struct User* y = z->parent->parent->right;
			if(y->color == RED)
			{
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
				if(z->parent == NULL)
				{
					break;
				}
			}
			else if(z == z->parent->right) 
			{
				z = z->parent;
				rotate_left(T,z);
				
			}
			else if(z == z->parent->left)
			{
				z->parent->color = BLACK;
				z->parent->parent->color =RED;
				rotate_right(T,z->parent->parent);
			}
			
		}
		else if(z->parent == z->parent->parent->right)
		{
			struct User* y = z->parent->parent->left;
			
			if(y->color == RED)
			{
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
				if(z->parent == NULL)
				{
					break;
				}
			}
			else if(z == z->parent->left)
			{ 
				z = z->parent;
				rotate_right(T,z);
				
			}
			else if(z == z->parent->right)
			{
				z->parent->color = BLACK;
				z->parent->parent->color =RED;
				rotate_left(T,z->parent->parent);
			}
		}
		
		
	}

	T->RBTree->color = BLACK;
}
void RB_Insert(struct HashTable* T, struct User* z)
{
	struct User* y = create_T_nil();
	struct User* x = T->RBTree;
	
	if(x == NULL)
	{	
		z->color = BLACK;
		T->RBTree = z;
		return ;
	}

	
	while( x->id_integer != 0 )
	{
		
		y=x;
		
		
		if( z->id_integer < x->id_integer)
			x = x->left;
		else
			x = x->right;
		
	}

	
	z->parent = y;
	
	
	if(z->id_integer <y->id_integer)
	{
		y->left = z;
	}
	else 
	{
		y->right = z;
	}
	//z->left = create_T_nil();
	//z->right = create_T_nil();
	z->color = RED;
	
	RB_Insert_Fixup(T,z);
}




void insert_user_in_table(struct HashTable *Table, char id[], char sign_up_date[], char screen_name[])
{
	int key = find_key(id);
	struct User* user = create_user(id,sign_up_date,screen_name);
	struct HashTable* p = find_HashTable(Table,key);
	RB_Insert( p , user );
}


struct User *sibling(struct User *n)
{
 	if (n == n->parent->left)
 		 return n->parent->right;
 	else
  		return n->parent->left;
}

void RB_Transplant(struct HashTable* T, struct User* u , struct User* v)
{	
	
	
	if(T->RBTree->id_integer == u->id_integer)
	{
		T->RBTree = v;
	}
	else if(u==u->parent->left)
	{	
		u->parent->left = v;
	}
	else
	{	
		u->parent->right = v;
	}
	if(v->id_integer != 0)
		v->parent = u->parent;
	
}

struct User* Tree_Minimum(struct User* x)
{
	struct User* u = NULL;
	u=x;

	struct User* k;
	k = NULL;

	
	while(u->left !=NULL)
	{
		
		
		k = u;

		u= u->left;
	}

	
	
	return k;
}


void RB_Delete_Fixup(struct HashTable* T,struct User* x)
{	
	while( x != T->RBTree && x->color == BLACK)
	{
		if( x == x->parent->left )
		{
			struct User* w = x->parent->right;
			if(w->color == RED)
			{
				w->color = BLACK;
				x->parent->color =RED;
				rotate_left(T,x->parent);
				w = x->parent->right;
			}
			if(w->left->color == BLACK && w->right->color == BLACK)
			{
				w->color = RED;
				x = x->parent;
			}
			else if(w->right->color == BLACK)
			{
				w->left->color = BLACK;
				w->color = RED;
				rotate_right(T,w);
				w = x->parent->right;
			}
			else
			{
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				rotate_left(T,x->parent);
				x = T->RBTree;
			}
		}
		else
		{
			struct User* w = x->parent->left;
			if(w->color == RED)
			{
				w->color = BLACK;
				x->parent->color =RED;
				rotate_right(T,x->parent);
				w = x->parent->left;
			}
			if(w->right->color == BLACK && w->left->color == BLACK)
			{
				w->color = RED;
				x = x->parent;
			}
			else if(w->left->color == BLACK)
			{
				w->right->color = BLACK;
				w->color = RED;
				rotate_left(T,w);
				w = x->parent->left;
			}
			else
			{
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				rotate_right(T,x->parent);
				x = T->RBTree;
			}
		}
	}
	x->color = BLACK;
}
void print(struct User* root,int space);
void RB_Delete(struct HashTable* T, struct User* z)
{	
	
	

	struct User* y = NULL;
	y=z;
	int y_original_color = y->color;
	struct User* x = NULL;
	


	if(z->left -> id_integer == 0)
	{
		x = z->right;
		RB_Transplant(T,z,z->right);
	}
	else if(z->right ->id_integer == 0)
	{
		x = z->left;
		RB_Transplant(T,z,z->left);
	}
	else
	{	

		y = Tree_Minimum(z->right);

		
		y_original_color = y->color;
		
		x = y->right;
		

		if(y->parent == z)
		{
				x->parent = y;
		}
		else
		{	
			

			RB_Transplant(T,y,y->right);
			

			y->right = z->right;
			
			y->right->parent = y;
		}


		


		RB_Transplant(T,z,y);
		
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
		
	}
	
	
	/*
	if(y_original_color == BLACK)
	{
		//RB_Delete_Fixup(T,x);
	}
	*/
}


struct User* search(struct HashTable* Table, char id[]);

void delete(struct HashTable* Table,char id[])
{
	
	

	struct User* user = NULL;
	user = search(Table,id);

	
	
	int key ;
	key = find_key2(id);
	
	


	struct HashTable* place = NULL;
	place = find_HashTable(Table,key);
	
	

	if(user != NULL)
	{	
		
		RB_Delete(place,user);
	}
}




struct User* search_in_tree(struct User* user, int value)
{
	if(user == NULL)
		return NULL;

	if(user->id_integer < value)
	{
		search_in_tree(user->right,value);
	}
	else if(user->id_integer > value)
	{
		search_in_tree(user->left,value);
	}
	else if(user->id_integer == value)
	{
		return user;
	}
}

struct User* search(struct HashTable* Table, char id[])
{
	int value = char_to_int(id);
	int key = find_key(id);
	struct HashTable* p = find_HashTable(Table, key);
	
	struct User* root = p->RBTree;
	struct User* search_user = search_in_tree(root,value);
	
	

	
	if(search_user != NULL)
	{
		//printf("id = %d \n",search_user->id_integer);
		//printf("parent = %d \n",search_user->parent->id_integer);
	}
	else
		printf("user가 존재하지 않습니다\n");
	
	return search_user;
	
}
int a = 0;

void print_Table(struct HashTable* Table)
{
	struct HashTable* t = Table;
	
}
void print(struct User* root,int space)
{
	int i;
	struct User* p = root;
	
	
	if(p != NULL)
	{
		if(p->right != NULL)
		{	
			space = space + 1;
			print(p->right,space);
		}
		
		for(i=0;i<space;i++)
		{
			printf("   ");
		}
		
		printf(" %d \n", p->id_integer);
		
	
		if(p->left != NULL)
		{
			space = space + 1;
			print(p->left,space);
		}
	
	}
	
	
}


void add_following_follower(struct HashTable* Table,char id[],char friend_id[])
{
	struct User* me = search(Table,id);
	struct User* other = search(Table,friend_id);
	
	struct following* following = (struct following*)malloc(sizeof(struct following));
	sscanf(friend_id,"%s",following->following_id);
	if(me->following == NULL)
	{
		me->following = following;
	}
	else
	{
		following->next = me->following;
		me->following = following;
	}
	
	struct follower* follower = (struct follower*)malloc(sizeof(struct follower));
	sscanf(id,"%s",follower->follower_id);
	if(other->follower == NULL)
	{
		other->follower = follower;
	}
	else
	{
		follower->next = other->follower;
		other->follower = follower;	
	}	
	
}

struct word** insert_word_in_word_list(struct word** word_list,char id[],char tweet_date[],char word[])
{
	struct word* tweet = (struct word*)malloc(sizeof(struct word));
	sscanf(id,"%s",tweet->id);
	strcpy(tweet->tweet_date,tweet_date);
	strcpy(tweet->word,word);

	if(word_list == NULL)
	{
		*word_list = tweet;
	}
	else
	{	
		tweet->next = *word_list;
		*word_list = tweet;
	}
	return word_list;
}

void insert_tweet_in_tree(struct HashTable* Table,char id[],char tweet_date[],char word[])
{
	struct word* tweet = (struct word*)malloc(sizeof(struct word));
	sscanf(id,"%s",tweet->id);
	strcpy(tweet->tweet_date,tweet_date);
	strcpy(tweet->word,word);
	
	struct User* u = search(Table,id);
	
	if(u != NULL)
	{
		if(u->tweet == NULL)
		{
			u->tweet = tweet;
		}
		else
		{
			tweet->next = u->tweet;
			u->tweet = tweet;
		}
	}
	
}

void Read_data_files(struct HashTable *Table, struct word** word_list)
{	
	int Total_users = 0;
	int Total_friendship_records = 0;
	int Total_tweets = 0;
	
	int i;
	
	struct HashTable* table = Table;
	
	char tstr[1000];
	char id[20];
	char sign_up_date[50];
	char screen_name[20];
	
	FILE *fa;
	fa = fopen("user.txt","r");

	int line=0;
	

	
	while( fgets(tstr,100,fa) != NULL )
	{
		if(line%4 == 0)
		{
			sscanf(tstr,"%s",id);
		}
		else if(line%4 == 1)
		{ 
			strcpy(sign_up_date,tstr);
		}
		else if(line%4 == 2)
		{	
			Total_users = Total_users + 1;
			sscanf(tstr,"%s",screen_name);
			insert_user_in_table(table,id,tstr,screen_name);
		}
		line = line + 1;
	}
	
	
	
	fa = fopen("friend.txt","r");
	line = 0;
	char friend_id[20];
	
	while( fgets(tstr,100,fa) != NULL )
	{
		if(line%3==0)
		{
			sscanf(tstr,"%s",id);
		}
		else if(line%3 == 1)
		{
			Total_friendship_records = Total_friendship_records + 1;
			sscanf(tstr,"%s",friend_id);
			add_following_follower(Table, id, friend_id); // following 추가 		
		}
		line = line + 1;
	}
	
	fa = fopen("word.txt","r");
	line = 0;
	char tweet_date[50];
	char word[500];
	
	while( fgets(tstr,500,fa) != NULL)
	{
		if(line%4 == 0)
		{
			sscanf(tstr,"%s",id);
		}
		else if(line%4 == 1)
		{
			strcpy(tweet_date,tstr);
		}
		else if(line%4 == 2)
		{
			Total_tweets = Total_tweets + 1;
			strcpy(word,tstr);
			word_list = insert_word_in_word_list(word_list,id,tweet_date,word);
			insert_tweet_in_tree(Table,id,tweet_date,word);	
		}
		line = line+1;
	}

	printf("\n\n\n\n\n ==================================================\n\n\n\n");
	printf("Total users: %d \n",Total_users);
	printf("Total friendship records : %d\n",Total_friendship_records);
	printf("Total tweets: %d \n",Total_tweets);
	printf("\n\n\n\n ==================================================\n");
	
}

int compute_numfriend_table(struct User* u, int * total_users, int *Maximum_of_friend, int *Minimum_of_friend)
{	
	if( u != NULL )
	{
		*total_users = *total_users + 1;
		
		if( u->id_integer == 0 )
		{
			*total_users = *total_users - 1;
			return 0;
		}
		else
		{	
			//printf("u->id  %d , u->right_id %d, u->left_id %d \n",u->id_integer,u->right->id_integer, u->left->id_integer);
			int my_friend = 0;
			int left_user_friend = 0;
			int right_user_friend = 0;
			
			if(u->following != NULL)
			{
				struct following* f = u->following;
				for( ;f != NULL; f= f->next)
				{
					//printf("friend id = %s \n",f->following_id);
					my_friend = my_friend + 1;
				}
				
				if(*Maximum_of_friend < my_friend )
				{
					*Maximum_of_friend = my_friend;
				}
				if(*Minimum_of_friend > my_friend)
				{
					*Minimum_of_friend = my_friend;
				}
				left_user_friend = compute_numfriend_table(u->left,total_users,Maximum_of_friend,Minimum_of_friend);
				right_user_friend = compute_numfriend_table(u->right,total_users,Maximum_of_friend,Minimum_of_friend);
			
				return my_friend + left_user_friend + right_user_friend;
			}
		}
	}
	else
	{
		return 0;
	}
}

void heapify(int arr[], int size)
{
	int i,j,tmp;
	
	for(i=1 ; i < size ; i++)
	{
		j = i;
		
		while( j != 0 )
		{
			if( arr[ (j-1)/2 ] > arr[j] )
			{
				tmp = arr[j];
				arr[j] = arr[ (j-1)/2 ] ;
				arr[ (j-1)/2 ] = tmp;
			}
			
			j = (j-1)/2;
		}
	}
}


void heapSort(int arr[], int size)
{
	int i, tmp;
	heapify(arr,size);
	for(i=size-1;i>0;i--)
	{	
		tmp = arr[i];
		arr[i] = arr[0];
		arr[0] = tmp;
		heapify(arr,i);
	}
}

void heapify_3(int arr[7087][3], int size)
{
	int i,j,tmp_0,tmp_1,tmp_2;
	
	for(i=1 ; i < size ; i++)
	{
		j = i;
		
		while( j != 0 )
		{
			if( arr[ (j-1)/2 ][1] > arr[j][1] )
			{
				tmp_0 = arr[j][0];
				tmp_1 = arr[j][1];
				tmp_2 = arr[j][2];
	
				arr[j][0] = arr[ (j-1)/2 ][0] ;
				arr[j][1] = arr[ (j-1)/2 ][1] ;
				arr[j][2] = arr[ (j-1)/2 ][2] ;

				arr[ (j-1)/2 ][0] = tmp_0;
				arr[ (j-1)/2 ][1] = tmp_1;
				arr[ (j-1)/2 ][2] = tmp_2;				
			}
			
			j = (j-1)/2;
		}
	}
}


void heapSort_3(int arr[7087][3], int size)
{
	int i, tmp_0, tmp_1, tmp_2;
	heapify_3(arr,size);
	for(i=size-1;i>0;i--)
	{	
		tmp_0 = arr[i][0];
		tmp_1 = arr[i][1];
		tmp_2 = arr[i][2];


		arr[i][0] = arr[0][0];
		arr[i][1] = arr[0][1];
		arr[i][2] = arr[0][2];

		arr[0][0] = tmp_0;
		arr[0][1] = tmp_1;
		arr[0][2] = tmp_2;
	
		heapify_3(arr,i);
	}
}

void display_statistics(struct HashTable* Table,struct word* word_list)
{
	int num_of_friend = 0;
	int total_users = 0;
	int Minimum_of_friends = 99999; // 크게 잡고 down 시키기
	int Maximum_of_friends = 0;
	struct HashTable* t = Table;
	for( ; t != NULL; t = t->next)
	{	
		
		num_of_friend = num_of_friend + compute_numfriend_table(t->RBTree,&total_users,&Maximum_of_friends,&Minimum_of_friends);
	}
	
	printf("==========================================================\n\n\n\n");
	printf("Average number of friends: %d \n",num_of_friend/total_users);
	printf("Maximum number of friends: %d \n",Maximum_of_friends);
	printf("Minimum number of friends: %d \n",Minimum_of_friends);
	printf("잠시만 기다려 주세요...\n");
	int total_tweet = 0;	
	struct word* w = word_list;
	for( ; w!= NULL; w= w->next)
	{
		total_tweet = total_tweet + 1;
	}
	
	int tweet_array[total_tweet];
	int i;
	for(i=0;i<total_tweet;i++)
	{
		tweet_array[i] = 0;
	}
	
	w = word_list;
	i=0;
	for( ; w!=NULL ; w=w->next )
	{
		if(search(Table,w->id) == NULL)
		{
			continue;	
		}
		tweet_array[i] = char_to_int(w->id);	
		i = i + 1;	
	}
	
	int num_real_tweet = i;
	
	heapSort(tweet_array,total_tweet);
	for(i=0;i<total_tweet;i++)
	{
		//printf("%d \n",tweet_array[i]);
	}
	
	int Maximum_tweets_per_user = 0;
	int Minimum_tweets_per_user = 1000;
	int current_num = 1;
	int current_id = tweet_array[0];

	for(i=1;i<total_tweet;i++)
	{
		if(current_id == tweet_array[i])
		{
			current_num = current_num + 1;
		}
		else
		{
			if(Maximum_tweets_per_user < current_num)
			{
				Maximum_tweets_per_user = current_num;
			}
			if(Minimum_tweets_per_user > current_num)
			{
				Minimum_tweets_per_user = current_num;
			}
			current_id = tweet_array[i];
			current_num = 1;	
		}
	}


	printf("\n\nAverage tweets per user: %d \n", num_real_tweet/total_users);
	printf("Minimum tweets per user: %d \n",Minimum_tweets_per_user);
	printf("Maximum tweets per user: %d \n",Maximum_tweets_per_user);
	printf("\n\n\n\n=======================================================\n\n\n\n");
	
}
void Top_5_most_tweeted_words(){}

void heapify_min(int arr[], int size)
{
	int i,j,tmp;
	
	for(i=1 ; i < size ; i++)
	{
		j = i;
		
		while( j != 0 )
		{
			if( arr[ (j-1)/2 ] < arr[j] )
			{
				tmp = arr[j];
				arr[j] = arr[ (j-1)/2 ] ;
				arr[ (j-1)/2 ] = tmp;
			}
			
			j = (j-1)/2;
		}
	}
}


void heapSort_min(int arr[], int size)
{
	int i, tmp;
	heapify(arr,size);
	for(i=size-1;i>0;i--)
	{
		tmp = arr[i];
		arr[i] = arr[0];
		arr[0] = tmp;
		heapify_min(arr,i);
	}
}



void Top_5_most_tweeted_users(struct word* word_list)
{
	printf("\n\n\n\n\n========================================================\n\n\n");
	printf("잠시만 기다려 주세요 \n\n\n");

	struct word* w = word_list;
	int size=0;
	for( ;w!= NULL; w= w->next)
		size = size + 1;
	
	int arr[size];
	int i;
	for(i=0;i<size;i++)
	{
		arr[i] = 0;
	}
	
	w = word_list;
	i=0;
	for(;w!=NULL;w=w->next)
	{
		arr[i] = char_to_int(w->id);
		i = i + 1;
	}
	heapSort(arr,size);
	
	int num_tweet_user = 1;
	i=1;	
	int before_num = arr[0];
	for(i=0;i<size;i++)
	{
		if(arr[i] != before_num)
		{
			before_num = arr[i];
			num_tweet_user = num_tweet_user + 1;
		}
	}
	
	int num_tweet_per_user[num_tweet_user][3]; //0 은 id 1은 갯수 2는 배열 넘버
	int j;
	int k;
	for(i=0;i<num_tweet_user;i++)
	{
		num_tweet_per_user[i][0] = 0;
		num_tweet_per_user[i][1] = 0;
		num_tweet_per_user[i][2] = i;
	}	
	
	before_num = arr[0];
	int num_tweet = 0;
	int user_id = arr[0];
	
	num_tweet_per_user[0][0] = user_id;
	j = 0;
	for(i=0;i<size;i++)
	{	
		if(arr[i] != before_num)
		{	
			num_tweet_per_user[j][0] = before_num;
			num_tweet_per_user[j][1] = num_tweet;	
			before_num = arr[i];
			j = j + 1;
			num_tweet = 1;
		}
		else
		{
			num_tweet = num_tweet + 1 ;
		}	
	}
	/*
	for(i=0;i<num_tweet_user;i++)
	{
		
		printf("user id = %d , tweet num = %d  array_num = %d\n",num_tweet_per_user[i][0],num_tweet_per_user[i][1],num_tweet_per_user[i][2]);
		
	}
	*/
	heapSort_3(num_tweet_per_user, 7087);
	/*
	printf("======================================================================\n\n");
	
	for(i=0;i<num_tweet_user;i++)
	{
		
		printf("user id = %d , tweet num = %d  array_num = %d\n",num_tweet_per_user[i][0],num_tweet_per_user[i][1],num_tweet_per_user[i][2]);
		
	}
	*/
	
	int tweet_number[5];
	for(i=0;i<5;i++)
	{
		tweet_number[i] = -1;
	}
	
	for(i=0;i<num_tweet_user;i++)
	{
		int tmp_num = num_tweet_per_user[i][1];
		int p=0;
		
		for(j=0;j<5;j++)
		{
			if(tweet_number[j] == tmp_num)
				break;
		}
		for(j=0;j<5;j++)
		{
			if(tweet_number[j] == -1)
			{
				tweet_number[j] = tmp_num;
				break;
			}
		}
		for(j=0;j<5;j++)
		{
			if(tweet_number[j] == -1)
				p = p + 1;
		}
		if(p==0)
			break;
	}
	
	for(i=0;i<num_tweet_user;i++)
	{
		if(tweet_number[0] == num_tweet_per_user[i][1])
			printf("Rank 1. ID: %d  Tweet number: %d\n\n\n",num_tweet_per_user[i][0],num_tweet_per_user[i][1]);
		else if(tweet_number[1] == num_tweet_per_user[i][1])
			printf("Rank 2. ID: %d  Tweet number: %d\n\n\n",num_tweet_per_user[i][0],num_tweet_per_user[i][1]);
		else if(tweet_number[2] == num_tweet_per_user[i][1])
			printf("Rank 3. ID: %d  Tweet number: %d\n\n\n",num_tweet_per_user[i][0],num_tweet_per_user[i][1]);
		else if(tweet_number[3] == num_tweet_per_user[i][1])
			printf("Rank 4. ID: %d  Tweet number: %d\n\n\n",num_tweet_per_user[i][0],num_tweet_per_user[i][1]);
		else if(tweet_number[4] == num_tweet_per_user[i][1])
			printf("Rank 5. ID: %d  Tweet number: %d\n\n\n",num_tweet_per_user[i][0],num_tweet_per_user[i][1]);
		else
			break;
	}
	printf("\n\n=========================================================\n\n");
}


void Find_strongly_connected_components(){}
void Find_shortest_path_from_a_given_user(){}	
void Find_all_people_who_are_friends_of_the_above_users(){}

void Delete_all_users_who_mentioned_a_word(struct HashTable* Table,struct word* word_list)
{
	struct User* u = NULL;
	char tstr[500];
	printf("\n\n\n\n\n==================================================\n\n\n\n");
	printf("삭제 단어를 입력하시오 해당 유저가 삭제됩니다. : ");
	scanf("%s",tstr);
	printf("\n\n\n\n");
	struct word* w = word_list;
	int a = 0;
	for( ; w!= NULL; w= w->next)
	{
		if(strstr( w->word, tstr ) != NULL)
		{
			
			
			u = search(Table,w->id);
			
			if(u!=NULL)
			{  
				a = a + 1;
				printf("==================================================\n");
				printf("User ID          : %s\n",u->id);
				printf("User Screen name : %s\n",u->profile->screen_name);
				printf("내용              : %s", w->word);
				printf("==================삭제됩니다========================\n");
				printf("==================================================\n\n\n");
				delete(Table,w->id);
			}
			
		}
	}
	
	if(a==0)
	{
		printf("찾으시는 단어가 존재하지 않습니다. \n\n\n\n");
	}
}
void Find_users_who_tweeted_a_wrod(struct HashTable* Table,struct word* word_list)
{
	struct User* u = NULL;
	char tstr[500];
	printf("\n\n\n\n\n==================================================\n\n\n\n");
	printf("찾으려는 단어를 입력하시오 : ");
	scanf("%s",tstr);
	printf("\n\n\n\n");
	struct word* w = word_list;
	int a = 0;
	for( ; w!= NULL; w= w->next)
	{
		if(strstr( w->word, tstr ) != NULL)
		{
			
			
			u = search(Table,w->id);
			
			if(u!=NULL)
			{  
				a = a + 1;
				printf("==================================================\n");
				printf("User ID          : %s\n",u->id);
				printf("User Screen name : %s\n",u->profile->screen_name);
				printf("내용              : %s", w->word);
				printf("==================================================\n\n\n");
			}
			
		}
	}
	
	if(a==0)
	{
		printf("찾으시는 단어가 존재하지 않습니다. \n\n\n\n");
	}
}
	
void main()
{
	
	//====================HashTable=========================
	int i;
	struct HashTable* Table = NULL;
	for(i=0;i<HASH_SIZE;i++) // size만큼 생성
	{
		Table = insert_HashTable(Table,i);
	}
	// =========================================================
	
	struct word* word_list=NULL;
	// =============================================================
	int select_menu;
	while(1)
	{
		printf("\n\n\n\n\n");
		printf("0. Read data files\n");
		printf("1. display statistics\n");
		printf("2. Top 5 most tweeted words\n");
		printf("3. Top 5 most tweeted users \n");
		printf("4. Find users who tweeted a word\n");
		printf("5. Find all people who are friends of the above users\n");
		printf("6. Delete all mentions of a word\n");
		printf("7. Delete all users who mentioned a word\n");
		printf("8. Find strongly connected components\n");
		printf("9. Find shortest path from a given user\n");
		printf("99. Quit\n");
		printf("Select Menu: ");
		scanf("%d",&select_menu);
		
		switch(select_menu)
		{
			case 0 : Read_data_files(Table,&word_list); continue;
			case 1 : display_statistics(Table,word_list); continue;
			case 2 : Top_5_most_tweeted_words(); continue; continue;
			case 3 : Top_5_most_tweeted_users(word_list); continue;
			case 4 : Find_users_who_tweeted_a_wrod(Table,word_list); continue;
			case 5 : Find_all_people_who_are_friends_of_the_above_users(); continue;
			case 7 : Delete_all_users_who_mentioned_a_word(Table,word_list); continue;
			case 8 : Find_strongly_connected_components(); continue;
			case 9 : Find_shortest_path_from_a_given_user(); continue;
			case 99: exit(1);
			default: printf("해당 메뉴는 존재하지 않습니다 \n"); continue;
		}
		
	}
}