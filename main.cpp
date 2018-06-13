/*
 *	Hash Table
 *	Written by Jason Shi
 *	6/12/18
 *	Allows the user to make and delete student (that contain information), 
 *	which is all stored in a hash table.
 *
 */

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
//struct for students (this was originally used in Student List)
struct Student{
  char* firstName;
  char* lastName;
  int studentID;
  float gpa;
};
//node struct that contains students
struct node{
  Student* content = NULL;
  node* next = NULL;
};
//declares the functions
int hashValue(int studentID, int size);
void rehash(node* newHash[], node* oldHash[], int &size);
void print(node* table[], int size);
bool add(node* hashTable[], int index, Student* s);
void deleteStudent(node* hashTable[], int studentID, int size);

int main(){
	//makes the input array
  char input[50];
  //makes the hashtable
  node** hashTable = new node*[100]; 
  //makes a vector which contains all the studentIDs, this is used to check for duplicates in ADD
  vector<int> studentIDs;
  //initializes the values of the hashtable, prevent seg faults when going through the array to check for nodes
  for(int i = 0; i < 100; i++){
    hashTable[i] = NULL; 
  }
  //sets a counter of the initial size of the hashtable, this is updated so we know how we have to iterate through
  int size = 100;
  while(true){
	  //prints commands
    cout << "Enter 'ADD' to add a student" << endl;
    cout << "Enter 'DELETE' to delete a student" << endl;
    cout << "ENTER 'PRINT' to print all the students" << endl;
    cout << "ENTER 'GEN' to generate random students" << endl;
    cout << "ENTER 'QUIT' to exit the program" << endl;
    cin.getline(input, 50);
    if(strcmp(input, "ADD") == 0){
      //intializes variables 
	    int studentID;
      float gpa;
      char* firstName = new char();
      char* lastName = new char();
      //get the info
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
      //checks if the ID is a repeat
      bool second = false;
      for(vector<int>::iterator it = studentIDs.begin(); it != studentIDs.end(); ++it){
	if((*it) == studentID){
	  second = true;
	}
      }//if not, add the student
      if(!second){
	//adds the ID to the vector
	studentIDs.push_back(studentID);
	//makes the student to be added
	Student* temp = new Student();
	temp->firstName = firstName;
	temp->lastName = lastName;
	temp->studentID = studentID;
	temp->gpa = gpa;
	//gets the key where the student should be added
	int index = hashValue(studentID, size);
	bool valid = add(hashTable, index, temp);
	if(!valid){
	  node** newHash = new node*[size * 2];
	  rehash(newHash, hashTable, size);
	  //replaces the old table with the new one
	  hashTable = newHash;
	}
      }
    }else if(strcmp(input, "GEN") == 0){
	   
    }else if(strcmp(input, "PRINT") == 0){
      print(hashTable, size);
    }else if(strcmp(input, "DELETE") == 0){
      int studentID;
      //gets the studentID
      cout << "Enter the StudentID:" << endl;
      cout << "> ";
      cin >> studentID;
      cin.get();
      studentIDs.push_back(studentID);
      //calls the delete function
      deleteStudent(hashTable, studentID, size);
    }else if(strcmp(input, "QUIT") == 0){
      //exits the program
      exit(0);
    }else{
      cout << "command not found, try again" <<endl;
    }
  } 
} 

//function that is called to rehash the table
void rehash(node* newHash[], node* oldHash[], int &size){
//initializes the values of the new table  
for(int i = 0; i < (size * 2); i++){
    newHash[i] = NULL;
  }
  int newSize = 2 * size;//makes the new size twice what it was before
  for(int i = 0; i < size; i++){
    if(oldHash[i] != NULL){
      node* list = oldHash[i];
      //transfers the old table into the new one, then changes the position to be updated
      while(list != NULL){
	if(list->content != NULL){
	  Student* temp = list->content;	  
	  int index = hashValue(temp->studentID, newSize);
	  add(newHash, index, temp);
	  list = list->next;
	}
      }
    }
  }
  //size is passed by reference, so it is updated to be new
  size = newSize;
}

//returns an integer value of the hash
//for this, I used a custom method of adding up the individual numbers of the studentID, 
//then just multiplying it by the size of the table/10
int hashValue(int studentID, int size){
  int total = 0;
  while(studentID != 0){
    total = total + studentID % 10;
    studentID = studentID / 10;
  }
  int multiplier = trunc(size/10);
  return ((total  * multiplier) % size);
}

//checks for whether the table is still a valid table, returns a boolean and then it will rehash in main if needed
//also adds the student into the table, chained or unchained
bool add(node* hashTable[], int index, Student* s){
  //adds it into the hashTable, if no chaining is required
	if(hashTable[index] == NULL){
    node* temp = new node();
    temp->content = s;
    hashTable[index] = temp;
    return true;
  }
	//adds to the next of the node in the index, if it is commupied
  node* list = hashTable[index];
  node* temp = new node();
  temp->next = list;
  temp->content = s;
  hashTable[index] = temp;
  list = temp;
  int count = 1;
  while(list != NULL){
    list = list->next;
    count++;
  }
  //determines if rehashing is needed or not (based on the length of the linked list of nodes at the index
  if(count > 3){
    return false;
  }else{
    return true;
  }
}

//prints out the list of students
void print(node* table[], int size){
  cout << fixed;
	cout << setprecision(2);
  node* list;
  //goes through the hashtable and prints out the linked list in each index
  for(int i = 0; i < size; i++){
    if(table[i] != NULL){
	    //list is the index in the hashtable
      list = table[i];
      while(list != NULL){
	      //iterates through the list
	if(list->content != NULL){
		//prints info
	  cout << "First name: " << list->content->firstName << endl;
	  cout << "Last name: " << list->content->lastName << endl;
	  cout << "Student ID: " << list->content->studentID << endl;
	  cout << "GPA: " << list->content->gpa << endl;
	  cout << endl;
	}
	list = list->next;
      }
    }
  }
}

//deletes a student from the hash table
void deleteStudent(node* hashTable[], int studentID, int size){
  //gets the index of the student using the studentID
	int hashNum = hashValue(studentID, size);
  node* list = hashTable[hashNum];
  node* previous = NULL;
  //index is empty, student not found
  if(list == NULL){
    return;
  }
  //iterates through the linked list
  while(list != NULL){
    if(list->content->studentID == studentID){
      //linked list no longer needed, set to NULL
      if(previous == NULL){
	hashTable[hashNum] = NULL;
      }else{
	//sets the previous to the next, removes the student from the list
	previous->next = list->next;
      }
      return;
    }
    previous = list;
    list = list->next;
  }
  
  return;
}
