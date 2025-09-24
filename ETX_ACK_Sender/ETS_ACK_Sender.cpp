#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "Serial.h"

using namespace std;

// Konstanten im Programm
static constexpr uint8_t ACK = 0x06; // moderne, saubere Variante (C++11+)
static const int ETX = 0x03;         // klassische Variante

/*static bool waitForCTS(Serial& com, int timeout_ms = 5000) {
    using namespace std::chrono;
    const auto deadline = steady_clock::now() + milliseconds(timeout_ms);
    while (!com.isCTS()) {
        if (steady_clock::now() >= deadline) return false;
        std::this_thread::sleep_for(milliseconds(50)); // kein Busy-Wait
    }
    return true;
}*/

int main() {
    cout << "COM-Port Nummer: ";
    string portNr = "1";
    cin >> portNr;

    string port = "COM" + portNr;

    Serial com(port, 9600, 8, ONESTOPBIT, NOPARITY);

    if (!com.open()) {
        cerr << "Fehler beim Öffnen von " << port << endl;
        return -1;
    }

    // Erweiterung:
    //if (!waitForCTS(com)) { cout << "CTS LOW!\n"; return 1; }

    // Warten, bis Empfaenger über CTS betriebsbereit ist.
    while (!com.isCTS()) { // CTS ist NICHT blockiernd; deshalb ist eine Schleife hier notwendig.
        cout << "Warte auf RTS Signal..." << endl;
    }
    cout << "Empfaenger betriebsbereit.\n";

    cout << "Sende ACK\n";
    com.write(ACK);
    
    // 1. Variante: write(int)
    // Experimentiere: Sende eine Zahl, einen Buchstaben und einen Text.  Wie kommt dieser, warum an?
     char msg = 'X'; // Zeichen für "Abbruch"
    do {
        cout << "Sende das Byte: \n";
        cin >> msg;
        com.write(msg);
    } while (msg != 'X');

    //// 2. Variante: write(Puffer, Groesse)
    //// Experimentiere: Sende Char ohne Binäre Null, mehr bzw. weniger Zeichen als erwartet.
    //char puf[] = { 'T','E','S','T','\0'};
    //cout << "Sende Byte-Puffer (TEST)...\n";
    //com.write(puf, (int)sizeof(puf));

    //// 3. Variante: write(string)
    //// Experimentiere: Sende ohne '\n'!
    //string msg = "Hallo serielle Welt!\n";
    //do{
    //cin >> msg;
    //msg += '\n';
    //cout << "Sende String...\n";
    //com.write(msg);
    //} while (msg != "X\n"); // Warum hier NICHT "X", wie beim Empfaenger?!

    // Abschluss: ETX
    cout << "Sende ETX...\n";
    com.write(ETX);

    cout << "Uebertragung abgeschlossen.\n";
    return 0;
}

