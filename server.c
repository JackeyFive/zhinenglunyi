#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>  
#include<errno.h>  
#include<netinet/in.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<pthread.h>
#include<mysql.h>

#define MAX 10240

typedef struct xinxi
{	
	float jingdu;
	float weidu;

	float wendu;
	float shidu;
	float rate;
	float spo2;
	
}xinxi;


void request_error();
void locate(char *usr,char *psw);
int distinguish_id(char *usr,char *psw);
void getxinxi(int id,xinxi * newxinxi);
void create_html(xinxi *newxinxi);
void sql_init();

void updatexinxi(char **dat);

void test();


void readline(int fd,char* buf);



int fd_n,newfd;
int pid;

	MYSQL mysql;
	MYSQL_RES* res=NULL;
	MYSQL_FIELD* fields=NULL;
	MYSQL_ROW row;


void main(){

	struct sockaddr_in ad_in;

	char *addr="127.0.0.1";
	char *client="client:";

	memset(&ad_in,0,sizeof(ad_in));

	if(inet_pton(AF_INET,(char *)addr,(void *)&(ad_in.sin_addr))<1){
	printf("htn");exit(0);
	}	
			
	if((fd_n=socket(AF_INET,SOCK_STREAM,0))<0){
		printf("socket");exit(0);}

	ad_in.sin_family = AF_INET;
	//ad_in.sin_addr.s_addr = htonl(INADDR_ANY);
	ad_in.sin_port=htons(8000);


	if( bind(fd_n, (struct sockaddr*)&ad_in, sizeof(ad_in)) <0){
	printf("bind");exit(0);
	}

	if( listen(fd_n, 10) == -1){
	printf("listen");exit(0);
	}


	while(1)
	{
		struct sockaddr_in conad_in;
		size_t len=sizeof(conad_in);
		memset(&conad_in,0,len);
		conad_in.sin_family = AF_INET;

		//	biggest leession:if(newfd=accept(fd_n, (struct sockaddr*)NULL, NULL)<0)
		if((newfd = accept(fd_n, (struct sockaddr*)&conad_in, (socklen_t*)&len))<0){
				printf("accept");exit(0);}

		if(!fork())
		{

			char* http_buf=malloc(sizeof(char)*MAX);
			readline(newfd,http_buf);


			if(strstr(http_buf,"GET")==NULL)
				request_error();

			else  if(strstr(http_buf,"locate")!=NULL)
				{
					strtok(http_buf,"?");
							
					char* arg[2];
					arg[0]=strtok(NULL,"&");
					arg[1]=strtok(NULL," ");	
					locate( arg[0],arg[1] );
					//test();
				}

			else  if(strstr(http_buf,"update")!=NULL)
				{
					strtok(http_buf,"?");
				
					char* dat[6];
					dat[0]=strtok(NULL,"&");
					dat[1]=strtok(NULL,"&");
					dat[2]=strtok(NULL,"&");
					dat[3]=strtok(NULL,"&");
					dat[4]=strtok(NULL,"&");

					dat[5]=strtok(NULL," ");
					sql_init();
					updatexinxi(dat);
				}
				
			else 	
				request_error();
			
			mysql_free_result(res);
			mysql_close(&mysql);
			free(http_buf);
			close(newfd);
			close(fd_n);
			exit(0);

		}
		sleep(1);
		close(newfd);
	}

		close(fd_n);
		exit(0);
}




void locate(char *usr,char *psw)
{	
	int id;
	xinxi newxinxi;

	newxinxi.jingdu=116.404;
	newxinxi.weidu=39.915;
	sql_init();				//先连接数据库？	
	//id=distinguish_id(usr,psw);//判断身份，也就是验证密码
	getxinxi(1,&newxinxi); // 获取经纬度
	create_html(&newxinxi);//生成网页

}

void sql_init()
{
	if(mysql_init(&mysql)==NULL){
		perror("init");
		fflush(stdout);
		exit(0);
	}

	if(mysql_real_connect(&mysql,"localhost","usr","passwd","test",0,NULL,0)==NULL)
	{
		perror("connect");
		fflush(stdout);
		exit(0);
	}

}


int distinguish_id(char *usr,char *psw)
{
		//连接数据库，读用户表，目前读法为 select num（usr）from 表 where usr=usr and psw=psw
		//	若通过 返回id ，若不返回 0 
	return 0;
}


void getxinxi(int id, xinxi *newxinxi)
{

		//根据id从表中获取信息
	int row_num=0;
	int field_num=0;
	int i;
	
	if(mysql_query(&mysql,"select xinxi from test where id=1;")!=0)
	{
		perror("query");
		fflush(stdout);
		exit(0);
	}

	if((res=mysql_store_result(&mysql))==NULL)
	{
		perror("store");
		fflush(stdout);
		exit(0);
	}
	
	row_num=mysql_num_rows(res);
	field_num=mysql_num_fields(res);
	fields=mysql_fetch_fields(res);

		printf("\n");
	for(i=0;i<field_num;i++){
		//printf("%s\t",fields->name);
		fields++;
	}


	while(row=mysql_fetch_row(res)){

		printf("\n");

		for(i=0;i<field_num;i++){

			xinxi *temp;
			temp=(xinxi*)row[i];
			
			newxinxi->jingdu=temp->jingdu;
			newxinxi->weidu=temp->weidu;
			newxinxi->wendu=temp->wendu;
			newxinxi->shidu=temp->shidu;
			newxinxi->rate=temp->rate;
			newxinxi->spo2=temp->spo2;

		}
	}
}


