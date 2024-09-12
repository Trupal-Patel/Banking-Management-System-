#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdbool.h>

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

//Global Variables for collecting optionselected at the current moment and userid of the host
int option,UserID;
void chooseOption(int sd);
void showMenu(int sd);
void searchAccount(int sd);
void modifyAccount(int sd);
void deleteAccount(int sd);
void addAccount(int sd);
void viewDetails(int sd);
void changePassword(int sd);
void balanceEnquiry(int sd);
void withdraw(int sd);
void deposit(int sd);
void LoginasAdmin(int sd);
void LoginasJU(int sd);
void LoginasNU(int sd);

int main()
{
	struct sockaddr_in server;
	int sd,msgsizegth;
	char buff[50];
	char result;

	//Connecting to the Socket
	sd=socket(AF_INET,SOCK_STREAM,0);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1"); //same machine
	server.sin_port=htons(5555);

	//Connecting to the Server
	connect(sd,(struct sockaddr *)&server,sizeof(server));

	chooseOption(sd);
	showMenu(sd);	

	close(sd);

	return 0;
}

void LoginasNU(int sd)
{
	bool result;
	normalUser User;

	write(1,"User ID : ",sizeof("User ID : "));
	scanf("%d",&User.userID);
	UserID=User.userID;
	write(1,"Password : ",sizeof("Password : "));
	scanf("%s",User.password);

	//Writing this to the SERVER for authentication
	write(sd,&option,sizeof(int));
	write(sd,&User,sizeof(normalUser));

	read(sd,&result,sizeof(result));

	if(!result)
	{
		write(1,"Invalid User Credentials!!\n\n",sizeof("Invalid User Credentials!!\n\n"));
		chooseOption(sd);
	}
	else
	{
		write(1,"Logged In!!\n\n",sizeof("Logged In!!\n\n"));
	}
	return;
}

void LoginasJU(int sd)
{
	bool result;
	jointUser User;

	write(1,"User ID : ",sizeof("User ID : "));
	scanf("%d",&User.userID);
	UserID=User.userID;
	write(1,"Password : ",sizeof("Password : "));
	scanf("%s",User.password);

	write(sd,&option,sizeof(int));
	write(sd,&User,sizeof(jointUser));

	read(sd,&result,sizeof(result));

	if(!result)
	{
		write(1,"Invalid User Credentials!!\n\n",sizeof("Invalid User Credentials!!\n\n"));
		chooseOption(sd);
	}
	else
	{
		write(1,"Logged In !!\n\n",sizeof("Logged In !!\n\n"));
	}
	return;
}

void LoginasAdmin(int sd)
{
	bool result;
	admin User;
	write(1,"User ID : ",sizeof("User ID : "));
	scanf("%d",&User.userID);
	UserID=User.userID;
	write(1,"Password : ",sizeof("Password : "));
	scanf("%s",User.password);

	write(sd,&option,sizeof(int));
	write(sd,&User,sizeof(admin));

	read(sd,&result,sizeof(result));

	if(!result)
	{
		write(1,"Invalid User Credentials!!\n\n",sizeof("Invalid User Credentials!!\n\n"));
		chooseOption(sd);
	}
	else
	{
		write(1,"Logged In !!\n\n",sizeof("Logged In !!\n\n"));
	}
	return;
}

void deposit(int sd)
{
	float amt;
	int option2=1;
	bool result;

	write(1,"Amount to be Deposited : Rs.",sizeof("Amount to be Deposited : Rs."));
	scanf("%f",&amt);

	while(amt<=0)
	{
		printf("Enter a valid amount!!\n");
		write(1,"Amount to be Deposited : Rs.",sizeof("Amount to be Deposited : Rs."));
		scanf("%f",&amt);
	}

	write(sd,&option2,sizeof(int));
	write(sd,&amt,sizeof(float));

	read(sd,&result,sizeof(result)); 

	if(!result)
	{
		write(1,"Error while depositing your Money!!\n\n",sizeof("Error while depositing your Money!!\n\n"));
	}
	else{
		write(1,"Succesfully deposited!!\n\n",sizeof("Succesfully deposited!!\n\n"));
	}
	showMenu(sd);
	return;
}

