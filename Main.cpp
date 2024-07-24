#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/sizer.h>
#include<funtions.hpp>
#include <wx/datectrl.h>

const wxString title = "Secure-Sens";
wxIcon icon;
wxIcon appIcon;

class SetupFrame :public wxFrame {
public:
    SetupFrame(const wxString& title);
private:
    void OnOptionSelected(wxCommandEvent& event);
    void Onconfirmclicked(wxCommandEvent& event);
    void ShowControl(int selection); 
    void OnBrowse(wxCommandEvent& event);
    wxBoxSizer* vbox;
    wxTextCtrl* m_dirPathCtrl;
    wxString dirpath = "";
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
    wxPanel* panel; 
    wxGridSizer* grid;
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
    void onchangesectionbutton(wxCommandEvent& event);
    void onappointleavebutton(wxCommandEvent& event);
    void onclosebutton(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};

class inputdetailsframe :public wxFrame{
public:
    inputdetailsframe(const wxString& title);
private:
    void OnSubmitClicked(wxCommandEvent& event);
    wxGauge* gauge;
    wxTimer timer;
    wxBoxSizer* vbox;
    wxTextCtrl* namectrl;
    wxTextCtrl* rnoctrl;
    wxTextCtrl* secctrl;
    wxDECLARE_EVENT_TABLE();
};

class LeaveFrame : public wxFrame {
public:
    LeaveFrame(const wxString& title);
private:
    void OnIDSelected(wxCommandEvent& event);
    void OnLeaveButton(wxCommandEvent& event);
    wxChoice* IDctrl;
    vector<string> IDS;
    int index;
    wxDatePickerCtrl* datePicker;
    wxDatePickerCtrl* datePicker2;
    wxDECLARE_EVENT_TABLE();
};

class ChangeSectionFrame:public wxFrame{
public:
    ChangeSectionFrame(const wxString& title);
private:
    void onChangeSecButtonClicked(wxCommandEvent& event);
    wxTextCtrl* Sectionctrl;
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
    button_submit =9,
    button_apoint_leave=10,
    button_change_sec=11,
    button_leave=12,
    button_sec=13,
    button_close=14,
    ID_choose=15
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
EVT_BUTTON(button_apoint_leave, mainframe::onappointleavebutton)
EVT_BUTTON(button_change_sec, mainframe::onchangesectionbutton)
EVT_BUTTON(button_close, mainframe::onclosebutton)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(inputdetailsframe, wxFrame)
EVT_BUTTON(button_submit, inputdetailsframe::OnSubmitClicked)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(LeaveFrame, wxFrame)
EVT_BUTTON(button_leave, LeaveFrame::OnLeaveButton)
EVT_CHOICE(ID_choose, LeaveFrame::OnIDSelected)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(ChangeSectionFrame, wxFrame)
EVT_BUTTON(button_sec,ChangeSectionFrame::onChangeSecButtonClicked)
wxEND_EVENT_TABLE()

class SecureSens : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(SecureSens);

bool SecureSens::OnInit() {
    if (isFirstAccount()) {
        SignupFrame* signupFrame = new SignupFrame(title);
        signupFrame->Show(true);
    }
    else if (!isFirstAccount()) {
        LoginFrame* loginFrame = new LoginFrame(title);
        loginFrame->Show(true);
    }
    return true;
}

LoginFrame::LoginFrame(const wxString& title): wxFrame(NULL, wxID_ANY, title, wxPoint(550,250), wxSize(500, 400)){
    wxPanel* panel = new wxPanel(this, -1);
    this->SetBackgroundColour(wxColour(0, 0, 0));
    panel->SetBackgroundColour(wxColour(60, 60, 60));
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    wxGridSizer* grid = new wxGridSizer(2,2,15,15);
    icon.LoadFile("favicon.ico", wxBITMAP_TYPE_ICO);
    appIcon.CopyFromBitmap(icon);
    SetIcon(appIcon);

    wxStaticText* placeholderlabel0 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel1 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel4 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel5 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel6 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel7 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel2 = new wxStaticText(panel, wxID_ANY, "Logging In", wxPoint(150, 30), wxSize(200, 40));
    wxStaticText* placeholderlabel3 = new wxStaticText(panel, wxID_ANY, "", wxPoint(150, 30), wxSize(200, 40));
    placeholderlabel2->SetFont(wxFont(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_SLANT, wxFONTWEIGHT_BOLD));
    placeholderlabel2->SetForegroundColour(wxColour(255, 255, 255));
    placeholderlabel2->SetBackgroundColour(wxColour(60, 60, 60));
    placeholderlabel3->SetFont(wxFont(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_SLANT, wxFONTWEIGHT_BOLD));
    placeholderlabel3->SetForegroundColour(wxColour(255, 255, 255));
    placeholderlabel3->SetBackgroundColour(wxColour(60, 60, 60));

    wxStaticText* u = new wxStaticText(panel, wxID_ANY, "Username:", wxDefaultPosition, wxSize(90, 20));
    u->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    u->SetForegroundColour(wxColour(255, 255, 255));
    u->SetBackgroundColour(wxColour(60, 60, 60));

    wxStaticText* p = new wxStaticText(panel, wxID_ANY, "Password:", wxDefaultPosition, wxSize(90, 20));
    p->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    p->SetForegroundColour(wxColour(255, 255, 255));
    p->SetBackgroundColour(wxColour(60, 60, 60));

    usernameCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 30));
    usernameCtrl->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    usernameCtrl->SetToolTip(wxT("Username"));
    usernameCtrl->SetForegroundColour(wxColour(200, 200, 200));
    usernameCtrl->SetBackgroundColour(wxColour(0, 0, 0));

    passwordCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 30), wxTE_PASSWORD);
    passwordCtrl->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    passwordCtrl->SetToolTip(wxT("Password"));
    passwordCtrl->SetForegroundColour(wxColour(200, 200, 200));
    passwordCtrl->SetBackgroundColour(wxColour(0, 0, 0));

    grid->Add(placeholderlabel0, 1, wxALIGN_RIGHT);
    grid->Add(placeholderlabel1, 1, wxALIGN_LEFT);
    grid->Add(placeholderlabel2, 1, wxALIGN_RIGHT | wxID_JUSTIFY_CENTER);
    grid->Add(placeholderlabel3, 1, wxALIGN_LEFT);
    grid->Add(placeholderlabel4, 1, wxALIGN_RIGHT);
    grid->Add(placeholderlabel5, 1, wxALIGN_LEFT);
    grid->Add(u, 1, wxALIGN_RIGHT);
    grid->Add(usernameCtrl, 1, wxSHRINK);
    grid->Add(p, 1, wxALIGN_RIGHT);
    grid->Add(passwordCtrl, 1, wxSHRINK);
    grid->Add(placeholderlabel6, 1, wxALIGN_RIGHT);
    grid->Add(placeholderlabel7, 1, wxALIGN_LEFT);

    wxButton* loginButton = new wxButton(panel, ID_Button_Login, "Login", wxDefaultPosition, wxSize(90, 30));
    loginButton->SetFont(wxFont(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    loginButton->SetToolTip(wxT("Login"));
    loginButton->SetBackgroundColour(wxColour(60, 60, 60));
    loginButton->SetForegroundColour(wxColour(60, 255, 0));

    vbox->Add(grid, 0, wxCENTER);
    vbox->Add(loginButton, 0, wxALIGN_CENTER | wxDOWN | wxALL);
    panel->SetSizer(vbox);
}

SignupFrame::SignupFrame(const wxString& title): wxFrame(NULL, wxID_ANY, title, wxPoint(550, 225), wxSize(500, 450)) {
    panel = new wxPanel(this, -1);
    this->SetBackgroundColour(wxColour(0, 0, 0));
    panel->SetBackgroundColour(wxColour(60, 60, 60));
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    grid = new wxGridSizer(2, 2, 15, 15);
    icon.LoadFile("favicon.ico", wxBITMAP_TYPE_ICO);
    appIcon.CopyFromBitmap(icon);
    SetIcon(appIcon);

    wxStaticText* placeholderlabel0 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel1 = new wxStaticText(panel, wxID_ANY, "",wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel2 = new wxStaticText(panel, wxID_ANY, "Signning Up",wxPoint(150,30), wxSize(200, 40));
    wxStaticText* placeholderlabel3 = new wxStaticText(panel, wxID_ANY, "", wxPoint(150, 30), wxSize(200, 40));
    wxStaticText* placeholderlabel4 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel5 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel6 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel7 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    placeholderlabel2->SetFont(wxFont(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_SLANT, wxFONTWEIGHT_BOLD));
    placeholderlabel2->SetForegroundColour(wxColour(255, 255, 255));
    placeholderlabel2->SetBackgroundColour(wxColour(60, 60, 60));
    placeholderlabel3->SetFont(wxFont(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_SLANT, wxFONTWEIGHT_BOLD));
    placeholderlabel3->SetForegroundColour(wxColour(255, 255, 255));
    placeholderlabel3->SetBackgroundColour(wxColour(60, 60, 60));


    wxStaticText*  u= new wxStaticText(panel, wxID_ANY, "Username:", wxDefaultPosition, wxSize(90,20));
    u->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    u->SetForegroundColour(wxColour(255, 255, 255));
    u->SetBackgroundColour(wxColour(60, 60, 60));

    wxStaticText* p= new wxStaticText(panel, wxID_ANY, "Password:", wxDefaultPosition, wxSize(90,20));
    p->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    p->SetForegroundColour(wxColour(255, 255, 255));
    p->SetBackgroundColour(wxColour(60, 60, 60));

    wxStaticText* rp= new wxStaticText(panel, wxID_ANY, "Confirm Password:", wxDefaultPosition, wxSize(150,20));
    rp->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    rp->SetForegroundColour(wxColour(255, 255, 255));
    rp->SetBackgroundColour(wxColour(60, 60, 60));

    usernameCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 30));
    usernameCtrl->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    usernameCtrl->SetToolTip(wxT("Type username"));
    usernameCtrl->SetForegroundColour(wxColour(200, 200, 200));
    usernameCtrl->SetBackgroundColour(wxColour(0, 0, 0));

    passwordCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 30), wxTE_PASSWORD);
    passwordCtrl->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    passwordCtrl->SetToolTip(wxT("Type password"));
    passwordCtrl->SetForegroundColour(wxColour(200, 200, 200));
    passwordCtrl->SetBackgroundColour(wxColour(0, 0, 0));
    
    repasswordCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 30),wxTE_PASSWORD);
    repasswordCtrl->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    repasswordCtrl->SetToolTip(wxT("Re-type password"));
    repasswordCtrl->SetForegroundColour(wxColour(200,200,200));
    repasswordCtrl->SetBackgroundColour(wxColour(0, 0, 0));
 
    grid->Add(placeholderlabel0, 1, wxALIGN_RIGHT);
    grid->Add(placeholderlabel1, 1, wxALIGN_LEFT);
    grid->Add(placeholderlabel2, 1, wxALIGN_RIGHT | wxID_JUSTIFY_CENTER);
    grid->Add(placeholderlabel3, 1, wxALIGN_LEFT);
    grid->Add(placeholderlabel4, 1, wxALIGN_RIGHT);
    grid->Add(placeholderlabel5, 1, wxALIGN_LEFT);
    grid->Add(u, 1, wxALIGN_RIGHT);
    grid->Add(usernameCtrl, 1,   wxSHRINK);
    grid->Add(p, 1, wxALIGN_RIGHT);
    grid->Add(passwordCtrl, 1, wxSHRINK);
    grid->Add(rp, 1, wxALIGN_RIGHT);
    grid->Add(repasswordCtrl, 1, wxSHRINK);
    grid->Add(placeholderlabel6, 1, wxALIGN_RIGHT);
    grid->Add(placeholderlabel7, 1, wxALIGN_LEFT);

    wxButton* signupButton = new wxButton(panel, ID_Button_Signup, "Sign Up", wxDefaultPosition, wxSize(90, 30));
    signupButton->SetFont(wxFont(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    signupButton->SetToolTip(wxT("Sign Up"));
    signupButton->SetBackgroundColour(wxColour(60, 60, 60));
    signupButton->SetForegroundColour(wxColour(60,255,0));
    
    vbox->Add(grid, 0, wxCENTER);
    vbox->Add(signupButton, 0, wxALIGN_CENTER |wxDOWN|wxALL);
    panel->SetSizer(vbox);
}

inputdetailsframe::inputdetailsframe(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxPoint(500, 225), wxSize(600, 450)) {
    wxPanel* panel = new wxPanel(this, -1);
    vbox = new wxBoxSizer(wxVERTICAL);
    this->SetBackgroundColour(wxColour(0, 0, 0));
    panel->SetBackgroundColour(wxColour(60, 60, 60));
    wxGridSizer* grid = new wxGridSizer(2, 2, 15, 15);
    appIcon.CopyFromBitmap(icon);
    SetIcon(appIcon);

    wxString sd,sec(getSection()), occupation(getOccupation());
    if (occupation == "Student") sd = "Section:";
    else if (occupation == "Employee") sd = "Department";

    wxStaticText* placeholderlabel0 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel1 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel2 = new wxStaticText(panel, wxID_ANY, occupation+" Details", wxPoint(150, 30), wxSize(250, 40));
    wxStaticText* placeholderlabel3 = new wxStaticText(panel, wxID_ANY, "", wxPoint(150, 30), wxSize(180, 40));
    wxStaticText* placeholderlabel4 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel5 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel6 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel7 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    placeholderlabel2->SetFont(wxFont(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_SLANT, wxFONTWEIGHT_BOLD));
    placeholderlabel2->SetForegroundColour(wxColour(255, 255, 255));
    placeholderlabel2->SetBackgroundColour(wxColour(60, 60, 60));
    placeholderlabel3->SetFont(wxFont(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_SLANT, wxFONTWEIGHT_BOLD));
    placeholderlabel3->SetForegroundColour(wxColour(255, 255, 255));
    placeholderlabel3->SetBackgroundColour(wxColour(60, 60, 60));


    wxStaticText* namelabel = new wxStaticText(panel, wxID_ANY, occupation + "\'s Full Name:", wxDefaultPosition, wxSize(180, 20));
    namelabel->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    namelabel->SetForegroundColour(wxColour(255, 255, 255));
    namelabel->SetBackgroundColour(wxColour(60, 60, 60));

    wxStaticText* idlabel = new wxStaticText(panel, wxID_ANY, occupation + "\'s ID:", wxDefaultPosition, wxSize(120, 20));
    idlabel->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    idlabel->SetForegroundColour(wxColour(255, 255, 255));
    idlabel->SetBackgroundColour(wxColour(60, 60, 60));

    wxStaticText* seclabel = new wxStaticText(panel, wxID_ANY, occupation + "\'s "+sd+" (optional):", wxDefaultPosition, wxSize(200, 20));
    seclabel->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    seclabel->SetForegroundColour(wxColour(255, 255, 255));
    seclabel->SetBackgroundColour(wxColour(60, 60, 60));

    namectrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxPoint(150, 100), wxSize(200, 30));
    namectrl->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    namectrl->SetToolTip(wxT("Name"));
    namectrl->SetForegroundColour(wxColour(200, 200, 200));
    namectrl->SetBackgroundColour(wxColour(0, 0, 0));

    rnoctrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxPoint(150, 200), wxSize(200, 30));
    rnoctrl->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    rnoctrl->SetToolTip(wxT("ID"));
    rnoctrl->SetForegroundColour(wxColour(200, 200, 200));
    rnoctrl->SetBackgroundColour(wxColour(0, 0, 0));

    secctrl = new wxTextCtrl(panel, wxID_ANY, sec, wxPoint(150, 300), wxSize(200, 30));
    secctrl->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    secctrl->SetToolTip(wxT("Section/Department name"));
    secctrl->SetForegroundColour(wxColour(200, 200, 200));
    secctrl->SetBackgroundColour(wxColour(0, 0, 0));

    grid->Add(placeholderlabel0, 1, wxALIGN_RIGHT);
    grid->Add(placeholderlabel1, 1, wxALIGN_LEFT);
    grid->Add(placeholderlabel2, 1, wxALIGN_RIGHT | wxID_JUSTIFY_CENTER);
    grid->Add(placeholderlabel3, 1, wxALIGN_LEFT);
    grid->Add(placeholderlabel4, 1, wxALIGN_RIGHT);
    grid->Add(placeholderlabel5, 1, wxALIGN_LEFT);
    grid->Add(namelabel, 1, wxALIGN_RIGHT);
    grid->Add(namectrl, 1, wxSHRINK);
    grid->Add(idlabel, 1, wxALIGN_RIGHT);
    grid->Add(rnoctrl, 1, wxSHRINK);
    grid->Add(seclabel, 1, wxALIGN_RIGHT);
    grid->Add(secctrl, 1, wxSHRINK);
    grid->Add(placeholderlabel6, 1, wxALIGN_RIGHT);
    grid->Add(placeholderlabel7, 1, wxALIGN_LEFT);

    wxButton* submitButton = new wxButton(panel, button_submit, "Submit", wxDefaultPosition, wxSize(90, 30));
    submitButton->SetFont(wxFont(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    submitButton->SetToolTip(wxT("Submit Details"));
    submitButton->SetBackgroundColour(wxColour(60, 60, 60));
    submitButton->SetForegroundColour(wxColour(60, 255, 0));

    vbox->Add(grid, 0, wxCENTER);
    vbox->Add(submitButton, 0, wxALIGN_CENTER | wxDOWN | wxALL);
    panel->SetSizer(vbox);
}

SetupFrame::SetupFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxPoint(500, 200), wxSize(600, 500)) {
    wxPanel* panel = new wxPanel(this, -1);
    this->SetBackgroundColour(wxColour(0, 0, 0));
    panel->SetBackgroundColour(wxColour(60, 60, 60));
    wxGridSizer* grid = new wxGridSizer(2, 2, 15, 15);
    vbox = new wxBoxSizer(wxVERTICAL);
    appIcon.CopyFromBitmap(icon);
    SetIcon(appIcon);

    wxStaticText* labell = new wxStaticText(panel, wxID_ANY, "Using for:", wxDefaultPosition, wxSize(120, 20));
    labell->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    labell->SetForegroundColour(wxColour(255, 255, 255));
    labell->SetBackgroundColour(wxColour(60, 60, 60));

    wxStaticText* DIRLABEL = new wxStaticText(panel, wxID_ANY, "Choose file directory:", wxDefaultPosition, wxSize(190, 20));
    DIRLABEL->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    DIRLABEL->SetForegroundColour(wxColour(255, 255, 255));
    DIRLABEL->SetBackgroundColour(wxColour(60, 60, 60));

    inputLabel = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(220, 20));
    inputLabel->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    inputLabel->SetForegroundColour(wxColour(255, 255, 255));
    inputLabel->SetBackgroundColour(wxColour(60, 60, 60));

    wxStaticText* placeholderlabel0 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel1 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel2 = new wxStaticText(panel, wxID_ANY, "Welcome", wxPoint(150, 30), wxSize(200, 40));
    wxStaticText* placeholderlabel3 = new wxStaticText(panel, wxID_ANY, "", wxPoint(150, 30), wxSize(200, 40));
    wxStaticText* placeholderlabel4 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel5 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel6 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel7 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel8 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    placeholderlabel2->SetFont(wxFont(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_SLANT, wxFONTWEIGHT_BOLD));
    placeholderlabel2->SetForegroundColour(wxColour(255, 255, 255));
    placeholderlabel2->SetBackgroundColour(wxColour(60, 60, 60));
    placeholderlabel3->SetFont(wxFont(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_SLANT, wxFONTWEIGHT_BOLD));
    
    wxArrayString choices;
    choices.Add("School");
    choices.Add("Office");
    choice = new wxChoice(panel, ID_Choice_Selected, wxDefaultPosition, wxDefaultSize, choices);
    choice->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    choice->SetToolTip(wxT("Using for"));
    choice->SetForegroundColour(wxColour(200, 200, 200));
    choice->SetBackgroundColour(wxColour(0, 0, 0));

    inputCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 30));
    inputCtrl->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    inputCtrl->SetToolTip(wxT("Section/Department"));
    inputCtrl->SetForegroundColour(wxColour(200, 200, 200));
    inputCtrl->SetBackgroundColour(wxColour(0, 0, 0));
    inputCtrl->Hide();

    m_dirPathCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(250, 25));
    m_dirPathCtrl->SetFont(wxFont(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    m_dirPathCtrl->SetToolTip(wxT("Choose Folder"));
    m_dirPathCtrl->SetForegroundColour(wxColour(200, 200, 200));
    m_dirPathCtrl->SetBackgroundColour(wxColour(0, 0, 0));
    
    grid->Add(placeholderlabel0, 1, wxALIGN_RIGHT);
    grid->Add(placeholderlabel1, 1, wxALIGN_LEFT);
    grid->Add(placeholderlabel2, 1, wxALIGN_RIGHT | wxID_JUSTIFY_CENTER);
    grid->Add(placeholderlabel3, 1, wxALIGN_LEFT);
    grid->Add(placeholderlabel4, 1, wxALIGN_RIGHT);
    grid->Add(placeholderlabel5, 1, wxALIGN_LEFT);
    grid->Add(labell, 1, wxALIGN_RIGHT);
    grid->Add(choice, 1, wxSHRINK);
    grid->Add(inputLabel, 1, wxALIGN_RIGHT);
    grid->Add(inputCtrl, 1, wxSHRINK);
    grid->Add(DIRLABEL, 1, wxLEFT);
    grid->Add(placeholderlabel6, 1, wxALIGN_LEFT);
    grid->Add(m_dirPathCtrl, 1,wxCENTER);

    wxButton* browseButton = new wxButton(panel, button_browse, "Browse", wxDefaultPosition, wxSize(100, 30));
    browseButton->SetFont(wxFont(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    browseButton->SetToolTip(wxT("Browse Folder"));
    browseButton->SetBackgroundColour(wxColour(60, 60, 60));
    browseButton->SetForegroundColour(wxColour(250, 250, 50));
    grid->Add(browseButton, 0, wxALIGN_LEFT);
    grid->Add(placeholderlabel7, 1, wxALIGN_RIGHT);
    grid->Add(placeholderlabel8, 1, wxALIGN_LEFT);

    wxButton* confirmButton = new wxButton(panel, button_confirm, "Confirm", wxDefaultPosition, wxSize(110, 30));
    confirmButton->SetFont(wxFont(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    confirmButton->SetToolTip(wxT("Confirm"));
    confirmButton->SetBackgroundColour(wxColour(60, 60, 60));
    confirmButton->SetForegroundColour(wxColour(60, 255, 0));
    vbox->Add(grid, 0, wxCENTER);
    vbox->Add(confirmButton, 0, wxALIGN_CENTER | wxDOWN | wxALL);
    panel->SetSizer(vbox);
}

mainframe::mainframe(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxPoint(600, 190), wxSize(400,520)) {
    wxPanel* panel = new wxPanel(this, -1);
    this->SetBackgroundColour(wxColour(0, 0, 0));
    panel->SetBackgroundColour(wxColour(60, 60, 60));
    wxGridSizer* grid = new wxGridSizer(2, 1, 15, 15);
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    appIcon.CopyFromBitmap(icon);
    SetIcon(appIcon);

    wxString sd, occupation(getOccupation());
    if (occupation == "Student") sd = "section";
    else if (occupation == "Employee") sd = "department name";

    wxStaticText* placeholderlabel0 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel1 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 5));
    wxStaticText* placeholderlabel4 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 5));
    wxStaticText* placeholderlabel5 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 5));
    wxStaticText* placeholderlabel2 = new wxStaticText(panel, wxID_ANY, "Secure-Sens", wxPoint(150, 30), wxSize(200, 40));
    wxStaticText* placeholderlabel3 = new wxStaticText(panel, wxID_ANY, "", wxPoint(150, 30), wxSize(200, 40));
    placeholderlabel2->SetFont(wxFont(22, wxFONTFAMILY_SWISS, wxFONTSTYLE_SLANT, wxFONTWEIGHT_BOLD));
    placeholderlabel2->SetForegroundColour(wxColour(255, 255, 255));
    placeholderlabel2->SetBackgroundColour(wxColour(60, 60, 60));
    placeholderlabel3->SetFont(wxFont(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_SLANT, wxFONTWEIGHT_BOLD));

    wxButton* start = new wxButton(panel, button_start, "Start", wxDefaultPosition, wxSize(150, 30));
    start->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    start->SetToolTip(wxT("Begin"));
    start->SetBackgroundColour(wxColour(60, 60, 60));
    start->SetForegroundColour(wxColour(60, 255, 0));

    wxButton* addButton = new wxButton(panel, button_addface, "Capture new face" ,wxDefaultPosition,wxSize(160,30));
    addButton->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    addButton->SetToolTip(wxT("Add face"));
    addButton->SetBackgroundColour(wxColour(60, 60, 60));
    addButton->SetForegroundColour(wxColour(60, 255, 0));

    wxButton* newacc = new wxButton(panel, button_newacc, "Create account", wxDefaultPosition, wxSize(150, 30));
    newacc->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    newacc->SetToolTip(wxT("New Account"));
    newacc->SetBackgroundColour(wxColour(60, 60, 60));
    newacc->SetForegroundColour(wxColour(60, 255, 0));

    wxButton* changesec = new wxButton(panel, button_change_sec, "Change "+sd, wxDefaultPosition, wxSize(230, 30));
    changesec->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    changesec->SetToolTip(wxT("Chane"));
    changesec->SetBackgroundColour(wxColour(60, 60, 60));
    changesec->SetForegroundColour(wxColour(60, 255, 0));

    wxButton* appointleave = new wxButton(panel, button_apoint_leave, "Appoint Leave", wxDefaultPosition, wxSize(150, 30));
    appointleave->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    appointleave->SetToolTip(wxT("Appoint Leave"));
    appointleave->SetBackgroundColour(wxColour(60, 60, 60));
    appointleave->SetForegroundColour(wxColour(60, 255, 0));

    wxButton* close = new wxButton(panel, button_close, "Close", wxDefaultPosition, wxSize(90, 30));
    close->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    close->SetToolTip(wxT("Close"));
    close->SetBackgroundColour(wxColour(60, 60, 60));
    close->SetForegroundColour(wxColour( 255,0, 0));
    
    vbox->Add(grid, 0, wxCENTER);
    grid->Add(placeholderlabel0, 1, wxALIGN_CENTER);
    grid->Add(placeholderlabel2, 1, wxALIGN_CENTER | wxID_JUSTIFY_CENTER);
    grid->Add(placeholderlabel3, 1, wxALIGN_CENTER);
    grid->Add(start, 0, wxALIGN_CENTER );
    grid->Add(placeholderlabel1, 0, wxALIGN_CENTER);
    grid->Add(addButton, 0, wxALIGN_CENTER);
    grid->Add(placeholderlabel4, 0, wxALIGN_CENTER);
    grid->Add(newacc, 0, wxALIGN_CENTER);
    grid->Add(placeholderlabel4, 0, wxALIGN_CENTER);
    grid->Add(changesec, 0, wxALIGN_CENTER);
    grid->Add(placeholderlabel4, 0, wxALIGN_CENTER);
    grid->Add(appointleave, 0, wxALIGN_CENTER );
    grid->Add(placeholderlabel4, 0, wxALIGN_CENTER);
    grid->Add(close, 0, wxALIGN_CENTER|wxALL);
    panel->SetSizer(vbox);
}

