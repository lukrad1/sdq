/** \file uart.c
* \brief file contains uart engine and all the most important uart 
* function.
*
* \author Lukasz Radola
* \date 28.08.15
* 
*/
#ifdef __cplusplus
  extern "C" {
#endif
/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
/* std C library */
/* ... */
/* other (non local) library */
/* ... */
/* local library */

#include "gpio.h"
#include "uart.h"

/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition (local) */

/* Typedef definition (local) */


/* UART status union. In this union are uart flags for example transmision flag */
static volatile union uart__status_u
{
	uint8_t status; /* status variable, which is used to clear all uart status flag */
	struct
	{
		uint8_t timerTickFlag: 1;       
		uint8_t txBusyFlag: 1;        
		uint8_t rxBusyFlag: 1;          
		uint8_t receivedData: 1;        
		uint8_t txEnabledFlag: 1;    
		uint8_t flag5: 1;               
		uint8_t fullTransmision: 1; 		
		uint8_t flag7: 1;
	};
}
uart__status_u = {0}; /* Uart__status_u union declaraction. The variable name
                         is this same: uart__status_u. */



/* If we want to send uart message we have to in first step set uart try send flag.
It means, that the frame will be send if uart engine will be ready to send. 
This flags will be automatically clear, when the send data flag will be set. */
static volatile union uart__trySendData_u
{
	uint16_t trySendData;       
	struct
	{
		uint8_t ack: 1;  /* try send ack */
		uint8_t nack: 1; /* try send nack etc. */
		uint8_t busy: 1;
		uint8_t error: 1;
		uint8_t gpioReport: 1;
		uint8_t adcReport: 1;
		uint8_t dataReport: 1;
		uint8_t serialReport: 1;
		uint8_t idReport: 1;
		uint8_t idle: 1;
		uint8_t extended:1;
		uint8_t zwSoftwareGet:1;
		uint8_t zwSoftwareReport:1;
		uint8_t valueReport:1;
		uint8_t zw_ack:1;
		uint8_t flag15:1;

	};
} uart__trySendData_u = {0};


/* This flags are set automatically if uart engine is ready to send and try 
send flag is active. It is last stage before send message */
static volatile union uart__sendData_u
{
	uint16_t sendData;       
	struct
	{
		uint8_t ack: 1;
		uint8_t nack: 1;
		uint8_t busy: 1;
		uint8_t error: 1;
		uint8_t gpioReport: 1;
		uint8_t adcReport: 1;
		uint8_t dataReport: 1;
		uint8_t serialReport: 1;
		uint8_t idReport: 1;
		uint8_t idle: 1;
		uint8_t extended:1;
		uint8_t zwSoftwareGet:1;
		uint8_t zwSoftwareReport:1;
		uint8_t valueReport:1;
		uint8_t zw_ack:1;
		uint8_t flag15:1;
	};
} uart__sendData_u = {0};

/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */


/* Buffor with data to send */
uint8_t uart__txBuffer_t[UART__TXBUFFER];   
/* Index (help variable) in recive data array */
uint16_t uart__txDataCnt = 0;
/* Help variable which is stoped data transmision */
uint16_t uart__txDataToSend = 0;


/* below are receive data help variables */
uint8_t uart__rxBuffer_t[UART__RXBUFFER];
uint8_t uart__rxValue[UART__RXBUFFER - 6];
uint8_t uart__rxId = 0;

uint16_t uart__rxCnt = 0;
uint16_t uart__rxLen = 0;
int16_t uart__rxTimeout = 0; 
volatile static uint8_t uart__retransmisionCnt = 0;
volatile uint16_t uart__retransmisionTime = 0;
/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/
/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions 2. Local exported functions */
/* 3. Interface (exported) functions) */


/**
* @brief Wyslij ramke bez retransmisji.
* @param id - typ ramki (rodzaj wiadomosci)
* @param *data - wskaznik do tablicy z danymi do nadania
* @param dataCnt - liczba bajtow w polu ladunkowym
* @retval void
*/
static void uart__TxData(uint8_t id, uint8_t addr, uint8_t *data, uint16_t dataCnt)
{
	
}

/******************************* END FUNCTION *********************************/


/*===================== IDReport ===============================*/
static volatile union uart__IDReport
{
   uint8_t tab[2];
   struct 
   {
      uint16_t id;
   };
} uart__IDReport = {0};

void UART__SendIDReport(uint16_t id) 
{
	uart__IDReport.id = id;
	uart__trySendData_u.idReport = 1;
}

/******************************* END FUNCTION *********************************/

/*===================== ACK ===============================*/
static volatile union uart__ACK
{
   uint8_t tab[1];
   struct 
   {
      uint8_t type;
   };
} uart__ACK = {0};

void UART__SendACK(uint8_t type) 
{
	uart__ACK.type = type;
	uart__trySendData_u.ack = 1;
}

/******************************* END FUNCTION *********************************/


/*===================== NACK ===============================*/
static volatile union uart__NACK
{
   uint8_t tab[1];
   struct 
   {
      uint8_t type;
   };
} uart__NACK = {0};

void UART__SendNACK(uint8_t type) 
{
	uart__NACK.type = type;
	uart__trySendData_u.nack = 1;
}

/******************************* END FUNCTION *********************************/

/*===================== IDLE ===============================*/
static volatile union uart__IDLE
{
   uint8_t tab[1];
   struct 
   {
      uint8_t type;
   };
} uart__IDLE = {0};

void UART__SendIDLE(void) 
{
	uart__trySendData_u.idle = 1;
}

/******************************* END FUNCTION *********************************/

/*===================== ERROR ===============================*/
static volatile union uart__ERROR
{
   uint8_t tab[1];
   struct 
   {
      uint8_t error;
   };
} uart__ERROR = {0};

void UART__SendERROR(uint8_t error) 
{
	uart__ERROR.error = error;
	uart__trySendData_u.error = 1;
}

/******************************* END FUNCTION *********************************/

/*===================== BUSY ===============================*/
static volatile union uart__BUSY
{
   uint8_t tab[1];
   struct 
   {
      uint8_t type;
   };
} uart__BUSY = {0};

void UART__SendBUSY(void) 
{
	uart__trySendData_u.busy = 1;
}

/******************************* END FUNCTION *********************************/
/**
* @brief  UART transmision function.
*
* If uart send data flag is set then send message.
*/
static void UART__TransmitData(void)
{
  if (uart__sendData_u.ack) 
  {

    uart__sendData_u.ack = 0;
  }
  else if(uart__sendData_u.idReport)
  {

    uart__sendData_u.idReport = 0;
  }

}

/******************************* END FUNCTION *********************************/

/**
* @brief Ustawianie flag wyslania danych
* @param void
* @retval void
*/
static void uart__SetSendData(void)
{
	uint8_t i;

	//Sprawdzenie czy jest cos do wyslania 
  if(UART__CheckSendData())
	{
		if (!uart__sendData_u.sendData)            // Jezeli nie wysylamy danych ani potwierdzenia, czyli sendData i sendAck = 0 to
		{
			for (i = 0; i < 16; i++)
			{
				if (uart__trySendData_u.trySendData & (uint16_t)(1 << i))        // sprawdz flagi proby wyslania danych, jezeli jakas ustawiona
				{
					uart__trySendData_u.trySendData &= (uint16_t)(~(1 << i));      // wyzeruj flage proby wyslania danych
					uart__sendData_u.sendData |= (uint16_t)(1 << i);               // ustaw flage wyslania danych
					i = 16;                                                      // wyjscie z fora
				}
			}
		}
	}
}

/******************************* END FUNCTION *********************************/

void UART__Init(uint32_t baudrate)
{
  
}

/******************************* END FUNCTION *********************************/


/**
* @brief UART activation function
*
* This function activates gpio uart pins, uart clock etc.
*/
void UART__Start(void)
{

}

/******************************* END FUNCTION *********************************/


/**
* @brief UART deactivation function
*
* This function deactivates gpio uart pins, uart clock etc.
*/
/**
* @brief Zerowanie UARTu
*
* Przed kazdym uruchomieniem transmisji danych, musi nastapic deinicjalizacja (zatrzymanie) danego protokolu, by wyzerowac wszystkie rejestry i pozniej moc ustawic zadane parametry
* @param void
* @retval void
*/
void UART__Stop(void)
{
	// zerowanie flag wyslania danych
	uart__sendData_u.sendData = 0;
	uart__trySendData_u.trySendData = 0;
}

/******************************* END FUNCTION *********************************/

/**
* @brief Ustaw flage odebrania danych
*
* Zwroc wartosc flagi "odebrano ramke". Jezeli flaga byla ustawiona to zostanie ona zerowana
* Funkcja ta jest wykorzystywana w uart_Poll(void), jako pierwszy warunek wejscia do ifa w RX
* @param void
* @retval 1 jezeli flaga ustawiona, w przeciwnym wypadku zwroc 0.
*/
uint8_t UART__ReceivedDataGetFlag(void)
{
	if (uart__status_u.receivedData)
	{
		uart__status_u.receivedData = 0;
		return 0x01;
	}
	else
	{
		return 0x00;
	}
}

/******************************* END FUNCTION *********************************/

/**
* @brief Wyzeruj flage zajetosci
* @param void
* @retval void
*/
void UART__TxBusyResetFlag(void)
{
	uart__status_u.txBusyFlag = 0;
}

/******************************* END FUNCTION *********************************/

/**
* @brief Ustaw flage zajetosci
* @param void
* @retval void
*/

void UART__TxBusySetFlag(void)
{
	uart__status_u.txBusyFlag = 1;
}

/******************************* END FUNCTION *********************************/



// ***** WYKONYWANIE OPERACJI NA UARCIE ***** //

/**
* @brief Funkcja wykonywania operacji na UARCIE
*
* W tym miejscu znajduja sie wszystkie informacje dotyczace wlasciwego wyslania i odbioru danych.
* @param void
* @retval void
*/

void UART__Poll(void)
{

}

/******************************* END FUNCTION *********************************/


/**
* @brief Funkcja odmierzajaca czas retransmisji danych
*
* Uart Tick wywolywana co czas przepelnienia timera4 w przerwaniu licznika.(Aktualnie 2 ms) (Plik stm8l_15_it.c)
* Metoda realizuje funkcje timeoutu odbieranych znakow oraz odmierza czas miedzy retransmisjami
* @param void
* @retval void
*/
void UART__Tick(void)
{

}

/******************************* END FUNCTION *********************************/

/**
* @brief Funkcja odbierajace dane z bufora odbiorczego Rx wywolywana w przerwaniu UARTu.
* @param data - bajt odczytany z UARTu (Rejestr odbiorczy Rx - USART1->DR).
* @retval void
*/
void UART__RxInterrupt(uint8_t data)
{

}

/******************************* END FUNCTION *********************************/

/**
* @brief Funkcja przekazujaca dane bezposrednio do bufora nadawczego procesora.
*
* Wywolywana w przerwaniu od UART-U.
* @param void
* @retval void
*/
void UART__TxInterrupt(void)
{

}

/******************************* END FUNCTION *********************************/

/**
* @brief Funkcja zwracajaca wartosc prawda, jezeli dane sa wysylane, falsz gdy brak wysylania.
* @param void
* @retval void
*/
uint8_t UART__CheckSendData(void)
{
	if((uart__trySendData_u.trySendData)||
		(uart__sendData_u.sendData)||
		(uart__status_u.txBusyFlag))
		return 1;
	else
		return 0;
}

/******************************* END FUNCTION *********************************/

/**
* @brief Funkcja zwracajaca wartosc prawda, jezeli dane sa odbierane, falsz gdy brak odbierania.
* @param void
* @retval void
 */
uint8_t UART__CheckReceive(void)
{
    if (uart__status_u.receivedData)
			return 1;
    else
			return 0;
}

/******************************* END FUNCTION *********************************/

#ifdef __cplusplus
  }
#endif




