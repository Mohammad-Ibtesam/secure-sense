#include<chrono>
#include<direct.h>
#include<conio.h>
#include<iostream> 
#include <stdio.h>
#include <fstream>
#include<string.h>
#include <sstream> 
#include<vector>
#include<ctime>
#include<opencv2\opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/face.hpp>
using namespace cv;
using namespace cv::face;
using namespace std;

string personfile, attendancefile;
const string DataFile = "SecureSenseFiles/private/data.txt";
const string loginfile = "SecureSenseFiles/private/login.txt";

class User {
    string username, password;
    bool checkPassword(string p) const { return password == p; }
    bool checkUsername(string u) const { return username == u; }
    User(string u, string p) :username(u), password(p) {}
public:
    bool checkAccount(string u, string p) {
        if (checkUsername(u) && checkPassword(p)) return true;
        return false;
    }
    void setUsername(string a) { username = a; }
    void setPassword(string a) { password = a; }
    string getUsername() { return username; }
    string getPassword() { return password; }
    int friend login(string u, string p);
};

static bool isFirstAccount() {
    bool status = true;
    ifstream inFile(loginfile);
    if (inFile) status = false;
    inFile.close();
    return status;
}

static int checkUsername(string u) {
    ifstream inFile(loginfile, ios::in);
    if (!inFile) return -1;
    vector<string> un;
    string c, b;
    while (inFile >> c >> b) un.push_back(c);
    inFile.close();
    for (int i = 0; i < un.size(); i++) if (un[i] == u) return 1;
    return 0;
}

static int login(string u, string p) {
    string un, pw;
    ifstream inFile(loginfile, ios::in);
    if (!inFile) return -1;
    vector<User> admins;
    int cnt = 0;
    do {
        User temp(un, pw);
        admins.push_back(temp);
    } while (inFile >> un >> pw);
    inFile.close();
    for (int i = 0; i < admins.size(); i++) if (admins[i].checkAccount(u, p) == 1) return 1;
    return 0;
}

static bool signup(string u, string p) {
    fstream File;
    if (isFirstAccount()) File.open(loginfile, ios::out);
    else File.open(loginfile, ios::app);
    if (!File.is_open()) return false;
    File << u << " " << p << endl;
    File.close();
    return true;
}

bool CreateFiles(string path, int option = 0, string sec = "") {

    for (int i = 0; i < sec.size(); i++) if (sec[i] == ' ') sec[i] = '_';
    for (int i = 0; i < path.size(); i++) if (path[i] == '\\') path[i] = '/';

    string occupation;
    if (option == 0) occupation = "Student";
    else occupation = "Employee";

    personfile = path + '/' + occupation + "sList_" + sec + ".csv";
    attendancefile = path + "/AttendaceList_" + sec + '_';

    fstream data_file(DataFile, ios::out);
    if (!data_file) return false;
    data_file << "0\n" << sec << endl << occupation << endl << personfile << endl << attendancefile << endl;
    data_file.close();
    return true;
}

int getLabel() {
    int x;
    fstream Label_file(DataFile, ios::in);
    if (Label_file) {
        char* s = new char;
        Label_file >> s;
        Label_file.close();
        x = atoi(s);
    }
    else x = 0;
    Label_file.close();
    return ++x;
}

string getSection() {
    fstream sec_file(DataFile, ios::in);
    if (!sec_file) return "";
    string sec;
    for (int i = 0; i < 2; i++) getline(sec_file, sec);
    sec_file.close();
    return sec;
}

string getOccupation() {
    fstream file(DataFile, ios::in);
    if (!file) return "";
    string s;
    for (int i = 0; i < 3; i++) getline(file, s);
    file.close();
    return s;
}

string getPersonFile() {
    fstream path_file(DataFile, ios::in);
    if (!path_file) return "";
    string filename;
    for (int i = 0; i < 4; i++) getline(path_file, filename);
    path_file.close();
    return filename;
}

string getAttendanceFile() {
    fstream path_file(DataFile, ios::in);
    if (!path_file) return "";
    string filename;
    for (int i = 0; i < 5; i++) getline(path_file, filename);
    path_file.close();
    return filename;
}

bool ChangeSection(string sec) {
    fstream f(DataFile, ios::in);
    if (!f) return false;
    string arr[5];
    for (int i = 0; i < 5; i++) getline(f, arr[i]);
    f.close();
    fstream f1(DataFile, ios::out);
    for (int i = 0; i < 5; i++) {
        if (i == 1) f1 << sec << endl;
        else f1 << arr[i] << endl;
    }
    f1.close();
    return true;
}

