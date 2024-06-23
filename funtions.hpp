#include<chrono>
#include<direct.h>
#include<conio.h>
#include<iostream> 
#include <stdio.h>
#include <fstream>
#include <sstream> 
#include<vector>
#include<ctime>
/*#include<opencv2\opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/face.hpp>
using namespace cv;
using namespace cv::face;*/
using namespace std;

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
    const string login_info = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\Login Information.txt";
    bool status = true;
    ifstream inFile(login_info);
    if (inFile) status = false;
    inFile.close();
    return status;
}

static int checkUsername(string u) {
    const string login_info = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\Login Information.txt";
    ifstream inFile(login_info, ios::in);
    if (!inFile) return -1;
    vector<string> un;
    string c, b;
    while (inFile >> c >> b) un.push_back(c);
    inFile.close();
    for (int i = 0; i < un.size(); i++) if (un[i] == u) return 1;
    return 0;
}

int login(string u, string p) {
    const string login_info = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\Login Information.txt";
    string un, pw;
    ifstream inFile(login_info, ios::in);
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
    const string login_info = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\Login Information.txt";
    fstream File;
    if (isFirstAccount()) File.open(login_info, ios::out);
    else File.open(login_info, ios::app);
    if (!File.is_open()) return false;
    File << u << " " << p << endl;
    File.close();
    return true;
}

string personfile, attendancefile;
const string pathsoflists = "pathsoflists.txt", sectionfile = "sec/dept.txt", labelfile = "_lastlabel.txt", loginfile = "login.txt", csvfile = "_images.csv", trainerfile = "_facerec.xml", SorEfile = "SorE.txt";

static string read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    ifstream file(filename.c_str(), ios::in);
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

string generate_Trainer(const string& fn_csv, const string& sec) {
here:
    vector<Mat> images;
    vector<int> labels; // load data
    bool test = true;
    string filename = sec + csvfile;
    string trainer_name = sec + trainerfile;
    string msg;
    msg=read_csv(filename, images, labels);
    if (images.size() <= 1) msg = "This demo needs at least 2 images to work. Please add more images to your data set!";
    Mat testSample = images[images.size() - 1];
    int testLabel = labels[labels.size() - 1];
    images.pop_back();
    labels.pop_back();
    Ptr<BasicFaceRecognizer> model = EigenFaceRecognizer::create();
    model->train(images, labels);
    model->write(trainer_name);
    int predictedLabel = model->predict(testSample);
    if (predictedLabel != testLabel) {
        if (test) {
            test = false;
            goto here;
        }
        else msg = "Trainer testing failed. Kindly! close the applicaion and repeat the process again.\n";
    }
    waitKey(0);
}

void setfilenames(string path,int option,string sec="") {

    for (int i = 0; i < path.size(); i++) if (path[i] == '\\') path[i] = '/';

    string occupation;
    if (option == 0) occupation = "Student";
    else occupation = "Employee";

    personfile = path + '/' + occupation + "sList_" + sec + ".csv";
    attendancefile = path + "/AttendaceList_" + sec + '_';

    fstream sore_file(SorEfile, ios::out);
    sore_file << occupation;
    sore_file.close();

    fstream path_file(pathsoflists, ios::out);
    path_file << personfile << endl << attendancefile;
    path_file.close();

    fstream sec_file(sectionfile, ios::out);
    sec_file << sec;
    sec_file.close();
}

string getSection() {
    fstream sec_file(sectionfile, ios::in);
    if (!sec_file) return "";
    string sec;
    getline(sec_file,sec);
    return sec;
}

string getPersonFile() {
    fstream path_file(pathsoflists, ios::in);
    if (!path_file) return "";
    string filename;
    getline(path_file, filename);
    return filename;
}

string getAttendanceFile() {
    fstream path_file(pathsoflists, ios::in);
    if (!path_file) return "";
    string filename;
    getline(path_file, filename);
    getline(path_file, filename);
    return filename;
}

string getOccupation() {
    fstream file(SorEfile, ios::in);
    if (!file) return "";
    string s;
    getline(file, s);
    return s;
}

class Student {
    string name, sec, roll_no, student_record;
public:
    Student(string n, string id, string s) :name(n), sec(s), roll_no(id) {}
    string save_student_data(int& img_label) const {
        string filename = getPersonFile();
        bool check = true;
        fstream check_file(filename, ios::in);
        if (!check_file) check = false;
        check_file.close();
        fstream Student_File;
        if (!check) {
            Student_File.open(filename, ios::out);
            Student_File << "\n,S.no,Roll no,Name,Sec";
        }
        else Student_File.open(filename, ios::app);
        if (!Student_File) return "Unable to access " + getOccupation() + "s list.";
        Student_File << "\r, " << img_label << ',' << roll_no << ',' << name << ',' << sec;
        Student_File.close();
    }
    friend class Add;
};

class Add {
    string errormsg,frame_text, instructions;
    const string face_detector = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT\\opencv\\haarcascade_frontalface_default.xml";
    VideoCapture camera;
    CascadeClassifier face_cascade;
    Mat frame, img_gary, faceImg, desired_frame;  //camera frame
    vector<Rect> objects; //The faces coordinates. //Gradation pictures.
    int ret, img_label = 0, frame_count = 0, img_count = 0;
    char key;
    Student *student;
    void manage_frame_display();
    void set_instructions();
    void set_label();
    void update_label()const;
    int save_img(Mat& faceIn, fstream& file, string roll_no, int  img_num, int& label);
public:
    Add(string a, string b, string c);
};

