#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include <string.h>
#include <stdio.h>
#include<fcntl.h>
#include <stdlib.h>
#include <unistd.h>

/*Split + tac command*/

void makedir(char *dest);

int main(int argc, char *argv[])
{
	if(argc!=4) {printf("ERROR");return 0;}

	int fp,fd,chk;
	int file_ct=0;	// number of chunks for each file
	char path[15], filepath[30];
	char c=' ',temp;
	int len,k;

	len= atoi(argv[3]);printf("%d\n",len);
	
	char **mat = (char **) malloc(sizeof(char *)*len);
	char *val="";
    	int x=0;

    	for(x=0; x<len; x++)
    	{mat[x] = (char *) malloc(sizeof(char)*144);} 				/* 2D array dynamic allocation  */
	
	
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir (argv[1])) != NULL)	      				/* list all the files and directories within directory */
	{	makedir(argv[2]);
	  
		while ((ent = readdir (dir)) != NULL)
		{
			if(ent->d_name[0]=='.') {continue;} 			/*do not work with hidden files*/

			snprintf(path,20, "%s%s", argv[1],ent->d_name );	/*open the files to read from*/
			//printf("%s\n",path);
			fp= open(path,O_RDONLY);

			file_ct=0;
			
			if(fp==-1)	{	printf("ERROR\n");continue;	}
			chk=read(fp,&c,1);
			temp=c;
			 x=0;
			
			/*Reading from source file and writing to destination file*/
			while(chk>0)
			{
				while (x<len && (chk=read(fp,&c,1))>0 )		/*Read from file &store in mat*/  					{ 	
					if(temp=='\n') {sprintf(mat[x],"%s\n",val);x++;val="";}
					else 	{ sprintf(mat[x],"%s%c",val,temp); val=mat[x];}
					temp=c;
				}

				if(x==len) {x--;}
				if(chk==0) {sprintf(mat[x],"%s\n",val);}
				file_ct++;

				if(argv[2][strlen(argv[2])-1]!='/')		/*set destination filename and write */
					sprintf(filepath,"%s/%s_%d",argv[2],ent->d_name,file_ct);
				else
					sprintf(filepath,"%s%s_%d",argv[2],ent->d_name,file_ct);
				
				fd = open(filepath, O_WRONLY| O_APPEND | O_CREAT, 0644);
				if(fd==-1) { printf("File Creation ERROR");}
				for(k=x;k>=0;k--)
						{	printf("%s",mat[k]);	
							write(fd,mat[k],strlen(mat[k]));
						}
				close(fd);
				x=0;
				val="";
			}
	
			close(fp);
			
		//printf ("%s\n", path);
	 	 }
	  	closedir (dir);
	}
	 else {printf ("ERROR");}
return 0;
}

void makedir(char *dest)
{
	int len,last;
	char path[30],*temp="";
	last=strlen(dest);
	if(dest[last-1]!='/') {path[last]='/';last++;}
	struct stat st = {0};
	for(len=0;len<last;len++)
	{
	if(dest[len]=='/' && stat(path, &st) == -1) 
				{
    					mkdir(path, 0700);sprintf(path,"%s%c",temp,dest[len]);
				}
	else {	
		sprintf(path,"%s%c",temp,dest[len]);temp=path;}
	}
}