void withdraw(int sd)
{
	float amt;
	int option2=2;
	bool result;

	write(1,"Amount to be Withdrawn : Rs.",sizeof("Amount to be Withdrawn : Rs."));
	scanf("%f",&amt);

	while(amt<=0)
	{
		printf("Enter a valid amount!!\n");
		write(1,"Amount to be Withdrawn : Rs.",sizeof("Amount to be Withdrawn : Rs."));
		scanf("%f",&amt);
	}

	write(sd,&option2,sizeof(int));
	write(sd,&amt,sizeof(float));

	read(sd,&result,sizeof(result)); 

	if(!result){
		write(1,"Error while withdrawing your Money !!\n\n",sizeof("Error while withdrawing your Money !!\n\n"));
	}
	else{
		write(1,"Succesfully Withdrawn!!\n\n",sizeof("Succesfully Withdrawn!!\n\n"));
	}
	showMenu(sd);
	return;
}

void balanceEnquiry(int sd)
{
	float amt;
	int option2 = 3;

	write(sd,&option2,sizeof(int));
	read(sd,&amt,sizeof(float));

	write(1,"Current Available Balance: Rs.",sizeof("Current Available Balance: Rs."));
	printf("%0.4f\n\n",amt);

	showMenu(sd);
	return;
}

void changePassword(int sd)
{
	int option2 = 4;
	char newPass[10];
	bool result;

	write(1,"Enter the new password : ",sizeof("Enter the new password : "));
	scanf("%s",newPass);

	write(sd,&option2,sizeof(int));
	write(sd,newPass,sizeof(newPass));

	read(sd,&result,sizeof(result)); 

	if(!result)
	{
		write(1,"Error while changing your password!!\n\n",sizeof("Error while changing your password!!\n\n"));
	}
	else
	{
		write(1,"Succesfully changed your password!!\n\n",sizeof("Succesfully changed your password!!\n\n"));
	}
	showMenu(sd);
	return;
}

void viewDetails(int sd)
{
	int option2=5;

	write(sd,&option2,sizeof(int));

	if(option==1)
	{
		normalUser User1;
		read(sd,&User1,sizeof(normalUser));
		
		printf("User ID : %d\n",User1.userID);
		printf("Name : %s\n",User1.name);
		printf("Account Number : %d\n",User1.account_no);
		printf("Available Balance : Rs.%0.4f\n",User1.balance);
		printf("Status : %s\n\n",User1.status);
	}
	else if(option==2)
	{
		jointUser User2;
		read(sd,&User2,sizeof(jointUser));
		
		printf("User ID : %d\n",User2.userID);
		printf("Main Account Holder's Name : %s\n",User2.name1);
		printf("Other Account Holder's Name : %s\n",User2.name2);
		printf("Account Number : %d\n",User2.account_no);
		printf("Available Balance : Rs.%0.4f\n",User2.balance);
		printf("Status : %s\n\n",User2.status);
	}
	showMenu(sd);
	return;
}

void addAccount(int sd)
{
	int option2=1;
	int type;
	bool result;

	write(sd,&option2,sizeof(int));

	write(1,"Enter the type of account, you want to create (1. Normal Account 2. Joint Account) : ",sizeof("Enter the type of account, you want to create (1. Normal Account 2. Joint Account) : "));
	scanf("%d",&type);

	write(sd,&type,sizeof(int));

	if(type==1)
	{
		normalUser User1;
		write(1,"Account Holder's Name : ",sizeof("Account Holder's Name : "));
		scanf(" %[^\n]",User1.name);
		write(1,"Password : ",sizeof("Password : "));
		scanf("%s",User1.password);
		write(1,"Initial Deposit : Rs.",sizeof("Initial Deposit : Rs."));
		scanf("%f",&User1.balance);
		write(sd,&User1,sizeof(normalUser));
	}

	if(type==2)
	{
		jointUser User2;
		write(1,"Name of the primary account holder : ",sizeof("Name of the primary account holder : "));
		scanf(" %[^\n]",User2.name1);
		write(1,"Name of the other account holder : ",sizeof("Name of the other account holder : "));
		scanf(" %[^\n]",User2.name2);
		write(1,"Password : ",sizeof("Password : "));
		scanf("%s",User2.password);
		write(1,"Initial Deposit : Rs.",sizeof("Initial Deposit : Rs."));
		scanf("%f",&User2.balance);
		write(sd,&User2,sizeof(jointUser));
	}
	
	read(sd,&result,sizeof(result)); 

	if(!result){
		write(1,"Error while adding your account!!\n\n",sizeof("Error while adding your account!!\n\n"));
	}
	else{
		write(1,"Succesfully added your account!!\n\n",sizeof("Succesfully added your account!!\n\n"));
	}
	showMenu(sd);
	return;
}

