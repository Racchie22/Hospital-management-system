#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include<windows.h>

using namespace std;

map<int, string> meds = {
    {101, "Paracetamol"},
    {102, "Ibuprofen"},
    {103, "Amoxicillin"},
    {104, "Cough Syrup"},
    {105, "Antacid"},
    {106, "Cetirizine"},
    {107, "Vitamin C"},
    {108, "Azithromycin"}};

map<int, string> departmentNames = {
    {1, "Cardiology"},
    {2, "Neurology"},
    {3, "Orthopedics"},
    {4, "General Physician"},
};

map<int, int> medicinePrice = {
    {101, 10},
    {102, 12},
    {103, 20},
    {104, 25},
    {105, 15},
    {106, 8},
    {107, 5},
    {108, 30}};

void delay(string message){
    for (char c : message) {
        cout << c << flush;
        Sleep(60);
    }
    return;
 }

class person
{
public:
    string name;
    int age;
    char gender;
    string address;
    string contact;
};

class doctor
{
private:
    person *p; // Aggregation: person is not owned
    string specialization;
    int department;

public:
    doctor()
    {
        p = nullptr;
    }
    
    ~doctor(){}

    doctor(person *personPtr, string specialization, int department)
    {
        p = personPtr;
        this->specialization = specialization;
        this->department = department;
    }

    void displayDoctorInfo()
    {
        if (p)
        {
            cout << "Name: " << p->name << endl;

        }
        cout << "Specialization: " << specialization << endl;
        cout << "Department: " << departmentNames[department] << endl;
    }

    int getDepartment() { return department; }
    string getSpecialization() { return specialization; }
    string getName() { return p->name; }
};

vector<doctor> doctors;

class bills
{
public:
    string doctorName;
    int doctorFees;
    map<int, int> medQty;

    bills()
    {
        doctorName = "";
        doctorFees = 0;
    }

    ~bills(){}

    void addMedicine(int medId, int qty)
    {
        medQty[medId] += qty;
    }

    void setDoctor(int apdept)
    {
        if (apdept != -1)
        {
            doctorName = doctors[apdept].getName();
            if (apdept == 7 || apdept == 8)
                doctorFees = 800;
            else
                doctorFees = 1500;
        }
    }

    void generateBillFile(string patientId, int apdept)
    {
        setDoctor(apdept);
        string filename = patientId;
        reverse(filename.begin(), filename.end());
        for (int i = 0; i < 3; i++)
            filename.pop_back();
        filename += "LLIB";
        reverse(filename.begin(), filename.end());
        filename += ".txt";

        int previousMedCost = 0;
        string existingContent;
        ifstream fin(filename);
        if (fin.is_open())
        {
            string line;
            while (getline(fin, line))
            {
                existingContent += line + "\n";
                if (line.find("Total Medicine Cost: Rs.") != string::npos)
                {
                    string costStr = line.substr(line.find("Rs.") + 4);
                    previousMedCost = stoi(costStr);
                }
            }
            fin.close();
        }

        int newMedCost = 0;
        string medicineDetails = "Medicines:\n";
        for (auto item : medQty)
        {
            int id = item.first;
            int qty = item.second;
            int cost = medicinePrice[id] * qty;
            newMedCost += cost;
            medicineDetails += meds[id] + " - " + to_string(qty) + " tablets x Rs. " + to_string(medicinePrice[id]) + " = Rs. " + to_string(cost) + "\n";
        }

        int totalMedCost = previousMedCost + newMedCost;
        int total = doctorFees + totalMedCost;

        ofstream fout(filename);
        fout << "--- Hospital Bill ---\n";
        fout << "Patient ID: " << patientId << "\n";
        fout << "Doctor: " << doctorName << " | Fees: Rs. " << doctorFees << "\n\n";
        fout << medicineDetails;
        fout << "\nTotal Medicine Cost: Rs. " << totalMedCost << "\n";
        fout << "Total Bill: Rs. " << total << "\n";
        fout.close();

        cout << "Bill updated and saved as '" << filename << "'\n";
    }

