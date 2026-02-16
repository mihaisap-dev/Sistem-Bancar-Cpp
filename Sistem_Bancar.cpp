#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class BankAccount {
private:
    string accountHolder;
    double balance;
    int accountNumber;
    string password;

public:
    BankAccount( int id, string name, double initialDeposit, string pass) {
        accountHolder = name;
        accountNumber = id;
        balance = initialDeposit;
        password=pass;
    }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Succes! Suma depusa. Sold actual: " << balance << endl;
        }
    }

    void withdraw(double amount) {
        if (amount > balance) 
            cout << "Fonduri insuficiente!" << endl;
        else if (amount <= 0) 
            cout << "Suma invalida!" << endl;
        else {
            balance -= amount;
            cout << "Retragere reusita! Sold ramas: " << balance << endl;
        }
    }

    void displayAccount() {
        cout << "Contul #" << accountNumber << " (" << accountHolder << ") - Sold: " << balance << " RON" << endl;
    }

    bool checkPassword(string pass){
        return password==pass;
    }

    string getName(){ return accountHolder;}
    double getBalance(){ return balance;}
    int getID(){ return accountNumber; }
    string getPassword(){ return password; }
};

void save(BankAccount &ba){
    ofstream fout("bank_data.txt", ios::app);
    if(fout.is_open()){
        fout<<ba.getID()<<endl;
        fout<<ba.getName()<<endl;
        fout<<ba.getBalance()<<endl;
        fout<<ba.getPassword()<<endl;
        fout.close();
    }
}

void load(vector<BankAccount> &accounts){
    ifstream fin("bank_data.txt");
    int accountNumber;
    string accountHolder;
    double balance;
    string password;

    while(fin>>accountNumber){
        fin.ignore();
        getline(fin, accountHolder);
        getline(fin, password);
        fin>>balance;

        accounts.push_back(BankAccount(accountNumber, accountHolder, balance, password));
    }

    fin.close();
    }

void update(vector<BankAccount> &accounts){
    ofstream fout("bank_data.txt");
    if(fout.is_open()){
        for(int i=0; i<accounts.size(); i++){
            fout<<accounts[i].getID()<<endl;
            fout<<accounts[i].getName()<<endl;
            fout<<accounts[i].getBalance()<<endl;
            fout<<accounts[i].getPassword()<<endl;
        }
        fout.close();
        cout<<"Baza de date a fost actualizata!"<<endl;
    }
}

