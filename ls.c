#include<dirent.h>
#include <string.h>
#include <stdio.h>
#include<fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
/*THE ls COMMAND */

int *sortchar(char **dirpath, int file_ct);
int *sortint(long int *dirpath, long int file_ct);
int modes(char* path);

char l='0',R='0',a='0',t='0',S='0',ind='0';								//flags for ls operations


int main(int argc, char *argv[])
{
	int flag=0;																		// number of chunks for each file
	int len,file_ct=-1,x,z=1;
	
	char **dirpath = (char **) malloc(sizeof(char *)*argc);
    for(x=0; x<argc; x++)
    	{dirpath[x] = (char *) malloc(sizeof(char)*144);} 

    sprintf(dirpath[0],".");
    struct stat status = {0};
	
	while(z<argc)																// reading the arguments for performing ls operations
	{
		len=strlen(argv[z]);
 		if(argv[z][0]=='-')
		{x=1;
		 while(len>x)
			{ switch(argv[z][x])
				{
				case 'l': l='1';break;
				case 'R': R='1';break;
				case 'a': a='1';break;
				case 't': t='1';S='0';break;
				case 'S': S='1';t='0';break;
				case 'i': ind='1';break;
				default : printf("ls: invalid option -- '%c'\n",argv[z][len-1]);return (0);break;
				}x++;
			}
		}
		else
		{	flag=1;
			if(stat(argv[z], &status) == -1) 
				{
 				printf("ls: cannot access %s: No such file or directory\n",argv[z]);
				}
			else {
			sprintf(dirpath[++file_ct],"%s",argv[z]);}
		}
	z++;
	}
	if(file_ct==-1 && flag==1){return 0;}

	if(file_ct==-1 && flag==0) {file_ct=0;}
	else {sortchar(dirpath,file_ct);}
	
	for(z=0;z<=file_ct;z++)
	{	
		/* list all the files and directories within directory */	  	
	  			if(file_ct>0 || R=='1') {printf("%s:\n\n",dirpath[z]);}
    			modes(dirpath[z]);
        			
        	}
	return 0;
}


int* sortchar(char **dirpath, int file_ct)						/*sort the character array in ascending order*/
{
 int i,j,x;
 char temp[100];
 int *seq =(int *) malloc(sizeof(int)*file_ct);

 for(i=0;i<=file_ct;i++)
 	{seq[i]=i; }

 for (i = 0; i <=file_ct; i++) 				
	{
      		for (j = 0; j <= file_ct-1; j++)
			 {
       			if (strcasecmp(dirpath[j], dirpath[j + 1])> 0) 
				{
				sprintf(temp, "%s",dirpath[j]);
			 	sprintf(dirpath[j],"%s", dirpath[j + 1]);
			 	sprintf(dirpath[j+1],"%s", temp);
			 	x= seq[j]; seq[j]=seq[j+1];seq[j+1]=x; 
        		 }
      		}
   	}
   	return seq;
}


int* sortint(long int *dirpath, long int file_ct)				/*sorting integers in reverse order*/
{
	long int i,j,x;
  	long int temp;
  	int *seq =(int *) malloc(sizeof(int)*file_ct);
  	long int cpy[5000];

   	for(i=0;i<=file_ct;i++)
 	{ 	seq[i]=i;
 		cpy[i]=dirpath[i]; }

 	for (i = 0; i <=file_ct; i++) 				
	{
      		for (j = 0; j <= file_ct-1-i; j++)
			 {
       			if (cpy[j]<cpy[j + 1]) 
				{
				temp=cpy[j];
			 	cpy[j]= cpy[j + 1];
			 	cpy[j+1]=temp;
			 	x= seq[j]; seq[j]=seq[j+1];seq[j+1]=x; 
        	 	}
      		}
   	}
   	return seq;
}

