#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/sizer.h>
#include<funtions.hpp>

class SetupFrame :public wxFrame {
public:
    SetupFrame(const wxString& title);
private:
    void OnOptionSelected(wxCommandEvent& event);
    void Onconfirmclicked(wxCommandEvent& event);
    void ShowControl(int selection); 
    void OnBrowse(wxCommandEvent& event);
    wxTextCtrl* m_dirPathCtrl;
    wxString dirpath = "";
    wxBoxSizer* vbox;
    wxChoice* choice;
    wxTextCtrl* inputCtrl;
    wxStaticText* inputLabel;
    wxDECLARE_EVENT_TABLE();
};

class LoginFrame : public wxFrame{
public:
    LoginFrame(const wxString& title);

private:
    void OnLoginButtonClicked(wxCommandEvent& event);
    wxTextCtrl* usernameCtrl;
    wxTextCtrl* passwordCtrl;
    wxDECLARE_EVENT_TABLE();
};

class SignupFrame : public wxFrame {
public:
    SignupFrame(const wxString& title);
private:
    void OnSignupButtonClicked(wxCommandEvent& event);
    wxTextCtrl* usernameCtrl;
    wxTextCtrl* passwordCtrl;
    wxTextCtrl* repasswordCtrl;
    wxDECLARE_EVENT_TABLE();
};

class mainframe : public wxFrame {
public:
    mainframe(const wxString& title);
private:
    void onaddbutton(wxCommandEvent& event);
    void onstartbutton(wxCommandEvent& event);
    void onnewaccbutton(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};

class inputdetailsframe :public wxFrame{
public:
    inputdetailsframe(const wxString& title);
private:
    void OnSubmitClicked(wxCommandEvent& event);
    wxTextCtrl* namectrl;
    wxTextCtrl* rnoctrl;
    wxTextCtrl* secctrl;
    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_Button_Login =1,
    ID_Button_Signup = 2,
    button_addface=3,
    button_start=4,
    button_newacc=5,
    ID_Choice_Selected=6,
    button_browse=7,
    button_confirm=8,
    button_submit =9
};

wxBEGIN_EVENT_TABLE(SetupFrame, wxFrame)
EVT_BUTTON(button_browse, SetupFrame::OnBrowse)
EVT_BUTTON(button_confirm, SetupFrame::Onconfirmclicked)
EVT_CHOICE(ID_Choice_Selected, SetupFrame::OnOptionSelected)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(LoginFrame, wxFrame)
EVT_BUTTON(ID_Button_Login, LoginFrame::OnLoginButtonClicked)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(SignupFrame, wxFrame)
EVT_BUTTON(ID_Button_Signup, SignupFrame::OnSignupButtonClicked)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(mainframe,wxFrame)
EVT_BUTTON(button_addface,mainframe::onaddbutton)
EVT_BUTTON(button_start, mainframe::onstartbutton)
EVT_BUTTON(button_newacc, mainframe::onnewaccbutton)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(inputdetailsframe, wxFrame)
EVT_BUTTON(button_submit, inputdetailsframe::OnSubmitClicked)
wxEND_EVENT_TABLE()


class SecureSens : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(SecureSens);

bool SecureSens::OnInit() {
    if (isFirstAccount()) {
        SignupFrame* signupFrame = new SignupFrame("Sign Up");
        signupFrame->Show(true);
    }
    else if (!isFirstAccount()) {
        LoginFrame* loginFrame = new LoginFrame("Admin Login");
        loginFrame->Show(true);
    }
    return true;
}

LoginFrame::LoginFrame(const wxString& title): wxFrame(NULL, wxID_ANY, title, wxPoint(550,300), wxSize(500, 250)){
    wxPanel* panel = new wxPanel(this, -1);
    wxBoxSizer* vbox = new wxBoxSizer(wxBOTH);
    wxGridSizer* grid = new wxGridSizer(2,2,15,5);
    wxStaticText* usernameLabel = new wxStaticText(panel, wxID_ANY, "Username:",wxDefaultPosition, wxSize(60, 15));
    wxStaticText* passwordLabel = new wxStaticText(panel, wxID_ANY, "Password:", wxDefaultPosition, wxSize(60,15));
    usernameCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,wxDefaultPosition, wxSize(120,20));
    passwordCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(120, 20), wxTE_PASSWORD);
    grid->Add(usernameLabel, 0, wxALIGN_RIGHT);
    grid->Add(usernameCtrl, 1, wxSHRINK);
    grid->Add(passwordLabel, 0, wxALIGN_RIGHT);
    grid->Add(passwordCtrl, 1, wxSHRINK);
    wxButton* loginButton = new wxButton(panel, ID_Button_Login, "Login",wxDefaultPosition, wxSize(50, 25));
    vbox->Add(grid, 1, wxALL | wxSHAPED, 10);
    vbox->Add(loginButton, 0, wxALIGN_CENTER | wxALL);
    panel->SetSizer(vbox);
}