void deleteAccount(int sd)
{
	int option2=2;
	int type,userID;
	bool result;

	write(sd,&option2,sizeof(int));

	write(1,"Enter the type of account(1. Normal Account 2. Joint Account) : ",sizeof("Enter the type of account(1. Normal Account 2. Joint Account) : "));
	scanf("%d",&type);
	
	write(sd,&type,sizeof(int));

	write(1,"User ID : ",sizeof("User ID : "));
	scanf("%d",&userID);
	write(sd,&userID,sizeof(int));
	
	read(sd,&result,sizeof(result)); 

	if(!result)
	{
		write(1,"Error while deleting your account !!\n\n",sizeof("Error while deleting your account !!\n\n"));
	}
	else
	{
		write(1,"Succesfully deleted your account!!\n\n",sizeof("Succesfully deleted your account!!\n\n"));
	}
	showMenu(sd);
	return;
}

void modifyAccount(int sd)
{
	int option2=3;
	int type;
	bool result;

	write(sd,&option2,sizeof(int));

	write(1,"Enter the type account(1: Normal Account 2: Joint Account) : ",sizeof("Enter the type account(1: Normal Account 2: Joint Account) : "));
	scanf("%d",&type);

	write(sd,&type,sizeof(int));

	if(type==1)
	{
		normalUser User1;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&User1.userID);
		write(1,"Account Number : ",sizeof("Account Number : "));
		scanf("%d",&User1.account_no);
		write(1,"New Account Holder's Name : ",sizeof("New Account Holder's Name : "));
		scanf(" %[^\n]",User1.name);
		write(1,"New Password : ",sizeof("New Password : "));
		scanf("%s",User1.password);
		write(1,"New Balance : ",sizeof("New Balance : "));
		scanf("%f",&User1.balance);
		write(sd,&User1,sizeof(normalUser));
	}

	if(type==2)
	{
		jointUser User2;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&User2.userID);
		write(1,"Account Number : ",sizeof("Account Number : "));
		scanf("%d",&User2.account_no);
		write(1,"New Name of the primary account holder : ",sizeof("New Name of the primary account holder : "));
		scanf(" %[^\n]",User2.name1);
		write(1,"New Name of the other account holder : ",sizeof("New Name of the other account holder : "));
		scanf(" %[^\n]",User2.name2);
		write(1,"New Password : ",sizeof("New Password : "));
		scanf("%s",User2.password);
		write(1,"New Balance : ",sizeof("New Balance : "));
		scanf("%f",&User2.balance);
		write(sd,&User2,sizeof(jointUser));
	}
	
	read(sd,&result,sizeof(result)); //from the server

	if(!result){

		write(1,"Error while modifying your account!!\n\n",sizeof("Error while modifying your account!!\n\n"));
	}
	else
	{
		write(1,"Succesfully modified your account!!\n\n",sizeof("Succesfully modified your account!!\n\n"));
	}
	showMenu(sd);
	return;
}

void searchAccount(int sd)
{
	int option2=4;
	int type,size;
	bool result;

	write(sd,&option2,sizeof(int));

	write(1,"Enter the type account(1: Normal Account 2: Joint Account) : ",sizeof("Enter the type account(1: Normal Account 2: Joint Account) : "));
	scanf("%d",&type);

	write(sd,&type,sizeof(int));

	if(type==1)
	{
		normalUser User1;
		int userID;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&userID);
		write(sd,&userID,sizeof(int));
		
		size=read(sd,&User1,sizeof(normalUser));
		if(size==0 || User1.userID < 0)
		{
			write(1,"Please re-check your User ID!!\n\n",sizeof("Please re-check your User ID!!\n\n"));
		}
		else if(User1.userID > 0)
		{
			printf("User ID : %d\n",User1.userID);
			printf("Name : %s\n",User1.name);
			printf("Account Number : %d\n",User1.account_no);
			printf("Available Balance : Rs.%0.4f\n",User1.balance);
			printf("Status : %s\n\n",User1.status);
		}
	}

	else if(type==2)
	{
		jointUser User2;
		int userID1;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&userID1);
		write(sd,&userID1,sizeof(int));
		
		size=read(sd,&User2,sizeof(jointUser));
		if(size==0 || User2.userID < 0)
		{
			write(1,"Please re-check your User ID!!\n\n",sizeof("Please re-check your User ID!!\n\n"));
		}
		else if(User2.userID >0)
		{
			printf("User ID : %d\n",User2.userID);
			printf("Main Account Holder's Name : %s\n",User2.name1);
			printf("Other Account Holder's Name : %s\n",User2.name2);
			printf("Account Number : %d\n",User2.account_no);
			printf("Available Balance : Rs.%0.4f\n",User2.balance);
			printf("Status : %s\n\n",User2.status);
		}
	}
	showMenu(sd);
	return;
}

