#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include<fcntl.h>
#include<pthread.h>

//Basic Account Structures
typedef struct normalUser
{
	int userID;
	char name[30];
	char password[10];
	int account_no;
	float balance;
	char status[20];
}normalUser;

typedef struct jointUser
{
	int userID;
	char name1[30];
	char name2[30];
	char password[10];
	int account_no;
	float balance;
	char status[20];
}jointUser;

typedef struct admin
{
	int userID;
	char username[30];
	char password[10];
}admin;

//Same Procedure has been carried out to get the details of different type of Accounts

normalUser getNU(int ID)
{
	int realid=ID-10000;
	normalUser User;
	int fd=open("NormalUserdb",O_RDONLY,0744);
	
	//Record File Locking (in Read Mode)
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(realid)*sizeof(normalUser); 
	if(lseek(fd, 0 , SEEK_END) <= lock.l_start)
	{
		User.account_no =-1;
		User.balance = -1;
		User.userID = -1;
		return(User);
	}   	     
	lock.l_len=sizeof(normalUser);	             
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock); //Sat the Lock	

	lseek(fd,(realid)*sizeof(normalUser),SEEK_SET); //Getting to the UserInfo Location 
	read(fd,&User,sizeof(normalUser));

	lock.l_type=F_UNLCK; //Unlocked it
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return User;
}

jointUser getJU(int ID)
{
	int realid=ID-10000;
	jointUser User;
	int fd=open("JointUserdb",O_RDONLY,0744);
	
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(realid)*sizeof(jointUser);    
	if(lseek(fd, 0 , SEEK_END) <= lock.l_start)
	{
		User.account_no =-1;
		User.balance = -1;
		User.userID = -1;
		return(User);
	}  	     
	lock.l_len=sizeof(jointUser);	             
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);	

	lseek(fd,(realid)*sizeof(jointUser),SEEK_SET); 
	read(fd,&User,sizeof(jointUser));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return User;
}

admin getAdmin(int ID)
{
	int realid=ID-10000;
	admin User;
	int fd=open("Admindb",O_RDONLY,0744);

	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(realid)*sizeof(admin);    	     
	lock.l_len=sizeof(admin);	             
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);	

	lseek(fd,(realid)*sizeof(admin),SEEK_SET);  
	read(fd,&User,sizeof(admin));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return User;
}

//Same procedure has been done for normal, joint and admin accounts for authentication

bool authenticateNU(normalUser user)
{
	int realid = user.userID-10000;
	int fd = open("NormalUserdb",O_RDONLY,0744);

	bool result;
	normalUser temp;

	//Record Locking(Read) for reading the user's credentials
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(realid)*sizeof(normalUser);    	     
	lock.l_len=sizeof(normalUser);	             
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);	//Locked the Record

	lseek(fd,(realid)*sizeof(normalUser),SEEK_SET);  
	read(fd,&temp,sizeof(normalUser));
	if(!strcmp(temp.password,user.password) && !strcmp(temp.status,"ACTIVE"))	result=true;
	else	result=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock); //Unlocked the record

	close(fd);
	return result;
}