LeaveFrame::LeaveFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxPoint(550, 275), wxSize(500, 350)) {
    wxPanel* panel = new wxPanel(this, -1);
    this->SetBackgroundColour(wxColour(0, 0, 0));
    panel->SetBackgroundColour(wxColour(60, 60, 60));
    wxGridSizer* grid = new wxGridSizer(2, 2, 15, 15);
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    appIcon.CopyFromBitmap(icon);
    SetIcon(appIcon);

    wxString occupation(getOccupation());

    wxStaticText* idlabel = new wxStaticText(panel, wxID_ANY, occupation + "\'s ID:", wxDefaultPosition, wxSize(180, 20));
    idlabel->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    idlabel->SetForegroundColour(wxColour(255, 255, 255));
    idlabel->SetBackgroundColour(wxColour(60, 60, 60));

    wxStaticText* placeholderlabel0 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel1 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel4 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel5 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel2 = new wxStaticText(panel, wxID_ANY, "From:", wxPoint(150, 30), wxSize(40, 30));
    wxStaticText* placeholderlabel3 = new wxStaticText(panel, wxID_ANY, "To:", wxPoint(150, 30), wxSize(20, 30));
    placeholderlabel2->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    placeholderlabel2->SetForegroundColour(wxColour(255, 255, 255));
    placeholderlabel2->SetBackgroundColour(wxColour(60, 60, 60));
    placeholderlabel3->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    placeholderlabel3->SetForegroundColour(wxColour(255, 255, 255));
    placeholderlabel3->SetBackgroundColour(wxColour(60, 60, 60));
    
    wxArrayString ids;
    IDS = returnAllIDs();
    for (int i = 0; i < IDS.size(); i++) {
        wxString id(IDS[i]);
        ids.Add(id);
    }

    IDctrl = new wxChoice(panel, ID_choose, wxDefaultPosition, wxSize(120, 30),ids);
    IDctrl->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    IDctrl->SetToolTip(wxT("ID"));
    IDctrl->SetForegroundColour(wxColour(200, 200, 200));
    IDctrl->SetBackgroundColour(wxColour(0, 0, 0));
    
    datePicker = new wxDatePickerCtrl(panel, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT);
    datePicker->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    datePicker->SetToolTip(wxT("From"));
    datePicker->SetForegroundColour(wxColour(200, 200, 200));
    datePicker->SetBackgroundColour(wxColour(0, 0, 0));
    
    datePicker2 = new wxDatePickerCtrl(panel, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT);
    datePicker2->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    datePicker2->SetToolTip(wxT("To"));
    datePicker2->SetForegroundColour(wxColour(200, 200, 200));
    datePicker2->SetBackgroundColour(wxColour(0, 0, 0));

    grid->Add(placeholderlabel0, 1, wxALIGN_RIGHT);
    grid->Add(placeholderlabel1, 1, wxALIGN_LEFT);
    grid->Add(idlabel, 1, wxALIGN_RIGHT);
    grid->Add(IDctrl, 1, wxSHRINK); 
    grid->Add(placeholderlabel2, 1, wxALIGN_RIGHT);
    grid->Add(datePicker, 1, wxALIGN_LEFT);
    grid->Add(placeholderlabel3, 1, wxALIGN_RIGHT);
    grid->Add(datePicker2, 1, wxALIGN_LEFT);
    grid->Add(placeholderlabel4, 1, wxALIGN_RIGHT);
    grid->Add(placeholderlabel5, 1, wxALIGN_LEFT);
    
    wxButton* LeaveButton = new wxButton(panel, button_leave, "Mark Leave", wxDefaultPosition, wxSize(120, 30));
    LeaveButton->SetFont(wxFont(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    LeaveButton->SetToolTip(wxT("Mark Leave"));
    LeaveButton->SetBackgroundColour(wxColour(60, 60, 60));
    LeaveButton->SetForegroundColour(wxColour(60, 255, 0));
    vbox->Add(grid, 0, wxCENTER);
    vbox->Add(LeaveButton, 0, wxALIGN_CENTER | wxDOWN | wxALL);
    panel->SetSizer(vbox);
}

ChangeSectionFrame::ChangeSectionFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxPoint(550, 300), wxSize(500, 220)) {
    wxPanel* panel = new wxPanel(this, -1);
    this->SetBackgroundColour(wxColour(0, 0, 0));
    panel->SetBackgroundColour(wxColour(60, 60, 60));
    wxGridSizer* grid = new wxGridSizer(2, 2, 15, 15);
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    appIcon.CopyFromBitmap(icon);
    SetIcon(appIcon);

    wxString sd, occupation(getOccupation());
    if (occupation == "Student") sd = "Section";
    else if (occupation == "Employee") sd = "Department";

    wxStaticText* SectionLabel = new wxStaticText(panel, wxID_ANY, "New "+sd+" Name:", wxDefaultPosition, wxSize(150, 20));
    SectionLabel->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    SectionLabel->SetForegroundColour(wxColour(255, 255, 255));
    SectionLabel->SetBackgroundColour(wxColour(60, 60, 60));
    
    wxStaticText* placeholderlabel = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel2 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel6 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    wxStaticText* placeholderlabel7 = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(60, 30));
    
    Sectionctrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxPoint(150, 100), wxSize(200, 30));
    Sectionctrl->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
    Sectionctrl->SetToolTip(wxT("New name"));
    Sectionctrl->SetForegroundColour(wxColour(200, 200, 200));
    Sectionctrl->SetBackgroundColour(wxColour(0, 0, 0));
    
    grid->Add(placeholderlabel, 1, wxALIGN_RIGHT);
    grid->Add(placeholderlabel2, 1, wxALIGN_LEFT);
    grid->Add(SectionLabel, 1, wxALIGN_RIGHT);
    grid->Add(Sectionctrl, 1, wxSHRINK);
    grid->Add(placeholderlabel6, 1, wxALIGN_RIGHT);
    grid->Add(placeholderlabel7, 1, wxALIGN_LEFT);

    wxButton* ChangeSectionButton = new wxButton(panel, button_sec, "Change "+sd,wxDefaultPosition, wxSize(150, 30));
    ChangeSectionButton->SetFont(wxFont(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    ChangeSectionButton->SetToolTip(wxT("Change"));
    ChangeSectionButton->SetBackgroundColour(wxColour(60, 60, 60));
    ChangeSectionButton->SetForegroundColour(wxColour(60, 255, 0));

    vbox->Add(grid, 0, wxCENTER);
    vbox->Add(ChangeSectionButton, 0, wxALIGN_CENTER | wxDOWN | wxALL);
    panel->SetSizer(vbox);
}

void inputdetailsframe::OnSubmitClicked(wxCommandEvent& event) {
    wxString name = namectrl->GetValue(), id = rnoctrl->GetValue(), sec = secctrl->GetValue(), sec2 = secctrl->GetValue();
    for (int i = 0; i < name.size(); i++) if (!((name[i] >= 65 && name[i] <= 90) || (name[i] >= 97 && name[i] <= 122) || (name[i] = ' '))) wxMessageBox("Please type fullname properly.", "Stop", wxOK | wxICON_STOP);
    if (name.size()<3) wxMessageBox("Please type fullname.", "Stop", wxOK | wxICON_STOP);
    else if (id == "") wxMessageBox("Please type ID.", "Stop", wxOK | wxICON_STOP);
    else {
        wxMessageDialog dialog(this, "Are the details provided above appropriate?", "Ask", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
        if (dialog.ShowModal() == wxID_YES) {
            wxString error(CaptureFace(name.ToStdString(), id.ToStdString(), sec.ToStdString()));
            if (error != "") wxMessageBox(error, "Error", wxOK | wxICON_STOP);
            else {
                wxMessageDialog dialog2(this, "Scanning completed. Do you want to scan another face?", "Ask", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
                if (dialog2.ShowModal() == wxID_YES) {
                    wxString s;
                    string o = getOccupation();
                    if (o == "Employee") s = "Department";
                    else s = "Section";
                    wxMessageBox("Please make sure the " + s + " name must be same in order to avoid mismanagement.", "Scanning Multiple Faces", wxOK | wxICON_WARNING);
                }
                else if (dialog2.ShowModal() == wxID_NO) {
                    wxString error2(generate_Trainer(sec2.ToStdString()));
                    if (error2 != "") wxMessageBox(error2, "Error", wxOK | wxICON_STOP);
                    else wxMessageBox("Ready to perform face recognition.", "Done", wxOK);
                    mainframe* Frame = new mainframe(title);
                    Frame->Show(true);
                    this->Close(true);
                }
                else;
            } 
        }
    }
}
    
void SetupFrame::Onconfirmclicked(wxCommandEvent& event) {
   wxString sec = inputCtrl->GetValue();
    int option = choice->GetSelection();
    if(option==wxNOT_FOUND) wxMessageBox("Please choose the current place (school or office).", "Stop", wxOK | wxICON_STOP);
    else if(dirpath =="") wxMessageBox("Please select the directory.", "Stop", wxOK | wxICON_STOP);
    else if(dirpath != "" && option != wxNOT_FOUND) {
        CreateFiles(dirpath.ToStdString(),option,sec.ToStdString());
        mainframe* securesens = new mainframe(title);
        securesens->Show(true);
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
    else label = "Department (optional):";
    inputLabel->SetLabel(label);
    inputLabel->Show();
    inputCtrl->Show();
    vbox->Layout();
}

void LoginFrame::OnLoginButtonClicked(wxCommandEvent& event){
    wxString username = usernameCtrl->GetValue();
    wxString password = passwordCtrl->GetValue();
    int a = login(username.ToStdString(), password.ToStdString());
    switch (a){
        case 0: {
            wxMessageBox("Incorrect username or password.", "Error", wxOK | wxICON_STOP);
            break;
        }
        case 1: {
            mainframe* securesens = new mainframe(title);
            securesens->Show(true);
            this->Close(true); // Close the login window
            break;
        }
        default: {
            wxMessageBox("No accounts created before. Creating new account.", "Stop", wxOK | wxICON_STOP);
            SignupFrame* signupFrame = new SignupFrame(title);
            signupFrame->Show(true);
            this->Close(true); // Close the login window
            break;
        }
    }
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
            SetupFrame* setupframe = new SetupFrame(title);
            setupframe->Show();
            this->Close(true);
        }
        else {
            wxMessageBox("Successfully created new account.", "Done", wxOK);
            mainframe* Frame = new mainframe(title);
            Frame->Show(true);
            this->Close(true);
        }
    }
    else wxMessageBox("System is unable to save your account information. File containing login data may be deleted, misplaced, or renamed.", "Error", wxOK | wxICON_STOP);
}

void mainframe::onaddbutton(wxCommandEvent& event) {
    wxString occupation(getOccupation());
    inputdetailsframe* inputdetails = new inputdetailsframe(title);
    inputdetails->Show();
    this->Close(true);
}
void mainframe::onstartbutton(wxCommandEvent& event) {
    this->Close();
    wxString error(AttendanceWithFaceRecognition());
    if(error!="") wxMessageBox(error, "Error", wxOK | wxICON_STOP);
    else wxMessageBox("The attendance has been successfully marked and updated in the file.", "Attendance Completed", wxOK);
}
void mainframe::onnewaccbutton(wxCommandEvent& event) {
    SignupFrame* signupFrame = new SignupFrame(title);
    signupFrame->Show(true);
    this->Close(true);
}
void mainframe::onappointleavebutton(wxCommandEvent& event) {
    LeaveFrame* leaveframe = new LeaveFrame(title);
    leaveframe->Show();
    this->Close(true);
}
void mainframe::onchangesectionbutton(wxCommandEvent& event) {
    SetupFrame* changesection = new SetupFrame(title);
    changesection->Show();
    this->Close();
}
void mainframe::onclosebutton(wxCommandEvent& event) { this->Close(); }

void LeaveFrame::OnIDSelected(wxCommandEvent& event) {
    index = event.GetSelection();
}

void LeaveFrame::OnLeaveButton(wxCommandEvent& event) {
    wxDateTime from=datePicker->GetValue();
    wxDateTime to = datePicker2->GetValue();
    int yf = from.GetYear();
    int yt = to.GetYear();
    int mf = from.GetMonth();
    int mt = to.GetMonth();
    int df = from.GetDay();
    int dt = to.GetDay();
    if(setLeave(IDS[index], yf, yt, mf, mt, df, dt)) wxMessageBox("Leave marked successfully.", "Leave Marked", wxOK);
    else wxMessageBox("Unable to mark leave.", "Error", wxOK | wxICON_STOP);
    this->Close();
    mainframe* frame = new mainframe(title);
    frame->Show();
}

void ChangeSectionFrame::onChangeSecButtonClicked(wxCommandEvent& event) {
    wxString section=Sectionctrl->GetValue();
    wxString sd, occupation(getOccupation());
    if (occupation == "Student") sd = "Section";
    else if (occupation == "Employee") sd = "Department name";
    if(ChangeSection(section.ToStdString())) wxMessageBox(sd+" has been changed successfully.", "Successfully Changed!", wxOK);
    else wxMessageBox("Unable to change "+sd+".", "Error", wxOK | wxICON_STOP);
    this->Close();
}