#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

//Basic Structures for different types of Users

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

//Initialising the Databases

int main()
{
	int fd=open("Admindb",O_RDWR | O_CREAT,0744);
	int choice ; int id = 10000; //Starting value of the ids
	admin admin;
	printf("Enter the name of the admin: ");
	scanf(" %s",admin.username);
	printf("Enter password: ");
	scanf(" %s",admin.password);
	admin.userID=id++;
	printf("Your userID is : %d\n",admin.userID);
	write(fd,&admin,sizeof(admin));
	printf("Do you want to create More Admins(1/0)?\n");
	scanf("%d",&choice);
	while(choice == 1)
	{
		printf("Enter the name of the admin: ");
		scanf(" %[^\n]", admin.username);
		printf("Enter password: ");
		scanf(" %[^\n]", admin.password);
		admin.userID=id++;
		printf("Your userID is : %d\n",admin.userID);
		write(fd,&admin,sizeof(admin)); //New Admin Written
		printf("Do you want to create More Admins(1/0)? \n");
		scanf("%d",&choice);
	}
	close(fd);

	//Same procedure has been followed for NormalUser and JOintUser
	fd=open("NormalUserdb",O_RDWR | O_CREAT,0744);
	choice = 'Y'; id = 10000;
	normalUser NU;
	printf("Enter the name of the normal user: ");
	scanf(" %[^\n]",NU.name);
	printf("Enter password: ");
	scanf(" %[^\n]",NU.password);
	NU.userID=id++;
	NU.balance=9999; //Initial Balance
	NU.account_no=(NU.userID-10000)+1000000; //Accountid has been derived from userid for simplicity
	printf("Your userID is : %d\n",NU.userID);
	strcpy(NU.status,"ACTIVE");
	write(fd,&NU,sizeof(normalUser));
	printf("Do you want to create More NormalUser Accounts(1/0)? \n");
	scanf("%d",&choice);
	while(choice == 1)
	{
		printf("Enter the name of the normal user: ");
		scanf(" %[^\n]",NU.name);
		printf("Enter password: ");
		scanf(" %[^\n]",NU.password);
		NU.userID=id++;
		NU.balance=9999;
		NU.account_no=(NU.userID-10000)+1000000;
		printf("Your userID is : %d\n",NU.userID);
		strcpy(NU.status,"ACTIVE");
		write(fd,&NU,sizeof(normalUser));
		printf("Do you want to create More NormalUser Accounts(1/0)? \n");
		scanf("%d",&choice);
	}
	close(fd);

	fd=open("JointUserdb",O_RDWR | O_CREAT,0744);
	choice='Y'; id = 10000;
	jointUser JU;
	printf("Enter the main name of the joint user: ");
	scanf(" %[^\n]",JU.name1);
	printf("Enter the second name of the joint user: ");
	scanf(" %[^\n]",JU.name2);
	printf("Enter password: ");
	scanf(" %[^\n]",JU.password);
	JU.userID=id++;
	JU.balance=9999;
	JU.account_no=(JU.userID-10000)+1000000;
	printf("Your userID is : %d\n",JU.userID);
	strcpy(JU.status,"ACTIVE");
	write(fd,&JU,sizeof(jointUser));
	printf("Do you want to create More JointUser Accounts(1/0)? \n");
	scanf("%d",&choice);
	while(choice == 1)
	{
		printf("Enter the main name of the joint user: ");
		scanf(" %[^\n]",JU.name1);
		printf("Enter the second name of the joint user: ");
		scanf(" %[^\n]",JU.name2);
		printf("Enter password: ");
		scanf(" %[^\n]",JU.password);
		JU.userID=id++;
		JU.balance=9999;
		JU.account_no=(JU.userID-10000)+1000000;
		printf("Your userID is : %d\n",JU.userID);
		strcpy(JU.status,"ACTIVE");
		write(fd,&JU,sizeof(jointUser));
		printf("Do you want to create More JointUser Accounts(1/0)? \n");
		scanf("%d",&choice);
	}
	close(fd);
	return 0;
}


