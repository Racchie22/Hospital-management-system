#include<iostream>
#include<fstream>
#include<string>
#include<map>


using namespace std;

map<int, string> departmentNames = {
    {0, "No Department"},
    {1, "Cardiology"},
    {2, "Neurology"},
    {3, "Orthopedics"},
    {4, "Emergency"},
    //will Add more as needed
};


class person{
    protected:
    string name;
    int age ;
    char gender ;
    string address;
    string contact;

    public:
    
};

class patient : public person{
    protected : 
    string bloodgp;
    string patientid;
    int requiredep;
    int appointmentstatus=0; //0= not booked,1=pending,2 = confirmed ->this will be changed from staff menu

    public :
    string getID() { return patientid; }
    string getname() { return name; }
    int getage() { return age; }
    char getgender() { return gender; }
    string getbloodgroup() { return bloodgp; }
    string getcontact() { return contact; }
    string getaddress() { return address; }
    int getdepartment() { return requiredep; }
    int getappointmentstatus() { return appointmentstatus; }

    void createpat();
    void loadpat(string patid);
    void generatePatientID();
    void showAppointmentStatus();
    void savepat();
    void healthanalysis();
    void addappointment(int dep);
};

class employee : public person{
    protected : 
    string empid;
    int department;

    public :


};

int findpatient(string patid);  // this function finds the entered patient id from patient file
void patientmenu(patient* pat); //this function gives choices to patient and the progrmme coninues further
void saveupdatedpat(patient* pat);


int main(){
    int user;
    char pat;
    while(1){
        cout<<"1. Enter as a Patient : "<<endl;
        cout<<"2. Enter as a Employee : "<<endl;
        cout<<"3. Return.... : "<<endl;
        cout<<"YOUR RESPONSE : ";
        cin>>user ;
         
        switch(user){
            case 1:
            cout<<"Hello Patient!!\n";
            cout<<"Have you registered previously (y/n) : ";
            cin>>pat;
            if(pat=='y'){
                cout<<"Enter your patient id : ";
                string patid;
                cin>>patid;
                if(findpatient(patid)){
                    patient* pat = new patient();
                    pat->loadpat(patid);
                    patientmenu(pat);
                }
                else {
                    cout<<"No Registered patients found "<<endl;
                    cout<<"Kindly enter as a fresh patient ! \n"<<endl;
                }
            }
            else if(pat=='n'){
                cout<<"Making a new registration : \n";
                patient* pat = new patient();
                pat->createpat();
                pat->savepat();
                patientmenu(pat);
            }
            else cout<<"Incorrect input kindly enter again "<<endl;

            break;

            case 2:

            break;

            case 3:

            return 0;
            break;

            default:
            cout<<"Incorrect input recieved please try again : "<<endl;

        }
    }
    return 0;
}

void saveupdatedpat(patient* pat){
    ifstream fin("patients.txt");
    ofstream fout("temp.txt");

    string id;
    while (fin >> id) {
        if (id == pat->getID()) {
            // Skip old entry and write updated one
            string skip;
            getline(fin, skip); // skip rest of old line
            fout << pat->getID() << " " << pat->getname() << " " << pat->getage() << " " 
                 << pat->getgender() << " " << pat->getbloodgroup() << " " << pat->getcontact() << " " 
                 << pat->getaddress() << '|' << pat->getdepartment() << " " << pat->getappointmentstatus() << "\n";
        } else {
            string rest;
            getline(fin, rest);
            fout << id << rest << "\n";
        }
    }

    fin.close();
    fout.close();

    remove("patients.txt");
    rename("temp.txt", "patients.txt");
    cout << "Patient record updated successfully!\n";
}

int findpatient(string patid) {
    ifstream fin("patients.txt");
    string id;
    while (fin >> id) {
        if (id == patid) {
            fin.close();
            return 1; // found
        }
        string temp;
        getline(fin, temp); // skip rest of the line
    }
    fin.close();
    return 0; // not found
}