void create_html(xinxi *newxinxi)
{
	int one,two,three;
int san;
	
	one=open("./one.txt",O_RDONLY,0);
	two=open("./two.txt",O_RDONLY,0);
	three=open("./three.txt",O_RDONLY,0);
san=open("./san.txt",O_WRONLY,0);

	struct stat staone,statwo,stathree;
	fstat(one,&staone);
	fstat(two,&statwo);
	fstat(three,&stathree);
	size_t sizone=staone.st_size;
	size_t siztwo=statwo.st_size;
	size_t sizthree=stathree.st_size;
	
	char bufone[MAX];
	char buftwo[MAX];
	char bufthree[MAX];
	read(one,bufone,sizone);
	read(two,buftwo,siztwo);
	read(three,bufthree,sizthree);

	char x[]="var x=";
	char y[]="var y=";

	
	char wen[]="wendu: ";
	char shi[]="  shidu: ";
	char rate[]="  heart rate: ";
	char spo2[]="  spo2: ";


	char temp[MAX];
	sprintf(temp,"%s%f;\n%s%f;\n",x,newxinxi->jingdu,y,newxinxi->weidu);	
	
	size_t siz=strlen(temp);
	
	strncat(bufone,temp,siz);
	strncat(bufone,buftwo,siztwo);


	sprintf(temp,"\"%s%.2f%s%.2f%s%.2f%s%.2f\"",wen,newxinxi->wendu,shi,newxinxi->shidu,rate,newxinxi->rate,spo2,newxinxi->spo2);	

	strncat(bufone,temp,strlen(temp));
	strncat(bufone,bufthree,sizthree);

	siz+=strlen(temp);


write(san,bufone,sizone+siztwo+sizthree+siz);

	char buf[MAX];

	strcpy(buf,"HTTP/1.0 200 OK\r\n");
	strcat(buf,"Connection: close\r\n");
	sprintf(buf,"%sContent-length: %d\r\n",buf,sizone+siztwo+sizthree+siz); 
	strcat(buf,"Content-type: text/html\r\n\r\n");

	strncat(buf,bufone,sizone+siztwo+sizthree+siz);
	//strncat(buf,"\0",1);
	
	char *ch=buf;	
	while((*ch)!='\0')
	{
	write(newfd,ch,1);
	ch++;
	}

	close(one);
	close(two);
	close(san);

}



void updatexinxi(char **dat)
{
	xinxi newxinxi;
	newxinxi.jingdu=atof(dat[0]);
	newxinxi.weidu=atof(dat[1]);
	newxinxi.wendu=atof(dat[2]);
	newxinxi.shidu=atof(dat[3]);
	newxinxi.rate=atof(dat[4]);
	newxinxi.spo2=atof(dat[5]);

	char *sql=malloc(sizeof(char)*256);
	char *temp=sql;

	strcpy(sql,"update test set xinxi=\"");
	temp+=strlen(sql);

	temp+=mysql_real_escape_string(&mysql,temp,(char *)&newxinxi,sizeof(xinxi));

	*(temp++)='\"';*(temp++)='\0';
	strcat(sql," where id=1");
	
	if(mysql_real_query(&mysql,sql,temp-sql)!=0)
	{
		perror("query");
		fflush(stdout);
		exit(0);
	}

	
	//free(buf);
	free(sql);
	
	char buf[MAX];

	strcpy(buf,"HTTP/1.0 200 OK\r\n");
	strcat(buf,"Connection: close\r\n");
	sprintf(buf,"%sContent-length: %d\r\n",buf,2); 
	strcat(buf,"Content-type: text/html\r\n\r\n");

	strncat(buf,"ok",2);
	//strncat(buf,"\0",1);
	
	char *ch=buf;	
	while((*ch)!='\0')
	{
	write(newfd,ch,1);
	ch++;
	}



}



void readline(int fd,char* buf)
{	
	char ch;
	read(fd,&ch,1);
	while((ch)!='\n')
	{
	memcpy(buf,&ch,1);
	buf++;
	read(fd,&ch,1);
	
	}
	strncat(buf,"\0",1);
}


void request_error(){
	char* temp="error";

	int siz_temp=strlen(temp);
	
	char buf[MAX];

	strcpy(buf,"HTTP/1.0 200 OK\r\n");
	strcat(buf,"Connection: close\r\n");
	sprintf(buf,"%sContent-length: %d\r\n",buf,siz_temp);
	strcat(buf,"Content-type: text/html\r\n\r\n");
	strncat(buf,temp,siz_temp);
	strncat(buf,"\0",1);
	char *ch=buf;	
	while((*ch)!='\0')
	{
	write(newfd,ch,1);
	ch++;
	}

}