double getValidNumber(string mesaj){
    double valoare;
    while(true){
        cout<<mesaj;
        if(cin>>valoare){
            if(cin.peek()=='\n' || cin.peek()==' ' || cin.peek()==EOF){
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return valoare;
            }
            else{
                cout<<"Eroare: Nu puteti pune litere!"<<endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');        
            }
        }
        else {
            cout<<"Eroare: Trebuie sa introduceti un numar valid!"<<endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
    }
}

string getValidString(string mesaj){
    string text;
    while(true){
        cout<<mesaj;
        getline(cin, text);
        bool ok=false;
        for(int i=0; i<text.length(); i++){
            if(isdigit(text[i])) 
                ok=true;
        }
        if(!text.empty() && !ok)
            return text;
        else if(text.empty())
            cout<<"Eroare: Numele nu poate fi gol!"<<endl;
        else if(ok)
            cout<<"Eroare: Numele nu poate contine cifre!"<<endl;
    }
}

string getValidPassword(string mesaj){
    string pass;
    while(true){
        cout<<mesaj;
        getline(cin,pass);

        if(pass.length()>=4)
            return pass;
        else
            cout<<"Eroare: Parola trebuie sa aiba cel putin 4 caractere!";
    }
}
int main(){
    vector<BankAccount> accounts;
    load(accounts);

    int option;
    do{
        cout<<"\n--- Sistem Multi-Bancar ---"<<endl;
        cout<<"1. Creare Cont Nou"<<endl;
        cout<<"2. Vizualizare toate conturile"<<endl;
        cout<<"3. Operatiuni pe un cont existent"<<endl;
        cout<<"4. Stergere cont"<<endl;
        cout<<"5. Salvare si Iesire"<<endl;
        option=(int)getValidNumber("Alege optiunea: ");

        if(option==1){
            int accountNumber;
            while(true){
                accountNumber=(int)getValidNumber("ID Cont: ");

                bool exists=false;
                for(int i=0;i<accounts.size();i++){
                    if(accounts[i].getID()==accountNumber)
                        exists=true;
                }
                if(!exists) 
                    break;
                else 
                    cout<<"Eroare: Acest ID exista deja!"<<endl;
            }

            string accountHolder=getValidString("Numele titularului: ");

            double balance;
            while(true){
                balance=getValidNumber("Sold Initial: ");
                if(balance>=0)
                    break;
                else    
                    cout<<"Eroare: Soldul nu poate fi negativ!"<<endl;
            }
            
            string password=getValidPassword("Setati o parola pentru cont: ");
            BankAccount newAcc(accountNumber, accountHolder, balance, password);
            accounts.push_back(newAcc);
            save(newAcc);
            cout<<"Cont creat cu succes!"<<endl;
        }

        else if(option==2){
            cout<<"\n--- Lista Conturi ---"<<endl;
            for(int i=0; i<accounts.size(); i++){
            accounts[i].displayAccount();
            }
        }

        else if(option==3){
            int searchID;
            searchID = (int)getValidNumber("Introdu ID-ul contului: ");

            bool found=false;
            for(int i=0; i<accounts.size(); i++){
                if(accounts[i].getID()==searchID){
                    found=true;
                    int subOption;
                    double sum;
                    do{
                        cout<<"\n--- Operatiuni pentru: "<<accounts[i].getName()<<" ---"<<endl;
                        cout<<"1. Vizualizare Sold"<<endl;
                        cout<<"2. Depunere"<<endl;
                        cout<<"3. Retragere"<<endl;
                        cout<<"4. Inapoi la meniul principal"<<endl;
                        subOption=(int)getValidNumber("Alege optiunea: ");

                        switch(subOption){
                            case 1:
                                accounts[i].displayAccount();
                                break;
                            case 2:
                                while(true){
                                    sum=getValidNumber("Suma de depus: ");
                                    if(sum>0){
                                        accounts[i].deposit(sum);
                                        break;
                                    }
                                    else{
                                        cout<<"Eroare: Suma depusa trebuie sa fie pozitiva!"<<endl;
                                    }
                                }
                                break;
                            case 3:
                                while(true){
                                    sum=getValidNumber("Suma de retras: ");
                                    if(sum<=0)
                                        cout<<"Eroare: Suma retrasa trebuie sa fie mai mare de 0!"<<endl;
                                    else if(sum>accounts[i].getBalance())
                                        cout<<"Eroare: Fonduri insuficiente! Sold actual: "<<accounts[i].getBalance()<<" RON"<<endl;
                                    else{
                                        accounts[i].withdraw(sum);
                                        break;
                                    }
                                }
                                break;
                            case 4:
                                update(accounts);
                                cout<<"Revenire..."<<endl;
                                break;
                            default:
                                cout<<"Optiune invalida!"<<endl;
                        }
                    } while(subOption!=4);
                    break;
                }
            }
            if(!found)
                cout<<"Contul nu a fost gasit!"<<endl;
        }

        else if(option==4){
            int deleteID=(int)getValidNumber("Introdu ID-ul contului: ");
            bool found=false;

            for(int i=0; i<accounts.size(); i++){
                if(accounts[i].getID()==deleteID){
                    found=true;

                    cout<<"Introdu parola pentru contul lui "<<accounts[i].getName()<<": ";
                    string password;
                    getline(cin, password);
                    if(accounts[i].checkPassword(password)){
                        cout<<"Parola corecta! Scrieti 'DA' pentru a confirma stergerea contului: ";
                        string confirm;
                        cin>>confirm;

                        if(confirm=="DA"){
                            accounts.erase(accounts.begin()+i);
                            update(accounts);
                            cout<<"Succes: Contul a fost sters!"<<endl;
                        }
                        else
                            cout<<"Anulat: Confirmare incorecta."<<endl;
                    }
                    else
                        cout<<"Eroare: Parola incorecta! Acces refuzat."<<endl;
                    break;
                }
            }
            if(!found)
                cout<<"Contul nu a fost gasit!"<<endl;

        }
    } while(option!=5);

    return 0;
}
