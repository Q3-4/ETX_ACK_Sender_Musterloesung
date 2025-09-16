#include "Serial.h"

using namespace std;

bool sender(Serial*);

int main()
{
	string port_nr = "";
	cout << "COM Port Nummer: ";

	cin >> port_nr;
	//port_nr = "1";
	
	string port("COM");
	port += port_nr;
	Serial* com = new Serial((string)port, 9600, 8, NOPARITY, ONESTOPBIT);

	sender(com);

	system("pause");
	return 0;
}

bool sender(Serial* com)
{
	const int ACK = 0x06; // Nullmodem-Kabel
	const int ETX = 0x03;

	// Test mit String und Char-Arrays
	string string_text = "Hallo Welt!"; //für write(s: String):void
	char char_text = {'\0'};			//für >> write(value: int):void UND write(b: byte[], len: int):void
	
	
	char* ack = "6";			//Variante für Virtual Serial Port Tool, da der write-Befehl mit einem Buchstaben nicht funktioniert
	char* etx = "3";

	if (com->open())
	{
		cout << "Sender gestartet!" << endl;

		while (!com->isCTS()) { cout << "Warte auf RTS Signal" << endl; }
		cout << "Empf\x84nger betriebsbereit" << endl;

		cout << "Warte auf ACK" << endl;
 		while (com->read() != *ack);  // Variante mit Virtual Serial Port Tool
		//while (comm->read() != ACK); // Variante mit Nullmodel-Kabel der Schule

		do {
			cout << "ACK erhalten" << endl;
			cout << "Nachricht: " << endl;
			
			//cin >> char_text;

			// EXTRA aus der Aufgabenstellung zu entnehmen, wenn Nullmodem Kabel unterbrochen wird.
			while (!com->isCTS()) {
				cout << "Leitung unterbrochen!" << endl;
				com->close();
				return false;
			}
			// 1. Methode write():int
			// nicht möglich mit virtuellem Nullmodem!!!!
				//comm->write((int)char_text);
				//comm->write(ETX);
	
		
			// 2. Methode write(Ziel, Zielgroesse):int
			//comm->write(char_text,100); //für >> write(value: int):void UND write(b: byte[], len: int):void
			//comm->write(etx);	

			// 3. Methode write(s: string)
			cin >> string_text;
			com->write(string_text + '\n'); // Muss mit einem Zeilenendezeichen abgeschlossen werden.
			com->write(etx, strlen(etx));  // Variante mit virtuellem Nullmodem

			//comm->write(etx);				// Variante mit Nullmodel-Kabel
			//comm->write('2');
			// 
			if (char_text == 'X' || string_text == "X") break;
			//if (char_text[0] == 'X') break;


			cout << "Warte auf ACK" << endl;
		//} while (x == ACK); // Variante mit Nullmodel-Kabel
		} while (com->read() == *ack);  // Variante mit virtuellem Nullmodem

		while (com->isCTS()); 
		cout<<"Empf\x84nger nicht mehr betriebsbereit"<<endl;

		com->close(); 
		cout << "\x9a \bbertragung beendet." << endl;
	}
	else
	{
		cout<<"Fehler beim \x99\bffnen"<<endl;
	}
	return true;
}
