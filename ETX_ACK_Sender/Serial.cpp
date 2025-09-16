	/*
	!!! Wichtig !!!:
	Zeichensatz einstellen unter Projekteigenschaften >> Konfigurationseigenschaften >> Erweitert >> Zeichensatz 
		=> 2 Varianten
		   1.) Unicode-Zeichensatz (Standard/ CreateFileW wird genutzt)
		   2.) Multi-Byte-Zeichensatz (CreateFileA wird genutzt)
		   --> Variante 1 geht nicht <--
	*/
	
	
#include "Serial.h"

Serial::Serial(string portName, int baudrate, int dataBits, int stopBits, int parity)
{
  this->portName = portName;
  this->baudrate = baudrate;
  this->dataBits = dataBits;
  this->stopBits = stopBits;
  this->parity = parity;
  this->handle = INVALID_HANDLE_VALUE;
}

Serial::~Serial()
{
  close ();
}

// Schnittstelle öffnen
bool Serial::open() {
  handle = CreateFile(portName.c_str(),
                     GENERIC_READ | GENERIC_WRITE,
                     0,
                     0,
                     OPEN_EXISTING,
                     FILE_ATTRIBUTE_NORMAL,
                     NULL);

  if (handle == INVALID_HANDLE_VALUE)
    return (FALSE);

  DCB dcb;
  ZeroMemory (&dcb, sizeof(dcb));
  dcb.DCBlength = sizeof(DCB);

  if (!GetCommState (handle, &dcb)) {
    close ();
    return (FALSE);
  }

  dcb.BaudRate = baudrate;
  dcb.ByteSize = (BYTE)dataBits;
  dcb.StopBits = (BYTE)stopBits;
  dcb.Parity   = (BYTE)parity;

  dcb.fParity = (dcb.Parity != NOPARITY);

  if (!SetCommState(handle, &dcb)) {
    close ();
    return (FALSE);
  }

  return(TRUE);
}

// Schnittstelle schließen
void Serial::close(void)
{
  if (INVALID_HANDLE_VALUE != handle) {
    CloseHandle(handle);
    handle   = INVALID_HANDLE_VALUE;
  }
}

// Anzahl Zeichen, die sich im Empfangspuffer befinden, ermitteln
int Serial::dataAvailable()
{
  COMSTAT comStat;
  DWORD e;

  if (handle != INVALID_HANDLE_VALUE)
    if (ClearCommError(handle, &e, &comStat))
      return comStat.cbInQue;
  return 0;
}

// Byte übertragen
// value: niederwertige 8 Bit werden übertragen
void Serial::write(int value)
{
  if (INVALID_HANDLE_VALUE != handle)
    TransmitCommChar(handle, (BYTE)value);
}

// Daten übertragen
// buffer: Zeiger auf zu übertragende Daten
// bytesToWrite: Umfang der zu übertragenden Daten
void Serial::write(const char *buffer, int bytesToWrite)
{
  if (INVALID_HANDLE_VALUE != handle) {
    DWORD bytesWritten = 0;
    WriteFile(handle, buffer, bytesToWrite, &bytesWritten, NULL);
  }
}

// Text übertragen
// s: Zu sendender Text
void Serial::write(string s)
{
  DWORD bytesWritten;
  if (handle != INVALID_HANDLE_VALUE)
    WriteFile(handle, s.c_str(), s.length(), &bytesWritten, NULL);
}

int Serial::read() {
    // if (INVALID_HANDLE_VALUE == handle) //Orginalcode
    if (dataAvailable() == 0) {
       // return (-1);
    }

  DWORD dwRead = 0;
  char chRead;

  ReadFile(handle, &chRead, 1, &dwRead, NULL);
  return chRead;
}

// Daten empfangen (Empfangspuffer leeren)
// buffer: Zeiger auf Puffer, welcher die Daten übernehmen soll
// bufSize: Größe des Puffers
// Ergebnis: Anzahl Datenbytes, die tatsächlich in den Puffer geschrieben wurden
int Serial::read(char *buffer, int bufSize)
{
  if (INVALID_HANDLE_VALUE == handle) {
    return (0);
  }

  DWORD bytesRead = 0;
  char ch;
  int i = 0;

  while (ReadFile(handle, &ch, 1, &bytesRead, NULL)) {
    if (bytesRead != 1)
      break;
    buffer[i++] = ch;
    if (i == bufSize)
       break;
  }
  return (i);
}

// Text empfangen bis LF(Linefeed) empfangen
// Ergebnis: Eingelesener Text
string Serial::readLine()
{
  const int LF = 0x0A;
  int ch;
  string result = "";
  if (handle != INVALID_HANDLE_VALUE) {
    ch = read();
    while (ch != LF) {
       result += char(ch);
       ch = read();
    }
  }
  return result;
}

//-------------- Erweiterungen zum Setzen und Abfragen der Handshake-Leitungen

// setzt RTS-Leitung auf high (arg=true) oder low (aeg=false).
void Serial::setRTS(bool arg)
{
  if (handle != INVALID_HANDLE_VALUE)
    if (arg == true)
      EscapeCommFunction(handle, SETRTS);
    else
      EscapeCommFunction(handle, CLRRTS);
}

// setzt DTR-Leitung auf high (arg=true) oder low (arg=false).
void Serial::setDTR(bool arg)
{
  if (handle != INVALID_HANDLE_VALUE)
    if (arg == true)
      EscapeCommFunction(handle, SETDTR); //setzen
    else
      EscapeCommFunction(handle, CLRDTR); // löschen
}

// Modem-Statusinformationen ermitteln (hier CTS); wie getCTS
bool Serial::isCTS()
{
  DWORD status;
  GetCommModemStatus(handle, &status);
    if(status & MS_CTS_ON) 
    { 
        return true; 
    } 
   return false;
}

// Modem-Statusinformationen ermitteln (hier DSR); getDSR
bool Serial::isDSR()
{
  DWORD status;
  GetCommModemStatus(handle, &status);
    if(status & MS_DSR_ON) 
    { 
        return true; 
    } 
    return false; 
}