    void viewBill(string patientId)
    {
        string filename = patientId;
        reverse(filename.begin(), filename.end());
        for (int i = 0; i < 3; i++)
            filename.pop_back();
        filename += "LLIB";
        reverse(filename.begin(), filename.end());

        ifstream fin(filename + ".txt");
        if (!fin.is_open())
        {
            cout << "No bill generated for patient ID: " << patientId << "\n";
            return;
        }

        delay("\n--- Hospital Bill ---\n");

        string line;
        string doctorName;
        int doctorFees = 0;
        int totalMedCost = 0;

        while (getline(fin, line))
        {
            if (line.find("Patient ID:") != string::npos)
            {
                cout << "Patient ID: " << line.substr(line.find(":") + 2) << "\n";
            }
            else if (line.find("Doctor:") != string::npos)
            {
                cout << "Doctor: " << line.substr(line.find(":") + 2) << "\n";
            }
            else if (line.find("Medicines:") != string::npos)
            {
                cout << "Medicines:\n";
            }
            else if (line.find("Total Medicine Cost:") != string::npos)
            {
                cout << "Total Medicine Cost: " << line.substr(line.find(":") + 2) << "\n";
            }
            else if (line.find("Total Bill:") != string::npos)
            {
                cout << "Total Bill: " << line.substr(line.find(":") + 2) << "\n";
            }
        }

        fin.close();
    }
};

class patient : public person
{
protected:
    string bloodgp;
    string patientid;
    int appointmentDept = -1; // -1 = not booked by default

public:
    bills bill;
    int loadpat(string patid);

    patient()
    {
        char n;
        //cout << "Hello Patient!!\n";
        cout << "Have you registered previously (y/n) : ";
        cin >> n;
        int fnd = 1;

        if (n == 'y' || n == 'Y')
        {
            while (1)
            {
                cout << "Enter your patient id or -1 if you want to register as new: ";
                string patid;
                cin >> patid;
                fnd = loadpat(patid);
                if (fnd == 0)
                    return;
                if (patid == "-1")
                    break;
            }
        }

        cout << "Hello patient kindly enter the asked details correctly ! \n";
        cout << "Enter your Name : ";
        cin >> name;
        cout << "Enter your Age : ";
        cin >> age;

        while(1){
            cout << "Enter your Gender (M/F) : ";
            cin >> gender;
            try{
                if(gender != 'M' && gender != 'F'){
                    throw "Invalid Gender Entered Error Occured! ";
                }
                else break;
            }
            catch(const char* msg){
                cout<<msg<<endl;
            }
        }

        while(1){
            cout << "Enter your blood group : ";
            cin >> bloodgp;
            try{
                if(bloodgp =="O+" || bloodgp=="O-" || bloodgp=="A+" || bloodgp=="A-" || bloodgp=="B+" || bloodgp=="B-" || bloodgp=="AB+" || bloodgp=="AB-"){
                    break;
                }
                else {
                    throw "Invalid Blood group Entered!! \nkindly Re-enter it!! ";
                }
            }catch(const char* msg){
                cout<<msg<<endl;
            }
        }

        cout << "Enter your contact details (10 digit valid phone number) : ";
        cin >> contact;
        cout << "Enter your address : ";
        getchar();
        getline(cin, address);

        string idContact = contact.substr(contact.length() - 4);
        patientid = "PAT" + to_string(age) + idContact + bloodgp;

        cout << "Registration is done successfully !! \n";
        delay("Generating your patient id ....\n");
        cout << "\nYour patient id is : " << patientid << endl;
        savepat();
    }

    ~patient(){}

    string getID() { return patientid; }
    string getname() { return name; }
    int getage() { return age; }
    char getgender() { return gender; }
    string getbloodgroup() { return bloodgp; }
    string getcontact() { return contact; }
    string getaddress() { return address; }

    void savepat();
    void healthanalysis();
    void updateProfile();
    void bookAppointment(int dept);
    void viewProfile();

    int getAppointmentDept() { return appointmentDept; }
};

