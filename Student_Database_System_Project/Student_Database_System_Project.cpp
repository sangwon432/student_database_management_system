#include<iostream>
#include<fstream>
#include<iomanip>

using namespace std;

int RollNoToBeUpdated = 0;

class student
{
private:
    char Student_Name[50];
    int Roll_no;
    int SocialStudies_Marks, Statistics_Marks, Math_Marks, English_Marks, Computer_Marks;
    double Student_Percentage;
    char Student_Grade;
    bool flag;/*flag=false if input roll no does not exist in student.dat
                flag=true if it exists */
    void CalculateGrade();

public:
    student();
    void getdata();
    void CheckForDuplicationInFile();
    void showdata();// displays student data on the output screen
    void TabularDisplay();// displays the result of all students in a table
    int GetStudentRollNo();// returns the roll no. of the current student
    void Write_Student_Record_In_File();// writes user input in student.dat
    void ShowStudentRecord(int);// displays the record of a student with a particular roll no
    void Update_Student_Record(int);// accepts roll no and updates the record
    void Delete_Student_Record(int);// deletes a student record
    void Delete_From_Duplicate_File(int);// deletes record from file DuplicateCheckingFile.txt
    void Show_Result_Menu();// shows the Result Menu to the user that takes choice from the user
    void Display_Class_Result();// displays the result of complete class on the output screen  
};

void student::getdata()
{
    flag = false;
    cout << "\nEnter the roll no of the student = ";
    cin >> Roll_no;

    if (RollNoToBeUpdated != Roll_no)
        CheckForDuplicationInFile();

    if (flag == true)
    {
        cout << "Error duplicate record" << endl;
        return;
    }
    //if flag==false
    cout << "\n\nEnter The Name of student ";
    cin.ignore();
    cin.getline(Student_Name, 50);
    cout << "\nEnter The marks in SocialStudies out of 100 : ";
    cin >> SocialStudies_Marks;
    cout << "\nEnter The marks in Statistics out of 100 : ";
    cin >> Statistics_Marks;
    cout << "\nEnter The marks in Math out of 100 : ";
    cin >> Math_Marks;
    cout << "\nEnter The marks in English out of 100 : ";
    cin >> English_Marks;
    cout << "\nEnter The marks in Computers out of 100 : ";
    cin >> Computer_Marks;
    CalculateGrade();

}

void student::CheckForDuplicationInFile()
{
    int Current_RollNo = 0;
    flag = false;
    ifstream infile("DuplicateCheckingFile.txt");
    while (!infile.eof())
    {
        infile >> Current_RollNo;
        if (Current_RollNo == Roll_no)
        {
            flag = true;
        }
    }
    infile.close();
    if (flag == false)// no duplicate
    {
        ofstream outfile("DuplicateCheckingFile.txt", ios::app);
        outfile << "\n" << Roll_no;
        outfile.close();
    }
}

void student::showdata()
{
    cout << "\nRoll Number of student = " << Roll_no;
    cout << "\nName of student = " << Student_Name;
    cout << "\nMarks in SocialStudies = " << SocialStudies_Marks;
    cout << "\nMarks in Statistics = " << Statistics_Marks;
    cout << "\nMarks in Math = " << Math_Marks;
    cout << "\nMarks in English = " << English_Marks;
    cout << "\nMarks in Computer = " << Computer_Marks;
    cout << "\nPercentage of student = " << Student_Percentage;
    cout << "\nGrade Of student = " << Student_Grade;
}

student::student()
{

    ofstream outFile("student.dat", ios::binary | ios::app);
    outFile.close();

    ofstream outDFile("DuplicateCheckingFile.txt", ios::app);
    outDFile.close();
}

void student::Write_Student_Record_In_File()
{
    ofstream outFile;
    outFile.open("student.dat", ios::binary | ios::app);

    getdata();

    if (flag == false)// no duplicates
    {
        outFile.write(reinterpret_cast<char*>(this), sizeof(student));
        outFile.close();
        cout << "\n\n Student record has been created";
    }

    if (flag == true)
    {
        cout << "record not created due to duplication\n";
        outFile.close();
    }
    cin.ignore();
    cin.get();
}

