#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <array>
using namespace std;

struct Student {
    int id;
    string name;
    string course;
};

struct Book {
    string code;
    string name;
    bool available;
    int studentid;
};


struct Grade {
    int studentid;
    array<int,5> marks;
    float avg;
    char grade;
};


struct Attendance {
    int studentid;
    int total;
    int present;
};

vector<Student> allstudents;
vector<Book> allbooks;
vector<Grade> allgrades;
vector<Attendance> allattendance;

vector<string> splitCSV(const string &line) {
    vector<string> tokens;
    string token;
    for (size_t i = 0; i <= line.size(); ++i) {
        if (i == line.size() || line[i] == ',') {
            tokens.push_back(token);
            token.clear();
        } else {
            token.push_back(line[i]);
        }
    }
    return tokens;
}

void showstudents() {
    cout << "\n=== STUDENT LIST ===\n";
    for(size_t i=0; i<allstudents.size(); i++) {
        cout << "ID: " << allstudents[i].id << " | ";
        cout << "Name: " << allstudents[i].name << " | ";
        cout << "Course: " << allstudents[i].course << endl;
    }
}

void savedata(); // forward

void addstudent() {
    Student s;
    cout << "Enter student ID: ";
    cin >> s.id;
    cout << "Enter name: ";
    cin.ignore();
    getline(cin, s.name);
    cout << "Enter course: ";
    getline(cin, s.course);
    allstudents.push_back(s);
    savedata();
    cout << "Student added!\n";
}

void showbooks() {
    cout << "\n=== BOOK LIST ===\n";
    for(size_t i=0; i<allbooks.size(); i++) {
        cout << "Code: " << allbooks[i].code << " | ";
        cout << "Book: " << allbooks[i].name << " | ";
        if(allbooks[i].available) {
            cout << "Available\n";
        } else {
            cout << "Issued to ID: " << allbooks[i].studentid << endl;
        }
    }
}

void addbook() {
    Book b;
    cout << "Enter book code: ";
    cin >> b.code;
    cout << "Enter book name: ";
    cin.ignore();
    getline(cin, b.name);
    b.available = true;
    b.studentid = 0;
    allbooks.push_back(b);
    savedata();
    cout << "Book added!\n";
}

void issuebook() {
    string code;
    int sid;
    cout << "Enter book code: ";
    cin >> code;
    cout << "Enter student ID: ";
    cin >> sid;
    for(size_t i=0; i<allbooks.size(); i++) {
        if(allbooks[i].code == code) {
            if(!allbooks[i].available) {
                cout << "Book is already issued to ID: " << allbooks[i].studentid << "\n";
                return;
            }
            allbooks[i].available = false;
            allbooks[i].studentid = sid;
            savedata();
            cout << "Book issued!\n";
            return;
        }
    }
    cout << "Book not found!\n";
}

void returnbook() {
    string code;
    cout << "Enter book code: ";
    cin >> code;
    for(size_t i=0; i<allbooks.size(); i++) {
        if(allbooks[i].code == code) {
            allbooks[i].available = true;
            allbooks[i].studentid = 0;
            savedata();
            cout << "Book returned!\n";
            return;
        }
    }
    cout << "Book not found!\n";
}

void showgrades() {
    cout << "\n=== GRADES ===\n";
    for(auto &g : allgrades) {
        cout << "ID: " << g.studentid << " | Avg: " << g.avg << " | Grade: " << g.grade << " | Marks: ";
        for(int m : g.marks) cout << m << " ";
        cout << endl;
    }
}

void entergrades() {
    int sid;
    array<int,5> marks{};
    int total=0;
    cout << "Enter student ID: ";
    cin >> sid;
    cout << "Enter 5 subject marks:\n";
    for(int i=0; i<5; i++) {
        cout << "Subject " << i+1 << ": ";
        cin >> marks[i];
        total += marks[i];
    }
    float avg = total/5.0f;
    char grade;
    if(avg >= 90) grade = 'A';
    else if(avg >= 80) grade = 'B';
    else if(avg >= 70) grade = 'C';
    else if(avg >= 60) grade = 'D';
    else grade = 'F';
    Grade g;
    g.studentid = sid;
    g.marks = marks;
    g.avg = avg;
    g.grade = grade;
    allgrades.push_back(g);
    savedata();
    cout << "Average: " << avg << "%\n";
    cout << "Grade: " << grade << endl;
}

void showattendance() {
    cout << "\n=== ATTENDANCE ===\n";
    for(auto &a : allattendance) {
        float percent = (a.total==0) ? 0.0f : (a.present * 100.0f) / a.total;
        cout << "ID: " << a.studentid << " | " << a.present << "/" << a.total << " (" << percent << "% )" << endl;
    }
}

void enterattendance() {
    Attendance a;
    cout << "Enter student ID: ";
    cin >> a.studentid;
    cout << "Total working days: ";
    cin >> a.total;
    cout << "Days present: ";
    cin >> a.present;
    allattendance.push_back(a);
    savedata();
    float percent = (a.total==0) ? 0.0f : (a.present * 100.0f) / a.total;
    cout << "Attendance: " << percent << "%\n";
}