static string read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    fstream file(filename.c_str(), ios::in);
    if (!file) return "System is unable to detect CSV file.";
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if (!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
    return "";
}

string generate_Trainer(string sec) {
here:
    string msg = "";
    vector<Mat> images;
    vector<int> labels; // load data
    bool test = true;
    string filename = "SecureSenseFiles/private/" + sec + "_image.csv";
    msg = read_csv(filename, images, labels);
    if (msg != "") return msg;
    if (images.size() < 2) return "Insufficient number of images.";
    Mat testSample = images[images.size() - 1];
    int testLabel = labels[labels.size() - 1];
    images.pop_back();
    labels.pop_back();
    string trainer_name = "SecureSenseFiles/private/" + sec + "_trainer.xml";
    Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
    model->train(images, labels);
    model->write(trainer_name);
    int predictedLabel = model->predict(testSample);
    if (predictedLabel != testLabel) {
        if (test) {
            test = false;
            goto here;
        }
        else return "Oops! something went wrong. Please restart this application.\n";
    }
    waitKey(0);
    return "";
}

int getMonthNumber(string a) {
    if (a == "Jan") return 0;
    else if (a == "Fab") return 1;
    else if (a == "Mar") return 2;
    else if (a == "Apr") return 3;
    else if (a == "May") return 4;
    else if (a == "Jun") return 5;
    else if (a == "Jul") return 6;
    else if (a == "Aug") return 7;
    else if (a == "Sep") return 8;
    else if (a == "Oct") return 9;
    else if (a == "Nov") return 10;
    else return 11;
}

class Student {
    string name, sec, roll_no, student_record;
public:
    Student(string n, string id, string s) :name(n), sec(s), roll_no(id) {}
    string save_student_data(int& img_label) const {
        const string filename = getPersonFile();
        if (filename == "") return "Unable to access " + getOccupation() + "s list.";
        bool check = true;
        fstream check_file(filename, ios::in);
        if (!check_file) check = false;
        check_file.close();
        string sd, occ = getOccupation();
        if (occ == "Student") sd = "Section";
        else sd = "Deparmtment";
        fstream Student_File;
        if (!check) {
            Student_File.open(filename, ios::out);
            Student_File << "S.no,Roll no,Name,"<<sd<<",\n";
        }
        else Student_File.open(filename, ios::app);
        if (!Student_File) return "Unable to access " + getOccupation() + "s list.";
        Student_File << img_label << ',' << roll_no << ',' << name << ',' << sec << ",\n";
        Student_File.close();
        return "";
    }
    friend class Add;
};

class Add {
    string frame_text, instructions;
    int frame_count = 0, img_count = 1, img_label = -1;
    char key;
    string errormsg = "";
    Student* student;
    VideoCapture camera;
    CascadeClassifier face_cascade;
    Mat frame, img_gary, faceImg;//desired_frame;  //camera frame
    vector<Rect> objects; //The faces coordinates. //Gradation pictures.
    void manage_frame_display();
    void set_instructions();
    string update_label();
    int save_img(Mat& faceIn, fstream& file, string roll_no, int  img_num, int& label);
public:
    string add(string name, string id, string sec);
};

string Add::add(string name, string id, string sec) {
    student = new Student(name, id, sec);
    camera.open(1);
    bool checkfile = face_cascade.load("SecureSenseFiles/private/haarcascade_frontalface_default.xml");
    if (!checkfile) return "Unable to access face detection file.";
    sec = getSection();
    string filename = "SecureSenseFiles/private/" + sec + "_image.csv";
    bool check = true;
    fstream check_file(filename, ios::in);
    if (!check_file) check = false;
    check_file.close();
    fstream CSV_File;
    if (!check) {
        CSV_File.open(filename, ios::out);
        fstream unrec_file("SecureSenseFiles/private/images.csv", ios::in);
        if (!CSV_File || !unrec_file) return "Unable to access CSV file.";
        else {
            string line;
            while (getline(unrec_file, line)) CSV_File << line << endl;
        }
    }
    else {
        CSV_File.open(filename, ios::app);
        if (!CSV_File) return "Unable to access CSV file.";
    }
    img_label = getLabel();
    while (true) {
        camera >> frame;
        if (frame.empty()) return "Please connect your camera.";
        ++frame_count;
        key = waitKey(1);
        if (key == 27) break;
        if (frame.empty()) continue;
        cvtColor(frame, img_gary, COLOR_BGR2GRAY); //Transform frame as the gradation picture, note imshow is still the original frame.
        equalizeHist(img_gary, img_gary); //Histogram equalization, which is helpful to improve the quality of pictures.
        face_cascade.detectMultiScale(img_gary, objects, 1.1, 3, 0, Size(50, 50));//Face detection
        for (size_t i = 0; i < objects.size(); i++)  rectangle(frame, objects[i], Scalar(0, 255, 0), 2);
        if (frame_count % 5 == 0 && objects.size() == 1 && img_count <= 150) {
            faceImg = frame(objects[0]);
            int ret = save_img(faceImg, CSV_File, student->roll_no, img_count, img_label);
            if (errormsg != "") return errormsg;
            if (ret == 0) ++img_count;
        }
        resize(frame, frame, Size(1024, 768), 0, 0);
        manage_frame_display();
        imshow("Scanning Face...", frame);
    }
    destroyAllWindows();
    CSV_File.close();
    if (img_count <= 145) return "You have quited from scanning process.";
    update_label();
    if (errormsg != "") return errormsg;
    student->save_student_data(img_label);
    return "";
}

