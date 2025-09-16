#pragma once
#include <iostream>
#include <string>
#include <list>
#include <Windows.h>

using namespace std;

class Serial  
{
private:
  // Der Port-Name (COM1, COM2)
 string portName;
  // Die Datenübertragungsrate
  int baudrate;
  // Die Anzahl der Datenbits (5 .. 8)
  int dataBits;
  // Der Anzahl der Stoppbits (ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS)
  int stopBits;
  // Festlegung der Parität (EVENPARITY, NOPARITY, ODDPARITY)
  int parity;

  /** Handle für den Com-Port */
  HANDLE handle;

public:
  Serial(string portName, int baudrate, int dataBits, int stopBits, int parity); // der Konstruktor Initialisiert die serielle Schnittstelle ohne sie zu öffnen
  ~Serial();

  bool open(); // öffnet die serielle Schnittstelle; liefert true, wenn die Schnittstelle verwendbar ist
  void close(); // schließt die serielle Schnittstelle; die Schnittstelle ist dann solange nicht mehr 

  int dataAvailable(); // liefert die Anzahl der Byte, die von der seriellen Schnittstelle gelesen werden können

  int read(); // liest ein Byte (0..255) von der serielle Schnittstelle; Ist kein Byte verfügbar, liefert sie -1
  int read(char *buffer, int bufSize); // liest mehrere Bytes von der seriellen Schnittstelle in ein Byte- Array; liefert die Anzahl der gelesenen Bytes oder -1, wenn keine Bytes verfügbar sind
  string readLine(); // liest eine Zeile von der serielle Schnittstelle; eine Zeile wird durch ein Zeilenendezeichen abgeschlossen; das Zeilenendezeichen wird jedoch nicht in den zurückgegebene String übernommen
  void write(int value); // schreibt ein Byte auf die serielle Schnittstelle; ist die Schnittstelle nicht geöffnet geschieht nichts
  void write(const char *buffer, int bytesToWrite); // schreibt mehrere Bytes auf die serielle Schnittstelle; ist die Schnittstelle nicht bereit geschieht nichts
  void write(string s); // schreibt einen String auf die serielle Schnittstelle; ist die Schnittstelle nicht geöffnet geschieht nichts 
  void setRTS(bool arg); // setzt den Modem-Steuerausgangs RTS (request to send)
  void setDTR(bool arg); // setzt den Modem-Steuerausgangs DTR (data terminal ready)
  bool isCTS(); // liefert den Status des Modem-Meldeeingangs CTS (clear to send)
  bool isDSR(); // liefert den Status des Modem-Meldeeingangs DSR (data set ready).

};
