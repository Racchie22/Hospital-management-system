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


};

class employee : public person{
    protected : 
    string empid;
    int department;
    

    public :


};

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
            cout<<"Are you already registered previously (y/n) : ";
            cin>>pat;
            if(pat=='y'){
                cout<<"Enter your patient id : ";
                
            }
            else if(pat=='n'){
                cout<<"Making a new registration : \n";
                
            }
            else cout<<"Incorrect input kindly enter again "<<endl;

            case 2:

            case 3:

        }
    }
    return 0;
}
