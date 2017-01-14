#include<windows.h>
#include "mysql.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#pragma comment(lib,"libmysql.lib")

MYSQL *conn;
char username[50], password[50], year[50], quarter[3], next_y[50], next_q[3],dis_type[50];
int dis_num = 9999;

void login();

void student_menu();

void show_course_details(char *c_num) {
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	char query[200], year[10], quarter[10],next_y[10],next_q[10];
	memset(query, 0, 200);
	memset(quarter, 0, 10);
	memset(year, 0, 10);
	printf("****************************\n");
	printf("Course number:\t%s\n", c_num);
	//Course name
	sprintf(query, "SELECT UoSName FROM unitofstudy WHERE UoSCode=\"%s\" ", c_num);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	row = mysql_fetch_row(res_set);
	printf("Course Title:\t%s\n", row[0]);
	mysql_free_result(res_set);
	memset(query, 0, 200);
	//Year
	sprintf(query, "SELECT Year FROM transcript WHERE UoSCode=\"%s\" and StudId=\"%s\" ", c_num, username);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	row = mysql_fetch_row(res_set);
	printf("Year:\t%s\n", row[0]);
	strcpy(year, row[0]);
	mysql_free_result(res_set);
	memset(query, 0, 200);
	//Semester
	sprintf(query, "SELECT Semester FROM transcript WHERE UoSCode=\"%s\"and StudId=\"%s\"  ", c_num, username);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	row = mysql_fetch_row(res_set);
	printf("Semester:\t%s\n", row[0]);
	strcpy(quarter, row[0]);
	mysql_free_result(res_set);
	memset(query, 0, 200);
	//enrollment
	sprintf(query, "SELECT Enrollment FROM uosoffering WHERE UoSCode=\"%s\"and Year = \"%s\" and Semester=\"%s\" ", c_num, year, quarter);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	row = mysql_fetch_row(res_set);
	printf("Student Enrollment:\t%s\n", row[0]);
	mysql_free_result(res_set);
	memset(query, 0, 200);
	//maxenrollment
	sprintf(query, "SELECT MaxEnrollment FROM uosoffering WHERE UoSCode=\"%s\"and Year = \"%s\" and Semester=\"%s\" ", c_num, year, quarter);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	row = mysql_fetch_row(res_set);
	printf("Student MaxEnrollment:\t%s\n", row[0]);
	mysql_free_result(res_set);
	memset(query, 0, 200);
	//leturer name
	sprintf(query, "SELECT Name FROM faculty WHERE Id in (select InstructorId from uosoffering where UoSCode=\"%s\"and Year = \"%s\" and Semester=\"%s\") ", c_num, year, quarter);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	row = mysql_fetch_row(res_set);
	printf("Lecturer Name:\t%s\n", row[0]);
	mysql_free_result(res_set);
	memset(query, 0, 200);
	//Student Grade
	sprintf(query, "SELECT Grade FROM transcript WHERE UoSCode=\"%s\"and StudId=\"%s\"  ", c_num, username);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	row = mysql_fetch_row(res_set);
	printf("Your Grade:\t%s\n", row[0]);
	mysql_free_result(res_set);
	memset(query, 0, 200);
	printf("****************************\n");
}

void display_transcript_operation() {
	printf("Which operation you want to take?\n");
	printf("1.Show course Details.\n");
	printf("2.Back to Student Menu.\n");
	printf("****************************\n");
	printf("Please enter the operation number.\n");
	int op_num = 0;
	scanf(" %d", &op_num);
	switch (op_num) {
	case 1: {printf("Please enter the course number to show the details.\n");
		char c_num[20];
		scanf(" %s", &c_num);
		system("cls");
		printf("Now show the details of course %s.\n", c_num);
		show_course_details(c_num);
		display_transcript_operation();
		break;
	}
	case 2:printf("Go back to Student Menu.\n"); system("cls"); student_menu(); break;
	default:printf("Operation number is wrong. Please enter the correct number!\n"); getchar(); getchar(); system("cls");  display_transcript_operation();
	}
}

