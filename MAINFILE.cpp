#include<iostream>
using namespace std;

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

    public :

    void createpat(){
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
        cin >>address;
        
    }

    void loadpat(string patid){

    }
};

class employee : public person{
    protected : 
    string empid;
    int department;

    public :


};

int findpatient(string patid);
void patientchoices(patient* pat);

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
                cout<<"Registration done successfully !! \n"<<endl;
                patientchoices(pat);
            }
            else cout<<"Incorrect input kindly enter again "<<endl;

            case 2:

            case 3:

        }
    }
    return 0;
}
