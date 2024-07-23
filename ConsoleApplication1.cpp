/*#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap(0);

    // Check if the camera opened successfully
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera" << std::endl;
        return -1;
    }

    // Create a window to display the video
    cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);

    while (true) {
        cv::Mat frame;

        // Capture a new frame from the camera
        cap >> frame;

        // Check if the frame is empty (camera disconnected or other issues)
        if (frame.empty()) {
            std::cerr << "Error: Empty frame" << std::endl;
            break;
        }

        // Display the frame in the window
        cv::imshow("Webcam", frame);

        // Exit the loop if the user presses the 'q' key
        if (cv::waitKey(30) == 'q') {
            break;
        }
    }

    // Release the camera and close the window
    cap.release();
    cv::destroyAllWindows();

    return 0;
}*/


/*#include<opencv2\opencv.hpp>
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


class Student {
    string student_data = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\All Sections\\Class Information [";
    string name, sec, roll_no, student_record;
public:
    void set_Student_Data(string a,string b,string c);
    void save_student_data(int& img_label) const;
    friend class Add;
}; 

void Student::save_student_data(int& img_label) const {
    string filename = student_data + sec + "].csv";
    fstream Student_File(filename, ios::app);
    if (Student_File) {
        Student_File << "\r, " << img_label << ',' << roll_no << ',' << name << ',' << sec;
        Student_File.close();
    }
    else 
    Student_File.open(filename, ios::out);
    Student_File << "\n,Label,Roll no,Name,Sec";

}

void Student::set_Student_Data(string n,string id,string s) {
    char c;
one:
    name = n;
    sec = s;
    roll_no = id;
    cout << "\n\nPlease make sure the details provided above must be appropriate.\n\tPress 'enter' to submit form or any other key to change the above details...\n\n";
    c = _getch();
    if (c != 13) goto one;
}



class Add {
    string frame_text, instructions, CSV_file = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\imgdata ";
    const string face_detector = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT\\opencv\\haarcascade_frontalface_default.xml";
    const string label_file = "C:\\Users\\Afzaal Khan\\Desktop\\PROJECT 2\\Files\\label.txt";
    VideoCapture camera;
    CascadeClassifier face_cascade;
    Mat frame, img_gary, faceImg, desired_frame;  //camera frame
    vector<Rect> objects; //The faces coordinates. //Gradation pictures.
    int ret, img_label=0, frame_count = 0, img_count = 0;
    char key;
    Student student;
    void manage_frame_display();
    void set_instructions();
    void set_label();
    void update_label()const;
    int save_img(Mat& faceIn, fstream& file, string roll_no, int  img_num, int& label);
public: 
    Add(string a, string b, string c);
};

Add::Add(string a,string b,string c) {
    student.set_Student_Data(a,b,c);
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
        while (getline(unrec_file, line)) CSV_File << line << endl;
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
void Add::set_label() {
    fstream Label_file(label_file, ios::in);
    if (Label_file) {
        Label_file >> img_label;
        Label_file.close();
    }
    ++img_label;
}

void Add::update_label()const {
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
    putText(frame, instructions, Point(10, 70), FONT_HERSHEY_SIMPLEX, 0.65, CV_RGB(0, 153, 153));
    putText(frame, frame_text, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.75, CV_RGB(0, 255, 0), 2);
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
//cin
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
        admins.push_back(temp); // rect
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
    Attendance(int cnt);
    void markPresent(const vector<Students>& students, int freq[]);
    void updateAttendencefile(const vector<Students>& students,const string sec) {
        auto start = chrono::system_clock::now();
        auto const time = chrono::system_clock::to_time_t(start);
        char buf[26];
        ctime_s(buf, sizeof(buf), &time);
        string date, yr;
        for (int i = 4; i < 10; i++) date.push_back(buf[i]);
        if (buf[21] == '0') {
            yr.push_back(buf[22]);
            yr.push_back(buf[23]);
        }
        else {
            yr.push_back(buf[21]);
            yr.push_back(buf[22]);
        }
        string filename = attendance_path+ "( " + sec + date + ", 20" + yr + " ).csv";
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
void Attendance::markPresent(const vector<Students>&students, int freq[]) {
    for (int i = 0; i < students.size(); i++)
        if (freq[students[i].getFaceID()] > frameLimit) {
            attendance[i] = true;
            mark[i] = 'P';
        }
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
void FACERECOGNIZER::set_section(string s) {
    cout << "\n\n\tClass Section: ";
    section = s;
}
void FACERECOGNIZER::manageAttendence() {
    studentAttendance->markPresent(students,recognized_faces_freq);
    studentAttendance->updateAttendencefile(students,section);
}
void FACERECOGNIZER::recognizeFace(FACE& f){
    resize(f.face, f.face_resized, Size(img_width,img_height), 1.0, 1.0, INTER_CUBIC);
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
}
//cin
int main() {
    cout << "\n\t\t\t\t\t\t\tSECURE SENSE\n\t\t\t\t\t\t\tWELCOME TO THE FUTURE\n\t\t\t\t\tWhere Security is of the utmost importance\n\n\n\t1. Add Face\n\t2. Proceed to Attendance\n\nEnter Choice: ";
    int c;
    cin >> c;
    if (c == 1) {
    one:
        string a, b, d;
        cin >> a >> b >> d;
        Add student(a,b,d);
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
    else FACERECOGNIZER facerec("2E");
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
const string DataFolder = "F:\\PROJECT\\ConsoleApplication1\\SecureSenseFiles/private/";
const string RandomImageCSV = "F:\\PROJECT\\ConsoleApplication1\\SecureSenseFiles/private/images.csv";
const string DataFile = "F:\\PROJECT\\ConsoleApplication1\\SecureSenseFiles/private/data.txt";
const string loginfile = "F:\\PROJECT\\ConsoleApplication1\\SecureSenseFiles/private/login.txt";
const string csvfile = "_image.csv", trainerfile = "_trainer.xml";
const string face_detector = "F:\\PROJECT\\ConsoleApplication1\\SecureSenseFiles/private/haarcascade_frontalface_default.xml";

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
    return "";// to_string(images.size());
}

string generate_Trainer(string sec) {
here:
    string msg = "";
    vector<Mat> images;
    vector<int> labels; // load data
    bool test = true;
    string filename = "F:\\PROJECT\\ConsoleApplication1\\SecureSenseFiles/private/" + sec + csvfile;
    const string trainer_name = "F:\\PROJECT\\ConsoleApplication1\\SecureSenseFiles/private/" + sec + trainerfile;
    msg = read_csv(filename, images, labels);
    if (msg != "") return msg;
    if (images.size() < 2) return "Insufficient number of images.";
    Mat testSample = images[images.size() - 1];
    int testLabel = labels[labels.size() - 1];
    images.pop_back();
    labels.pop_back();
    //Ptr<BasicFaceRecognizer> model = LBPHFaceRecognizer::create();
    Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
    model->train(images, labels);
    model->write(trainer_name);
    int predictedLabel = model->predict(testSample);
    if (predictedLabel != testLabel) {
        if (test) {
            test = false;
            goto here;
        }
        else return "Oops! something went wrong. Please close the applicaion and repeat the process again.\n";
    }
    waitKey(0);
    return "";
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
    Mat frame, img_gary, faceImg;// desired_frame;  //camera frame
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
    camera.open(0);
    bool checkfile = face_cascade.load("F:/PROJECT/opencv/haarcascade_frontalface_default.xml");//face_detector); //Load the face cascadeclassifier. >> frame 
    if (!checkfile) return "Unable to access face detection file.";
    sec = getSection();
    string filename = "F:\\PROJECT\\ConsoleApplication1\\SecureSenseFiles/private/" + sec + csvfile;
    bool check = true;
    fstream check_file(filename, ios::in);
    if (!check_file) check = false;
    check_file.close();
    fstream CSV_File;
    if (!check) {
        CSV_File.open(filename, ios::out);
        fstream unrec_file(RandomImageCSV, ios::in);
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
        if (frame.empty()) return "End of video stream.";
        ++frame_count;
        key = waitKey(1);
        if (key == 27) break;
        if (frame.empty()) continue;
        cvtColor(frame, img_gary, COLOR_BGR2GRAY); //Transform frame as the gradation picture, note imshow is still the original frame.
        equalizeHist(img_gary, img_gary); //Histogram equalization, which is helpful to improve the quality of pictures.
        face_cascade.detectMultiScale(img_gary, objects, 1.1, 3, 0, Size(50, 50));//Face detection
        for (size_t i = 0; i < objects.size(); i++) rectangle(frame, objects[i], Scalar(0, 255, 0), 2);
        if (frame_count % 5 == 0 && objects.size() == 1 && img_count <= 150) {
            faceImg = frame(objects[0]);
            int ret = save_img(faceImg, CSV_File, student->roll_no, img_count, img_label);
            if (errormsg != "") return errormsg;
            if (ret == 0) ++img_count;
        }
        resize(frame, frame, Size(1500, 800), 0, 0);
        manage_frame_display();
        imshow("Scanning Face...", frame);
    }
    destroyAllWindows();
    CSV_File.close();
    if (img_count <= 145) return "Incomplete Scanning! You have quited from scanning process.";
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
        string strname = format("%s %s (%d).jpg", "F:\\PROJECT\\ConsoleApplication1\\SecureSenseFiles/images/", roll_no, img_num);
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
    putText(frame, instructions, Point(10, 70), FONT_HERSHEY_SIMPLEX, 0.65, CV_RGB(0, 153, 153));
    putText(frame, frame_text, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.75, CV_RGB(0, 255, 0), 2);
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
    string* mark;//tot
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
    mmm = 1;
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
    void display_Face_Information(FRAME& f, const Students& student) const {
        if (label == student.getFaceID())
            putText(f.original, student.getName(), Point(pos_x + 10, pos_y - 5), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 153, 153), 1);
        else putText(f.original, "Unknown", Point(pos_x + 10, pos_y - 5), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(255, 0, 0), 1);
    }
    friend class FRAME;
    friend class FACERECOGNIZER;
};

class FACERECOGNIZER {
    const string sample_image =/* "SecureSenseFiles/images/ (1).jpg";*/ "F:\\PROJECT\\ConsoleApplication1/SecureSenseFiles/images/(1).jpg";
    const string window = "MRKING ATTENDENCE";
    string errormsg = "";//F:\PROJECT\ConsoleApplication1\SecureSenseFiles\images
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
        if (!face_cascade.load(face_detector)) return "ERROR! Unable to load detection file.";
        model = LBPHFaceRecognizer::create();
        const string filename = "F:\\PROJECT\\ConsoleApplication1\\SecureSenseFiles/private/" + section + "_trainer.xml";
        model->read(filename);
        testSample = imread(sample_image, 0);
        img_width = testSample.cols;
        img_height = testSample.rows;
        cap.open(0);
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
                resize(frameobj.original, frameobj.original, Size(1500, 800), 0, 0);
                imshow(window, frameobj.original);
            }
            else return "End of video stream.";
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
    if (f.label != -1 && f.confidence<120) {
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

int main() {
    cout << AttendanceWithFaceRecognition();// generate_Trainer("1E");  //CaptureFace("52XCRYPTOO", "555", "1E");
    return 0;
}