string Add::update_label() {
    fstream f(DataFile, ios::in);
    if (!f) return "Unable to update label.";
    string arr[5];
    for (int i = 0; i < 5; i++) getline(f, arr[i]);
    f.close();
    fstream f1(DataFile, ios::out);
    for (int i = 0; i < 5; i++) {
        if (i == 0) f1 << img_label << endl;
        else f1 << arr[i] << endl;
    }
    f1.close();
    return "";
}

int Add::save_img(Mat& faceIn, fstream& file, string roll_no, int  img_num, int& label) {
    Mat faceOut;
    if (faceIn.empty()) {
        errormsg = "No face detected.";
        return -1;
    }
    if (faceIn.cols > 100) {
        resize(faceIn, faceOut, Size(92, 112)); //Resize and Keep a match with the train database.
        string strname = format("%s%s(%d).jpg", "SecureSenseFiles/images/", roll_no, img_num);
        if (label >= -1) file << strname << ";" << label << endl;
        bool isimagewritten = imwrite(strname, faceOut); //save image. Note the file suffix.
        if (!isimagewritten) {
            errormsg = "Image write failed! Please check filename is legal or not!";
            return -1;
        }
    }
    waitKey(20);
    return 0;
}

void Add::set_instructions() {
    if (img_count < 30) instructions = "Look stright.";
    else if (img_count < 60) instructions = "Slightly move your head up.";
    else if (img_count < 90) instructions = "Slightly move your head down.";
    else if (img_count < 120) instructions = "Slightly turn left.";
    else if (img_count < 150) instructions = "Slightly turn right.";
    else instructions = "Press 'esc' to close..";
}

void Add::manage_frame_display() {
    set_instructions();
    if (img_count <= 150) {
        if ((frame_count % 40) < 20) frame_text = "Scanning Face.";
        else if ((frame_count % 20) < 10) frame_text = "Scanning Face..";
        else frame_text = "Scanning Face...";
    }
    else frame_text = "Scanning Completed!";
    putText(frame, instructions, Point(15, 90), FONT_HERSHEY_SIMPLEX, 1.0, CV_RGB(0, 153, 153),2);
    putText(frame, frame_text, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.2, CV_RGB(0, 153, 153), 3);
}

class Person {
protected:
    int label;
    string name, ID, sec;
    Person() :label(0), name(""), ID(""), sec("") {}
    Person(int l, string n, string id, string s) :label(l), name(n), ID(id), sec(s) {}
public:
    string getName() const { return name; }
    string getID() const { return ID; }
    string getSec() const { return sec; }
    int getFaceID() const { return label; }
};
class FACERECOGNIZER;
class Students :public Person {
public:
    Students() :Person() {}
    Students(int l, string n, string id, string s) :Person(l, n, id, s) {  }
    Students(const Students& student) {
        label = student.label;
        ID = student.ID;
        name = student.name;
        sec = student.sec;
    }
};

class Attendance {
    string* mark;
    const int frameLimit = 15;
    string errormsg,date, mon, yr = "20";;
    int dd,mmm,yyyy;
public:
    Attendance(vector<Students>& students);
    bool isOnLeave(string id, int y, int m, int d);
    void markPresent(const vector<Students>& students, int freq[]);
    string updateAttendencefile(const vector<Students>& students, const string sec) {
        string filename = getAttendanceFile() + date+ "-" + mon + "-" + yr + ".csv";
        fstream attendance_file;
        attendance_file.open(filename, ios::out);
        if (!attendance_file) return "Error! Unable to create attendence file.";
        attendance_file << endl << "\r,Name,Roll no.,Section,Attendance,";
        for (int i = 0; i < students.size(); i++) attendance_file << "\r," << students[i].getName() << ',' << students[i].getID() << ',' << students[i].getSec() << ',' << mark[i];
        attendance_file.close();
        return "";
    }
};

