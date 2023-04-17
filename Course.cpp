/*
* Kody Greenberg
* CS 300
* Project Two
* Course.cpp 
*/

#include <iostream>
#include <fstream>
#include "CSVparser.hpp"

using namespace std;

//==============================================================================//
// Global definitions visible to all methods and classes
//==============================================================================//
// Definition of Course structure
struct Course {
	string courseNum; 
	string courseName; 
	vector<string> preReqs; // Prerequisites of the course

	Course() {} // Default constructor
};

// Definition of Node structure
struct Node {
	Course course; // Course info
	Node* left; 
	Node* right; 

	Node() : left(nullptr), right(nullptr) {} // Default constructor

	Node(const Course& aCourse) : Node() { // Constructor with Course parameter
		this->course = aCourse; // Initialize course info
	}
};
//==============================================================================//
// CourseBST Class Definition
//==============================================================================//
//*****START*****
class CourseBST {
	private:
		Node* root;
		void addNode(Node* node, Course course);
		void printSampleSchedule(Node* node);
		void printCourseInformation(Node* node, string courseNum);

	public:
		CourseBST();
		virtual ~CourseBST();
		void DeleteRecursive(Node* node);
		void Insert(Course course);
		int NumPrerequisiteCourses(Course course);
		void PrintSampleSchedule();
		void PrintCourseInformation(string courseNum);
};

//Construct the Binary Search Tree and initialize the root
CourseBST::CourseBST() {
	root = nullptr;
}

/* Deconstruct the Binary Search Tree by passing the root
*  to the function that recursively deletes each node
*/
CourseBST::~CourseBST() {
	DeleteRecursive(root);
}
/* Delete the Binary Search Tree nodes recursively
*  this is used by the deconstructor
*/
void CourseBST::DeleteRecursive(Node* node) {
	if (node) {
		DeleteRecursive(node->left);
		DeleteRecursive(node->right);
		delete node;
	}
}

//Insert a node into the Binary Search Tree
void CourseBST::Insert(Course course) {
	//If the root is not initialized
	if (root == nullptr) {
		//initialize it with the current course info
		root = new Node(course);
	}
	/* Else, send the node to the addNode method
	*  to be added to the Binary Search Tree
	*/
	else {
		this->addNode(root, course);
	}
}

//Returns the number of prereqs the passed course has
int CourseBST::NumPrerequisiteCourses(Course course) { 
	int count = 0; 
	for (int i = 0; i < course.preReqs.size(); i++) { 
		if (course.preReqs.at(i).length() > 0) {
			count++;
		}
	}
	return count; 
}

//Passes the Binary Search Tree root to the private method
void CourseBST::PrintSampleSchedule() { 
	this->printSampleSchedule(root); 
}

/* Passes the Binary Search Tree rootand the course Id
*  to be searched for and displayed if found
*/ 
void CourseBST::PrintCourseInformation(string courseNum) { 
	this->printCourseInformation(root, courseNum); 
	cout << endl;
	return;
}

/* Utilizes the public Insert method determines and sets 
*  correct alphanumeric location and if the current child 
*  is a nullptr, adds the node to the Binary Search Tree,
*  else keeps traversing Binary Search Tree until a 
*  nullptr is found
*/
void CourseBST::addNode(Node * node, Course course) {
	// Current courseNum is less than the current node's courseNum
	if (node->course.courseNum.compare(course.courseNum) > 0) {
		if (node->left == nullptr) {
			node->left = new Node(course);
		}
		else {
			this->addNode(node->left, course);
		}
	}
	// Current courseNum is equal or greater than the current node's courseNum
	else {
		if (node->right == nullptr) {
			node->right = new Node(course);
		}
		else {
			this->addNode(node->right, course);
		}
	}
}
//Recursively prints the loaded courses IN ORDER
void CourseBST::printSampleSchedule(Node* node) {
	if (node != nullptr) {
		printSampleSchedule(node->left);
		cout << node->course.courseNum << ", " << node->course.courseName << endl;
		printSampleSchedule(node->right);
	}
	return;
}

//Displays a single courses information including its prereqs
void CourseBST::printCourseInformation(Node* curr, string courseNum) {
	// Traverse Binary Search Tree until bottom reached or matching courseNum found
	while (curr != nullptr) {
		// Passed courseNum matches the current courseNum
		if (curr->course.courseNum.compare(courseNum) == 0) {
			// Display course and get num prereqs
			cout << endl << curr->course.courseNum << ", " << curr->course.courseName << endl;
			unsigned int size = NumPrerequisiteCourses(curr->course);
			cout << "Prerequisite(s): ";
			// If prereqs exist, display each prereq with proper formatting
			unsigned int i = 0;
			for (i = 0; i < size; i++) {
				cout << curr->course.preReqs.at(i);
				if (i != size - 1) {
					cout << ", ";
				}
			}
			// If no prereqs, inform user and return to stop the search
			if (i == 0) {
				cout << "No prerequisites required.";
				cout << endl;
				return;
			}
			break; // Exit the loop after the course has been found and printed
		}
		// Passed courseNum is smaller than the current courseNum so traverse left
		else if (courseNum.compare(curr->course.courseNum) < 0) {
			curr = curr->left;
		}
		// Passed courseNum is larger than the current courseNum so traverse right
		else {
			curr = curr->right;
		}
	}
	// nullptr has been reached but course hasn't been found/printed, so inform user
	if (curr == nullptr) {
		cout << "Course " << courseNum << " not found." << endl;
	}
}

