/*#include<opencv2\opencv.hpp>
#include<direct.h>
#include<conio.h>
#include<iostream>
#include <fstream>
using namespace std;
using namespace cv;
using namespace cv::face;

const int totalStudents = 43, current = 2;
int recognized_face_IDs[totalStudents] = { 0 };
class Person {
    int faceID;
    string name, ID, sec;
public:
    virtual string getName() const = 0;
    virtual string getID() const = 0;
    virtual string getSec() const = 0;
    virtual int getFaceID() const = 0;
};
class User {
    string username, password;
public:
    User(string u, string p) :username(u), password(p) {}
    bool checkPassword(string p) const{ return password == p; }
    bool checkUsername(string u) const{ return username == u; }
    void setUsername(string a) { username = a; }
    void setPassword(string a) { password = a; }
    string getUsername() { return username; }
    string getPassword() { return password; }
};

class Students:public Person {
    int faceID;
    string name, ID, sec;
public:
    static int current;
    string getName() const { return name; }
    string getID() const { return ID; }
    string getSec() const { return sec; }
    int getFaceID() const { return faceID; }
    Students() :faceID(0), name(""), ID(""), sec("") {}
    Students(int fid, string n, string id, string s) :faceID(fid), name(n), ID(id), sec(s) { current++; }

    void copyInfo(const Students& student) {
        ID = student.ID;
        name = student.name;
        sec = student.sec;
    }
    static bool checkStudent(Students& recognizedStudent, int label);
};
int Students::current = 0;
Students* students[totalStudents];
bool Students::checkStudent(Students& recognizedStudent, int label) {
    for (int i = 0; i < current; i++) 
        if ((students[i]->faceID) == label) {
            recognizedStudent.copyInfo(*students[i]);
            ++recognized_face_IDs[label];
            return true;
        }
    return false;
}

static void setallStudentsData() {
    students[0] = new Students(3, "Yahya Shaikh", "[23K-0718]", "BCS 2E");
    students[1] = new  Students(9, "M. Ibtesam", "[23K-0738]", "BCS 2E");
    students[2] = new  Students(1, "Aliyan Navaid", "[23K-1353]", "BCS 2E");
    students[3] = new  Students(2, "Muhammad Farooq", "[23K-4524]", "BCS 2E");
    students[4] = new  Students(4, "Faizan Javaid", "[23K-2572]", "BCS 2E");
    students[5] = new  Students(5, "Shaheer Uddin", "[23K-7543]", "BCS 2E");
    students[6] = new  Students(6, "Izaan Sheikh", "[23K-8677]", "BCS 2E");
    students[7] = new  Students(7, "Affan Basit", "[23K-0004]", "BCS 2E");
    students[8] = new  Students(8, "Saad Hussain", "[23K-4674]", "BCS 2E");
    students[9] = new  Students(10, "Usama Basharat", "[23K-1467]", "BCS 2E");
}

class Attendance {
    bool attendance[totalStudents];
    string mark[totalStudents];
public:

    Attendance() {
        for (int i = 0; i < Students::current; i++)
            attendance[i] = false;
    }
    const int frameLimit = 50;
    void markPresent() {
        for (int i = 0; i < Students::current; i++)
            if (recognized_face_IDs[students[i]->getFaceID()] > frameLimit) {
                attendance[i] = true;
                mark[i] = "Present";
            }
            else mark[i] = "Absent";
    }
    void updateAttendencefile() const {
        fstream attendence_file;
        attendence_file.open("C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Attendence 2E.xlxs", ios::out);
        if (!attendence_file) {
            cout << "Error! Unable to open attendence file.";
        }
        attendence_file << "\n\t\tFAST NUCES, Karachi\n\t\t13th May,2024\n\t\tBCS - 2E\n\n\tS no.\tRoll no.\tName\tSection\tAttendence";
        for (int i = 0; i < Students::current; i++) {
            attendence_file << "\n\t" << i + 1 << "." << "\t" << students[i]->getID() << "\t" << students[i]->getName() << "\t" << students[i]->getSec() << "\t" << mark[i];
        }
        attendence_file.close();
    }
    void displayAttendance() const {
        cout << "\n\n\n\tSuccessfully Updated the attendence\n\n";
        /*for (int i = 0; i < Students::current; i++) {
            cout << i + 1 << ".\t" << students[i]->getID() << "\t" << students[i]->getName() << "\t" << students[i]->getSec() << "\t";
            if (attendance[i]) cout << "Present\n";
            else cout << "Absent\n";
        }
        cout << "\nNOTE: Attendence can only be marked (Present/Absent) only if the data of the student is available." << endl;
    }
};

class FRAME {
    string text_on_frame;
    static int count;
    Mat frame;
    Mat graySacleFrame;
public:
    vector<Rect> faces;
    Mat original;

    FRAME() { count++; }

    bool checkFrame() const { return !frame.empty(); }

    void captureFrame(VideoCapture& cap) { cap >> frame; }

    void convertColour() {
        original = frame.clone();
        cvtColor(original, graySacleFrame, COLOR_BGR2GRAY);
    }

    void display_frame_information() const {
        putText(original, "Frame Count: " + to_string(count), Point(30, 60), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(255, 255, 0), 1);
        putText(original, "Persons detected: " + to_string(faces.size()), Point(30, 90), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(255, 255, 0), 1);
    }

    friend class FACERECOGNIZER;
    friend class FACE;
};
int FRAME::count = 0;

class FACE {
public:
    bool isMatched = false;
    int label = -1, pos_x = 0, pos_y = 0;
    double confidence = 0;
    Rect face_i;
    Mat face;
    Mat face_resized;
    Students* Recognized_Student;

    FACE(FRAME& f, int index) {
        Recognized_Student = new Students();
        face_i = f.faces[index];
        face = f.graySacleFrame(face_i);
    }

    void matching() {
        if (Students::checkStudent(*Recognized_Student, label)) {
            isMatched = true;
            cout << "Face recognized successfully." << endl;
        }
        if (!isMatched) cout << "Face recognition unsuccessfull." << endl;
    }

    void highlightFace(FRAME& f) { rectangle(f.original, face_i, CV_RGB(0, 255, 0), 2); }

    void display_Face_Information(FRAME& f) const{
        if (label != 3 && label != 9) {
            string text_on_frame = "Warning: Stranger Detected";
            putText(f.original, text_on_frame, Point(pos_x, pos_y), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(255, 0, 0), 1);
        }
        else {
            string text_on_frame = Recognized_Student->getName() + " " + Recognized_Student->getID() + " " + Recognized_Student->getSec();
            putText(f.original, text_on_frame, Point(pos_x, pos_y), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 235, 203), 1);
        }
    }
};

class FACERECOGNIZER {
    CascadeClassifier face_cascade;
    Mat testSample;
    int img_width = 0, img_height = 0;
    static string DetectorFile, sample_image, trainer_file, window;
    Attendance* studentAttendance;

public:
    friend class Detection;
    Ptr<FaceRecognizer> model;

    FACERECOGNIZER() {
        studentAttendance = new Attendance();
        if (!face_cascade.load(DetectorFile)) cout << "ERROR! Unable to load detection file.";
        model = EigenFaceRecognizer::create();
        model->read(trainer_file);
        testSample = imread(sample_image, 0);
        img_width = testSample.cols;
        img_height = testSample.rows;
    }

    int getWidth() const { return img_width; }
    int getHeight() const { return img_height; }

    void FaceDetection(FRAME& f) { face_cascade.detectMultiScale(f.graySacleFrame, f.faces, 1.1, 3, 0, cv::Size(90, 90)); }

    void createWindow() { namedWindow(window, 1); }

    void show_frame_on_window(Mat& original) { imshow(window, original); }

    void manageAttendence() {
        studentAttendance->markPresent();
        studentAttendance->updateAttendencefile();
        studentAttendance->displayAttendance();
    }

    void recognizeFace(FACE& f)const {
        resize(f.face, f.face_resized, Size(getWidth(), getHeight()), 1.0, 1.0, INTER_CUBIC);
        model->predict(f.face_resized, f.label, f.confidence);
        f.matching();
        f.pos_x = max(f.face_i.tl().x - 10, 0);
        f.pos_y = max(f.face_i.tl().y - 10, 0);
    }

};

string FACERECOGNIZER::DetectorFile = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT\\opencv\\haarcascade_frontalface_default.xml";
string FACERECOGNIZER::sample_image = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\facerec pictures\\i8.jpg ";
string FACERECOGNIZER::trainer_file = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\XML Files\\modeltrainer.xml";
string FACERECOGNIZER::window = "MRKING ATTENDENCE";


bool checkUsername(string a) {
    ifstream inFile("C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Login Information.txt", ios::in);
    if (!inFile) {
        cout << "\nError! Unable to access Log-In data.\n";
        exit(0);
    }
    vector<string> un;
    string c, b;
    while (inFile >> c >> b) un.push_back(c);
    inFile.close();
    for (int i = 0; i < un.size(); i++) if (un[i] == a) return true;
    return false;
}

static bool login() {
    string un, pw;
    ifstream inFile("C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Login Information.txt", ios::in);
    if (!inFile) {
        cout << "\nError! Unable to access Log-In data.\n";
        exit(0);
    }
    if (!(inFile >> un >> pw)) {
        inFile.close();
        cout << "\nOops! Cannot Log-In without creating any account first.\n";
        return false;
    }
    cout << "\n\t\tLogging In\n\n";
    vector<User> admins;
    int cnt = 0;
    do {
        User temp(un, pw);
        admins.push_back(temp); // Rect  fa
    } while (inFile >> un >> pw);
    inFile.close();
start2:
    cout << "\tUsername: ";
    cin >> un;
    pw = "";
    cout << "\tPassword: ";
    while (true) {
        char c = _getch();
        if (c == 13) break;
        else if (c == 8) {
            pw.pop_back();
            cout << "\b \b";
            continue;
        }
        pw.push_back(c);
        cout << '*';
    }
    for (int i = 0; i < admins.size(); i++)
        if (admins[i].checkUsername(un) && admins[i].checkPassword(pw)) {
            cout << "\n\nSuccessfully Logged In!" << endl;
            return true;
        }
    cnt++;
    if (cnt < 3) {
        cout << "\n\nOops! Incorrect log-in information.\n\nPlease try again.\n" << endl;
        goto start2;
    }
    cout << "\n\nThe application has been locked.\n\nClose the application and log-in again.\n" << endl;
    exit(0);
}

bool signup() {

    string un, pw;

    ifstream File("C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Login Information.txt", ios::in);
    if (!File) {
        cout << "\nError! Unable to access Log-In data.\n";
        exit(0);
    }

    else if (File >> un >> pw) {
        File.close();
        cout << "\nNote: To add another account, log-in first with any of the previously added admin account(s).\n\n";
        login();
    }
    else File.close();

loco:
    cout << "\n\t\tSigning Up\n";
    cout << "\n\tUsername: ";
    cin >> un;
    if (checkUsername(un)) {
        cout << "\nOops! The username you entered is already taken. Please re-enter different username\n";
        goto loco;
    }
    pw = "";
    cout << "\tPassword: ";
    while (true) {
        char c = _getch();
        if (c == 13) break;
        else if (c == 8) {
            pw.pop_back();
            cout << "\b \b";
            continue;
        }
        pw.push_back(c);
        cout << '*';
    }
ins:
    string p = "";
    cout << "\n\tConfirm Password: ";
    while (true) {
        char c = _getch();
        if (c == 13) break;
        else if (c == 8) {
            p.pop_back();
            cout << "\b \b";
            continue;
        }
        p.push_back(c);
        cout << '*';
    }
    if (p != pw) {
        cout << "\n\nPassword Mismatched. Please retype.\n";
        goto ins;
    }
    fstream inFile("C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Login Information.txt", ios::app);
    if (!inFile) {
        cout << "\n\nError! Unable to find Log-In information file\n";
        exit(0);
    }
    inFile << '\n' << un << " " << pw << endl;
    inFile.close();
    cout << "\n\n\tSuccessfully created new administrator account. Now Log-In to proceed.\n";
    login();
    return true;
}

int main() {
    cout << "\n\t\t\t\t\t\t\t SECURE SENSE" << endl;
    cout << "\t\t\t\t\t\t    WELCOME TO THE FUTURE" << endl;
    cout << "\t\t\t\t\tWhere Security is of the utmost importance" << endl;    
    here:
    cout << "1. Log In\n2. Sign Up\n\nEnter Choice: ";
    int n;
    cin >> n;
    switch (n) {
        case 1: {
            if (!login()) signup();
            break;
        }
        case 2: {
            signup();
            break;
        }
        default: {
            cout << "\nError! Invalid input.\n";
            goto here;
        }
    }
    setallStudentsData();
    FACERECOGNIZER facerec;
    //VideoCapture cap(0);
    // 2 "C:\\Users\\Afzaal Khan\\Pictures\\Camera Roll\\vvv.mp4"
    //C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\VIDEOS\\qwerty.mp4  C:\\Users\\Afzaal Khan\\Pictures\\Camera Roll\\WIN_20240513_06_05_26_Pro.mp4
    VideoCapture cap("C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\VIDEOS\\bablu.mp4");
    if (!cap.isOpened()) return -1;
    facerec.createWindow();
    while (true) {
        FRAME frameobj;
        frameobj.captureFrame(cap);
        if (frameobj.checkFrame()) {
            frameobj.convertColour();
            facerec.FaceDetection(frameobj);
            for (int i = 0; i < frameobj.faces.size(); i++) {
                FACE faceobj(frameobj, i);
                facerec.recognizeFace(faceobj);
                faceobj.highlightFace(frameobj);
                faceobj.display_Face_Information(frameobj);
            }
            frameobj.display_frame_information();
            facerec.show_frame_on_window(frameobj.original);
        }
        int key = waitKey(30);
        if (key == 27) break;
    }
    facerec.manageAttendence();
    return 0;
}*/


