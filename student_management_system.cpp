#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
using namespace std;

struct Student {
    int rollNo;
    char name[50];
    char branch[30];
    int year;
    float cgpa;
    char email[50];
};

void addStudent() {
    Student s;
    ofstream fout("students.dat", ios::binary | ios::app);

    cout << "\nEnter Roll Number : ";
    cin >> s.rollNo;
    cin.ignore();

    cout << "Enter Name        : ";
    cin.getline(s.name, 50);

    cout << "Enter Branch      : ";
    cin.getline(s.branch, 30);

    cout << "Enter Year (1-4)  : ";
    cin >> s.year;

    cout << "Enter CGPA        : ";
    cin >> s.cgpa;
    cin.ignore();

    cout << "Enter Email       : ";
    cin.getline(s.email, 50);

    fout.write((char*)&s, sizeof(s));
    fout.close();

    cout << "\nRecord added successfully!";
}

void displayAll() {
    Student s;
    ifstream fin("students.dat", ios::binary);

    if (!fin) {
        cout << "\nNo records found.";
        return;
    }

    int count = 0;
    cout << "\n--- All Students ---\n";

    while (fin.read((char*)&s, sizeof(s))) {
        cout << "\nRoll No : " << s.rollNo;
        cout << "\nName    : " << s.name;
        cout << "\nBranch  : " << s.branch;
        cout << "\nYear    : " << s.year;
        cout << "\nCGPA    : " << fixed << setprecision(2) << s.cgpa;
        cout << "\nEmail   : " << s.email;
        cout << "\n--------------------";
        count++;
    }

    fin.close();

    if (count == 0)
        cout << "\nNo records found.";
    else
        cout << "\nTotal: " << count << " student(s)";
}

void searchStudent() {
    Student s;
    int roll;
    bool found = false;

    cout << "\nEnter Roll Number to search: ";
    cin >> roll;

    ifstream fin("students.dat", ios::binary);
    if (!fin) {
        cout << "\nFile not found.";
        return;
    }

    while (fin.read((char*)&s, sizeof(s))) {
        if (s.rollNo == roll) {
            cout << "\nRecord Found:";
            cout << "\nRoll No : " << s.rollNo;
            cout << "\nName    : " << s.name;
            cout << "\nBranch  : " << s.branch;
            cout << "\nYear    : " << s.year;
            cout << "\nCGPA    : " << fixed << setprecision(2) << s.cgpa;
            cout << "\nEmail   : " << s.email;
            found = true;
            break;
        }
    }

    fin.close();

    if (!found)
        cout << "\nStudent not found.";
}

void updateStudent() {
    Student s;
    int roll;
    bool found = false;

    cout << "\nEnter Roll Number to update: ";
    cin >> roll;

    fstream file("students.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "\nFile not found.";
        return;
    }

    while (file.read((char*)&s, sizeof(s))) {
        if (s.rollNo == roll) {
            cout << "\nCurrent Name   : " << s.name;
            cout << "\nCurrent Branch : " << s.branch;
            cout << "\nCurrent Year   : " << s.year;
            cout << "\nCurrent CGPA   : " << s.cgpa;
            cout << "\nCurrent Email  : " << s.email;

            cout << "\n\nEnter new details:\n";
            cin.ignore();

            cout << "New Name   : ";
            cin.getline(s.name, 50);

            cout << "New Branch : ";
            cin.getline(s.branch, 30);

            cout << "New Year   : ";
            cin >> s.year;

            cout << "New CGPA   : ";
            cin >> s.cgpa;
            cin.ignore();

            cout << "New Email  : ";
            cin.getline(s.email, 50);

            // move back and overwrite
            file.seekp(-(int)sizeof(s), ios::cur);
            file.write((char*)&s, sizeof(s));

            cout << "\nRecord updated successfully!";
            found = true;
            break;
        }
    }

    file.close();

    if (!found)
        cout << "\nStudent not found.";
}

void deleteStudent() {
    Student s;
    int roll;
    bool found = false;

    cout << "\nEnter Roll Number to delete: ";
    cin >> roll;

    ifstream fin("students.dat", ios::binary);
    ofstream fout("temp.dat", ios::binary);

    if (!fin) {
        cout << "\nFile not found.";
        return;
    }

    while (fin.read((char*)&s, sizeof(s))) {
        if (s.rollNo == roll) {
            found = true;
            continue; // skip this record
        }
        fout.write((char*)&s, sizeof(s));
    }

    fin.close();
    fout.close();

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found)
        cout << "\nRecord deleted successfully!";
    else
        cout << "\nStudent not found.";
}

int main() {
    int choice;

    cout << "Welcome to Student Management System";

    do {
        cout << "\n\n===== Menu =====";
        cout << "\n1. Add Student";
        cout << "\n2. Display All";
        cout << "\n3. Search Student";
        cout << "\n4. Update Student";
        cout << "\n5. Delete Student";
        cout << "\n0. Exit";
        cout << "\n================";
        cout << "\nEnter choice: ";
        cin >> choice;

        switch(choice) {
            case 1: addStudent();    break;
            case 2: displayAll();    break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 0: cout << "\nGoodbye!\n"; break;
            default: cout << "\nInvalid choice!";
        }

    } while(choice != 0);

    return 0;
}