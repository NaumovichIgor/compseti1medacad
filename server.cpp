#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <map>

using namespace std;

void processMedicalData(const string& clientName, const string& data) {
    string response;
    string lastName;
    double height, weight;

    size_t pos = data.find(',');
    if (pos != string::npos) {
        lastName = data.substr(0, pos);
        height = stod(data.substr(pos + 1, data.find(',', pos + 1) - pos - 1));
        weight = stod(data.substr(data.find(',', pos + 1) + 1));

        double bmi = weight / (height * height);
        if (bmi < 18.5) {
            response = lastName + ": Underweight";
        }
        else if (bmi >= 18.5 && bmi < 24.9) {
            response = lastName + ": Normal";
        }
        else {
            response = lastName + ": Overweight";
        }
    }

    ofstream outFile(clientName + ".txt", ios::app);
    if (outFile.is_open()) {
        outFile << response << endl;
        outFile.close();
    }
}

void processAcademicData(const string& clientName, const string& data) {
    string response;
    string lastName;
    int grades[4];
    int total = 0;

    size_t pos = data.find(',');
    if (pos != string::npos) {
        lastName = data.substr(0, pos);
        size_t start = pos + 1;
        for (int i = 0; i < 4; ++i) {
            pos = data.find(',', start);
            grades[i] = stoi(data.substr(start, pos - start));
            total += grades[i];
            start = pos + 1;
        }

        bool hasDebts = false;
        for (int grade : grades) {
            if (grade < 3) {
                hasDebts = true;
                break;
            }
        }

        if (hasDebts) {
            response = lastName + ": Has debts";
        }
        else if (total / 4 >= 4) {
            response = lastName + ": Eligible for scholarship";
        }
        else {
            response = lastName + ": Not eligible for scholarship";
        }
    }

    ofstream outFile(clientName + ".txt", ios::app);
    if (outFile.is_open()) {
        outFile << response << endl;
        outFile.close();
    }
}

void checkRequests() {
    ifstream inFile("con.txt");
    string line;

    while (true) {
        if (inFile.is_open()) {
            while (getline(inFile, line)) {
                size_t pos = line.find(':');
                if (pos != string::npos) {
                    string clientName = line.substr(0, pos);
                    string data = line.substr(pos + 1);
                    if (data.find(',') != string::npos) {
                        if (data.find("medical") != string::npos) {
                            processMedicalData(clientName, data);
                        }
                        else if (data.find("academic") != string::npos) {
                            processAcademicData(clientName, data);
                        }
                    }
                }
            }
            inFile.close();
        }
        this_thread::sleep_for(chrono::seconds(1));
        inFile.open("con.txt");
    }
}

int main() {
    checkRequests();
    return 0;
}
