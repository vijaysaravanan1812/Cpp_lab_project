
#include<iostream>
#include<string>
using namespace std;
#include"btree.cpp"

// Driver program to test above functions
int main()
{
	system("clear");
	Btree <int> t; // A B-Tree with minium degree 3

	int count = 1; 
	while (count)
	{
		system("clear");
		int choice;
		printf("Insert - 1 \tDisplay -2 \tRemove -3 \tEdit -4\n");
		
		cin>>choice;
		if (choice == 1)
		{
			int time;
			
			printf("How Many records you need to insert\t");
			cin>>time;
			for (int i = 1; i <= time; i++)
			{		
				int roll_no;   
				printf("Roll Number should be Unique\nStudent %d roll number \t",i);
				cin>>roll_no;
				t.insert(roll_no);
			}
			
		}
		else if (choice == 2)
		{
			t.traverse();
			getchar();
		}
		else if (choice == 3)
		{
			int roll_no;
			printf("Enter roll no to remove that record\t");
			cin>>roll_no;
			t.remove(roll_no);
			
		}
		else if (choice == 4)
		{
			int roll_no;
			printf("Enter Old Roll_Number\t");
			cin>>roll_no;
			t.search(roll_no);
		}
		else
		{
			char ch;
			printf("\t\t<><><><><><><><><><><><><><><><>\n");
			printf("\t\t| Are you sure do want to exix |\n");
			printf("\t\t<><><><><><><><><><><><><><><><>\n");
			printf("\t\t|%-30s|\n","   Yes - y/Y  |  No - n/N  ");
			printf("\t\t<><><><><><><><><><><><><><><><>\n\t\t");
			cin>>ch;
			if (ch == 'Y' || ch == 'y')
				count = 0;
			else if(ch == 'n' || ch == 'N')
				count = 1;
			else
			{
				printf("Invalid Input!!\n");
				getchar();
			}
		}

		getchar();
		
	}
	
	return 0;
}