Add::Add(string name, string id, string sec) {
    student=new Student(name,id,sec);
    camera.open("C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\VIDEOS\\fs.mp4"); //C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2/VIDEOS\\random.mp4"
    if (!camera.isOpened()) errormsg = "Unable to access camera.";
    ret = face_cascade.load(face_detector); //Load the face cascadeclassifier.
    if (!ret) errormsg = "Unable to access face detection file.";
    set_label();
    string filename = personfile;
    bool check = true;
    fstream check_file(filename, ios::in);
    if (!check_file) check = false;
    check_file.close();
    fstream CSV_File;
    if (!check) {
        CSV_File.open(filename, ios::out);
        fstream unrec_file(sec+csvfile, ios::in);
        if (!CSV_File || !unrec_file) errormsg = "Unable to access images.";
        string line;
        while (getline(unrec_file, line)) CSV_File << line << endl;
    }
    else {
        CSV_File.open(filename, ios::app);
        if (!CSV_File) errormsg = "Unable to access CSV file.";
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
void Add::set_label() {
    fstream Label_file(labelfile, ios::in);
    char s;
    if (Label_file) {
        Label_file >> s;
        Label_file.close();
    }
    img_label=1+atoi(s);
}

void Add::update_label()const {
    fstream Label_file2(label_file, ios::out);
    if (!Label_file2) errormsg = "Unable to access Label file.";
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
    putText(frame, instructions, Point(10, 70), FONT_HERSHEY_SIMPLEX, 0.65, CV_RGB(0, 153, 153));
    putText(frame, frame_text, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.75, CV_RGB(0, 255, 0), 2);
}
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
}*/
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
    const string attendance_path = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Attendance Record\\";
    bool* attendance;//tot
    char* mark;//tot
    const int frameLimit = 15;
public:
    Attendance(int cnt);
    void markPresent(const vector<Students>& students, int freq[]);
    void updateAttendencefile(const vector<Students>& students, const string sec) {
        auto start = chrono::system_clock::now();
        auto const time = chrono::system_clock::to_time_t(start);
        char buf[26];
        ctime_s(buf, sizeof(buf), &time);
        string date, yr;
        for (int i = 4; i < 10; i++) date.push_back(buf[i]);
        date[3] = '-';
        if (date[9] != ' ') {
            date.push_back('-');
            yr.push_back(buf[22]);
            yr.push_back(buf[23]);
        }
        else {
            date[9] = '-';
            yr.push_back(buf[21]);
            yr.push_back(buf[22]);
        }
        string filename = getAttendanceFile()+ date + "20" + yr + ".csv";
        fstream attendance_file;
        attendance_file.open(filename, ios::out);
        if (!attendance_file) {
            cout << "Error! Unable to create attendence file.";
        }
        attendance_file << endl << "\r,Name,Roll no.,Section,Attendance";
        for (int i = 0; i < students.size(); i++) attendance_file << ",\n\r," << students[i].getName() << ',' << students[i].getID() << ',' << students[i].getSec() << ',' << mark[i];
        attendance_file.close();
    }
};

Attendance::Attendance(int cnt) {
    attendance = new bool[cnt];
    mark = new char[cnt];
    for (int i = 0; i < cnt; i++) {
        attendance[i] = false;
        mark[i] = 'A';
    }
}
void Attendance::markPresent(const vector<Students>& students, int freq[]) {
    for (int i = 0; i < students.size(); i++)
        if (freq[students[i].getFaceID()] > frameLimit) {
            attendance[i] = true;
            mark[i] = 'P';
        }
}

/*class FRAME {
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
    void display_Face_Information(FRAME& f, const Students& student) const {
        if (label == student.getFaceID()) putText(f.original, student.getName(), Point(pos_x + 10, pos_y - 5), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 153, 153), 1);
    }
    friend class FRAME;
    friend class FACERECOGNIZER;
};

class FACERECOGNIZER {
    const string class_trainer = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\recmodel ";
    const string video = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\VIDEOS\\rage.mp4";
    const string DetectorFile = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT\\opencv\\haarcascade_frontalface_default.xml";
    const string sample_image = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Image data\\ (1).jpg";
    const string window = "MRKING ATTENDENCE";
    Ptr<FaceRecognizer> model, model0;
    Attendance* studentAttendance;
    Students* recognized_student;
    vector<Students> students;
    CascadeClassifier face_cascade;
    VideoCapture cap;
    Mat testSample;
    int img_width = 0, img_height = 0, recognized_faces_freq[100] = { 0 };
    string section;
    void set_section(string s);
    void recognizeFace(FACE& f);
    void setallStudentsData();
    void FaceDetection(FRAME& f);
    void manageAttendence();
public:
    FACERECOGNIZER(string s) {
        set_section(s);//imread
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
                    faceobj.display_Face_Information(frameobj, *recognized_student);
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
    string filename = getPersonFile + section + "].csv";
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
void FACERECOGNIZER::set_section(string s) {
    cout << "\n\n\tClass Section: ";
    section = s;
}
void FACERECOGNIZER::manageAttendence() {
    studentAttendance->markPresent(students, recognized_faces_freq);
    studentAttendance->updateAttendencefile(students, section);
}
void FACERECOGNIZER::recognizeFace(FACE& f) {
    resize(f.face, f.face_resized, Size(img_width, img_height), 1.0, 1.0, INTER_CUBIC);
    model->predict(f.face_resized, f.label, f.confidence);
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
}*/
