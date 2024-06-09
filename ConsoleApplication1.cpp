#include<opencv2\opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/face.hpp>
#include<chrono>
#include<direct.h>
#include<conio.h>
#include<iostream> 
#include <stdio.h>
#include <fstream>
#include <sstream> 
#include<vector>
#include<ctime>
using namespace std;
using namespace cv;
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
        cin.ignore(1, '\n');
        getline(cin, name);
        cout << "\t\tSection (for example 1A): ";
        cin >> sec;
        cout << "\t\tRoll Number (XXXX): ";
        cin >> roll_no;
        cout << "\n\nPlease make sure the details provided above must be appropriate.\n\tPress 'enter' to submit form or any other key to change the above details...\n\n";
        c = _getch();
        if (c != 13) goto one;
    }
    void save_student_data(int& img_label) const {
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
        if (!check) {
            CSV_File.open(filename, ios::out);
            fstream unrec_file("C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\images.csv", ios::in);
            if (!CSV_File || !unrec_file) {
                cout << "Error! Unable to access images.\n";
                exit(0);
            }
            string line;
            while (getline(unrec_file, line)) CSV_File << line<<endl ;
        }
        else {
            CSV_File.open(filename, ios::app);
            if (!CSV_File) {
                cout << "Error! Unable to access CSV file\n";
                exit(0);
            }
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
        cout << "\nPlease wait..\n\n";
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
void Add::manage_frame_display() { 
    set_instructions();
    if (img_count <= 150) {
        if ((frame_count % 40) < 20) frame_text = "Scanning Face.";
        else if ((frame_count % 20) < 10) frame_text = "Scanning Face..";
        else frame_text = "Scanning Face...";
    }
    else frame_text = "Scanning Completed!";
    putText(frame, instructions, Point(10, 70), FONT_HERSHEY_SIMPLEX, 0.6, CV_RGB(255, 0, 0));
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
void generate_Trainer(const string& fn_csv, const string& sec) {
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


class User {
    string username, password;
public:
    User(string u, string p) :username(u), password(p) {}
    bool checkPassword(string p) const { return password == p; }
    bool checkUsername(string u) const { return username == u; }
    void setUsername(string a) { username = a; }
    void setPassword(string a) { password = a; }
    string getUsername() { return username; }
    string getPassword() { return password; }
};

static bool checkUsername(string a) {
    const string login_info = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\Login Information.txt";
    ifstream inFile(login_info, ios::in);
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
    const string login_info = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\Login Information.txt";
    string un, pw;
    ifstream inFile(login_info, ios::in);
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

static bool signup() {
    const string login_info = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\Login Information.txt";
    string un, pw;

    ifstream File(login_info, ios::in);
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
    fstream inFile(login_info, ios::app);
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
public: //static int current;
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
    const string attendance_path = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Attendance Record\\";
    bool *attendance;//tot
    char *mark;//tot
    const int frameLimit = 15;
public:
    Attendance(int cnt) {
        attendance = new bool[cnt];
        mark = new char[cnt];
        for (int i = 0; i < cnt; i++) {
            attendance[i] = false;
            mark[i] = 'A';
        }
    }
    void markPresent(const vector<Students>& students,int freq[]) {
        for (int i = 0; i < students.size(); i++)
            if (freq[students[i].getFaceID()] > frameLimit) {
                attendance[i] = true;
                mark[i] = 'P';
            }
    }
    void updateAttendencefile(const vector<Students>& students,const string sec) {
        //auto start = chrono::system_clock::now();
        //auto const time = chrono::system_clock::to_time_t(start);
        //char buf[20];
        //ctime_s(buf, sizeof(buf),&time);
        //string date="";
        //for (int i = 4; i < 11; i++) date.push_back(buf[i]);
        //date.push_back(' ');
        //date.push_back(',');
        string filename = attendance_path + sec + " (9/6/24).csv";
        fstream attendance_file;
        attendance_file.open(filename, ios::out);
        if (!attendance_file) {
            cout << "Error! Unable to open attendence file.";
        }
        attendance_file << endl << "\r,Name,Roll no.,Section,Attendance";
        for (int i = 0; i < students.size(); i++) attendance_file << ",\n\r," << students[i].getName() << ',' << students[i].getID() << ',' << students[i].getSec() << ',' << mark[i];
        attendance_file.close();
    }
};

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
    void display_Face_Information(FRAME& f,const Students& student) const{
        if (label == student.getFaceID()) putText(f.original, student.getName(), Point(pos_x + 10, pos_y - 5), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 153, 153), 1);
    }
    friend class FRAME;
    friend class FACERECOGNIZER;
};

class FACERECOGNIZER {
    const string student_data = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\All Sections\\Class Information [";
    const string class_trainer = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\recmodel ";
    const string video = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\VIDEOS\\rage.mp4";
    const string DetectorFile = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT\\opencv\\haarcascade_frontalface_default.xml";
    const string sample_image = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Image data\\ (1).jpg";
    const string window = "MRKING ATTENDENCE";
    Ptr<FaceRecognizer> model,model0;
    Attendance* studentAttendance;
    Students* recognized_student;
    vector<Students> students;
    CascadeClassifier face_cascade;
    VideoCapture cap;
    Mat testSample;
    int img_width = 0, img_height = 0, recognized_faces_freq[100] = { 0 };
    string section;
    void set_section();
    void recognizeFace(FACE& f);
    void setallStudentsData();
    void FaceDetection(FRAME& f);
    void manageAttendence();
public:
    FACERECOGNIZER() {
        here:
        cout << "1. Log In\n2. Sign Up\n\n\tEnter Choice: ";
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
        set_section();//imread
        setallStudentsData();
        studentAttendance = new Attendance(students.size());
        if (!face_cascade.load(DetectorFile)) {
            cout << "ERROR! Unable to load detection file.";
            exit(0);
        }
        model = EigenFaceRecognizer::create();
        string filename = class_trainer + section + ".xml";
        model->read(filename);
        testSample = imread(sample_image, 0);
        img_width = testSample.cols;
        img_height = testSample.rows;
        cap.open(video);
        if (!cap.isOpened()) {
            cout << "\n\nError! Camera is not connected properly\n";
            exit(0);
        }
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
                    faceobj.display_Face_Information(frameobj,*recognized_student);
                }
                imshow(window, frameobj.original);
            }
            int key = waitKey(30);
            if (key == 27) break;
        }
        manageAttendence();
    }
    friend class Students;
    friend class FRAME;
    friend class FACE;
};
void FACERECOGNIZER::setallStudentsData() {
    string filename = student_data + section + "].csv";
    fstream class_file(filename, ios::in);
    if (!class_file) {
        cout << "\n\nSorry! This system is unable to mark attendance for section " << section << ".\n";
        exit(0);
    }
    string str;
    bool check = false;
    while (getline(class_file, str, '\r')) {
        if (!check) {
            check = true;
            continue;
        }
        string word, name, roll, sec;
        stringstream check1(str);
        int cnt = -1, label;
        bool status = true;
        while (getline(check1, word, ',')) {
            if (word == "Label") {
                break;
            }
            else if (word == "") {   
                if (status) {
                    status = false;
                    continue;
                }
                else break;
            }
            ++cnt;
            switch (cnt) {
            case 0: {
                label = stoi(word);
                break;
            }
            case 1: {
                roll = word;
                break;
            }
            case 2: {
                name = word;
                break;
            }
            case 3: {
                sec = word;
                break;
            }
            default: { break; }
            }
        }
        Students student(label, name, roll, sec);
        students.push_back(student);
    }
}
void FACERECOGNIZER::FaceDetection(FRAME& f) { face_cascade.detectMultiScale(f.graySacleFrame, f.faces, 1.1, 3, 0, cv::Size(90, 90)); }
void FACERECOGNIZER::set_section() {
    cout << "\n\n\tClass Section: ";
    cin >> section;
}
void FACERECOGNIZER::manageAttendence() {
    studentAttendance->markPresent(students,recognized_faces_freq);
    studentAttendance->updateAttendencefile(students,section);
}
void FACERECOGNIZER::recognizeFace(FACE& f){
    resize(f.face, f.face_resized, Size(img_width,img_height), 1.0, 1.0, INTER_CUBIC);
    cout << "1: " << f.label << endl;
    model->predict(f.face_resized, f.label, f.confidence);
    cout << "2: " << f.label << endl;
    //model0->predict(f.face_resized, f.label, f.confidence);
    //cout << "3: " << f.label << endl<<endl;
    if (f.label == -1) recognized_student = new Students();
    else {
        for (int i = 0; i < students.size(); i++) 
            if (f.label == students[i].getFaceID()) {
                recognized_student = new Students(students[i]);
                ++recognized_faces_freq[f.label];
            }
    }
    f.pos_x = max(f.face_i.tl().x - 10, 0);
    f.pos_y = max(f.face_i.tl().y - 10, 0);
}

int main() {   
    cout << "\n\t\t\t\t\t\t\tSECURE SENSE\n\t\t\t\t\t\t\tWELCOME TO THE FUTURE\n\t\t\t\t\tWhere Security is of the utmost importance\n\n\n\t1. Add Face\n\t2. Proceed to Attendance\n\nEnter Choice: ";
    int c;
    cin >> c;
    if (c == 1) {
    one:
        Add student;
        cout << "\n\n\t\t\tFace Added Successfully!\n\nPress:\n\t1. 'enter' to add next face\n\t2. Any key to quit.\n";
        char c = _getch();
        switch (c) {
        case 13:
            goto one;
            break;
        default:
            break;
        }
    }
    else FACERECOGNIZER facerec; 
    return 0;
}


//                                          ADD FACE
//static int resize_save(Mat& faceIn, fstream& file, string roll_no, int  img_num, int label);
//Input: current image, the path of file, the name or faceseq of images.  Output: None
//Function: resize the current image to (92, 112), which is same to the train data.
//int get_face();
//Input: the path of file.
//Output: None
//Function: Use face_cascade to detect if there are faces in the window, and save the faces through function reszie_save.
/*#include <opencv2/highgui/highgui.hpp>
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
        putText(frame, instructions, Point(10,70), FONT_HERSHEY_SIMPLEX, 0.6, CV_RGB(255, 0, 0));
        //putText(frame, "Keep your face close to the camera and follow instructions.", Point(10, 40), FONT_HERSHEY_SIMPLEX, 0.75, CV_RGB(0, 0, 0));
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
    cout << "\n\n\t\t\tFace Added Successfully!\n\nPress:\n\t1. 'enter' to add next face\n\t2. Any key to quit.\n";
    char c = _getch();
    switch (c) {
    case 13:
        goto one;
        break;
    default:
        break;
    }
    return 0;
}*/