Attendance::Attendance(vector<Students>& students) {
    auto start = chrono::system_clock::now();
    auto const time = chrono::system_clock::to_time_t(start);
    char buf[26];
    ctime_s(buf, sizeof(buf), &time);
    date.push_back(buf[8]);
    date.push_back(buf[9]);
    for (int i = 4; i < 7; i++) mon.push_back(buf[i]);
    yr.push_back(buf[22]);
    yr.push_back(buf[23]);
    dd = stoi(date);
    yyyy = stoi(yr);
    mmm = getMonthNumber(mon);
    mark = new string[students.size()];
    for (int i = 0; i < students.size(); i++) {
        if (isOnLeave(students[i].getID(), yyyy, mmm, dd)) mark[i] = "On-Leave";
        else mark[i] = "Absent";
    }
}

void Attendance::markPresent(const vector<Students>& students, int freq[]) {
    for (int i = 0; i < students.size(); i++)
        if (freq[students[i].getFaceID()] > frameLimit) mark[i] = "Present";
}

bool Attendance::isOnLeave(string id, int y, int m, int dt) {
    string line;
    fstream path_file(DataFile, ios::in);
    if (!path_file) return false;
    for (int i = 0; i < 5; i++) getline(path_file, line);
    while (getline(path_file, line)) {
        if (line.size() < 5) break;
        stringstream record(line);
        string word;
        getline(record, word, ' ');
        if (id == word) {
            vector<int> d;
            for (int i = 0; i < 6; i++) {
                getline(record, word, ' ');
                d.push_back(stoi(word));
            }
            if (d[0] > y || d[3] < y) return false;
            else if (d[0] == y) {
                if (d[1] > m) return false;
                else if (d[1] == m) {
                    if (d[2] > dt) return false;
                    else return true;
                }
                else return true;
            }
            else if (d[3] == y) {
                if (d[4] < m) return false;
                else if (d[4] == m && d[2]< dt) return false;
                else return true;
            }
            else return true;
        }
        continue;
    }
    path_file.close();
    return false;
}

class FRAME {
    vector<Rect> faces;
    Mat frame, graySacleFrame, original;
public:
    bool checkFrame() const { return (!frame.empty()); }
    void captureFrame(VideoCapture& cap) { cap >> frame; }
    void convertColour() {
        original = frame.clone();
        cvtColor(original, graySacleFrame, COLOR_BGR2GRAY);
    }
    friend class FACE;
    friend class FACERECOGNIZER;
};

class FACE {
public:
    bool isMatched = false;
    int label, pos_x = 0, pos_y = 0;
    double confidence = 0;
    Rect face_i;
    Mat face;
    Mat face_resized;
    FACE(FRAME& f, int index) {
        face_i = f.faces[index];
        face = f.graySacleFrame(face_i);
    }
    void highlightFace(FRAME& f) { rectangle(f.original, face_i, CV_RGB(0, 255, 0), 2); }
    void display_Face_Information(FRAME& f, const Students& student,int freq) const {
        if (label == student.getFaceID()) {
            if (freq<=20) putText(f.original, student.getName(), Point(pos_x + 10, pos_y - 5), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 153, 153), 1);
            else putText(f.original, "Marked Present", Point(pos_x + 10, pos_y - 5), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 153, 153), 1);
        }
        else putText(f.original, "Unknown", Point(pos_x + 10, pos_y - 5), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(255, 0, 0), 1);
    }
    friend class FRAME;
    friend class FACERECOGNIZER;
};

