/////////////////////////////////////////////////////////////
//
//          LIBRERIA I2C PER SHT71 ( RASPBERRY PI )
//
///////////////////////////////////////////////////////////
//
//	Autore	: Sergio Guastaferro
//	Data	: Maggio 2013
//
// Descrizione : Libreria per l'utilizzo dell'i2c
//               su Raspberry Pi con GPIO 

#include <stdio.h>	///  <---
#include <stdlib.h>	///  <--- LIBRERIA C di I/O LINUX
#include <unistd.h>	///  <---
#include <bcm2835.h> /// <--- LIBRERIA PROCESSORE RASPBERRY PI


/////////////////////////////////////
/// DEFINIZIONI I2C
/////////////////////////////////
//#define DATA_BIT	RPI_GPIO_P1_15
//#define CLOCK_BIT	RPI_GPIO_P1_11

#define DATA_IN		bcm2835_gpio_fsel(DATA_BIT, BCM2835_GPIO_FSEL_INPT)
#define DATA_OUT	bcm2835_gpio_fsel(DATA_BIT, BCM2835_GPIO_FSEL_OUTP);bcm2835_gpio_write(DATA_BIT, LOW)

#define DATA_LOW	bcm2835_gpio_write(DATA_BIT, LOW)
#define DATA_HIGH	bcm2835_gpio_write(DATA_BIT, HIGH)
#define DATA_READ	bcm2835_gpio_lev(DATA_BIT)

#define CLOCK_OUT	bcm2835_gpio_fsel(CLOCK_BIT, BCM2835_GPIO_FSEL_OUTP);bcm2835_gpio_write(CLOCK_BIT, LOW)
#define CLOCK_LOW	bcm2835_gpio_write(CLOCK_BIT, LOW)
#define CLOCK_HIGH	bcm2835_gpio_write(CLOCK_BIT, HIGH)

///////////////////////////////////////////////////////////////
//			ISTRUZIONE DI RITARDO!!!
///////////////////////////////////////////////////////////////
// - TESTATO ALL'OSCILLOSCOPIO CON BLINK-TEST (ON/OFF) : circa 2us
#define DELAY delayMicroseconds(0)
//////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
//			COMANDI SHT71 I2C
///////////////////////////////////////////////////////////
//	MISURA TEMPERATURA :	0000 0011 -> 0x03
//	MISURA UMIDITA :		0000 0101 -> 0x05
//	SOFT RESET :			0001 1110 -> 0x1E
#define CMD_TEMP     0x03
#define CMD_HUM      0x05

///////////////////////////////////////////////////////////////
//			VARIABILI LOCALI
////////////////////////////////////////////////////////////
char	DATA_BIT;
char	CLOCK_BIT;


////////////////////////////////////
// DICHIARAZIONE METODI
////////////////////////////////////
//static void I2C_init(char sda,char scl){}
//static void SendStart(void) {}
//static void SendReset(void) {}
//static int SendByte(unsigned char byte) {}
//static unsigned char ReadByte(int withack) {}
//static int ReadTemperature(void){}
//static int ReadHumidity(void){}



////////////////////////////////////
/// DEFINIZIONE DEI METODI
///////////////////////////////////

// -----------------------------------------------------------------------
//	INIZIALIZZA I2C
//	-Questo metodo inizializza l'I2C sul Raspberry : associa alla porta 
//	 dichiarata la funzione di DATA o SCL
static void I2C_init(char sda,char scl)
{
	DATA_BIT = sda;
	CLOCK_BIT = scl;
}

static void SendStart(void)
{
		// SCL basso e DATA alto
		CLOCK_OUT;
		DATA_OUT;
		DATA_HIGH;
		
		DELAY;
		DELAY;
		
		CLOCK_HIGH;
		DELAY;
		DELAY;
		DATA_LOW;
		DELAY;
		DELAY;
		CLOCK_LOW;

		//INTERVALLO DI START LETTO DAL SENSORE
		DELAY;
		DELAY;
		DELAY;
		DELAY;
		
		CLOCK_HIGH;
		DELAY;
		DELAY;
		DATA_HIGH;
		DELAY;
		DELAY;
		CLOCK_LOW;
		DELAY;
		DELAY;
		
		DATA_LOW;
}


// -----------------------------------------------------------------------
//	INVIA LA SEQUENZA DI RESET AL SENSORE
//	 - La sequenza di reset consiste nel mantenere la linea DATA a livello alto
//	   per un numero di 9 o più clock sulla linea SCL
static void SendReset (void)
{
	//inizializza porte
	CLOCK_OUT;
	DATA_OUT;
	
	DATA_HIGH;
	
    int k;

    for (k = 0; k < 12; k++) {
        CLOCK_HIGH;
        
		// BLOCCO DI RITARDO : RAPPRESENTA IL MANTENIMENTO ALTO DEL CLOCK
		DELAY;
		DELAY;
		DELAY;
		DELAY;
		
        CLOCK_LOW;
    }
}

