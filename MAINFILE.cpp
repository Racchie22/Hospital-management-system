#include<iostream>
using namespace std;

class person{
    protected:
    string name;
    int age ;
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
    
    return 0;
}