void student::Update_Student_Record(int n)
{

    RollNoToBeUpdated = n;
    bool found = false;
    fstream File;
    File.open("student.dat", ios::binary | ios::in | ios::out);

    if (!File)
    {
        cout << "File could not be open !! Press any Key...";
        cin.ignore();
        cin.get();
        return;
    }

    while (!File.eof() && found == false)
    {
        File.read(reinterpret_cast<char*> (this), sizeof(student));

        if (GetStudentRollNo() == n)
        {

            showdata();
            cout << "\n\nPlease Enter The New Details of student" << endl;

            getdata();
            if (flag == false)// no duplicates
            {
                int pos = (-1) * static_cast<int>(sizeof(*this));
                File.seekp(pos, ios::cur);
                File.write(reinterpret_cast<char*> (this), sizeof(student));
            }
            found = true;
        }
    }
    File.close();

    if (flag == true)
    {
        cout << "could not update because this RollNo already exists";
    }
    if (found == false)
    {
        cout << "\n\n Record Not Found";
    }
    cin.ignore();
    cin.get();

    if (flag == false && found == true && RollNoToBeUpdated != Roll_no)//
    {
        Delete_From_Duplicate_File(n);
    }
    RollNoToBeUpdated = 0;
}



void student::Delete_Student_Record(int n)
{

    ifstream inFile;

    inFile.open("student.dat", ios::binary);
    if (!inFile)
    {
        cout << "\nFile could not be opened..Press any key";
        cin.ignore();
        cin.get();
        return;
    }

    ofstream outFile;
    outFile.open("Temp.dat", ios::out);
    inFile.seekg(0, ios::beg);
    while (inFile.read(reinterpret_cast<char*> (this), sizeof(student)))
    {
        if (GetStudentRollNo() != n)
        {
            outFile.write(reinterpret_cast<char*> (this), sizeof(student));
        }

    }

    outFile.close();
    inFile.close();
    remove("student.dat");
    rename("Temp.dat", "student.dat");
    cout << "\n\nRecord deleted.. press any key";
    cin.ignore();
    cin.get();
    Delete_From_Duplicate_File(n);


}


void student::Delete_From_Duplicate_File(int RollNoOfRecord)
{
    int Current_RollNo;
    ifstream InFile("DuplicateCheckingFile.txt");
    ofstream OutFile("TempFile.txt");
    InFile.seekg(0, ios::beg);
    while (!InFile.eof())
    {
        InFile >> Current_RollNo;
        if (Current_RollNo != RollNoOfRecord)
        {
            OutFile << "\n" << Current_RollNo;
        }
    }

    InFile.close();
    OutFile.close();
    remove("DuplicateCheckingFile.txt");
    rename("TempFile.txt", "DuplicateCheckingFile.txt");
    cin.ignore();
    cin.get();

}

void student::CalculateGrade()
{
    Student_Percentage = (SocialStudies_Marks + Statistics_Marks + Math_Marks + English_Marks + Computer_Marks) / 5.0;
    if (Student_Percentage >= 80)
        Student_Grade = 'A';
    if (Student_Percentage >= 70 && Student_Percentage < 80)
        Student_Grade = 'B';
    if (Student_Percentage >= 60 && Student_Percentage < 70)
        Student_Grade = 'C';
    if (Student_Percentage < 60)
        Student_Grade = 'F';
}

void student::TabularDisplay()
{
    cout << Roll_no << setw(4) << " " << Student_Name << setw(20 - strlen(Student_Name)) << SocialStudies_Marks << setw(6) << Statistics_Marks << setw(6) << Math_Marks << setw(6) << English_Marks << setw(8) << Computer_Marks << setw(8) << Student_Percentage << setw(8) << Student_Grade << endl;
}

int student::GetStudentRollNo()
{
    return Roll_no;
}


void student::ShowStudentRecord(int n)
{
    ifstream inFile;
    inFile.open("student.dat", ios::binary);
    if (!inFile)
    {
        cout << "File could not be opened !! Press any key to continue";
        cin.ignore();
        cin.get();
        return;
    }
    bool flag = false;

    while (inFile.read(reinterpret_cast<char*> (this), sizeof(student)))
    {
        if (GetStudentRollNo() == n)
        {
            showdata();
            flag = true;
        }

    }
    inFile.close();
    if (flag == false)
        cout << "\n\nrecord does not exist";
    cin.ignore();
    cin.get();
}