bool authenticateJU(jointUser user)
{
	int realid=user.userID-10000;
	int fd=open("JointUserdb",O_RDONLY,0744);
	bool result;
	jointUser temp;
	
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(realid)*sizeof(jointUser);    	     
	lock.l_len=sizeof(jointUser);	             
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);	

	lseek(fd,(realid)*sizeof(jointUser),SEEK_SET);  
	read(fd,&temp,sizeof(jointUser));
	if(!strcmp(temp.password,user.password) && !strcmp(temp.status,"ACTIVE"))	result=true;
	else		result=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool authenticateAdmin(admin user)
{
	int realid=user.userID-10000;
	int fd = open("Admindb",O_RDONLY,0744);
	bool result;
	admin temp;
	
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(realid)*sizeof(admin);    	   
	lock.l_len=sizeof(admin);	            
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);	

	lseek(fd,(realid)*sizeof(admin),SEEK_SET);  
	read(fd,&temp,sizeof(admin));
	if(!strcmp(temp.password,user.password))	result=true;
	else		result=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool depositMoney(int accType,int ID,float amt)
{
	int realid=ID-10000;
	if(accType==1)
	{
		int fd=open("NormalUserdb",O_RDWR,0744);
		bool result;

		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(realid)*sizeof(normalUser);  
		lock.l_len=sizeof(normalUser);	             
		lock.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&lock);	

		normalUser User;
		lseek(fd,(realid)*sizeof(normalUser),SEEK_SET);  
		read(fd,&User,sizeof(normalUser));
		
		if(!strcmp(User.status,"ACTIVE")) //Checking the Status of the Account
		{
			User.balance += amt;
			lseek(fd,sizeof(normalUser)*(-1),SEEK_CUR);
			write(fd,&User,sizeof(normalUser)); //Rewriting the Details
			result=true;
		}
		else	result=false;

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;		
	}
	else if(accType==2)
	{
		int fd=open("JointUserdb",O_RDWR,0744);
		bool result;

		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(realid)*sizeof(jointUser);    
		lock.l_len=sizeof(jointUser);	             
		lock.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&lock);	

		jointUser User;
		lseek(fd,(realid)*sizeof(jointUser),SEEK_SET);  
		read(fd,&User,sizeof(jointUser));
		
		if(!strcmp(User.status,"ACTIVE"))
		{
			User.balance += amt;
			lseek(fd,sizeof(jointUser)*(-1),SEEK_CUR);
			write(fd,&User,sizeof(jointUser));
			result=true;
		}
		else	result=false;

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;	
	}
	return false;
}

bool withdrawMoney(int accType,int ID,float amt)
{
	int realid=ID-10000;
	if(accType==1)
	{
		int fd=open("NormalUserdb",O_RDWR,0744);
		bool result;
	
		struct flock lock;
		//Write Record Lock
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(realid)*sizeof(normalUser);   
		lock.l_len=sizeof(normalUser);	             
		lock.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&lock);	
	

		normalUser User;
		lseek(fd,(realid)*sizeof(normalUser),SEEK_SET);  
		read(fd,&User,sizeof(normalUser));
		
		if(!strcmp(User.status,"ACTIVE") && User.balance>=amt) //Checking whether the amount can be withdrawn or not
		{
			User.balance-=amt; //Reducing the Amount
			lseek(fd,sizeof(normalUser)*(-1),SEEK_CUR);
			write(fd,&User,sizeof(normalUser));
			result=true;
		}
		else	result=false;

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;	
	}
	else if(accType==2)
	{
		int fd=open("JointUserdb",O_RDWR,0744);
		bool result;
	
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(realid)*sizeof(jointUser);    
		lock.l_len=sizeof(jointUser);	            
		lock.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&lock);

		jointUser User;
		lseek(fd,(realid)*sizeof(jointUser),SEEK_SET); 
		read(fd,&User,sizeof(jointUser));
		
		if(!strcmp(User.status,"ACTIVE") && User.balance>=amt)
		{
			User.balance-=amt;
			lseek(fd,sizeof(jointUser)*(-1),SEEK_CUR);
			write(fd,&User,sizeof(jointUser));
			result=true;
		}
		else	result=false;

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	return false;
}

float getBalance(int accType,int ID)
{
	int realid=ID-10000;
	float result;
	if(accType==1)
	{
		int fd=open("NormalUserdb",O_RDONLY,0744);
		normalUser User;
		//Normal Read Lock
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(realid)*sizeof(normalUser);    	   
		lock.l_len=sizeof(normalUser);	            
		lock.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&lock);	

		lseek(fd,(realid)*sizeof(normalUser),SEEK_SET); 
		read(fd,&User,sizeof(normalUser));
		//Reading the Balance
		if(!strcmp(User.status,"ACTIVE"))	result=User.balance;
		else		result=0;

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	else if(accType==2)
	{
		int fd=open("JointUserdb",O_RDONLY,0744);
		jointUser User;
	
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(realid)*sizeof(jointUser);    	     
		lock.l_len=sizeof(jointUser);	            
		lock.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&lock);	

		lseek(fd,(realid)*sizeof(jointUser),SEEK_SET); 
		read(fd,&User,sizeof(jointUser));
		//Reading the Balance
		if(!strcmp(User.status,"ACTIVE"))	result=User.balance;
		else		result=0;

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	} 
	return 0;
}