void transcript() {
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	char query[200];
	printf("****************************\n");
	memset(query, 0, 200);
	sprintf(query, "SELECT Year,Semester,UoSCode,Grade FROM transcript where StudId=\"%s\" ", username);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	int numrows = (int)mysql_num_rows(res_set);
	printf("Year\tQuarter\tCourse Number\tGrade\n");
	for (int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(res_set);
		if (row != NULL)
		{
			
		
			printf("%s\t", row[0]);
			printf("%s\t", row[1]);
			printf("%s\t", row[2]);
			printf("%s\n", row[3]);
			
		}
	}
	printf("****************************\n");
	mysql_free_result(res_set);
	display_transcript_operation();
}


void enroll() {
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	char query[200];
	char tmp_y[50], tmp_q[3];
	int sym = 0;
	printf("****************************\n");
	//sprintf(query, "SELECT UoSCode,UoSName FROM unitofstudy where UoSCode in (select UoSCode from uosoffering where Year=\"%s\" and Semester=\"%s\" )", year, quarter);
	sprintf(query, "CALL find_enroll(\"%s\",\"%s\",\"%s\");", username, year, quarter);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	int numrows = (int)mysql_num_rows(res_set);
	printf("Here is some courses you can enroll in this quarter %s %s:\n",year,quarter);
	printf("Status\t       Course Number\t                 Course Title\t                     Enrollment\t          Max Enrollment\n");
	
	for (int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(res_set);
		if (row != NULL && atoi(row[8]) <= dis_num  &&  strcmp(row[9],dis_type)!=0 )
		{
			if (row[2] != NULL)
			{
				printf("Have enrolled\t");
			}
			else if (row[3] != NULL && (row[5] == NULL || row[6] == NULL || strcmp(row[6], "D") == 0))
			{
				printf("Not meet Re  \t");
			}
			else if (atoi(row[7]) >= atoi(row[8]))
			{
				printf("Max reached  \t");
			}
			else
			{
				printf("             \t");
			}
			printf("%s\t", row[0]);
			printf("%-50s\t", row[1]);
			printf("%s\t\t", row[7]);
			printf("      %s\n", row[8]);
		}
	}
	strcpy(next_q, quarter);
	strcpy(next_y, year);
	mysql_free_result(res_set);
	printf("****************************\n");
	char q2[] = "Q2";
	char q1[] = "Q1";
	char q4[] = "Q4";
	if (!(strcmp(next_q, q1)))
	{
		next_q[1] = next_q[1] + 1;
		next_y[3] = next_y[3] + 1;
	}
	else if (!(strcmp(next_q, q4)))
	{
		strcpy(next_q, q1);
	}
	else next_q[1] = next_q[1] + 1;
	mysql_close(conn);
	conn = mysql_init(NULL);
	mysql_real_connect(
		conn, "localhost", "root", "1234", "project3-nudb", 3310, NULL, NULL);
	sprintf(query, "CALL find_enroll(\"%s\",\"%s\",\"%s\");", username, next_y, next_q);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	numrows = (int)mysql_num_rows(res_set);
	printf("Here is some courses you can enroll in next quarter %s %s:\n", next_y, next_q);
	printf("Status\t       Course Number\t                 Course Title\t                     Enrollment\t          Max Enrollment\n");

	for (int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(res_set);
		if (row != NULL && atoi(row[8]) <= dis_num  &&  strcmp(row[9], dis_type) != 0)
		{
			if (row[2] != NULL)
			{
				printf("Have enrolled\t");
			}
			else if (row[3] != NULL && (row[5] == NULL || row[6]==NULL || strcmp(row[6],"D") == 0 ))
			{
				printf("Not meet Re  \t");
			}
			else if (atoi(row[7]) >= atoi(row[8]))
			{
				printf("Max reached  \t");
			}
			else
			{
				printf("             \t");
			}
			printf("%s\t", row[0]);
			printf("%-50s\t", row[1]);
			printf("%s\t\t", row[7]);
			printf("      %s\n", row[8]);
		}
	}
	mysql_free_result(res_set);
	mysql_close(conn);

	conn = mysql_init(NULL);
	mysql_real_connect(
		conn, "localhost", "root", "1234", "project3-nudb", 3310, NULL, NULL);
	printf("****************************\n");
	printf("Enroll class for this quarter or for the next quarter?(Input this/next)\n");
	char answer[20];
	scanf(" %s", answer);
	while (1)
	{
		if (strcmp(answer, "this") == 0)
		{
			strcpy(tmp_y, year);
			strcpy(tmp_q, quarter);
			break;
		}
		else if (strcmp(answer, "next") == 0)
		{
			strcpy(tmp_y, next_y);
			strcpy(tmp_q, next_q);
			break;
		}
		else
			printf("Wrong input!  Input this/next\n");
		scanf(" %s", answer);
	}
	//printf("%s %s", tmp_y, tmp_q);
	printf("Which Courses do you want to enroll?\n");
	printf("Please enter the Course Number.\n");
	char c_num[20];
	scanf(" %s", c_num);
	conn = mysql_init(NULL);
	mysql_real_connect(
		conn, "localhost", "root", "1234", "project3-nudb", 3310, NULL, NULL);
	sprintf(query, "CALL find_enroll(\"%s\",\"%s\",\"%s\");", username, tmp_y, tmp_q);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	numrows = (int)mysql_num_rows(res_set);
	for (int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(res_set);
		if (row != NULL && strcmp(row[0],c_num)==0)
		{
			if (row[2] != NULL)
			{
				printf("\n\nYou have enrolled this class!\n");
				sym = 5;
				break;
			}
			else if (row[3] != NULL && (row[5] == NULL || row[6] == NULL || strcmp(row[6], "D") == 0))
			{
				printf("\n\nYou don't meet the prerequisite!\n");
				sym = 2;
				break;
			}
			else if (atoi(row[7])>=atoi(row[8]))
			{
				printf("\n\nMax Enrollemt has reached!\n");
				sym = 5;
				break;
			}
			else
			{

				sym=1;
				break;
			}
			
		}
	}
	mysql_free_result(res_set);
	mysql_close(conn);
	conn = mysql_init(NULL);
	mysql_real_connect(
		conn, "localhost", "root", "1234", "project3-nudb", 3310, NULL, NULL);
	if (sym == 1)
	{
		sym = 0;
		sprintf(query, "CALL enroll_class(\"%s\",\"%s\",\"%s\",\"%s\");", username,c_num, tmp_y, tmp_q);
		//printf("%s", query);
		mysql_query(conn, query);
		mysql_close(conn);

		conn = mysql_init(NULL);
		mysql_real_connect(
			conn, "localhost", "root", "1234", "project3-nudb", 3310, NULL, NULL);
		sprintf(query, "SELECT c from monitor where item=\"cnt\"");
		mysql_query(conn, query);
		res_set = mysql_store_result(conn);
		row = mysql_fetch_row(res_set);
		if (strcmp(row[0], "0") == 0)
		{
			printf("");
		}
		else if (strcmp(row[0], "1") == 0)
		{
			printf("\nWarning: Enrollment number of the course goes below 50%% of the MaxEnrollment\n\n");
		}
		mysql_free_result(res_set);
		mysql_close(conn);
		
		printf("Enroll successfully! ");
		getchar(); getchar();
		conn = mysql_init(NULL);
		mysql_real_connect(
			conn, "localhost", "root", "1234", "project3-nudb", 3310, NULL, NULL);
		system("cls");
		student_menu();
	}
	else if (sym == 2)
	{
		sym = 0;
		sprintf(query, "SELECT PrereqUoSCode from requires where UoSCode=\"%s\";", c_num);
		//printf("%s", query);
		mysql_query(conn, query);
		res_set = mysql_store_result(conn);
		numrows = (int)mysql_num_rows(res_set);
		printf("\nThe prerequisite of course %s : \n\n",c_num);
		for (int i = 0; i < numrows; i++)
		{
			row = mysql_fetch_row(res_set);
			if (row != NULL)
			{
				printf("%s\n", row[0]);
			}
		}
		mysql_free_result(res_set);
		mysql_close(conn);
		getchar(); getchar();
		conn = mysql_init(NULL);
		mysql_real_connect(
			conn, "localhost", "root", "1234", "project3-nudb", 3310, NULL, NULL);
		system("cls");
		student_menu();
	}
	else if(sym == 5)
	{
		getchar(); getchar();
		system("cls");
		student_menu();
	}
	else
	{
		printf("\nCannot find the course %s!\n\n", c_num);
		getchar(); getchar();
		system("cls");
		student_menu();
	}
}