SignupFrame::SignupFrame(const wxString& title): wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 500)){
    wxPanel* panel = new wxPanel(this, -1);
    wxBoxSizer* vbox = new wxBoxSizer(wxBOTH);;
    wxGridSizer* grid = new wxGridSizer(2, 2, 15, 5);
    wxStaticText* usernameLabel = new wxStaticText(panel, wxID_ANY, "Username:", wxDefaultPosition, wxSize(60, 25));
    wxStaticText* passwordLabel = new wxStaticText(panel, wxID_ANY, "Password:", wxDefaultPosition, wxSize(60, 25));
    wxStaticText* repasswordLabel = new wxStaticText(panel, wxID_ANY, "Confirm Password:");
    usernameCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxPoint(150,100), wxSize(120, 20));
    passwordCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(150,200), wxSize(120, 20), wxTE_PASSWORD);
    repasswordCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(150,300), wxSize(120, 20), wxTE_PASSWORD);
    grid->Add(usernameLabel, 0, wxALIGN_RIGHT);
    grid->Add(usernameCtrl, 1, wxSHRINK);
    grid->Add(passwordLabel, 0, wxALIGN_RIGHT);
    grid->Add(passwordCtrl, 1, wxSHRINK);
    grid->Add(repasswordLabel, 0, wxALIGN_RIGHT);
    grid->Add(repasswordCtrl, 1, wxSHRINK);
    wxButton* signupButton = new wxButton(panel, ID_Button_Signup, "Sign Up", wxPoint(170,400), wxSize(60, 30));
    vbox->Add(grid, 1, wxALL | wxSHAPED, 10);
    vbox->Add(signupButton, 0, wxALIGN_CENTER | wxALL);
    panel->SetSizer(vbox);
}

mainframe::mainframe(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(460, 360)) {
    wxPanel* panel = new wxPanel(this, -1);
    wxBoxSizer* vbox = new wxBoxSizer(wxBOTH);
    wxGridSizer* grid = new wxGridSizer(2, 2, 15, 5);
    wxButton* addButton = new wxButton(panel, button_addface, "Capture new faces", wxPoint(200, 150));
    vbox->Add(grid, 1, wxALL | wxSHAPED, 10);
    vbox->Add(button_addface, 1, wxALIGN_CENTER | wxALL);
    wxButton* start = new wxButton(panel, button_start, "Start", wxPoint(200, 180));
    vbox->Add(grid, 1, wxALL | wxSHAPED, 10);
    vbox->Add(button_start, 2, wxALIGN_CENTER | wxALL);
    wxButton* newacc = new wxButton(panel, button_newacc, "Create new admin account", wxPoint(200, 210));
    vbox->Add(grid, 1, wxALL | wxSHAPED, 10);
    vbox->Add(button_newacc, 3, wxALIGN_CENTER | wxALL);
}

SetupFrame::SetupFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(450, 400)) {
    wxPanel* panel = new wxPanel(this, -1); 
    vbox = new wxBoxSizer(wxBOTH);
    wxBoxSizer* vmbox = new wxBoxSizer(wxVERTICAL);
    wxGridSizer* grid = new wxGridSizer(2, 2, 15, 5);
    wxStaticText* labell = new wxStaticText(panel, wxID_ANY, "Using for:");
    inputLabel = new wxStaticText(panel, wxID_ANY,"");
    wxArrayString choices;
    choices.Add("School");
    choices.Add("Office");
    choice = new wxChoice(panel, ID_Choice_Selected, wxDefaultPosition, wxDefaultSize, choices);
    inputCtrl = new wxTextCtrl(panel, wxID_ANY);
    inputCtrl->Hide();
    m_dirPathCtrl = new wxTextCtrl(panel, wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(200,20));
    grid->Add(labell, 0, wxALIGN_RIGHT);
    grid->Add(choice, 1, wxSHRINK);
    grid->Add(inputLabel, 0, wxALIGN_RIGHT);
    grid->Add(inputCtrl, 1, wxSHRINK);
    grid->Add(m_dirPathCtrl, 0, wxEXPAND, 10);
    wxButton* browseButton = new wxButton(panel, button_browse, "Browse", wxDefaultPosition);
    wxButton* confirmButton = new wxButton(panel, button_confirm, "Confirm");
    vbox->Add(grid, 1, wxALL | wxSHAPED, 10);
    vbox->Add(browseButton, 0, wxALIGN_LEFT | wxALL, 10);
    vbox->Add(confirmButton, 0, wxALIGN_CENTER, 10);
    panel->SetSizer(vbox);
}