bool alterPassword(int accType,int ID,char newPwd[10])
{
	int realid=ID-10000;
	if(accType==1)
	{
		int fd=open("NormalUserdb",O_RDWR,0744);
		bool result;

		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(realid)*sizeof(normalUser);    
		lock.l_len=sizeof(normalUser);	             
		lock.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&lock);	

		normalUser User;
		lseek(fd,(realid)*sizeof(normalUser),SEEK_SET);  
		read(fd,&User,sizeof(normalUser));
		
		if(!strcmp(User.status,"ACTIVE"))
		{
			strcpy(User.password,newPwd);
			lseek(fd,sizeof(normalUser)*(-1),SEEK_CUR);
			write(fd,&User,sizeof(normalUser));
			result=true;
		}
		else	result=false;

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	else if(accType==2)
	{
		int fd=open("JointUserdb",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(realid)*sizeof(jointUser);    
		lock.l_len=sizeof(jointUser);	            
		lock.l_pid=getpid();
	
		fl1=fcntl(fd,F_SETLKW,&lock);	

		jointUser User;
		lseek(fd,(realid)*sizeof(jointUser),SEEK_SET);  
		read(fd,&User,sizeof(jointUser));
		
		if(!strcmp(User.status,"ACTIVE"))
		{
			strcpy(User.password,newPwd);
			lseek(fd,sizeof(jointUser)*(-1),SEEK_CUR);
			write(fd,&User,sizeof(jointUser));
			result=true;
		}
		else	result=false;

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	return false;
}

bool addNormalUser(normalUser record)
{
	int fd=open("NormalUserdb",O_RDWR,0744); //We have to create a New Normal User
	bool result;
	
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_END;
	lock.l_start=(-1)*sizeof(normalUser);  //Only Last Record is being Locked
	lock.l_len=sizeof(normalUser);	         
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);	
	
	normalUser User;
	lseek(fd,(-1)*sizeof(normalUser),SEEK_END);  
	read(fd,&User,sizeof(normalUser));
		
	record.userID=User.userID+1; //Creating new userID and AccID
	record.account_no=User.account_no+1;
	strcpy(record.status,"ACTIVE");
	
	int j=write(fd,&record,sizeof(normalUser)); //Checking whether it got written
	if(j!=0)	result=true;
	else	result=false;
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool addJointUser(jointUser record)
{
	int fd=open("JointUserdb",O_RDWR,0744);
	bool result;
	
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_END;
	lock.l_start=(-1)*sizeof(jointUser);    
	lock.l_len=sizeof(jointUser);	           
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);	

	jointUser User;
	lseek(fd,(-1)*sizeof(jointUser),SEEK_END);  
	read(fd,&User,sizeof(jointUser));
		
	record.userID=User.userID+1;
	record.account_no=User.account_no+1;
	strcpy(record.status,"ACTIVE");
	
	int j=write(fd,&record,sizeof(jointUser)); 
	if(j!=0)	result=true;
	else	result=false;
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool deleteNormalUser(int ID)
{
	int realid=ID-10000;
	int fd=open("NormalUserdb",O_RDWR,0744);
	bool result;

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(realid)*sizeof(normalUser);    
	lock.l_len=sizeof(normalUser);	           
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);	

	normalUser User;
	lseek(fd,(realid)*sizeof(normalUser),SEEK_SET); 
	read(fd,&User,sizeof(normalUser));
	
	if(!strcmp(User.status,"ACTIVE"))
	{	
		strcpy(User.status,"CLOSED");
		User.balance=0;
		lseek(fd,(-1)*sizeof(normalUser),SEEK_CUR); 
		//Rewriting the Structure
		int j=write(fd,&User,sizeof(normalUser));
		if(j!=0)	result=true;
		else		result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool deleteJointUser(int ID)
{
	int realid=ID-10000;
	int fd=open("JointUserdb",O_RDWR,0744);
	bool result;
	
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(realid)*sizeof(jointUser);    
	lock.l_len=sizeof(jointUser);	            
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);	

	jointUser User;
	lseek(fd,(realid)*sizeof(jointUser),SEEK_SET); 
	read(fd,&User,sizeof(jointUser));
	
	if(!strcmp(User.status,"ACTIVE"))
	{	
		strcpy(User.status,"CLOSED");
		User.balance=0;
		
		lseek(fd,(-1)*sizeof(jointUser),SEEK_CUR); 
		int j=write(fd,&User,sizeof(jointUser));
		if(j!=0)	result=true;
		else		result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool modifyNormalUser(normalUser modUser)
{
	int realid=modUser.userID-10000;
	int fd=open("NormalUserdb",O_RDWR,0744);
	bool result=false;
	
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(realid)*sizeof(normalUser);   
	lock.l_len=sizeof(normalUser);	             
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);	

	normalUser User;
	lseek(fd,(realid)*sizeof(normalUser),SEEK_SET);  
	read(fd,&User,sizeof(normalUser));
	
	if(!strcmp(User.status,"ACTIVE") && (modUser.account_no==User.account_no))
	{	
		strcpy(modUser.status,"ACTIVE");
		lseek(fd,(-1)*sizeof(normalUser),SEEK_CUR); 
		//Rewriting
		int j=write(fd,&modUser,sizeof(normalUser));
		if(j!=0)	result=true;
		else		result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool modifyJointUser(jointUser modUser)
{
	int realid=modUser.userID-10000;
	int fd=open("JointUserdb",O_RDWR,0744);
	bool result=false;
	
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(realid)*sizeof(jointUser);   
	lock.l_len=sizeof(jointUser);	           
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);	

	jointUser User;
	lseek(fd,(realid)*sizeof(jointUser),SEEK_SET); 
	read(fd,&User,sizeof(jointUser));
	
	if(!strcmp(User.status,"ACTIVE")  && (modUser.account_no==User.account_no))
	{	
		strcpy(modUser.status,"ACTIVE");
		lseek(fd,(-1)*sizeof(jointUser),SEEK_CUR); 
		int j=write(fd,&modUser,sizeof(jointUser));
		if(j!=0)	result=true;
		else		result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

void provideService(int nsd)
{
	int len, option2, type, choice, acctype, userID;
	bool check;
	while(1)
	{
		read(nsd, &choice, sizeof(choice));
		printf("Option Selected: %d\n",choice);

		if(choice==1)
		{
			normalUser user1;
			acctype=1;
			len=read(nsd,&user1,sizeof(normalUser));
			printf("User's Credentials\nUsername : %d\n",user1.userID);
			printf("Password : %s\n",user1.password);
			userID=user1.userID;
			check=authenticateNU(user1);
			write(nsd,&check,sizeof(check));
		}
		else if(choice==2)
		{
			jointUser user2;
			acctype=2;
			len=read(nsd,&user2,sizeof(jointUser));
			userID=user2.userID;
			printf("User's Credentials\nUsername : %d\n",user2.userID);
			printf("Password : %s\n",user2.password);
			check=authenticateJU(user2);
			write(nsd,&check,sizeof(check));
		}
		else if(choice==3)
		{
			admin user3;
			acctype=3;
			len=read(nsd,&user3,sizeof(admin));
			userID=user3.userID;
			printf("User's Credentials\nUsername : %d\n",user3.userID);
			printf("Password : %s\n",user3.password);
			check=authenticateAdmin(user3);
			write(nsd,&check,sizeof(check));
		}
		else
		{
			check=false;
			write(nsd,&check,sizeof(check));
		}

		if(check)	break;		
	}

	while(1)
	{
		read(nsd,&option2,sizeof(int));
		//NOrmal or JOint Account
		if(choice==1 || choice==2)
		{
			if(option2==1)
			{
				float amt;
				read(nsd,&amt,sizeof(float));
				check = depositMoney(acctype,userID,amt);
				write(nsd,&check,sizeof(check));
			}
			else if(option2==2)
			{
				float amt;
				read(nsd,&amt,sizeof(float));
				check = withdrawMoney(acctype,userID,amt);
				write(nsd,&check,sizeof(check));
			}
			else if(option2==3){
				float amt;
				amt = getBalance(acctype,userID);
				write(nsd,&amt,sizeof(float));
			}
			else if(option2==4){
				char pwd[10];
				read(nsd,pwd,sizeof(pwd));
				check = alterPassword(acctype,userID,pwd);
				write(nsd,&check,sizeof(check));
			}
			else if(option2==5)
			{
				//Viewing Details of their respective Accounts
				if(choice==1)
				{
					normalUser user1 = getNU(userID);
					write(nsd,&user1,sizeof(normalUser));
				}
				else if(choice==2)
				{
					jointUser user2 = getJU(userID);
					write(nsd,&user2,sizeof(jointUser));
				}
			}
			else if(option2==6)	break;
		}
		//For admin
		else if(choice==3)
		{
			read(nsd,&type,sizeof(int));
			if(option2==1){
				if(type==1)
				{
					normalUser newbie;
					read(nsd,&newbie,sizeof(normalUser));
					check=addNormalUser(newbie);
					write(nsd,&check,sizeof(check));
				}
				else if(type==2)
				{
					jointUser newbie;
					read(nsd,&newbie,sizeof(jointUser));
					check=addJointUser(newbie);
					write(nsd,&check,sizeof(check));
				}
			}
			else if(option2==2)
			{
				if(type==1)
				{
					int TobeDeleted;
					read(nsd,&TobeDeleted,sizeof(int));
					check=deleteNormalUser(TobeDeleted);
					write(nsd,&check,sizeof(check));
				}
				else if(type==2)
				{
					int TobeDeleted;
					read(nsd,&TobeDeleted,sizeof(int));
					check=deleteJointUser(TobeDeleted);
					write(nsd,&check,sizeof(check));
				}
			}
			else if(option2==3)
			{
				if(type==1)
				{
					normalUser modUser1;
					read(nsd,&modUser1,sizeof(normalUser));
					check=modifyNormalUser(modUser1);
					write(nsd,&check,sizeof(check));
				}
				else if(type==2)
				{
					jointUser modUser2;
					read(nsd,&modUser2,sizeof(jointUser));
					check=modifyJointUser(modUser2);
					write(nsd,&check,sizeof(check));
				}
			}
			else if(option2==4)
			{
				if(type==1)
				{
					normalUser UserSearched;
					int userID1;
					read(nsd,&userID1,sizeof(int));
					UserSearched=getNU(userID1);
					write(nsd,&UserSearched,sizeof(normalUser));
				}
				else if(type==2)
				{
					jointUser UserSearched;
					int userID2;
					read(nsd,&userID2,sizeof(int));
					UserSearched=getJU(userID2);
					write(nsd,&UserSearched,sizeof(jointUser));
				}
			}
			else if(option2==5)	break;
		}
	}
	close(nsd);
	write(1,"CLIENT SESSION ENDED\n",sizeof("CLIENT SESSION ENDED\n"));
	return;
}

void *talktoClient(void *nsd) 
{
	int nsd1 = *(int*)nsd;
	provideService(nsd1);
}

int main()
{
	struct sockaddr_in server, client;
	int sd, nsd, sizeofcli;
	pthread_t thread;
	bool result;

	//AF_INET for internet communication
	sd=socket(AF_INET,SOCK_STREAM,0);

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5555);

	//Bind -> Listen -> Accept
	bind(sd,(struct sockaddr *)&server,sizeof(server));
	listen(sd,5);	
	
	//Writing to the terminal 
	write(1,"Waiting for the Client.....\n",sizeof("Waiting for the Client.....\n"));
	while(1)
	{
		sizeofcli = sizeof(client);
		nsd = accept(sd,(struct sockaddr *)&client,&sizeofcli);
		write(1,"\nConnection established with the Client\n",sizeof("\nConnection established with the Client\n"));
		
		if(pthread_create(&thread,NULL,talktoClient,(void*)&nsd)<0)
		{
			perror("Error while creating the thread!!");
			return 1;
		}		
	}

	pthread_exit(NULL);
	close(sd);
	return 0;
}