void withdraw() {
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	char query[200];
	char tmp_y[50], tmp_q[3];
	int sym = 0;
	printf("****************************\n");
	//sprintf(query, "SELECT UoSCode,UoSName FROM unitofstudy where UoSCode in (select UoSCode from uosoffering where Year=\"%s\" and Semester=\"%s\" )", year, quarter);
	sprintf(query, "CALL find_withdraw(\"%s\",\"%s\",\"%s\");", username, year, quarter);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	int numrows = (int)mysql_num_rows(res_set);
	printf("Here is some courses you can withdraw in this quarter %s %s:\n", year, quarter);
	printf("Course Number\t                 Course Title\n");

	for (int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(res_set);
		if (row != NULL)
		{
			if (row[2] != NULL)
			{
				printf("%s\t", row[0]);
				printf("%s\n", row[1]);
			}			
		}
	}
	strcpy(next_q, quarter);
	strcpy(next_y, year);
	mysql_free_result(res_set);
	printf("****************************\n");
	char q2[] = "Q2";
	char q1[] = "Q1";
	char q4[] = "Q4";
	if (!(strcmp(next_q, q1)))
	{
		next_q[1] = next_q[1] + 1;
		next_y[3] = next_y[3] + 1;
	}
	else if (!(strcmp(next_q, q4)))
	{
		strcpy(next_q, q1);
	}
	else next_q[1] = next_q[1] + 1;
	mysql_close(conn);
	conn = mysql_init(NULL);
	mysql_real_connect(
		conn, "localhost", "root", "1234", "project3-nudb", 3310, NULL, NULL);
	sprintf(query, "CALL find_withdraw(\"%s\",\"%s\",\"%s\");", username, next_y, next_q);
	mysql_query(conn, query);

	res_set = mysql_store_result(conn);
	numrows = (int)mysql_num_rows(res_set);
	printf("Here is some courses you can withdraw for the next quarter %s %s:\n", next_y, next_q);
	printf("Course Number\tCourse Title\n");

	for (int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(res_set);
		if (row != NULL)
		{
			if (row[2] != NULL)
			{
				printf("%s\t", row[0]);
				printf("%s\n", row[1]);
			}
		}
	}
	mysql_free_result(res_set);
	mysql_close(conn);

	printf("****************************\n");
	printf("Withdraw class for this quarter or for the next quarter?(Input this/next)\n");
	char answer[20];
	scanf(" %s", answer);
	while (1)
	{
		if (strcmp(answer, "this") == 0)
		{
			strcpy(tmp_y, year);
			strcpy(tmp_q, quarter);
			break;
		}
		else if (strcmp(answer, "next") == 0)
		{
			strcpy(tmp_y, next_y);
			strcpy(tmp_q, next_q);
			break;
		}
		else
			printf("Wrong input!  Input this/next\n");
		scanf(" %s", answer);
	}

	//printf("%s %s", tmp_y, tmp_q);
	printf("Which Courses do you want to withdraw?\n");
	printf("Please enter the Course Number.\n");
	char c_num[20];
	scanf(" %s", c_num);

	conn = mysql_init(NULL);
	mysql_real_connect(
		conn, "localhost", "root", "1234", "project3-nudb", 3310, NULL, NULL);
	sprintf(query, "CALL withdraw_class(\"%s\",\"%s\",\"%s\",\"%s\");", username, c_num, tmp_y, tmp_q);
	if (mysql_query(conn, query))
		sprintf(stderr, mysql_error(conn));
	mysql_close(conn);
	conn = mysql_init(NULL);
	mysql_real_connect(
		conn, "localhost", "root", "1234", "project3-nudb", 3310, NULL, NULL);
	sprintf(query, "SELECT c from monitor where item=\"cnt\"");
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	row = mysql_fetch_row(res_set);
	if (strcmp(row[0],"0")==0)
	{
		printf("");
	}
	else if (strcmp(row[0], "1") == 0)
	{
		printf("\nWarning: Enrollment number of the course goes below 50%% of the MaxEnrollment\n\n");
	}
	mysql_free_result(res_set);
	mysql_close(conn);
	conn = mysql_init(NULL);
	mysql_real_connect(
		conn, "localhost", "root", "1234", "project3-nudb", 3310, NULL, NULL);

	printf("Withdraw successfully! ");
	getchar(); getchar();
	system("cls");
	student_menu();
}