void loadDoctors()
{
    doctors.clear();
    ifstream fin("doctors.txt");
    if (!fin.is_open())
    {
        cout << "Doctors file not found, loading default doctors.\n";


        person *p1 = new person{"Dr. Arjun Mehta", 45, 'M', "201", "45673210"};
        person *p2 = new person{"Dr. Kavita Rao", 39, 'F', "202", "45673211"};
        person *p3 = new person{"Dr. Sameer Desai", 50, 'M', "304", "45673220"};
        person *p4 = new person{"Dr. Nandini Iyer", 42, 'F', "305", "45673221"};
        person *p5 = new person{"Dr. Rajat Kapoor", 48, 'M', "406", "45673230"};
        person *p6 = new person{"Dr. Meenal Shah", 36, 'F', "407", "45673231"};
        person *p7 = new person{"Dr. Anil Bhatia", 40, 'M', "101", "45673240"};
        person *p8 = new person{"Dr. Priya Menon", 35, 'F', "103", "45673241"};

        doctors.push_back(doctor(p1, "Interventional Cardiologist", 1));
        doctors.push_back(doctor(p2, "Heart Failure Specialist", 1));
        doctors.push_back(doctor(p3, "Stroke Specialist", 2));
        doctors.push_back(doctor(p4, "Epileptologist", 2));
        doctors.push_back(doctor(p5, "Joint Replacement Surgeon", 3));
        doctors.push_back(doctor(p6, "Spine Specialist", 3));
        doctors.push_back(doctor(p7, "General Medicine", 4));
        doctors.push_back(doctor(p8, "Family Physician", 4));

        return;
    }

    string line;
    while (getline(fin, line))
    {
        size_t pos1 = line.find("|");
        size_t pos2 = line.find("|", pos1 + 1);

        string name = line.substr(0, pos1);
        string specialization = line.substr(pos1 + 1, pos2 - pos1 - 1);
        int department = stoi(line.substr(pos2 + 1));

        person *p = new person{name};
        doctors.push_back(doctor(p, specialization, department));
    }

    fin.close();
}


void patientmenu(patient *pat);
void saveupdatedpat(patient *pat);

void saveupdatedpat(patient *pat)
{
    string filename = pat->getID() + ".txt";

    ofstream fout(filename);
    if (!fout)
    {
        cout << "Error updating patient record.\n";
        return;
    }

    fout << pat->getID() << " " << pat->getname() << " " << pat->getage() << " "
         << pat->getgender() << " " << pat->getbloodgroup() << " "
         << pat->getcontact() << " " << pat->getaddress() << '|' << " " << pat->getAppointmentDept() << "\n";

    fout.close();
    delay("Patient record updated successfully!\n");
}

void patient::viewProfile()
{
    delay("\n----- PATIENT PROFILE -----\n");
    cout << "Patient ID     : " << patientid << endl;
    cout << "Name           : " << name << endl;
    cout << "Age            : " << age << endl;
    cout << "Gender         : " << gender << endl;
    cout << "Blood Group    : " << bloodgp << endl;
    cout << "Contact        : " << contact << endl;
    cout << "Address        : " << address << endl;
    cout << "Appointment    : ";
    if (appointmentDept == -1)
        cout << "Not Booked\n";
    else
        cout << departmentNames[doctors[appointmentDept].getDepartment()] << " (Dr. "
             << doctors[appointmentDept].getName() << ") " << doctors[appointmentDept].getSpecialization() << endl;
    cout << "------------------------------\n";
}

