#include <string.h>
#include <stdio.h>
#include<fcntl.h>
#include <unistd.h>


int main(int argc , char *argv[])
{
	int fp;
	int i=1,len;
	int l=0,c=0,m=0,L=0,w=0,all=1;//flags for wc operations

	if(argc<2)
	{
	printf("ERROR\n");// If arguments are insufficient
	return 0;
	}

	while(i<argc)// reading the arguments for performing wc operations
	{
		len=strlen(argv[i]);
 		if(argv[i][0]=='-')
		{ while(len>1)
			{ switch(argv[i][len-1])
				{
				case 'l': l=1;all=0;break;
				case 'c': c=1;all=0;break;
				case 'm': m=1;all=0;break;
				case 'L': L=1;all=0;break;
				case 'w': w=1;all=0;break;
				default : printf("ERROR\n");return (0);break;
				}
			  len--;
			}
		}
		else
		{fp= open(argv[i],O_RDONLY); break;}
	i++;
	}
	
	char buf;
	int l_ct=0, m_ct=0, w_ct=0, L_ct=0,temp=0,flag_w=0, flag_l=0, whtspc_ct=0;

	
	if(fp==-1)				//file does not exist
	{	printf("ERROR\n");return 0;	
	}
	else
	{

		while (read(fp,&buf,1)> 0)	//reading through file character by character
  		{
			if(buf==' ' || buf=='\t' || buf=='\n')
			{	if(flag_w==1) w_ct++;

				flag_w=0;
				whtspc_ct++;
				if(buf== ' ') {temp++;}
				if(buf=='\t') {temp+=8;}
				if(buf == '\n')
				{ l_ct++;flag_l=1;temp=m_ct;
				}
			}
				
			else
			{ if((flag_l==1) && L_ct<temp) { L_ct=temp;temp=0;flag_l=0;} 
			  m_ct++;flag_w=1;temp++;
			}
		}
		if(temp>L_ct) {L_ct=temp;}
	}

	m_ct+=whtspc_ct;
	if(l==1 || all==1)	//printing line count if number of bytes>0
	{ if(m_ct>1)
		printf(" %d",l_ct);
	  else  printf("0 "); 
	}

	if(w==1 || all==1)	//printing word count if number of bytes>0
	{ if(m_ct>1)
		printf(" %d",w_ct);
	  else  printf("0 "); 
	}

	if(m==1 || all==1)	///printing character count if number of bytes>0
	{ if(m_ct>1)
		printf(" %d",m_ct);
	  else  printf("0 "); 
	}
	
	if(c==1)		//printing byte count if number of bytes>0
	{ if(m_ct>1)
		printf(" %d",m_ct);
	  else  printf("0 "); 
	}

	if(L==1)		//printing length of max line 
	{ printf(" %d",L_ct);
	}
	printf(" %s\n", argv[i]);
	close(fp);
	
return 0;
}

