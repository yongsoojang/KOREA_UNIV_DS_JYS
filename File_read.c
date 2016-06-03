#include <stdio.h>

void main()
{
	FILE *fa;
	char tstr[100];
	fa = fopen("word.txt","r");
	
	while( fgets(tstr,100,fa) != NULL )
	{
		printf("%s",tstr);
	}
	fclose(fa);
}