void personal_details() {
	char answer[10];
	printf("****************************\n\n");
	printf("Maximum preferred number of students:\t");
	if (dis_num == 9999)
	{
		printf("Not set\n\n");
	}
	else
	{
		printf("%d\n\n",dis_num);
	}
	printf("Non-preferred classroom type:\t");
	if (dis_type[0] == '0')
	{
		printf("Not set\n\n");
	}
	else
	{
		printf("%s\n\n", dis_type);
	}
	printf("****************************\n\n");
	printf("Do you want to set your maximum preferred number of students?(Y/N)\n");
	scanf(" %s", &answer);
	while (1)
	{
		if (!strcmp(answer, "N"))
		{
			printf("\n\n");
			break;
		}
		else if (!strcmp(answer, "Y"))
		{
			printf("Please input your maximum preferred number of students:   ");
			scanf("%d", &dis_num);
			printf("\nOK!\n\n");
			break;
		}
		else
		{
			printf("Wrong input! Please input Y/N \n");
		}
		scanf(" %s", &answer);
	}
	printf("Do you want to set your non-preferred classroom type?(Y/N)\n");
	scanf(" %s", &answer);
	while (1)
	{
		if (!strcmp(answer, "N"))
		{
			printf("\n\n");
			break;
		}
		else if (!strcmp(answer, "Y"))
		{
			printf("Please input your non-preferred classroom type:   ");
			scanf("%s", &dis_type);
			printf("\nOK!\n\n");
			break;
		}
		else
		{
			printf("Wrong input! Please input Y/N \n");
		}
		scanf(" %s", &answer);
	}
	getchar(); getchar();
	system("cls");
	student_menu();
}