void patient::updateProfile()
{
    int choice;
    string tempStr;
    int tempInt;
    char tempChar;

    while (true)
    {
        delay("\n--- PROFILE UPDATING MENU ---\n");
        cout << "1. Update Name : \n";
        cout << "2. Update Age : \n";
        cout << "3. Update Gender : \n";
        cout << "4. Update Blood Group : \n";
        cout << "5. Update Contact : \n";
        cout << "6. Update Address : \n";
        cout << "7. Save and Exit : \n";
        cout << "Your Choice : ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Enter new name : ";
            cin >> tempStr;
            name = tempStr;
            break;
        case 2:
            cout << "Enter new age : ";
            cin >> tempInt;
            age = tempInt;
            break;
        case 3:
            while(1){
                cout << "Enter new gender (M/F) : ";
                cin >> tempChar;
                try{
                    if(tempChar != 'M' && tempChar != 'F'){
                        throw "Invalid Gender Entered Error Occured! ";
                    }
                    else {
                        gender = tempChar;
                        cout<<"Gender updated successfully ! "<<endl;
                        break;
                    }
                }
                catch(const char* msg){
                    cout<<msg<<endl;
                }
            }
            break;
        case 4:
            while(1){
                cout << "Enter new blood group : ";
                cin >> tempStr;
                try{
                    if(tempStr =="O+" || tempStr=="O-" || tempStr=="A+" || tempStr=="A-" || tempStr=="B+" || tempStr=="B-" || tempStr=="AB+" || tempStr=="AB-"){
                        bloodgp = tempStr;
                        break;
                    }
                    else {
                        throw "Invalid Blood group Entered!! \nkindly Re-enter it!! ";
                    }
                }catch(const char* msg){
                    cout<<msg<<endl;
                }
            }
            break;
        case 5:
            cout << "Enter new contact number (10 Digit) : ";
            cin >> tempStr;
            contact = tempStr;
            break;
        case 6:
            cout << "Enter new address : ";
            getchar(); // to remove newline left
            getline(cin, tempStr);
            address = tempStr;
            break;
        case 7:
            saveupdatedpat(this); // save changes
            return;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}

int patient::loadpat(string patid)
{
    patid += ".txt";
    ifstream fin(patid);
    if (!fin.is_open())
    {
        cout << "User not found: " << patid << endl;
        return 1;
    }
    fin >> patientid >> name >> age >> gender >> bloodgp >> contact;
    fin.ignore();
    getline(fin, address, '|');
    fin >> appointmentDept;
    fin.close();

    delay("\nPatient data loaded successfully!\n");
    cout << "Name: " << name << "\nAge: " << age << "\nGender: " << gender << "\nBlood Group: " << bloodgp;
    cout << "\nContact: " << contact << "\nAddress: " << address << endl;
    return 0;
}

void patient::bookAppointment(int dept)
{
    vector<int> availableDoctors;

    cout << "\nAvailable doctors in " << departmentNames[dept] << " department:\n\n";

    int serial = 1;
    for (int i = 0; i < doctors.size(); ++i)
    {
        if (doctors[i].getDepartment() == dept)
        {
            cout << serial << ". ";
            doctors[i].displayDoctorInfo();
            availableDoctors.push_back(i); // Save doctor index
            ++serial;
            cout << endl;
        }
    }

    if (availableDoctors.empty())
    {
        cout << "Sorry, no doctors available in this department.\n";
        return;
    }

    cout << "Choose a doctor by serial number (1 to " << availableDoctors.size() << "): ";
    int choice;
    cin >> choice;

    if (choice < 1 || choice > availableDoctors.size())
    {
        cout << "Invalid choice. Appointment not booked.\n";
        return;
    }

    appointmentDept = availableDoctors[choice - 1];

    cout << "Appointment booked with " << doctors[appointmentDept].getName()
         << " (" << departmentNames[doctors[appointmentDept].getDepartment()] << " department " << doctors[appointmentDept].getSpecialization() << ").\n";

    savepat();
}

void patient::savepat()
{
    string s = patientid + ".txt";
    ofstream fout(s); // No append flag, overwrite by default
    if (!fout)
    {
        cout << "Error opening file to save patient data.\n";
        return;
    }

    fout << patientid << " " << name << " " << age << " " << gender << " " << bloodgp << " " << contact << " " << address << '|' << " " << appointmentDept << "\n";
    fout.close();
}

void patient::healthanalysis()
{
    delay( "----- QUICK HEALTH ANALYSIS -----\n");
    cout << "Please answer the following with 'y' (yes) or 'n' (no):\n";

    char headache, chestPain, stomachAche, jointPain, breathingIssue, dizziness;

    cout << "Do you have a headache? (y/n): ";
    cin >> headache;

    cout << "Do you have chest pain or discomfort? (y/n): ";
    cin >> chestPain;

    cout << "Do you have stomach ache or digestive issues? (y/n): ";
    cin >> stomachAche;

    cout << "Do you feel joint or bone pain? (y/n): ";
    cin >> jointPain;

    cout << "Are you facing breathing issues or shortness of breath? (y/n): ";
    cin >> breathingIssue;

    cout << "Do you feel dizzy or light-headed? (y/n): ";
    cin >> dizziness;

    cout << "\nBased on your symptoms, we suggest:\n";

    bool found = false;

    if (chestPain == 'y' || breathingIssue == 'y')
    {
        cout << "- Visit the **Cardiology** department.\n";
        found = true;
    }

    if (headache == 'y' || dizziness == 'y')
    {
        cout << "- Visit the **Neurology** department.\n";
        found = true;
    }

    if (jointPain == 'y')
    {
        cout << "- Visit the **Orthopedics** department.\n";
        found = true;
    }

    if (stomachAche == 'y')
    {
        cout << "- Visit the **Emergency** or **Gastroenterology** department.\n";
        found = true;
    }

    if (!found)
    {
        cout << "- No major issues detected. You may see a general physician for further clarification.\n";
    }

    cout << "----------------------------------\n";
}

void medic(patient &p)
{
    int choice, qty;

    delay("\n--- Available Medicines ---\n");
    for (auto med : meds)
    {
        cout << med.first << " : " << med.second << " (Rs. " << medicinePrice[med.first] << " per tablet)\n";
    }

    cout << "\nSelect multiple medicines (Enter -1 to finish) :\n";
    while (true)
    {
        cout << "Enter Medicine ID : ";
        cin >> choice;
        if (choice == -1)
            break;

        if (meds.find(choice) != meds.end())
        {
            cout << "Enter quantity : ";
            cin >> qty;
            p.bill.addMedicine(choice, qty);
            cout << meds[choice] << " x" << qty << " added.\n";
        }
        else
        {
            cout << "Invalid ID. Try again.\n";
        }
    }
}

int bi=0;

void patientmenu(patient *pat)
{
    int choice;
    while (1)
    {
        delay("\n----PATIENT MENU----\n");
        cout << "\n1. Quick health analysis : " << endl;
        cout << "2. View Profile : " << endl;
        cout << "3. Book Appointment : " << endl;
        cout << "4. Update Profile : " << endl;
        cout << "5. Buy Medicine : " << endl;
        cout << "6. View Bills : " << endl;
        cout << "7. Check Appointment Status : " << endl;
        cout << "8. Return.... :  " << endl;
        cout << "YOUR RESPONSE : ";
        cin >> choice;


        switch (choice)
        {
        case 1:
            pat->healthanalysis();
            break;

        case 2:
            pat->viewProfile();
            break;

        case 3:
        {
            int dept;
            cout << "Choose the  Department you want appointment in :\n";
            for (auto i : departmentNames)
                cout << i.first << " for " << i.second << endl;
            cout << "Enter any other no. to exit\n";
            cin >> dept;

            if (departmentNames.count(dept))
            {
                pat->bookAppointment(dept);
            }
            else
                cout << "Returning to menu.\n";
            break;
        }

        case 4:
            pat->updateProfile();
            break;

        case 5:
            medic(*pat);
            pat->bill.generateBillFile(pat->getID(), pat->getAppointmentDept());
            bi=1;
            break;
        case 6:
            pat->bill.viewBill(pat->getID());
            break;

        case 7:
            if (pat->getAppointmentDept() == -1)
                cout << "No appointment booked yet.\n";
            else
                cout << "Appointment booked: " << " ( " << doctors[pat->getAppointmentDept()].getName() << ") "
                     << doctors[pat->getAppointmentDept()].getSpecialization() << endl;
            break;
        case 8:
            cout << "Generating Bill : " << endl;
            if(bi==0)
            {pat->bill.generateBillFile(pat->getID(), pat->getAppointmentDept());}
            pat->bill.viewBill(pat->getID());
            delay("Returning back....\n");
            return;

        default:
            break;
        }
    }
}
void saveDoctors()
{
    ofstream fout("doctors.txt");
    if (!fout)
    {
        cout << "Error saving doctors data.\n";
        return;
    }

    for (doctor &d : doctors)
    {
        fout << d.getName() << "|" << d.getSpecialization() << "|" << d.getDepartment() << "\n";
    }

    fout.close();
    delay("Doctors saved successfully!\n");
}

void saveMedicines()
{
    ofstream fout("medicines.txt");
    if (!fout)
    {
        cout << "Error saving medicines data.\n";
        return;
    }

    for (auto &med : medicinePrice)
    {
        fout << med.first << "|" << meds[med.first] << "|" << med.second << "\n";
    }

    fout.close();
    delay("Medicines saved successfully!\n");
}

void loadMedicines()
{
    medicinePrice.clear();
    meds.clear();
    ifstream fin("medicines.txt");
    if (!fin.is_open())
    {
        cout << "Medicines file not found, loading default medicines.\n";

        // Default medicines manually daal do
        meds = {
            {101, "Paracetamol"},
            {102, "Ibuprofen"},
            {103, "Amoxicillin"},
            {104, "Cough Syrup"},
            {105, "Antacid"},
            {106, "Cetirizine"},
            {107, "Vitamin C"},
            {108, "Azithromycin"}
        };

        medicinePrice = {
            {101, 10},
            {102, 12},
            {103, 20},
            {104, 25},
            {105, 15},
            {106, 8},
            {107, 5},
            {108, 30}
        };

        return;
    }

    // Agar file mil gayi toh file se load karlo
    string line;
    while (getline(fin, line))
    {
        size_t pos1 = line.find("|");
        size_t pos2 = line.find("|", pos1 + 1);

        int id = stoi(line.substr(0, pos1));
        string name = line.substr(pos1 + 1, pos2 - pos1 - 1);
        int price = stoi(line.substr(pos2 + 1));

        meds[id] = name;
        medicinePrice[id] = price;
    }

    fin.close();
}


void staffmenu()
{
    int choice;
    while (true)
    {

        delay("\n--- ADMIN MENU ---\n");
        cout << "1. View All Doctors\n";
        cout << "2. Update Doctor Details\n";
        cout << "3. View All Medicines\n";
        cout << "4. Update Medicine Price\n";
        cout << "5. Exit Staff Menu\n";
        cout << "Your choice: ";
        cin >> choice;

        switch (choice)

        {
        case 1:
            for (int i = 0; i < doctors.size(); ++i)
            {
                cout << "\nDoctor #" << (i + 1) << ":\n";
                doctors[i].displayDoctorInfo();
            }
            break;

        case 2:
        {
            int docIndex;
            cout << "Enter doctor number (1-" << doctors.size() << "): ";
            cin >> docIndex;
            if (docIndex < 1 || docIndex > doctors.size())
            {
                cout << "Invalid doctor number.\n";
                break;
            }
            --docIndex; // 0-based index

            string newName,nName="Dr. ", newSpec;
            int newDept;

            cout << "Enter new name (enter '_' instead of spaces) : ";
            cin >> newName;
            nName+=newName;
            cout << "Enter new specialization (enter '_' instead of spaces) : ";
            cin >> newSpec;
            cout << "Enter new department number :\n";
            for (auto d : departmentNames)
                cout << d.first << " for " << d.second << endl;
            cin >> newDept;
            
            for(int i=0;i<nName.size();i++)
            {
                if(nName[i]=='_')
                nName[i]=' ';
            }
            for(int i=0;i<newSpec.size();i++)
            {
                if(newSpec[i]=='_')
                newSpec[i]=' ';
            }    

            // yaha direct access nahi hai private members ka, isiliye person pointer ko access karna padega
            doctors[docIndex] = doctor(new person{nName, 40, 'M', "Room", "Contact"}, newSpec, newDept);

            saveDoctors(); // save into file
            delay("Doctor details updated successfully!\n");
            break;
        }

        case 3:
            delay("\n--- Available Medicines ---\n");
            for (auto med : meds)
            {
                cout << med.first << " : " << med.second << " (Rs. " << medicinePrice[med.first] << ")\n";
            }
            break;

        case 4:
        {
            int medId;
            cout << "Enter Medicine ID to update price: ";
            cin >> medId;

            if (medicinePrice.find(medId) == medicinePrice.end())
            {
                cout << "Medicine ID not found.\n";
                break;
            }

            int newPrice;
            cout << "Enter new price: ";
            cin >> newPrice;

            medicinePrice[medId] = newPrice;
            saveMedicines(); // save into file
            delay("Medicine price updated successfully!\n");
            break;
        }

        case 5:
            return;

        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}



int main()
{
    int user;
    cout<<endl;
    loadDoctors();
    saveDoctors();

    loadMedicines();

    delay ("----------WELCOME TO MEDICARE+----------\n");
    delay ("ENTERING LOGIN WINDOW.....\n");

    while (1)
    {
        cout << "\n1. Enter as a Patient : " << endl;

        cout << "2. Enter as an Admin : " << endl;

        cout << "3. Return.... : " << endl;

        cout << "YOUR RESPONSE : ";
        cin >> user;

        if (user == 1)
        {
            patient pat;
            delay("\nWELCOME SIR.....\n");
            patientmenu(&pat);
        }
        else if (user == 2)
        {
            string passcode= "MediAdmin123";
            cout<<"Enter passcode for the admin : ";
            string pass;
            cin>>pass;
            try{
                if(pass==passcode){
                    delay("\nWELCOME SIR.....\n");
                    staffmenu();
                }
                else {
                    throw "Wrong password inputed \n";
                }
            }
            catch(const char* msg){
                cout<<msg<<"Try again..."<<endl;
            }

        }
        else if (user == 3)
        {
            return 0;
        }

        else cout<<"Invalid Input"<<endl;
    }
    return 0;
}