void showMenu(int sd)
{
	int option2;

	if(option==1 || option==2)
	{
		write(1,"Services we can provice -\n",sizeof("Services we can provice -\n"));
		write(1,"1 Deposit\n",sizeof("1 Deposit\n"));
		write(1,"2 Withdraw\n",sizeof("2 Withdraw\n"));
		write(1,"3 Balance Enquiry\n",sizeof("3 Balance Enquiry\n"));
		write(1,"4 Password Change\n",sizeof("4 Password Change\n"));
		write(1,"5 View Details\n",sizeof("5 View Details\n"));
		write(1,"6 Exit\n",sizeof("6 Exit\n"));
	
		write(1,"Enter Your Choice : ",sizeof("Enter Your Choice : "));
		scanf("%d",&option2);
		
		switch(option2)
		{
			case 1 :
				deposit(sd);
				break;
			case 2 :
				withdraw(sd);
				break;
			case 3 :
				balanceEnquiry(sd);
				break;
			case 4 :
				changePassword(sd);
				break;
			case 5 :
				viewDetails(sd);
				break;
			case 6 :
				write(sd,&option2,sizeof(int));
				write(1,"Thank you!! We were happy to serve you !!\n",sizeof("Thank you!! We were happy to serve you !!\n"));
				exit(0);
			default :
				write(1,"Invalid option!!\n\n",sizeof("Invalid option!!\n\n"));
				showMenu(sd);
				break;
		}
	}
	else if(option==3)
	{
		write(1,"Services we can provice -\n",sizeof("Services we can provice -\n"));
		write(1,"1 Add Account\n",sizeof("1 Add Account\n"));
		write(1,"2 Delete Account\n",sizeof("2 Delete Account\n"));
		write(1,"3 Modify Account\n",sizeof("3 Modify Account\n"));
		write(1,"4 Search Account\n",sizeof("4 Search Account\n"));
		write(1,"5 Exit\n",sizeof("6 Exit\n"));

		write(1,"Enter your Choice : ",sizeof("Enter your Choice : "));
		scanf("%d",&option2);
		printf("Option : %d\n",option2);
		
		switch(option2)
		{
			case 1 :
				addAccount(sd);
				break;
			case 2 :
				deleteAccount(sd);
				break;
			case 3 :
				modifyAccount(sd);
				break;
			case 4 :
				searchAccount(sd);
				break;
			case 5 :
				write(sd,&option2,sizeof(int));
				write(1,"Thank you!! We were happy to serve you !!\n",sizeof("Thank you!! We were happy to serve you !!\n"));
				exit(0);
			default :
				write(1,"Invalid option!!\n\n",sizeof("Invalid option!!\n\n"));
				showMenu(sd);
				break;
		}
	}
}

void chooseOption(int sd)
{
	while(1)
	{
		write(1,"1 Normal-User Login\n",sizeof("1 Normal-User Login\n"));
		write(1,"2 Joint Account-User Login\n",sizeof("2 Joint Account-User Login\n"));
		write(1,"3 Admin Login\n",sizeof("3 Admin Login\n"));
		
		write(1,"Enter your Choice : ",sizeof("Enter your Choice : "));
		scanf("%d",&option);

		if(option == 1)
		{
			LoginasNU(sd); 
			break;
		}
		else if(option == 2)
		{
			LoginasJU(sd);
			break;
		}
		else if(option == 3)
		{
			LoginasAdmin(sd);
			break;
		}
		else write(1,"Invalid option!!\n\n",sizeof("Invalid option!!\n\n"));
	}
	return;
}