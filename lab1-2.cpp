#include<iostream>
#include<fstream>
#include<string>
using namespace std;

void pretreatment();
void cal_keyword();
void cal_switch_case();
void cal_if_else(int input_level);

string str;
string key_word[32] =
{
	"auto","break","case","char","const","continue","default","do ",
	"double","else","enum","extern","float","for","goto","if","int",
	"long","register","return","short","signed","sizeof","stastic",
	"struct","switch","typedef","union","unsigned","void","volatile","while"
};


int main()
 {
	 int count = 0;
	 char filepath[100] = {'\0'};
	 int level;
	 cout<<"Please enter the file path: ";
	 cin.getline (filepath,100); //Read line by line
	 cout<<"Please enter the level: ";
	 cin >> level;
	 
	 fstream file; 
	 file.open (filepath);
	 if(!file.is_open())//is_open returns a Boolean type to determine whether it opened successfully
	 {
		 cout<<"File opening failure"<<endl;
		 return 0;
	 }
	 
	 string temp;
	 while ( file.good() )
	 {
		 getline(file,temp);        
		 str = str + temp + '\n';
	 }
	 
	 //Introducing preprocessor functions
	 pretreatment();
	 
	 //Introduce keyword statistics functions
	 cal_keyword();
	 
	 if (level > 1)
	 {
		 //Introduces the function that find switch-case structures.
		 cal_switch_case();
		 if (level > 2)
		 {
			 //Introduce a function that finds the number of if-else structures
			 cal_if_else(level);
		 }  
	 }
	 file.close();  
	 return 0;
 }
 
 //Delete the comment part of the file and process the file     
 void pretreatment()
 {
	 for (int i=0,j=i; i<(int)str.length(); i++ )
	 {
		 if (str[i]=='/' && str[i+1]=='/' )
		 {
			 j = i;
			 while ( str[j] != '\n' )
			 {
				 j ++;
			 }
			 str.erase(i,j-i+1);
		 }
		 if (str[i]=='/' && str[i+1]=='*' )
		 {
			 j = i + 2;
			 while ( str[j] != '*' )
			 {
				 j ++;
			 }
			 str.erase(i,j+2-i);
		 }
		 if ( str[i]=='"')
		 {
			 j = i+1;
			 while (str[j] != '"' )
			 {
				 j ++;
			 }
			 str.erase(i,j-i+1);
		 }
	 }
 }
 //Check the keyword table to find the number of keywords in the file and return
 void cal_keyword()
 {
	 int t_num=0;
	 for (int i=0;i<32;i++)
	 {
		 int count = 0;
		 size_t position = str.find(key_word[i]);
		 while ( position != str.npos )
		 {
			 count ++;
			 position = str.find(key_word[i],position+1);
		 }
		 t_num += count;
	 }
	 printf ("total num: %d\n",t_num);    
 }
 //Find several groups of switch-case structures and print them
 void cal_switch_case()
 {
	 size_t switch_position = str.find(key_word[25]); 
	 size_t case_position;
	 int switch_num = 0;
	 int case_num[100] = {0};
	 while ( switch_position != str.npos )
	 {
		 switch_num ++;
		 case_position = str.find(key_word[2],switch_position);
		 while ( case_position != str.npos )
		 {
			 case_num[switch_num-1] ++;
			 case_position = str.find(key_word[2],case_position+1);
		 }
		 switch_position = str.find(key_word[25],switch_position+1);
	 }
	 for (int i=0;i < switch_num-1;i++ )
	 {
		 case_num[i] -= case_num[i+1];
	 }
	 cout << "switch num: " << switch_num << endl;
	 cout << "case num: ";
	 for ( int i=0; i<switch_num; i++)
	 {
		 cout << case_num[i] << " ";
	 }
	 cout << endl;
 } 
 //Find the number of if-else and if-elseif-else constructs
 void cal_if_else(int input_level)
 {
	 int c[100] = {0};
	 int head = 0,ebp = 0;
	 int if_else = 0,if_elseif_else = 0;
	 size_t if_position = str.find(key_word[15]);
	 if (if_position != str.npos )
	 {
		 c[ebp] = 1;
		 ebp ++;
		 for ( int i = if_position+1; i < (int)str.length(); i++ )
		 {
			 if (str.compare(i,7,"else if") == 0)
			 {
				 c[ebp] = 2;
				 ebp ++;
				 i += 7;
			 }
			 else if (str.compare(i,2,"if") == 0 )
			 {
				 c[ebp] = 1;
				 ebp ++;
				 i += 2;
			 }
			 else if (str.compare(i,4,"else") == 0)
			 {
				 int j = ebp-1;
				 if (c[j] != 1 && c[j] == 2)
				 {
					 while (c[j] != 1)
					 {
						 j--;
					 }
					 int ebp_1 = j;
					 for (j; j<ebp; j++)
					 {
						 c[j] = 0;
					 }
					 ebp = ebp_1;
					 if_elseif_else ++;
				 }
				 else if (c[j] == 1)
				 {
					 if_else ++;
					 c[j] = 0;
					 ebp = j;
				 }
				 i += 4;                
			 }
			 
		 }
	 }
	 if (input_level >= 3) cout << "if-else num: " << if_else << endl;
	 if (input_level == 4)    cout << "if-elseif-else num: " << if_elseif_else << endl;
 }
 