inputdetailsframe::inputdetailsframe(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 500)) {
    wxPanel* panel = new wxPanel(this, -1);
    wxBoxSizer* vbox = new wxBoxSizer(wxBOTH);;
    wxGridSizer* grid = new wxGridSizer(2, 2, 15, 5);
    wxStaticText* namelabel = new wxStaticText(panel, wxID_ANY, getOccupation() + "\'s Full name: ", wxDefaultPosition, wxSize(60, 25));
    wxStaticText* idlabel = new wxStaticText(panel, wxID_ANY, getOccupation() + "\'s ID: ", wxDefaultPosition, wxSize(60, 25));
    wxStaticText* seclabel = new wxStaticText(panel, wxID_ANY, getOccupation() + "\'s Section (optional): ", wxDefaultPosition, wxSize(60, 25));
    namectrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxPoint(150, 100), wxSize(120, 20));
    rnoctrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxPoint(150, 100), wxSize(120, 20));
    secctrl = new wxTextCtrl(panel, wxID_ANY, getSection(), wxPoint(150, 100), wxSize(120, 20));
    grid->Add(namelabel, 0, wxALIGN_RIGHT);
    grid->Add(namectrl, 1, wxSHRINK);
    grid->Add(idlabel, 0, wxALIGN_RIGHT);
    grid->Add(rnoctrl, 1, wxSHRINK);
    grid->Add(seclabel, 0, wxALIGN_RIGHT);
    grid->Add(secctrl, 1, wxSHRINK);
    wxButton* submitButton = new wxButton(panel, button_submit, "Submit", wxPoint(170, 400), wxSize(60, 30));
    vbox->Add(grid, 1, wxALL | wxSHAPED, 10);
    vbox->Add(submitButton, 0, wxALIGN_LEFT | wxALL, 10);
}