int modes(char* path)									/*calculate attributes of input variables*/
{
	int i,j,n,flag=0;
	int dr_ct=0;
	char ch;
	long int total=0;
	char clk[100]={0};
	char temp[80],aux[80];memset(temp, 0, 80);memset(aux, 0, 80);
	time_t rawtime;

	struct dirent **namelist;
	n = scandir(path, &namelist, 0, alphasort);					/*n number of files if path is directory*/
	 
	struct stat fileStat={0};
	
	if(n==-1 && !stat(path, &fileStat) && S_ISREG(fileStat.st_mode)) {n=1;flag=1;}	/*if path is file, not directory*/

    char **filename = (char **) malloc(sizeof(char *)*n*2);
    char **permits	= (char **) malloc(sizeof(char *)*n*2);
    long int *modtime 	= (long int *) malloc(sizeof(long int *)*n*2);
    char **drctry_R = (char **) malloc(sizeof(char *)*n*2);
    char **owners 	= (char **) malloc(sizeof(char *)*n*2);
    char **groups 	= (char **) malloc(sizeof(char *)*n*2);
    long int *size 	= (long int *) malloc(sizeof(long int)*n*2);
    char **inode 	= (char **) malloc(sizeof(char *)*n*2);
    char **no_of_link 	= (char **) malloc(sizeof(char *)*n*2);

    for(i=0; i<n; i++)
    	{
    	filename[i]	= (char *) malloc(sizeof(char)*100);
    	permits[i] 	= (char *) malloc(sizeof(char)*15);
	drctry_R[i] = (char *) malloc(sizeof(char)*150);
	owners[i] 	= (char *) malloc(sizeof(char)*150);
	groups[i] 	= (char *) malloc(sizeof(char)*150);
	inode[i] 	= (char *) malloc(sizeof(char)*15);
	no_of_link[i] 	= (char *) malloc(sizeof(char)*56);
	}
	
	for (i = 0; i < n; i++) 							/*Calculating stats of file */
		{	
			if(flag==0) sprintf(filename[i],"%s",namelist[i]->d_name);
			else sprintf(filename[i],"%s",path);

			if(lstat(filename[i],&fileStat) < 0 || (S_ISDIR(fileStat.st_mode)==1 && strcmp(namelist[i]->d_name,".")!=0 && strcmp(namelist[i]->d_name,"..")!=0 ))
				{
					sprintf(temp,"%s/%s",path,namelist[i]->d_name);
					if(lstat(temp,&fileStat)<0){ printf("ls: Permissions denied %s\n",temp);   continue;}
					else{sprintf(drctry_R[dr_ct],"%s",temp);  dr_ct++;}
				 }
			struct passwd *pw = getpwuid(fileStat.st_uid);
			struct group  *gr = getgrgid(fileStat.st_gid);

			modtime[i]=fileStat.st_mtime;

			total+=(long int)fileStat.st_blocks/2;
			
			size[i]=(long int)fileStat.st_size;

			sprintf(no_of_link[i],"%d",(int)fileStat.st_nlink);

			sprintf(inode[i],"%d",(int)fileStat.st_ino);

			sprintf(owners[i],"%s", pw->pw_name);

			sprintf(groups[i],"%s", gr->gr_name);

			if(S_ISDIR(fileStat.st_mode)==1) {ch='d';}
			else if(S_ISLNK(fileStat.st_mode)) {ch='l';}
			else {ch='-';}
			sprintf(permits[i],"%c%s%s%s%s%s%s%s%s%s",ch,
				(fileStat.st_mode & S_IRUSR) ? "r" : "-",(fileStat.st_mode & S_IWUSR) ? "w" : "-",
				(fileStat.st_mode & S_IXUSR) ? "x" : "-", (fileStat.st_mode & S_IRGRP) ? "r" : "-",
				(fileStat.st_mode & S_IWGRP) ? "w" : "-", (fileStat.st_mode & S_IXGRP) ? "x" : "-",
				(fileStat.st_mode & S_IROTH) ? "r" : "-",(fileStat.st_mode & S_IWOTH) ? "w" : "-",
				(fileStat.st_mode & S_IXOTH) ? "x" : "-");
			
    	}

				/*--------------FOR DISPLAY OF CONTENTS ON std OUTPUT-------------------*/

    	int *sort_val= (int *) malloc (sizeof(int)*n);
    	int index;
    	char padding;
    	
    	if(l=='1') padding='\n';
    	else padding='\t';

    	if(S=='1') {sort_val=sortint(size,n-1);}					/*if S flag is on, sort on size */
    	else if(t=='1') {sort_val=sortint(modtime,n-1);}				/*if t flag is on, sort on time */
    	else {sort_val=sortchar(filename,n-1); }					/* else sort on name */
  			
    	i=-1;
    	if(l=='1') printf("total %ld\n", total);
    	time ( &rawtime );

    	while(i<n-1)
    	{	
    		i++;index=sort_val[i];
    		if((a=='0') && filename[index][0]=='.'){continue;}
    		if(ind=='1')
    		{	printf("%10s ",inode[index]);}
    		if(l=='1')
    		{	
    			if(permits[index][0]=='l')
    				{if(S=='0' && t=='0'){
    					readlink(filename[i], temp, sizeof(temp));
			     	 sprintf(aux,"%s->%s",filename[i],temp);sprintf(filename[i],"%s",aux);}
			     	 else {readlink(filename[index], temp, sizeof(temp));
			     	 sprintf(aux,"%s -> %s",filename[index],temp);sprintf(filename[index],"%s",aux);}
    				}
    			
    			if(difftime(rawtime,modtime[index])> 31536000) {j=7; sprintf(aux,"%ld ",modtime[index]/(60*60*24*365)+1970);}
  				else {sprintf(aux," ");j=12;}

  				strncpy (clk,ctime(&(modtime[index]))+4,j);
			     	
    			printf("%3s%3s%10s%10s %5ld %10s%s ",permits[index],no_of_link[index],owners[index],groups[index], size[index],clk,aux);
    		}
    		if(S=='0' && t=='0') {if(permits[index][0]=='d') printf("\033[22;34m%3s\033[0m",filename[i]);else printf("%3s",filename[i] );}
    		else {if(permits[index][0]=='d') printf("\033[22;34m%3s\033[0m",filename[index]);else printf("%3s",filename[index] );}
    		printf("%c",padding);
    	}

    	printf("\n");

    	if(R=='1' && dr_ct>0) { for(i=0;i<dr_ct;i++) {printf("%s:\n",drctry_R[i]);modes(drctry_R[i]);} }
    	
    	for(i=0;i<n;i++){free(filename[i]);free(groups[i]);
    	free(permits[i]);free(owners[i]);free(no_of_link[i]);}
    	free(filename);free(size);free(groups);free(namelist);
    	free(permits);free(owners);free(modtime);free(no_of_link);

	return 1;
}