void patientmenu(patient* pat){
    cout<<"Patient Menu : "<<endl;  
    int choice ;
    while(1){
        cout<<"1. Quick health analysis : "<<endl;
        cout<<"2. Book Appointment : "<<endl;
        cout<<"3. Update Profile : "<<endl;
        cout<<"4. View Bills : "<<endl;
        cout<<"5. Check Appointment Status : "<<endl;
        cout<<"6. Return.... :  "<<endl;
        cout<<"YOUR RESPONSE : ";
        cin>>choice;

        switch(choice){
            case 1:
            pat->healthanalysis();
            break;
            case 2:
            cout<<"In which Departments you want appointment : "<<endl;
            for (auto dept : departmentNames) {
                if (dept.first == 0) continue; // skip 'No Department' for selection
                cout << dept.first << ". " << dept.second << endl;
            }
            cout<<"Your Response : ";
            int dep;
            cin>>dep;
            pat->addappointment(dep);

            break;
            case 3:

            break;
            case 4:

            break;
            case 5:

            break;

            case 6:
            cout<<"Returning back...."<<endl;
            return;

            default:

            break;
        }
    }
}

void patient :: createpat(){
    cout<<"Hello patient kindly enter the asked details correctly ! \n";
    cout<<"Enter your Aame : ";
    cin>>name;
    cout<<"Enter your Age : ";
    cin>>age;
    cout<<"Enter your Gender : ";
    cin>>gender;
    cout<<"Enter your blood group : ";
    cin>>bloodgp;
    cout<<"Enter your contact details (10 digit valid phone number) : ";
    cin>>contact;
    cout<<"Enter your address : ";
    getchar();
    getline(cin,address);
    //patient id will be created automatically  related to its name, contact,gender,bloodgrp,age 
    generatePatientID();
    cout<<"Registration is done successfully !! \n";
    cout<<"Your patient id is : "<<patientid<<endl;
}

void patient :: loadpat(string patid) {
    ifstream fin("patients.txt");
    string id;
    while (fin >> id) {
        if (id == patid) {
            patientid = id;
            fin >> name >> age >> gender >> bloodgp >> contact;
            //fin.ignore(); // skip space before getline
            getline(fin, address, '|'); // read address until |
            fin >> requiredep>>appointmentstatus; // read required department
            break;
        } else {
            string temp;
            getline(fin, temp); // skip rest of the line
        }
    }
    fin.close();

    cout << "\nPatient data loaded successfully!\n";
    cout << "Name: " << name << "\nAge: " << age << "\nGender: " << gender << "\nBlood Group: " << bloodgp;
    cout << "\nContact: " << contact << "\nAddress: " << address << "\nRequired Dept: " << requiredep << "\nAppointment Status : "<<appointmentstatus<<endl;
}

void patient:: generatePatientID() {
    string idName = name.substr(0, 3);  // First 3 letters of name
    string idContact = contact.substr(contact.length() - 4); // Last 4 digits of contact

    patientid = "PAT" + idName + gender + to_string(age) + idContact +  bloodgp ;
}

void patient :: showAppointmentStatus() {
    cout << "Your Appointment Status : ";

    switch (appointmentstatus) {
        case 0: cout << "No Appointment Booked\n"; break;
        case 1: cout << "Pending Confirmation\n"; break;
        case 2: cout << "Confirmed\n"; break;
        default: cout << "Unknown Status\n"; break;
    }
}

void patient::savepat() {
    ofstream fout("patients.txt", ios::app); // open in append mode
    if (!fout) {
        cout << "Error opening file to save patient data.\n";
        return;
    }
    
    fout<<patientid<<" "<<name<<" "<<age<<" "<<gender<<" "<<bloodgp<<" "<<contact<<" "<<address<<'|'<<requiredep<<" "<<appointmentstatus<<"\n";
    fout.close();
    //cout << "Patient data saved successfully.\n";
}

void patient :: healthanalysis(){
    cout<<"-----QUICK HEALTH ANALYSIS PACKAGES------"<<endl;
    cout << "1. Basic Health Checkup(Rs 2000) -\n";
    cout << "2. Advanced Checkup(Rs 4000) -\n";
    cout << "3. Diabetes Panel(Rs 3000) -\n";
    cout << "4. Heart Care Package(Rs 5000) - \n";
    cout << "5. Full Body Checkup(Rs 7000) -\n";
    cout << "Select a package (1-5): ";

}

void patient :: addappointment(int dep) {
    if (departmentNames.find(dep) != departmentNames.end() && dep != 0) {
        requiredep = dep;
        appointmentstatus = 1; // appointment requested, pending confirmation
        cout << "Appointment request sent to department : " << departmentNames[dep] << "\n";
        cout << "Your status is now set to : Pending\n\n";
        saveupdatedpat(this);
    } else {
        cout << "Invalid department selected!\n";
    }
}