void savedata() {
    ofstream sf("students.txt");
    for(auto &s : allstudents) {
        sf << s.id << "," << s.name << "," << s.course << "\n";
    }
    sf.close();
    ofstream bf("books.txt");
    for(auto &b : allbooks) {
        bf << b.code << "," << b.name << "," << (b.available ? 1 : 0) << "," << b.studentid << "\n";
    }
    bf.close();
    ofstream gf("grades.txt");
    for(auto &g : allgrades) {
        gf << g.studentid;
        for(int m : g.marks) gf << "," << m;
        gf << "\n";
    }
    gf.close();
    ofstream af("attendance.txt");
    for(auto &a : allattendance) {
        af << a.studentid << "," << a.total << "," << a.present << "\n";
    }
    af.close();
}

void loaddata() {
    allstudents.clear();
    ifstream sf("students.txt");
    string line;
    if(sf) {
        while(getline(sf, line)) {
            if(line.empty()) continue;
            auto tok = splitCSV(line);
            if(tok.size() < 3) continue;
            Student s;
            try { s.id = stoi(tok[0]); } catch(...) { continue; }
            s.name = tok[1];
            s.course = tok[2];
            allstudents.push_back(s);
        }
        sf.close();
    }
    allbooks.clear();
    ifstream bf("books.txt");
    if(bf) {
        while(getline(bf, line)) {
            if(line.empty()) continue;
            auto tok = splitCSV(line);
            if(tok.size() < 4) continue;
            Book b;
            b.code = tok[0];
            b.name = tok[1];
            b.available = (tok[2] == "1" || tok[2] == "true");
            try { b.studentid = stoi(tok[3]); } catch(...) { b.studentid = 0; }
            allbooks.push_back(b);
        }
        bf.close();
    }
    allgrades.clear();
    ifstream gf("grades.txt");
    if(gf) {
        while(getline(gf, line)) {
            if(line.empty()) continue;
            auto tok = splitCSV(line);
            if(tok.size() < 6) continue; // id + 5 marks
            Grade g;
            try { g.studentid = stoi(tok[0]); } catch(...) { continue; }
            int total = 0;
            for(int i=0;i<5;i++) {
                try { g.marks[i] = stoi(tok[i+1]); } catch(...) { g.marks[i] = 0; }
                total += g.marks[i];
            }
            g.avg = total/5.0f;
            if(g.avg >= 90) g.grade = 'A';
            else if(g.avg >= 80) g.grade = 'B';
            else if(g.avg >= 70) g.grade = 'C';
            else if(g.avg >= 60) g.grade = 'D';
            else g.grade = 'F';
            allgrades.push_back(g);
        }
        gf.close();
    }
    allattendance.clear();
    ifstream af("attendance.txt");
    if(af) {
        while(getline(af, line)) {
            if(line.empty()) continue;
            auto tok = splitCSV(line);
            if(tok.size() < 3) continue;
            Attendance a;
            try { a.studentid = stoi(tok[0]); } catch(...) { continue; }
            try { a.total = stoi(tok[1]); } catch(...) { a.total = 0; }
            try { a.present = stoi(tok[2]); } catch(...) { a.present = 0; }
            allattendance.push_back(a);
        }
        af.close();
    }
}

void studentmenu() {
    int choice;
    do {
        cout << "\n=== STUDENT MENU ===\n";
        cout << "1. Add student\n";
        cout << "2. View students\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;
        if(choice == 1) addstudent();
        else if(choice == 2) showstudents();
    } while(choice != 0);
}

void bookmenu() {
    int choice;
    do {
        cout << "\n=== BOOK MENU ===\n";
        cout << "1. Add book\n";
        cout << "2. View books\n";
        cout << "3. Issue book\n";
        cout << "4. Return book\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;
        if(choice == 1) addbook();
        else if(choice == 2) showbooks();
        else if(choice == 3) issuebook();
        else if(choice == 4) returnbook();
    } while(choice != 0);
}

void grademenu() {
    int choice;
    do {
        cout << "\n=== GRADE MENU ===\n";
        cout << "1. Enter grades\n";
        cout << "2. View grades\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;
        if(choice == 1) entergrades();
        else if(choice == 2) showgrades();
    } while(choice != 0);
}

void attendancemenu() {
    int choice;
    do {
        cout << "\n=== ATTENDANCE MENU ===\n";
        cout << "1. Mark attendance\n";
        cout << "2. View attendance\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;
        if(choice == 1) enterattendance();
        else if(choice == 2) showattendance();
    } while(choice != 0);
}

int main() {
    loaddata();
    int choice;
    do {
        cout << "\n=== MAIN MENU ===\n";
        cout << "1. Students\n";
        cout << "2. Books\n";
        cout << "3. Grades\n";
        cout << "4. Attendance\n";
        cout << "5. Save data\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        if(choice == 1) studentmenu();
        else if(choice == 2) bookmenu();
        else if(choice == 3) grademenu();
        else if(choice == 4) attendancemenu();
        else if(choice == 5) { savedata(); cout << "Data saved!\n"; }
        else if(choice == 0) cout << "Goodbye!\n";
        else cout << "Wrong choice!\n";
    } while(choice != 0);
    return 0;
}