void inputdetailsframe::OnSubmitClicked(wxCommandEvent& event) {
    wxString name = namectrl->GetValue();
    wxString id = rnoctrl->GetValue();
    wxString sec = secctrl->GetValue();
    for (int i = 0; i < name.size(); i++) if (!((name[i] >= 65 && name[i] <= 90) || (name[i] >= 97 && name[i] <= 122) || (name[i] = ' '))) wxMessageBox("Please type fullname properly.", "Stop", wxOK | wxICON_STOP);
    if (name.size()<3) wxMessageBox("Please type fullname properly.", "Stop", wxOK | wxICON_STOP);
    else if (id == "") wxMessageBox("Please type ID.", "Stop", wxOK | wxICON_STOP);
    else {
        wxMessageDialog dialog(this, "Are the details provided above appropriate?", "Confirmation", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
        if (dialog.ShowModal() == wxID_YES) {
            this->Close();
        }
    }

    else wxMessageBox("", "Submit", wxOK);
}

void SetupFrame::Onconfirmclicked(wxCommandEvent& event) {
    wxString sec = inputCtrl->GetValue();
    dirpath = m_dirPathCtrl->GetValue();
    int option = choice->GetSelection();
    if(option==wxNOT_FOUND) wxMessageBox("Please select the purpose of using this application.", "Stop", wxOK | wxICON_STOP);
    else if(dirpath =="") wxMessageBox("Please select the directory.", "Stop", wxOK | wxICON_STOP);
    else if(dirpath != "" && option != wxNOT_FOUND) {
        wxMessageBox("Thank you so much for providing all the details. Now, login to begin.", "Let's begin", wxOK);
        LoginFrame* loginFrame = new LoginFrame("Admin Login");
        loginFrame->Show(true);
        setfilenames(dirpath.ToStdString(), option, sec.ToStdString());
        this->Close(true);
    }
}

void SetupFrame::OnBrowse(wxCommandEvent& event) {
    wxDirDialog dirDialog(this, "Choose a directory", "Choosing Directory", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
    if (dirDialog.ShowModal() == wxID_OK) {
        dirpath = dirDialog.GetPath();
        m_dirPathCtrl->SetValue(dirpath);
    }
}

void SetupFrame::OnOptionSelected(wxCommandEvent& event) {
    int selection = event.GetSelection();
    ShowControl(selection);
}

void SetupFrame::ShowControl(int selection) {
    wxString label;
    if (selection == 0) label = "Section (optional):";
    else label = "Department (optional): ";
    inputLabel->SetLabel(label);
    inputLabel->Show();
    inputCtrl->Show();
    vbox->Layout();
    wxString sec = inputCtrl->GetValue();
}

void LoginFrame::OnLoginButtonClicked(wxCommandEvent& event){
    wxString username = usernameCtrl->GetValue();
    wxString password = passwordCtrl->GetValue();
    // Re-layout the panel to accommodate the error message
//    GetSizer()->Layout();
    int a = login(username.ToStdString(), password.ToStdString());
    switch (a){
    case 0: {
        wxMessageBox("Incorrect username or password.", "Error", wxOK | wxICON_STOP);
        break;
    }
    case 1: {
        mainframe* securesens = new mainframe("Secure-Sens");
        securesens->Show(true);
        this->Close(true); // Close the login window
        break;
    }
    default: {
        wxMessageBox("No accounts created before. Creating new account.", "Stop", wxOK | wxICON_STOP);
        SignupFrame* signupFrame = new SignupFrame("Sign Up");
        signupFrame->Show(true);
        this->Close(true); // Close the login window
        break;
    }}
}

static int checkinvalid(string s) {
    for (int i = 0; i < s.size(); i++) if (s[0] == ' ') return 0;
    char *d = &s[0];
    if (*d >= 48 && *d <= 57) return -1;
    return 1;
}

void SignupFrame::OnSignupButtonClicked(wxCommandEvent& event){
    bool check = false;
    if (isFirstAccount()) check = true;
    wxString username = usernameCtrl->GetValue();
    wxString password = passwordCtrl->GetValue();
    wxString repassword = repasswordCtrl->GetValue(); //    GetSizer()->Layout(); // Re-layout the panel to accommodate the error message
    if (username == "") wxMessageBox("Please type your username.", "Stop", wxOK | wxICON_STOP);
    else if (checkinvalid(username.ToStdString()) == -1) wxMessageBox("Username must not start with a number.", "Invalid username", wxOK | wxICON_STOP);
    else if (username.size() < 3) wxMessageBox("Your username must contain at least 3 characters.", "Short username", wxOK | wxICON_STOP);
    else if (checkinvalid(username.ToStdString()) == 0) wxMessageBox("Spaces are not allowed in username.", "Invalid username", wxOK | wxICON_STOP);
    else if (checkUsername(username.ToStdString()) == 1) wxMessageBox("Username \'" + username + "\' already exists. Please try the new one.", "Stop", wxOK | wxICON_STOP);
    else if (password == "") wxMessageBox("Please type your password.", "Stop", wxOK | wxICON_STOP);
    else if (password.size() < 8) wxMessageBox("Your password must contain at least 8 characters.", "Short password", wxOK | wxICON_STOP);
    else if (repassword == "") wxMessageBox("Please confirm your password.", "Stop", wxOK | wxICON_STOP);
    else if (password != repassword) wxMessageBox("Password does not match.", "Password mismatched", wxOK | wxICON_STOP);
    else if (signup(username.ToStdString(), password.ToStdString())) {
        if (check) {
            SetupFrame* setupframe = new SetupFrame("Setting up things");
            setupframe->Show();
            this->Close(true);
        }
        else {
            wxMessageBox("Your admin account has been created. Now, log-in again to proceed.", "Sign-up Successful", wxOK);
            LoginFrame* loginFrame = new LoginFrame("Admin Login");
            loginFrame->Show(true);
            this->Close(true);
        }
    }
    else if (!signup(username.ToStdString(), password.ToStdString())) wxMessageBox("System is unable to save your account information. File containing login data may be deleted, misplaced, or renamed.", "Error", wxOK | wxICON_STOP);
}

void mainframe::onaddbutton(wxCommandEvent& event) {

}
void mainframe::onstartbutton(wxCommandEvent& event) {

}
void mainframe::onnewaccbutton(wxCommandEvent& event) {
    SignupFrame* signupFrame = new SignupFrame("Sign Up");
    signupFrame->Show(true);
    this->Close(true);
}

/*#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);

private:
    void OnOptionSelected(wxCommandEvent& event);
    void ShowControl(int selection);

    wxChoice* choice;
    wxStaticText* inputLabel;
    wxStaticText* labell;
    wxTextCtrl* inputCtrl;
    wxBoxSizer* vbox;

    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_Choice_Selected = 1
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_CHOICE(ID_Choice_Selected, MainFrame::OnOptionSelected)
wxEND_EVENT_TABLE()

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    MainFrame* frame = new MainFrame("Purpose");
    frame->Show(true);
    return true;
}

MainFrame::MainFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300)) {
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    labell = new wxStaticText(panel, wxID_ANY, "Purpose:", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    labell->Show();
    wxArrayString choices;
    choices.Add("For school");
    choices.Add("For office use");
    choice = new wxChoice(panel, ID_Choice_Selected, wxDefaultPosition, wxDefaultSize, choices);

    inputLabel = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    inputLabel->Hide();
    inputCtrl = new wxTextCtrl(panel, wxID_ANY);
    inputCtrl->Hide();

    vbox = new wxBoxSizer(wxVERTICAL);
    vbox->Add(labell, 0,wxALL | wxLEFT, 5);
    vbox->Add(choice, 0, wxALL | wxCENTER, 15);
    vbox->Add(inputLabel, 0, wxALL | wxEXPAND, 5);
    vbox->Add(inputCtrl, 0, wxALL | wxEXPAND, 5);

    panel->SetSizer(vbox);
}

void MainFrame::OnOptionSelected(wxCommandEvent& event) {
    int selection = event.GetSelection();
    ShowControl(selection);
}

void MainFrame::ShowControl(int selection) {
    wxString label;
    if (selection == 0) {
        label = "Type Section:";
    }
    else {
        label = "Type Department name: (optional)";
    }

    inputLabel->SetLabel(label);
    inputLabel->Show();
    inputCtrl->Show();
    wxString sec= inputCtrl->GetValue();
    vbox->Layout();
}*/


/*void LoginFrame::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);

    if (backgroundBitmap.IsOk()) // Draw the background image
    {
        dc.DrawBitmap(backgroundBitmap, 0, 0, false);
    }
}


////////////////////////////// login singnup buttons
/*#include <wx/wx.h>
#include <sstream>

class LoginFrame : public wxFrame
{
public:
    LoginFrame(const wxString& title);

private:
    void OnButtonClicked(wxCommandEvent& event);
    bool operation;
    void PerformOperation();
    wxDECLARE_EVENT_TABLE();
};

enum{
    ID_Button_SignUp =1,
    ID_Button_Login=2
};

wxBEGIN_EVENT_TABLE(LoginFrame, wxFrame)
EVT_BUTTON(ID_Button_SignUp, LoginFrame::OnButtonClicked)
EVT_BUTTON(ID_Button_Login, LoginFrame::OnButtonClicked)
wxEND_EVENT_TABLE()

class UserAuthentication : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(UserAuthentication);
bool UserAuthentication::OnInit(){
    LoginFrame* frame = new LoginFrame("Admin Login");
    frame->Show(true);
    return true;
}
LoginFrame::LoginFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1500, 800)){
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL); 
    wxGridSizer* grid = new wxGridSizer(1, 1, 1, 1);
    grid->Add(new wxButton(this, ID_Button_Login, "Login"), 0, wxALIGN_CENTER);
    grid->Add(new wxButton(this, ID_Button_SignUp, "Sign Up"), 0, wxEXPAND);
    vbox->Add(grid, 1, wxEXPAND);
    SetSizer(vbox);

}
void LoginFrame::OnButtonClicked(wxCommandEvent& event){
    int id = event.GetId();
    if (id == ID_Button_Login) {
        wxMessageBox("Username or password is incorrect", "Error", wxOK | ERROR_WX86_ERROR);
        operation = true;
    }
    else if (id == ID_Button_SignUp) operation = false;
}
void LoginFrame::PerformOperation()
{
    // operation tobe performed
}*/