// -----------------------------------------------------------------------
// INVIA UN BYTE AL SENSORE
// Ricordiamo che il dato sul bus deve essere inviato dal MSB al LSB
// Si utilizza una variabile temporanea (tempbyte) per acquisire un byte
// Si utilizza una variabile k :
//  - sia per creare un ciclo for di 8 operazioni consecutive, per i singoli bit
//  - sia per mascherare, durante il ciclo, il singolo bit del byte da inviare 
//  - vedi dimostrazione:
//
//	  XXXX XXXX
//   MSB     LSB
//   A7       A0
//          BINARY     HEX    DEC
//   A7 = 1000 0000 =  8 0  = 128
//   A6 = 0100 0000 =  4 0  = 64
//   A5 = 0010 0000 =  2 0  = 32
//   A4 = 0001 0000 =  1 0  = 16
//   A3 = 0000 1000 =  0 8  = 8
//   A2 = 0000 0100 =  0 4  = 4
//   A1 = 0000 0010 =  0 2  = 2
//   A0 = 0000 0001 =  0 1  = 1
// -----------------------------------------------------------------------
static int SendByte(unsigned char byte)
{
    unsigned char tempbyte;
    int k;
	
	//INIZIALIZZA PORTE DATA E CLOCK COME USCITE
	CLOCK_OUT;
	DATA_OUT;
    
    tempbyte = byte;
    for (k = 0x80; k > 0; k /= 2) {

        if (tempbyte & k)
            DATA_HIGH;
        else
            DATA_LOW;

        CLOCK_HIGH;
        
		//INTERVALLO DI LETTURA DEL SENSORE (DA CALIBRARE)
		DELAY;
		DELAY;
		DELAY;
		DELAY;
        
		CLOCK_LOW;
    }
    DATA_IN;
    CLOCK_HIGH;
    CLOCK_LOW;
    return 1;
}

// -----------------------------------------------------------------------
// Read a byte from the sensor
// withack
//   1 = send the ACK
//   0 = don't send the ACK
// -----------------------------------------------------------------------
static unsigned char ReadByte(int withack)
{
    unsigned char tempbyte;
    int k;
	
	//INIZIALIZZA PORTE
	DATA_IN;
	CLOCK_OUT;

    tempbyte = 0;
    for (k = 0x80; k > 0; k /= 2) {
        CLOCK_HIGH;
        if (DATA_READ)
            tempbyte |= k;
        CLOCK_LOW;
    }
    if (withack) {
        // Acknowledge del byte
        DATA_OUT;
        DATA_LOW;
        CLOCK_HIGH;
        CLOCK_LOW;
        DATA_IN;
    } else {
        // Senza acknowledge
        DATA_OUT;
        DATA_HIGH;
        CLOCK_HIGH;
        CLOCK_LOW;
        DATA_IN;
    }
    return tempbyte;
}

static int ReadTemperature(void)
{
    unsigned char Lsb, Msb, Chk;
	
	// INVIA LO START
    SendStart();
	
	// INVIA COMANDO : SE LA TX NON é ANDATA A BUONFINE, RITORNA
    if (! SendByte(CMD_TEMP))
        return 0;
	
	// ASPETTA FINTANTO CHE IL DATA SIA A LIVELLO ALTO
	//	- il sensore è in pausa per l'acquisizione della temperatura
	//	  in genere l'operazione richiede circa 80ms per la codifica a 12bit
	//	- Dopo il completamento della misura il sensore impone la linea DATA a livello basso 
    while (DATA_READ);
    
	Msb = ReadByte(1);

    Lsb = ReadByte(1);

	
    Chk = ReadByte(0); //CRC-8 CHECKSUM
	

    return (Msb << 8) + Lsb; 
}

static int ReadHumidity(void)
{
    unsigned char Lsb, Msb, Chk;

    SendStart();

    if (! SendByte(CMD_HUM))
        return 0;
    while (DATA_READ)
        ;
    Msb = ReadByte(1);

    Lsb = ReadByte(1);

	
    Chk = ReadByte(0); //CRC-8 CHECKSUM
	

    return (Msb << 8) + Lsb ;
}


///////////////////////////////////


int main(int argc, char ** argv)
{
	//INIZIALIZZA LIBRERIA PROCESSORE RASPBERRY PI
	bcm2835_init();

	//POWER ON : RPI_GPIO_P1_16 :::: dopo 11ms il sensore va in sleep mode
	bcm2835_gpio_fsel(RPI_GPIO_P1_16, BCM2835_GPIO_FSEL_OUTP);bcm2835_gpio_write(RPI_GPIO_P1_16, HIGH);
	delayMicroseconds(15000);
	
	
	int t,h;		//variabili temporanee : temperatura, umidità
    float RH, TC;	// UMIDITà RELATIVA & TEMPERATURA IN CENTIGRADI

	
	//INIZIALIZZA DATA E SCL DELLA I2C
	I2C_init( RPI_GPIO_P1_12, RPI_GPIO_P1_18 );
	
	//LEGGI I VALORI DEL'UMIDITà E TEMPERATURA
	h = ReadHumidity();
	t = ReadTemperature();
	
	//CONVERSIONE DEL SEGNALE IN USCITA
	RH = -2.0468 + (0.0367 * h) + (-1.5955E-6 * h * h);
	TC = -39.66 + (0.01 * t);
	
	//STAMPA A VIDEO IL RISULTATO
	printf ("%.2f&%.2f", RH, TC);
	
	bcm2835_close();
}