void display_menu_operation() {
	printf("****************************\n");
	printf("Which operation you want to take?\n");
	printf("1.Transcript\n");
	printf("2.Enroll\n");
	printf("3.Withdraw\n");
	printf("4.Personal Details\n");
	printf("5.Log out\n");
	printf("****************************\n");
	printf("Please input the operation number\n");
	int op_num;
	scanf(" %d", &op_num);
	switch (op_num) {
	case 1:printf("Transfer to Transcript screen!\n"); system("cls"); transcript(); break;
	case 2:printf("Transfer to Enroll screen!\n"); system("cls"); enroll(); break;
	case 3:printf("Transfer to Withdraw screen!\n"); system("cls"); withdraw(); break;
	case 4:printf("Transfer to Personal Details screen!\n"); system("cls"); personal_details(); break;
	case 5:printf("You are logging out your account.\n"); system("cls"); login(); break;
	default:printf("Operation number is wrong. Please enter the correct number!\n"); getchar(); getchar(); system("cls"); display_menu_operation();
	}
}

void student_menu() {
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	char query[200];
	struct tm *newtime;
	char tmpbuf[128];
	time_t lt;
	lt = time(NULL);
	memset(query, 0, 200);
	memset(quarter, 0, 3);
	memset(tmpbuf, 0, 128);
	memset(year, 0, 50);
	newtime = localtime(&lt);
	printf("****************************\n");
	strftime(tmpbuf, 128, "Today is %A, day %d of %B in the year %Y.\n", newtime);
	printf(tmpbuf);
	strftime(year, 20, "%Y", newtime);
	int month = newtime->tm_mon + 1;
	switch (month)
	{
	case 1:strcpy(quarter, "Q2"); break;
	case 2:strcpy(quarter, "Q2");  break;
	case 3:strcpy(quarter, "Q2");  break;
	case 4:strcpy(quarter, "Q3");  break;
	case 5:strcpy(quarter, "Q3");  break;
	case 6:strcpy(quarter, "Q3");  break;
	case 7:strcpy(quarter, "Q4");  break;
	case 8:strcpy(quarter, "Q4");  break;
	case 9:strcpy(quarter, "Q1");  break;
	case 10:strcpy(quarter, "Q1");  break;
	case 11:strcpy(quarter, "Q1");  break;
	case 12:strcpy(quarter, "Q1"); break;
	}
	printf("****************************\n");
	printf("You are taking courses as follows in this quarter:\n");
	sprintf(query, "SELECT UoSName FROM unitofstudy WHERE UoSCode in(SELECT UoSCode FROM transcript where StudId =\"%s\" and Year =\"%s\" and Semester =\"%s\");", username, year, quarter);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	int numrows = (int)mysql_num_rows(res_set);
	for (int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(res_set);
		if (row != NULL)
		{
			printf("%s\n", row[0]);
		}
	}
	mysql_free_result(res_set);
	display_menu_operation();

}

