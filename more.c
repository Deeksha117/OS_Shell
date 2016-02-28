//Implementation of more command
#include <string.h>
#include <stdio.h>
#include<fcntl.h>
#include <unistd.h>
int atoi(char *);
int file_open(char *,int, int); /*to open files to read and print*/
int calc_x(char, int);		/*to evaluate user input for more*/

int main(int argc , char *argv[])
{
	int fp;
	int len;
	int x;
	char ch=' ',g,buf='\0';
	if(argc<=2)		// If arguments are insufficient
	{
		printf("ERROR\n");
		return 0;
	}

	len=atoi(argv[1]);
	x=len;
 	int i=2;char temp='\r';

	fp = file_open(argv[i],argc,i);
	read(fp,&buf,1);temp=buf;
	if(fp==-1 && argc<4) return 0;//If no more files to read
	//if(fp>-1 && argc>3) {printf("::::::::\n%s\n::::::::\n",argv[i]);read(fp,&buf,1);temp=buf;}//If more than one files to read
	else if(fp==-1 && argc>=4) {scanf("%c",&ch);
				    if(ch==' ') {scanf("%c",&g);}
				    temp='\r';x=calc_x(ch,len);}
	i++;

	

	while(x>-1)
	{ 
	    while(temp=='\r' && argc>i && x>-1)
		{
			close(fp);
			fp= file_open(argv[i],argc,i);i++;
						 if(fp==-1 && argc==i) return 0;
						 if (fp==-1) {scanf("%c",&ch);if(ch==' ') {scanf("%c",&g);}
								x=calc_x(ch,len);temp='\r';}
			else 
			{printf("::::::::\n%s\n::::::::\n",argv[i-1]);
			read(fp,&buf,1);
			temp=buf; }
			
			
		}
 
	    while(x>0)
	 	{
		if(read(fp,&buf,1)> 0)
  			{ if(temp=='\n')
			  	{x--;}
			  printf("%c",temp);
			  temp=buf;
			}
		
		else {printf("%c",temp);x=0;temp='\r';
			//fp = file_open(argv[i],argc,i); if (fp==-1) {return 0;}
			
	             }
		}
		
	  	scanf("%c",&ch);if(ch==' ') {scanf("%c",&g);}x=calc_x(ch,len);
	  }
	close(fp);
	return 0;
}

int file_open(char *fname, int argc, int i)
{ 
 int fn= open(fname,O_RDONLY);
 if(fn==-1) {printf("%s ERROR\n",fname);}
 return fn;
}
int calc_x(char c, int l)
{
 if(c==' ')
	return l;
 else if(c=='\n')
	return 1;
 else if (c=='q')
	return -1;
 else return 0;
}
