#include<stdio.h>
#include<stdlib.h>

FILE *in;


enum ewords{
	IF=1,DO,OD,VAR,WHILE,ELSE,THEN,CONST,COMMBOX,BEGIN,END,DELAY,INT,BOOL,CMM,SC,OB,CB,CL,EQ,NL,ID,EF
	};
typedef struct tkn
{
	char name[100];
	int type;
	int val;
}tkn;

char * words[] = { "if","do","od","var","while","else","then","const","commbox","begin","end","delay","integer","boolean",",",";","(",")",":","=","\n"};
const int  word_c = 21;



int isAlpha(char c)
{
	int flg = 0;
	if ( (c >= 'a' && c<='z') || ( c<='A' && c>='Z' ))
	{
		flg = 1;
	}
	return flg;
}


int isNum(char c)
{
	int flg = 0;
	if ( (c >= '0' && c<='9') )
	{
		flg = 1;
	}
	return flg;
}

	


//to be used only in conjunction with isNum function
int val(char c)
{
	return c-'0';
}

int cmp(char *s,char *d)
{
	int flg = 1;
	while( *s != '\0' && *d != '\0' )
	{
		if( *s != *d )
		{
			flg = -1;
		}
		s++;
		d++;
	}
	if( *s != *d )
	{
		flg = -1;
	}
	return flg;
}

int match(char *c)
{
	int i=0;
	int flg = -1;
	int type=word_c+1;
	while( i<word_c && flg == -1 )
	{
		flg = cmp(c,words[i]);
		i=i+1;
	}
	type = i--;
	if( flg == -1 )
	{
		type = word_c + 1;
	}
	return type;
}

tkn next()
{
	tkn temp;
	char c;
	int i=1;
	while( (c=getc(in))==' ' || c=='\t' || c=='\n');
	temp.name[0] = c;
	if( isAlpha(c)==1 )
	{
			while( (c=getc(in)) != ' '  && c!='\n' && c!='\t' && (isAlpha(c)==1||isNum(c)==1))
			{
				temp.name[i] = c;
				i=i+1;
			}
			if( isAlpha(c)!=1 && isNum(c)!=1 )ungetc(c,in);

	}
	//if( i==1 ) ungetc(c,in);
	temp.name[i] = '\0';
	if(temp.name[0] != EOF )
	{
		temp.type = match(temp.name);
	}
	else temp.type = EF;
	return temp;
}


void ptkn(tkn i)
{
	printf("--%s--%d--\n",i.name,i.type);
}


int vComm()
{
	int flg=0;
	if( next().type == ID && next().type == SC )
	{	
		flg = 1;
	}
	return flg;
}

int inAsign()
{
	int flg=0;
	tkn t;
	if( ((t=next()).type == BOOL || t.type==INT) && next().type == CL && next().type == ID)
	{
		flg = 1;
	}
	return flg;
}

int validComm()
{
	int flag=0;
	if(next().type == ID)
	{
		if(next().type == SC)
		{
			flag = 1;
		}
	}
	return flag;
}




int rass()
{
	int flg=0;
	if( (flg=inAsign()) == 1 )
	{
		tkn t = next();
		if(t.type == CMM)
		{	
			flg=inAsign();
		}
		if( flg == 1 && t.type == SC )
		{
			flg = 1;
		}
	}
	return flg;
}

int vAsign()
{
	int flg = 0;
	if( rass()==1 && next().type==SC )
	{
		flg=1;
	}
	return flg;
}

void pptkn()
{
	tkn t;
	while( (t=next()).name[0] != EOF )
	{
		ptkn(t);
	}
}


void validHead()
{
	int flag=0;
	tkn t = next();
	if(t.type==COMMBOX && validComm()==1)
	{
		if( next().type == VAR && vAsign()==1 )
		{
			flag=1;
		}
	}
	if( flag == 1)
	{
		printf("Success\n");
	}
	printf("Done\n");

}


int main(int argc,char *args[])
{
	if( argc > 0 )
	{	
		in = fopen(args[1],"r");
	}
	validHead();
	//printf("%d %d %d",CL,NL,ID);

}



