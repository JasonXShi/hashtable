#include <iostream>
#include <vector>
#include <cstring>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <time.h>
#include <stdlib.h>

using namespace std;

struct Student{
  char* firstName;
  char* lastName;
  int studentID;
  float gpa;
};

struct node{
  Student* content = NULL;
  node* next = NULL;
};

int hashValue(int studentID, int size);
void rehash(node* newHash[], node* oldHash[], int &size);
void print(node* table[], int size);
bool add(node* hashTable[], int index, Student* s);

int main(){
  char input[50];
  node** hashTable = new node*[100]; 
  vector<int> studentIDs;
  for(int i = 0; i < 100; i++){
    hashTable[i] = NULL; 
  }
  int size = 100;
  while(true){
    cout << "Enter 'ADD' to add a student" << endl;
    cout << "Enter 'DELETE' to delete a student" << endl;
    cout << "ENTER 'PRINT' to print all the students" << endl;
    cout << "ENTER 'GEN' to generate random students" << endl;
    cout << "ENTER 'QUIT' to exit the program" << endl;
    cin.getline(input, 50);
    if(strcmp(input, "ADD") == 0){
      int studentID;
      float gpa;
      char* firstName = new char();
      char* lastName = new char();
      cout << "Enter the student's first name" << endl;
      cin.getline(firstName, 20);
      cout << "Enter the student's last name" << endl;
      cin.getline(lastName, 20);
      cout << "Enter the student's studentID number" << endl;
      cin >> studentID;
      cin.get();
      cout << "Enter the student's gpa " << endl;
      cin >> gpa;
      cin.get();
      
	studentIDs.push_back(studentID);
	Student* temp = new Student();
	temp->firstName = firstName;
	temp->lastName = lastName;
	temp->studentID = studentID;
	temp->gpa = gpa;
	int index = hashValue(studentID, size);
	bool valid = add(hashTable, index, temp);
	if(!valid){
	  node** newHash = new node*[size * 2];
	  rehash(newHash, hashTable, size);
	  //replaces the old table with the new one
	  hashTable = newHash;
	}
      
    }else if(strcmp(input, "GEN") == 0){
	   
    }else if(strcmp(input, "PRINT") == 0){
      print(hashTable, size);
    }else if(strcmp(input, "DELETE") == 0){
      int studentID;
      cout << "Enter the StudentID:" << endl;
      cout << "> ";
      cin >> studentID;
      cin.get();
      studentIDs.push_back(studentID);
      //deleteStudent(hashTable, studentID, size);
    }else if(strcmp(input, "QUIT") == 0){
      //exits the program
      exit(0);
    }else{
      cout << "command not found, try again" <<endl;
    }
  } 
} 


void rehash(node* newHash[], node* oldHash[], int &size){
  for(int i = 0; i < (size * 2); i++){
    newHash[i] = NULL;
  }
  int newSize = 2 * size;
  for(int i = 0; i < size; i++){
    if(oldHash[i] != NULL){
      node* current = oldHash[i];
      while(current != NULL){
	if(current->content != NULL){
	  Student* s = current->content;	  
	  int index = hashValue(s->studentID, newSize);
	  add(newHash, index, s);
	  current = current->next;
	}
      }
    }
  }
  size = newSize;
}

int hashValue(int studentID, int size){
  int sum = 0;
  while(studentID != 0){
    sum = sum + studentID % 10;
    studentID = studentID / 10;
  }
  int multiplier = trunc(size/7);
  return ((sum  * multiplier) % size);
}


bool add(node* hashTable[], int index, Student* s){
  if(hashTable[index] == NULL){
    node* temp = new node();
    temp->content = s;
    hashTable[index] = temp;
    return true;
  }
  node* current = hashTable[index];
  node* temp = new node();
  temp->next = current;
  temp->content = s;
  hashTable[index] = temp;
  current = temp;
  int count = 1;
  while(current != NULL){
    current = current->next;
    count++;
  }
  if(count > 3){
    return false;
  }else{
    return true;
  }
}

void print(node* table[], int size){
  cout << setprecision(2);
  node* current;
  for(int i = 0; i < size; i++){
    if(table[i] != NULL){
      current = table[i];
      while(current != NULL){
	if(current->content != NULL){
	  cout << "First name: " << current->content->firstName << endl;
	  cout << "Last name: " << current->content->lastName << endl;
	  cout << "Student ID: " << current->content->studentID << endl;
	  cout << "GPA: " << current->content->gpa << endl;
	  cout << endl;
	}
	current = current->next;
      }
    }
  }
}