//                                          ADD FACE
//static int resize_save(Mat& faceIn, fstream& file, string roll_no, int  img_num, int label);
//Input: current image, the path of file, the name or faceseq of images.  Output: None
//Function: resize the current image to (92, 112), which is same to the train data.
//int get_face();
//Input: the path of file.
//Output: None
//Function: Use face_cascade to detect if there are faces in the window, and save the faces through function reszie_save.
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/face.hpp>
#include <stdio.h>
#include <iostream>
#include<conio.h>
#include <fstream>
#include <sstream> 
#include<vector>
using namespace cv;
using namespace std;
using namespace cv::face;
void generate_Trainer(const string& fn_csv, const string& sec);

class Student {
    string student_data = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\All Sections\\Class Information [";
    string name, sec, roll_no, student_record;
public:
    Student() {}
    void set_Student_Data() {
        char c;
        one:
        cout << "\n\n\t\t\tStudent Information Form\n\t\tStudent Name: ";
        cin.ignore(1,'\n');
        getline(cin, name);
        cout << "\t\tSection (for example 1A): ";
        cin >> sec;
        cout << "\t\tRoll Number (XXXX): ";
        cin >> roll_no;
        cout << "\n\nPlease make sure the details provided above must be appropriate.\n\tPress 'enter' to submit form or any other key to change the above details...";
        c = _getch();
        if (c != 13) goto one;
    }
    void save_student_data(int& img_label) const{
        string filename = student_data + sec + "].csv";
        bool check = true;
        fstream check_file(filename, ios::in);
        if (!check_file) check = false;
        check_file.close();
        fstream Student_File;
        if (!check) {
            Student_File.open(filename, ios::out);
            Student_File << "\n,Label,Roll no,Name,Sec";
        }
        else Student_File.open(filename, ios::app);
        if (!Student_File) {
            cout << "\n\nError! Unable to save student deatils.\n";
            exit(0);
        }
        cin.ignore();
        Student_File << "\r, " << img_label << ',' << roll_no << ',' << name << ',' << sec;
        Student_File.close();//cout << "\n\n\tSuccessfully saved student information.\n";
    }
    friend class Add;
};

class Add {
    string frame_text, instructions, CSV_file = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\imgdata ";
    const string face_detector = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT\\opencv\\haarcascade_frontalface_default.xml";
    const string label_file = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\label.txt";
    VideoCapture camera;
    CascadeClassifier face_cascade;
    Mat frame, img_gary, faceImg, desired_frame;  //camera frame
    vector<Rect> objects; //The faces coordinates. //Gradation pictures.
    int ret, img_label, frame_count = 0, img_count = 0;
    char key;
    Student student;
    void manage_frame_display();
    void set_instructions();
    void set_label();
    void update_label();
    int save_img(Mat& faceIn, fstream& file, string roll_no, int  img_num, int& label);
public:
    Add() {//cout << "\n\t\t\tAdding Face\n\n";
        student.set_Student_Data();
        camera.open("C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\VIDEOS\\fs.mp4"); //C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2/VIDEOS\\random.mp4"
        if (!camera.isOpened()) {
            cout << "\n\nError! Unable to access camera." << endl;
            exit(0);
        }
        ret = face_cascade.load(face_detector); //Load the face cascadeclassifier.
        if (!ret) {
            cout << "Error! Unable to access face detection file." << endl;
            exit(0);
        }
        set_label();
        string filename = CSV_file + student.sec + ".csv";
        bool check = true;
        fstream check_file(filename, ios::in);
        if (!check_file) check = false;
        check_file.close();
        fstream CSV_File;
        if (!check) CSV_File.open(filename, ios::out);
        else CSV_File.open(filename, ios::app);
        if (!CSV_File) {
            cout << "Error! Unable to access CSV file\n";
            exit(0);
        }
        while (true) {
            camera >> frame;
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
                ret = save_img(faceImg, CSV_File, student.roll_no, img_count, img_label);
                if (ret == 0) ++img_count;
            }
            manage_frame_display();
            resize(frame, desired_frame, Size(1500, 800), 0, 0);
            imshow("Scanning Face...", desired_frame);
        }
        destroyAllWindows();
        CSV_File.close();
        update_label();
        student.save_student_data(img_label);
        generate_Trainer(CSV_file, student.sec);
    }
};
void Add::set_label() {
    fstream Label_file(label_file, ios::in);
    if (!Label_file) {
        cout << "Error! Unable to access Label file\n";
        exit(0);
    }
    Label_file >> img_label;
    Label_file.close();
    ++img_label;
}
void Add::update_label() {
    fstream Label_file2(label_file, ios::out);
    if (!Label_file2) {
        cout << "Error! Unable to access Label file\n";
        exit(0);
    }
    Label_file2 << img_label;
    Label_file2.close();
}
int Add::save_img(Mat& faceIn, fstream& file, string roll_no, int  img_num, int& label) {
    string strname;
    Mat faceOut;
    bool ret;
    if (faceIn.empty()) {
        printf("No face detected.\n");
        return -1;
    }
    if (faceIn.cols > 100) {
        resize(faceIn, faceOut, Size(92, 112)); //Resize and Keep a match with the train database.
        strname = format("%s\\%s (%d).jpg", "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\image data", roll_no, img_num); //mkdir
        file << strname << ";" << label << endl;
        ret = imwrite(strname, faceOut); //save image. Note the file suffix.
        if (ret == false) {
            printf("Image write failed!\n");
            printf("Please check filename[%s] is legal!\n", strname.c_str());
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
void Add::manage_frame_display(){
    if (img_count <= 150) {
        set_instructions();
        if ((frame_count % 40) < 20) frame_text = "Scanning Face.";
        else if ((frame_count % 20) < 10) frame_text = "Scanning Face..";
        else frame_text = "Scanning Face...";//max(objects[0].tl().x + 20, 0), max(objects[0].tl().y - 50, 0)
        putText(frame, instructions, Point(10,45), FONT_HERSHEY_SIMPLEX, 0.6, CV_RGB(255, 0, 0), 2);
        putText(frame, "Keep your face close to the camera and follow instructions.", Point(300, 500), FONT_HERSHEY_SIMPLEX, 0.75, CV_RGB(0, 0, 0), 2);
    }
    else frame_text = "Scanning Completed!";
    putText(frame, frame_text, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.75, CV_RGB(0, 255, 0), 2);
}

void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    ifstream file(filename.c_str(), ios::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        exit(1);
    }
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
}
void generate_Trainer(const string& fn_csv,const string& sec) {
    here:
    vector<Mat> images;
    vector<int> labels; // load data
    bool test = true;
    string filename = fn_csv + sec + ".csv";
    string trainer_name = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\recmodel " + sec + ".xml";
    try { read_csv(filename, images, labels); }
    catch (cv::Exception& e)
    {
        cerr << "Error opening file \"" << filename << "\". Reason: " << e.msg << endl;
        exit(1);
    }
    if (images.size() <= 1) {
        string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        exit(0); //Error::(CV_StsError, error_message);
    }
    Mat testSample = images[images.size() - 1];
    int testLabel = labels[labels.size() - 1];
    images.pop_back();
    labels.pop_back();
    Ptr<BasicFaceRecognizer> model0 = EigenFaceRecognizer::create();
    model0->train(images, labels);
    model0->write(trainer_name);
    int predictedLabel0 = model0->predict(testSample);
    if (predictedLabel0 != testLabel) {
        if (test) {
            test = false;
            goto here;
        }
        else {
            cout << "\n\nError! Trainer testing failed. Kindly! close the applicaion and repeat the process again.\n";
            exit(0);
        }
    }
    waitKey(0);
}

int main() {
    one:
    Add student;
    cout << "\n\n\t\t\tFace Added Successfully!\n\nPress:\n\t1. 'enter' to add next face\n\t2. Any key to quit.";
    char c;
    c = _getch();
    switch (c) {
    case 13:
        goto one;
        break;
    default:
        break;
    }
    return 0;
}




/*
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <stdio.h>
#include <opencv2/face.hpp>
#include <iostream>
#include <fstream>
#include <sstream> 
using namespace cv;
using namespace std;
using namespace cv::face;

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    ifstream file(filename.c_str(), ios::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        exit(1);
    }
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
}

static void train_model(const string& fn_csv) { // 2 containers to store image data and corresponding labels
    vector<Mat> images;
    vector<int> labels; // load data
    try { read_csv(fn_csv, images, labels); }
    catch (cv::Exception& e)
    {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        exit(1);
    }
    if (images.size() <= 1) {
        string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        exit(0); //Error::(CV_StsError, error_message);
    }
    Mat testSample = images[images.size() - 1];
    int testLabel = labels[labels.size() - 1];
    images.pop_back();
    labels.pop_back();
    Ptr<BasicFaceRecognizer> model0 = EigenFaceRecognizer::create();
    model0->train(images, labels);
    model0->write("C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\unrecmodel.xml");
    int predictedLabel0 = model0->predict(testSample);
    string result_message0 = format("Predicted class = %d / Actual class = %d.", predictedLabel0, testLabel);
    cout << result_message0 << endl;
    for (int i = 0; i < labels.size(); i++) cout << " " << labels[i];
    waitKey(0);
}

int main() {
    train_model("C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\images.csv");
    return 0;
}

/*static Mat norm_0_255(InputArray _src) {
    Mat src = _src.getMat();
    Mat dst; //Create and return a normalized image matrix:
    switch (src.channels()) {
    case 1:
        normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
} //load CSV file
*/

/*
static int add_face() {
    move:
    cout << "\n\t\t\tAdding Face\n\n";
    CascadeClassifier face_cascade;
    Mat frame, img_gary, faceImg, desired_frame;  //camera frame
    vector<Rect> objects; //The faces coordinates. //Gradation pictures.
    int ret, img_label, frame_count = 0, img_count = 0;
    char key,c;
    string name, sec, roll_no, frame_text,instructions, student_record;

    cout << "\n\t";
    one:
    cout << "Enter Student Details.\n\t\tStudent Name: ";
    getline(cin, name);
    cout << "\t\tSection (for example 1A): ";
    cin >> sec;
    cout << "\t\tRoll Number (XXXX): ";
    cin >> roll_no;
    cout << "\n\nThe record of the student will be saved in the file, please make sure the details provided above must be appropriate.\n";
    cout<<"\tPress 'enter' to submit or any key to change the above details...";
    c = _getch();
    if (c!=13) {
            cout << "\n\tRe-";
            goto one;
    }

    VideoCapture camera("C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2/VIDEOS\\random.mp4");
    if (!camera.isOpened()) {
        cout << "Error! Unable to access camera." << endl;
        return -1;
    }
    ret = face_cascade.load("C:\\Users\\Afzaal Khan\\Desktop\\PROJECT\\opencv\\haarcascade_frontalface_default.xml"); //Load the face cascadeclassifier.
    if (!ret) {
        cout << "Error! Unable to access face detection file." << endl;
        return -1;
    }

    fstream Label_file("C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\label.txt", ios::in);
    if (!Label_file) {
        cout << "Error! Unable to access Label file\n";
        return -1;
    }
    Label_file >> img_label;
    Label_file.close();
    ++img_label;
    fstream CSV_File("C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\images.csv", ios::app);
    if (!CSV_File) {
        cout << "Error! Unable to access CSV file\n";
        exit(0);
    }
    while (true) {
        camera >> frame;
        ++frame_count;
        key = waitKey(1);
        if (key == 27) break;
        if (frame.empty()) continue;
        cvtColor(frame, img_gary, COLOR_BGR2GRAY); //Transform frame as the gradation picture, note imshow is still the original frame.
        equalizeHist(img_gary, img_gary); //Histogram equalization, which is helpful to improve the quality of pictures.
        face_cascade.detectMultiScale(img_gary, objects, 1.1, 3, 0, Size(50, 50));//Face detection
        for (size_t i = 0; i < objects.size(); i++)  rectangle(frame, objects[i], Scalar(0, 255, 0), 2);
        if ( frame_count%5==0 && objects.size() == 1 && img_count <= 150) {
            faceImg = frame(objects[0]);
            ret = Add::save_img(faceImg, CSV_File,roll_no, img_count, img_label);
            if (ret == 0) ++img_count;
        }


        if (img_count < 30) instructions = "Look stright.";
        else if (img_count < 60) instructions = "Slightly move up.";
        else if(img_count < 90) instructions = "Slightly move down.";
        else if (img_count < 120) instructions = "Slightly turn left.";
        else if (img_count < 150) instructions = "Slightly turn right.";
        putText(frame, instructions, Point(max(objects[0].tl().x + 20, 0), max(objects[0].tl().y - 50, 0)), FONT_HERSHEY_SIMPLEX, 0.6, CV_RGB(255, 0, 0), 2);
        if (img_count <= 150) {
            if ((frame_count % 40) < 10) frame_text = "Scanning Face.";
            else if ((frame_count % 20) < 10) frame_text = "Scanning Face..";
            else frame_text = "Scanning Face...";
        }
        else frame_text = "Face added successfully!";
        putText(frame, frame_text, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.75, CV_RGB(0, 255, 0),2);
        putText(frame, "Keep your face close to the camera and follow instructions.", Point(50, 450), FONT_HERSHEY_SIMPLEX, 0.75, CV_RGB(0, 0, 0), 2);
        resize(frame, desired_frame, Size(1500, 800), 0, 0);
        imshow("Adding Face", desired_frame);
    }
    destroyAllWindows();
    CSV_File.close();

    fstream Label_file2("C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\label.txt", ios::out);
    if (!Label_file2) {
        cout << "Error! Unable to access Label file\n";
        return -1;
    }
    Label_file2 << img_label;
    Label_file2.close();

    fstream Student_File("C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\Student's Information.xlxs", ios::app);
    if (!Student_File) {
        cout << "Error! Unable to save student deatils.\n";
        return -1;
    }
    cin.ignore();
    Student_File<<"\r" << img_label << "\t" << roll_no << "\t" << name << "\t" << sec;
    student_record = roll_no + "\t" + name + "\t" + sec ;
    Student_File << "\r" << img_label << "\t" << student_record << endl;
    Student_File.close();

    cout << "\n\n\t\t\tFace Added Successfully\n\nPress 'enter' to add next face or any key to close...";
    c = _getch();
    if (c == 13) goto move;
    else
        exit(1);
    return 1;
}

*/