void login() {
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	int flag = 0;
	while (flag != 1)
	{
		mysql_query(conn, "SELECT * FROM student;");
		res_set = mysql_store_result(conn);
		int numrows = (int)mysql_num_rows(res_set);
		memset(username, 0, 50);
		memset(password, 0, 50);
		printf("****************************\n");
		printf("Please enter your username!\n");
		scanf(" %[^\n]", &username);
		printf("Please enter your password!\n");
		scanf(" %[^\n]", &password);
		printf("****************************\n");

		for (int i = 0; i < numrows; i++)
		{
			row = mysql_fetch_row(res_set);
			if (row != NULL)
			{
				if ((!(strcmp(username, row[0])) && !(strcmp(password, row[2]))))
					flag = 1;
			}
		}
		if (flag == 1) {
			system("cls");
			printf("Succeed logging! Now transfer to The STUDENT MENU.\n");
			student_menu();
		}
		else
		{
			printf("The username or password is not correct!\nInput Y to try again and input N to exit.\n\n");
			scanf(" %[^\n]", &username);
			system("cls");
			if (username[0] == 'N')
				break;
		}
	}
	mysql_free_result(res_set);
}



void main() {
	system("mode con cols=130 lines=50");
	dis_type[0] = '0';
	conn = mysql_init(NULL);
	mysql_real_connect(
		conn, "localhost", "root", "1234", "project3-nudb", 3310, NULL, CLIENT_MULTI_RESULTS);
	login();
	mysql_close(conn);
}


/*  Procedure find_enroll

delimiter $$
drop procedure if exists find_enroll $$
create procedure find_enroll(
in sid char(20),
in y char(20),
in sem char(20)
)
begin
select t1.UoSCode, t3.UoSName, t2.StudId,t4.PrereqUoSCode,t4.EnforcedSince,sum(t5.StudId),t5.Grade,t1.Enrollment,t1.MaxEnrollment,t7.Type, t6.ClassroomId
from (uosoffering t1 left outer join(select * from transcript where StudId = sid  ) as t2 on t1.UoSCode = t2.UoSCode) left outer join
(requires t4 left outer join(select * from transcript where StudId = sid  ) as t5 on t5.UoSCode = t4.PrereqUoSCode) on t1.UoSCode=t4.UoSCode,
unitofstudy t3,lecture t6,classroom t7
where t1.Year = y and
t1.Semester = sem and
t3.UoSCode = t1.UoSCode and
t6.UoSCode = t1.UoSCode and
t7.ClassroomId = t6.ClassroomId and
t6.Semester = sem and
t6.Year = y
group by t1.UoSCode;
end $$
delimiter ;

*/

/*
delimiter $$
drop procedure if exists enroll_class $$
create procedure enroll_class(
in sid char(20),
in cid char(20),
in y char(20),
in sem char(20)
)
begin
insert into transcript values(sid,cid,sem,y,null);
update uosoffering set Enrollment = Enrollment +1 where UoSCode = cid and Semester=sem and Year=y;
end $$
delimiter ;
*/

/*
delimiter $$
drop procedure if exists find_withdraw $$
create procedure find_withdraw(
in sid char(20),
in y char(20),
in sem char(20)
)
begin
select t1.UoSCode,t1.UoSName from unitofstudy t1,transcript t2
where t2.Grade is NULL and t2.Year=y and t2.Semester=sem and t1.UoSCode=t2.UoSCode and t2.StudId=sid;
end $$
delimiter ;
*/

/*
delimiter $$
drop procedure if exists withdraw_class $$
create procedure withdraw_class(
in sid char(20),
in cid char(20),
in y char(20),
in sem char(20)
)
begin
delete from transcript where StudId=sid and UoSCode=cid and Semester=sem and Year=y;
update uosoffering set Enrollment = Enrollment -1 where UoSCode = cid and Semester=sem and Year=y;
end $$
delimiter ;
*/



/*
drop table if exists monitor;
create table monitor(item char(20) primary key,c integer);
insert into monitor values('cnt',0);
update monitor set c=0 where item='cnt';
*/


/*
delimiter $$
drop trigger if exists enrollment_monitor $$
create trigger enrollment_monitor
after update on uosoffering
for each row
begin
if (new.MaxEnrollment > 2*new.Enrollment)
then update monitor set c=1 where item='cnt';
elseif(new.MaxEnrollment <= 2*new.Enrollment)
then update monitor set c=0 where item='cnt';
end if;
end $$
delimiter ;
*/