void student::Show_Result_Menu()
{

    char ch;
    int rno;
    system("cls");

    cout << "\n\n\n\tRESULT MENU";
    cout << "\n\n\n\t1. Class Result";
    cout << "\n\n\n\t2. Student Report Card";
    cout << "\n\n\n\t3. Back to Main Menu/Home Screen";
    cout << "\n\n\n\tEnter Choice (1/2/3)? ";
    cin >> ch;
    system("cls");

    switch (ch)
    {
    case '1':Display_Class_Result();
        break;
    case '2':cout << "\n\n\tEnter the Roll No of the Student = ";
        cin >> rno;
        ShowStudentRecord(rno);
        break;
    case '3':break;
    default: cout << "\a";
    }

}

void student::Display_Class_Result()
{
    ifstream inFile;
    inFile.open("student.dat", ios::binary);
    if (!inFile)
    {
        cout << "File could not be opened.. Press any key..";
        cin.ignore();
        cin.get();
        return;
    }

    cout << "\n\n\t\tALL CLASS RESULT \n\n";
    cout << "==========================================================\n";
    cout << "R.No        Name          Soc  Stats  M     E       CS    %age    Grade" << endl;
    cout << "==========================================================\n";

    while (inFile.read(reinterpret_cast<char*> (this), sizeof(student)))
    {
        TabularDisplay();
    }
    inFile.close();
    cin.ignore();
    cin.get();

}

void intro();
void entry_menu();
void display_all();

int main()
{
    system("cls");
    char ch;
    student std_obj;
    cout.setf(ios::fixed | ios::showpoint);
    cout << setprecision(2);
    intro();
    do
    {
        system("cls");
        intro();

        cout << "\n\n\n\t\t\t\tHOME SCREEN\n\n\n";
        cout << "\n\n\t1. Result Calculation System";
        cout << "\n\n\t2. Student Database Management System";
        cout << "\n\n\t3. Exit";
        cout << "\n\n\n\n\n\tPlease Select Your Option (1-3)";
        cin >> ch;
        switch (ch)
        {
        case '1':
            std_obj.Show_Result_Menu();
            break;
        case '2':
            entry_menu();
            break;
        case '3':
            break;
        default: cout << "\a";
        }
    } while (ch != '3');

    system("pause");
    return 0;
}

void intro()
{
    cout << "\n\n\n****  WELCOME TO STUDENT DATABASE MANAGEMENT AND RESULT CALCULATION SYSTEM  ****\n\n";
}

void entry_menu()
{
    char ch;
    int num;
    student std;
    system("CLS");
    cout << "\n\n\n\t\t\tSTUDENT DATABASE MANAGEMENT SYSTEM MENU";
    cout << "\n\n\t1. CREATE STUDENT RECORD";
    cout << "\n\n\t2. SHOW RECORDS OF ALL STUDENTS";
    cout << "\n\n\t3. SEARCH STUDENT RECORD";
    cout << "\n\n\t4. UPDATE STUDENT RECORD";
    cout << "\n\n\t5. DELETE STUDENT RECORD";
    cout << "\n\n\t6. BACK TO HOME SCREEN";
    cout << "\n\n\n\n\tPlease Enter Your Choice (1-6) ";
    cin >> ch;
    system("cls");
    switch (ch)
    {
    case '1':
        std.Write_Student_Record_In_File();
        break;
    case '2':
        display_all();
        break;
    case '3':
        cout << "\n\n\t Please Enter The Roll No";
        cin >> num;
        std.ShowStudentRecord(num);
        break;
    case '4':
        cout << "\n\n\tPlease Enter The roll number ";
        cin >> num;
        std.Update_Student_Record(num);
        break;
    case '5':
        cout << "\n\n\tPlease Enter The roll number ";
        cin >> num;
        std.Delete_Student_Record(num);
        break;
    case '6':
        break;
    default:cout << "\a"; entry_menu();

    }
}

void display_all()
{
    student st;
    ifstream inFile;
    inFile.open("student.dat", ios::binary);
    if (!inFile)
    {
        cout << "File could not be opened !! Press any Key...";
        cin.ignore();
        cin.get();
        return;
    }

    cout << "\n\n\n\t\tDISPLAY ALL RECORDs !!!\n\n";
    while (inFile.read(reinterpret_cast<char*> (&st), sizeof(student)))
    {
        st.showdata();
        cout << "\n\n====================================\n";
    }
    inFile.close();
    cin.ignore();
    cin.get();
}