class FACERECOGNIZER {
    const string sample_image ="SecureSenseFiles/images/(1).jpg";
    const string window = "MRKING ATTENDENCE";
    string errormsg = "";
    Ptr<FaceRecognizer> model;
    Attendance* studentAttendance;
    Students* recognized_student;
    vector<Students> students;
    CascadeClassifier face_cascade;
    VideoCapture cap;
    string section;
    Mat testSample;
    int img_width = 0, img_height = 0, recognized_faces_freq[100] = { 0 };
    void recognizeFace(FACE& f);
    string setallStudentsData();
    void FaceDetection(FRAME& f);
    void manageAttendence();
public:
    string StartRecognition() {
        section = getSection();
        errormsg = setallStudentsData();
        if (errormsg != "") return errormsg;
        studentAttendance = new Attendance(students);
        if (!face_cascade.load("SecureSenseFiles/private/haarcascade_frontalface_default.xml")) return "ERROR! Unable to load detection file.";
        model = LBPHFaceRecognizer::create();
        const string filename = "SecureSenseFiles/private/" + section + "_trainer.xml";
        model->read(filename);
        testSample = imread(sample_image, 0);
        img_width = testSample.cols;
        img_height = testSample.rows;
        cap.open(1);
        namedWindow(window, 1);
        while (true) {
            FRAME frameobj;
            frameobj.captureFrame(cap);
            if (frameobj.checkFrame()) {
                frameobj.convertColour();
                FaceDetection(frameobj);
                for (int i = 0; i < frameobj.faces.size(); i++) {
                    FACE faceobj(frameobj, i);
                    recognizeFace(faceobj);
                    faceobj.highlightFace(frameobj);
                    faceobj.display_Face_Information(frameobj, *recognized_student, recognized_faces_freq[faceobj.label]);
                }
                resize(frameobj.original, frameobj.original, Size(1024, 768), 0, 0);
                putText(frameobj.original, "Attendance Check-In", Point(10, 30), FONT_HERSHEY_SIMPLEX, 1, CV_RGB(0, 153, 153), 2);
                putText(frameobj.original, ":) Smile for the camera ", Point(10, 70), FONT_HERSHEY_SIMPLEX, 0.75, CV_RGB(0, 153, 153), 1);
                imshow(window, frameobj.original);
            }
            else return "Please connect your camera.";
            int key = waitKey(30);
            if (key == 27) break;
        }
        manageAttendence();
        if (errormsg != "") return errormsg;
        return "";
    }
    friend class Students;
    friend class FRAME;
    friend class FACE;
};
string FACERECOGNIZER::setallStudentsData() {
    fstream class_file(getPersonFile(), ios::in);
    if (!class_file)  return "Oops! This system is unable to mark attendance for section " + getSection() + '.';
    string str;
    getline(class_file, str);
    while (getline(class_file, str)) {
        stringstream check1(str);
        int cnt = -1, label;
        string word, name, roll, sec;
        getline(check1, word, ',');
        label = stoi(word);
        getline(check1, word, ',');
        roll = word;
        getline(check1, word, ',');
        name = word;
        getline(check1, word, ',');
        sec = word;
        Students student(label, name, roll, sec);
        students.push_back(student);
    }
    return "";
}
void FACERECOGNIZER::FaceDetection(FRAME& f) { face_cascade.detectMultiScale(f.graySacleFrame, f.faces, 1.1, 3, 0, cv::Size(90, 90)); }
void FACERECOGNIZER::manageAttendence() {
    studentAttendance->markPresent(students, recognized_faces_freq);
    errormsg = studentAttendance->updateAttendencefile(students, section);
}
void FACERECOGNIZER::recognizeFace(FACE& f) {
    resize(f.face, f.face_resized, Size(img_width, img_height), 1.0, 1.0, INTER_CUBIC);
    model->predict(f.face_resized, f.label, f.confidence);
    recognized_student = nullptr; // Ensure recognized_student is reset
    if (f.label != -1 && f.confidence<=100) {
        for (int i = 0; i < students.size(); i++) {
            if (f.label == students[i].getFaceID()) {
                recognized_student = new Students(students[i]);
                ++recognized_faces_freq[f.label];
                break; // Break once a match is found
            }
        }
    }
    if (recognized_student == nullptr) recognized_student = new Students; // Initialize with a default value if no match is found
    f.pos_x = max(f.face_i.tl().x - 10, 0);
    f.pos_y = max(f.face_i.tl().y - 10, 0);

}

string AttendanceWithFaceRecognition() {
    FACERECOGNIZER facerec;
    return facerec.StartRecognition();
}

string CaptureFace(string name, string id, string sec) {
    Add faces;
    return faces.add(name, id, sec);
}

vector<string> returnAllIDs(){
    string filename=getPersonFile();
    fstream f(filename,ios::in);
    vector<string> ids;
    string record;
    getline(f, record);
    while (getline(f, record)) {
        stringstream ss(record);
        string word;
        getline(ss, word, ',');
        getline(ss, word, ',');
        if (word != "") ids.push_back(word);
    }
    return ids;
}

bool setLeave(string ID, int yf, int yt, int mf, int mt, int df, int dt) {
    fstream file(DataFile, ios::app);
    if (!file) return false;
    file << ID << ' ' << yf << ' ' << mf << ' ' << df << ' ' << yt << ' ' << mt << ' ' << dt <<' ' << endl;
    file.close();
    return true;
}