//*****END*****
//==============================================================================//
// Static methods used for testing
//==============================================================================//

bool loadCourses(string csvPath, CourseBST* coursesBST) {
	try {
		// Open the CSV file
		ifstream courseFile(csvPath); 
		// Check if the file was successfully opened
		if (!courseFile) { 
			cerr << "Failed to open file: " << csvPath << endl;
			return false;
		}

		string line;
		// Loop through each line of the CSV file
		while (getline(courseFile, line)) { 
			// Create a string stream for each line
			istringstream iss(line); 
			vector<string> courseInfo;
			string field;
			// Parse each field of the line
			while (getline(iss, field, ',')) { 
				courseInfo.push_back(field);
			}

			// Check if the line has at least two fields
			if (courseInfo.size() >= 2) { 
				Course course;
				// Assign the course number
				course.courseNum = courseInfo[0];  
				// Assign the course name
				course.courseName = courseInfo[1];
				for (unsigned int i = 2; i < courseInfo.size(); i++) {
					// Assign the course prerequisites
					course.preReqs.push_back(courseInfo[i]); 
				}
				// Insert the course into the BST
				coursesBST->Insert(course); 
			}
			else {
				cerr << "Invalid CSV format: " << line << endl;
			}
		}
		// Close the CSV file
		courseFile.close(); 
		return true;
	}
	catch (const ifstream::failure& e) {
		cerr << "Error reading file: " << csvPath << endl;
		cerr << e.what() << endl;
		return false;
	}
	catch (const exception& e) {
		cerr << "Error loading courses: " << e.what() << endl;
		return false;
	}
}


//==============================================================================//
// Main method
//==============================================================================//
int main(int argc, char* argv[]) {
	// Process command line argument
	string csvPath, courseId;
	bool success = false;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;

	case 3:
		csvPath = argv[1];
		courseId = argv[2];
		break;
	default:
		csvPath = "";
		break;
	}
	// Define Binary Search Tree and Welcome User
	CourseBST* coursesBST = nullptr;
	cout << "\nWelcome to the course planner!\n" << endl;
	// Making user choice a string and converting first char to int prevents invalid data
	string choice = "0";
	int userChoice = choice[0] - '0';
	//if not 9 or exit
	while (userChoice != 9) {
		cout << "1. Load Data Structure" << endl;
		cout << "2. Print Course List" << endl;
		cout << "3. Print Course" << endl;
		cout << "9. Exit" << endl;
		cout << "\nWhat would you like to do? "; cin >> choice;
		// Check if user choice is a double digit
		if (choice.length() == 1) {
			userChoice = choice[0] - '0';
		}
		else {
			userChoice = 0;
			success = false;
		}
		// Handle users choice from menu
		switch (userChoice) {
			// Instantiate Binary Search Tree get file path name//and load courses into bst 
		case 1:
			if (coursesBST == nullptr) {
				coursesBST = new CourseBST();
			}
			if (csvPath.length() == 0) {
				cout << "Enter the file name that contains the course data: "; cin >> csvPath;
			}
			// Determine if the file was opened and load if successful
			success = loadCourses(csvPath, coursesBST);
			if (success) {
				cout << "Courses have been loaded.\n" << endl;
			}
			else {
				cout << "Error: File not found.\n" << endl;
				csvPath = "";
				break;
			}
			// Validate that a Binary Search Tree exists and print a sample schedule
		case 2:
			if (coursesBST != nullptr && success) {
				cout << "Here is a sample schedule:\n" << endl;
				coursesBST->PrintSampleSchedule();
				cout << endl;
				break;
			}
			else {
				cout << "Load courses first - option 1\n" << endl;
				break;
			}
			// Validate that a Binary Search Tree existsand search/display course info
		case 3:
			if (coursesBST != nullptr && success) {
				if (courseId.length() == 0) {
					cout << "What course do you want to know about? ";
					cin >> courseId;
					for (auto& userChoice : courseId) {
						userChoice = toupper(userChoice);
					}
				}
				coursesBST->PrintCourseInformation(courseId);
				cout << endl;
				break;
			}
			else {
				cout << "Load courses first - option 1\n" << endl;
				break;
			}
			// User entered invalid data or exited the program
		case 9:
			cout << "\nThank you for using the course planner!" << endl;
			if (coursesBST != nullptr) {
				delete coursesBST;
			}
			return 0;
		default:
			if (userChoice != 9) {
				cout << choice << " is not a valid option\n" << endl;
			}
			break;
		}
	}
}
