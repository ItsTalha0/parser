
#include<stdio.h>
#include<stdlib.h>

FILE *in;


enum ewords{
	IF=1,DO,OD,VAR,WHILE,ELSE,THEN,CONST,COMMBOX,BEGIN,END,DELAY,INT,BOOL,TRUE,FALSE,ADR,CMM,SC,OB,CB,CL,EQ,NL,ID,NUM,EF
	};

typedef struct tkn
{
	char name[100];
	int type;
	int val;
}tkn;


tkn glob_next;


char * words[] = { "if","do","od","var","while","else","then","const","box","begin","end","del","int","bool","true","false","adr",",",";","(",")",":","=","\n"};

const int  word_c = 24;

void throwError()
{
	printf("Invalid Number\n");
	exit(1);
}

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

	
int isValidNum(char *c)
{
	int flg=0;
	while( *(c) != '\0' && flg==0 )
	{
		if( isNum(*c) != 1 )
		{
			flg = 1;
		}
		c++;
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
	if( isAlpha(c)==1 || isNum(c)  )
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
	if(temp.name[0] != EOF && isNum(temp.name[0]) != 1)
	{
		temp.type = match(temp.name);
	}
	else if( temp.name[0] != EOF && isNum(temp.name[0]) == 1 )
	{
		temp.type = NUM;
		if( isValidNum(temp.name) == 0 )
		{
			temp.val = atoi(temp.name);
		}
		else
		{
			throwError();
		}
	}
	else temp.type = EF;
	glob_next = temp;
	return temp;
}


void ptkn(tkn i)
{
	printf("--%s--%d--%d\n",i.name,i.type,i.val);
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
	tkn b;
	tkn c;
	if( ((t=next()).type == BOOL || t.type==INT) && next().type == CL && next().type == ID && ( (b=next()).type == CMM || (b.type == EQ && ((((c=next()).type==TRUE||c.type==FALSE)&& t.type==BOOL) || (c.type==NUM && t.type==INT )))))
	{
		flg = 1;
	}
	return flg;
}

int vcheck()
{
	tkn t;
	tkn n;
	int flg = 0;
	if( ((t=next()).type == BOOL||t.type==INT)&& next().type==CL && next().type ==ID )
	{
		if( (n=next()).type==CMM )
		{
			flg=vcheck();
		}
		else if( n.type == EQ &&( ((next().type == NUM && t.type ==INT ) || (t.type == BOOL&& ( glob_next.type == TRUE || glob_next.type == FALSE))) ))
		{
			if( next().type == SC )
			{	
				flg = 1;
			}
			else if( glob_next.type == CMM)
			{
				flg = vcheck();
			}
		}
		else if( n.type == SC )
		{
			flg = 1;
		}
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



//old assignment checker;
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
//old assginment helpchecker;
int vAsign()
{
	int flg = 0;
	if( rass()==1 && next().type==SC )
	{
		flg=1;
	}
	return flg;
}
//pritns a token
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
		if( next().type == VAR && vcheck()==1 )
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
	if( argc > 1 )
	{	
		in = fopen(args[1],"r");
	}
	validHead();
	//printf("%d %d %d",CL,NL,ID);
	//printf("%d\n",vcheck());

}




