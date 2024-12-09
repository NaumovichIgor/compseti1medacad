#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

void writeToServer(const string& clientName, const string& data) {
    ofstream outFile("con.txt", ios::app);
    if (outFile.is_open()) {
        outFile << clientName << ":" << data << endl;
        outFile.close();
    }
    else {
        cerr << "Unable to open con.txt for writing." << endl;
    }
}

void readResponse(const string& clientName) {
    ifstream inFile(clientName + ".txt");
    string response;
    while (true) {
        if (inFile.is_open()) {
            while (getline(inFile, response)) {
                cout << "Response from server: " << response << endl;
            }
            inFile.close();
        }
        this_thread::sleep_for(chrono::seconds(1));
        inFile.open(clientName + ".txt");
    }
}

int main() {
    string clientName;
    cout << "Enter your name: ";
    getline(cin, clientName);

    thread responseThread(readResponse, clientName);
    string data;

    while (true) {
        cout << "Enter data (for medical: lastName,height,weight; for academic: lastName,grade1,grade2,grade3,grade4) or type 'exit' to quit: ";
        getline(cin, data);
        if (data == "exit") {
            break;
        }
        writeToServer(clientName, data);
    }

    responseThread.detach();
    return 0;
}
