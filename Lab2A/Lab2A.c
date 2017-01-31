/*
 * Lab2A.c
 *
 *  Created on: Jan 27, 2017
 *      Author: Mike Hopkins
 */


#include "RBELib/RBELib.h"
//For use of abs()
#include <stdlib.h>
#include "Lab2A.h"

int DAC0Val = 0;
int DAC1Val = 1023;
int direction =1;
int hzFlag = 0;
double integrationSumShoulder = 0;
double integrationSumElbow = 0;
double Kp = 0;
double Ki = 0;
double Kd = 0;
int lastPIDOutputShoulder = 0;
int lastPIDOutputElbow = 0;

void Lab2AInit()
{
	//initADC(2);
	initSPI();
	setDAC(0,600);
	//TimerInit100Hz();
	//DDRC = (1<<DDA0);
	//PORTC = 0;
}

void Lab2ALoop()
{
	setDAC(0,1023);

}

int updatePID(double desiredValue, int motor)
{
	int currentVal;
	double error;

	if(motor == SHOULDER_MOTOR)
	{
		currentVal = getADC(SHOLDER_MOTOR_ADC_CHANNEL);
		error = desiredValue - currentVal;
		integrationSumShoulder += error;
		lastPIDOutputShoulder=Kp*error+Ki*integrationSumShoulder+Kd*lastPIDOutputShoulder;
		return (int) lastPIDOutputShoulder;
	}

	if(motor == ELBOW_MOTOR)
	{
		currentVal = getADC(ELBOW_MOTOR_ADC_CHANNEL);
		error = desiredValue - currentVal;
		integrationSumElbow += error;
		lastPIDOutputElbow=Kp*error+Ki*integrationSumElbow+Kd*lastPIDOutputElbow;
		return (int) lastPIDOutputElbow;
	}
	return 0;
}


void TimerInit100Hz()
{
	//Timer Setup
	TIMSK0= (0<<OCIE0B)|
			(1<<OCIE0A)| //Enable compare A interrupt
			(0<<TOIE0);

	TCCR0A= (0<<COM0A1)|
			(0<<COM0A0)|
			(0<<COM0B1)|
			(0<<COM0B0)|
			(1<<WGM01)|
			(0<<WGM00);

	TCCR0B =(0<<FOC0A)|
			(0<<FOC0B)|
			(0<<WGM02)|
			(1<<CS02)|
			(0<<CS01)| //set clock divide to /1024
			(1<<CS00);
	OCR0A = 179; //the compare register not supposed to be binary
	sei(); //enable global interupts
}


/**
 *@brief Drives a motor based on a signed control signal between -1023 and 1023
 *
 *@param speed a value between -1023 and 1023 represented how fast the motor shoud be moved
 *@param motor a value of either 0 (SHOULDER_MOTOR) or 1 (ELBOW_MOTOR)
 */
void driveMotor(int speed,int motor)
{
	if(speed>1023)
	{
		speed=1023;
	}
	if(speed<-1023)
	{
		speed=-1023;
	}
	if((speed>=0) & (motor == SHOULDER_MOTOR))
	{
		setDAC(0,speed);
		setDAC(1,0);
	}

	if((speed<0) & (motor == SHOULDER_MOTOR))
	{
		setDAC(0,0);
		setDAC(1,speed*-1);
	}

	if((speed>=0) & (motor == ELBOW_MOTOR))
	{
		setDAC(2,speed);
		setDAC(3,0);
	}

	if((speed<0) & (motor == ELBOW_MOTOR))
	{
		setDAC(2,0);
		setDAC(3,speed*-1);
	}
}

/**
 * @brief interrupt to fire 100Hz
 */
ISR(TIMER0_COMPA_vect)
{
	hzFlag = 1;
}


/**
 *
 * @brief display 2 triangle waves phase shifted 180 degrees Part 5
 *
 */
void showTriangleWave()
{
	/*This is the code to display 2 triangle waves phase shifted 180 degrees Part 5*/
		setDAC(0,DAC0Val);
		setDAC(1,DAC1Val);
		_delay_ms(10);
		if(direction==1)
		{
			DAC0Val += 3;
			DAC1Val -= 3;

			if(DAC0Val>1024)
			{
				direction =0;
			}
		}else{
			DAC0Val -= 3;
			DAC1Val += 3;
			if(DAC0Val<0)
			{
				direction=1;
			}
		}
}

void initADC(int channel)
{

	ADMUX = (0<<REFS1)|
				(1<<REFS0)| //AVcc
				(0<<ADLAR)| //Left Adjust for 1024 Value
				(0<<MUX4)|
				(0<<MUX3)|
				(channel); //Sets MUX2:0 to proper input channel

		ADCSRA = (1<<ADEN)|
				(0<<ADSC)|
				(0<<ADATE)|
				(0<<ADIF)|
				(0<<ADIE)|
				(1<<ADPS2)|
				(1<<ADPS1)| //sets division factor to 64
				(0<<ADPS0);
		ADCSRB = 0b00000000;
		DIDR0 =  0b10000000;
		ADCSRA |= (1<<ADSC); //Starts conversion
}

unsigned short getADC(int channel)
{
	unsigned short lowByte=0;
	unsigned short highByte=0;
	unsigned short output=-55;


	ADMUX = (0<<REFS1)|
			(1<<REFS0)| //AVcc
			(0<<ADLAR)| //Left Adjust for 1024 Value
			(0<<MUX4)|
			(0<<MUX3)|
			(channel); //Sets MUX2:0 to proper input channel
	ADCSRA |= (1<<ADSC);

	while(1)//wait for conversion to complete
	{
		if((ADCSRA & 0b00010000))
		{
		lowByte = ADCL;
		highByte= ADCH;
		output = lowByte + (highByte<<8);
		//printf("Output Value: %i",output);

		return output;
		}
	}
}

double adcToDegreesArm1(short adcRead)
{
	return (adcRead * 90.0 /(ADCVERTARM1-ADCHORZARM1))-45;
}

/**
 * @brief Convert ADC value to millivolts
 *
 * @param adcRead The ADC reading given as a range between 0-1023
 *
 */
double adcToVolts(short adcRead)
{
	return adcRead*4.78; //map 0->1023 to 0-5000
}

void setDAC(int DACn, int SPIVal)
{
	if(SPIVal>1023) //Ensure that SPIVal is within constraints
	{
		SPIVal=1023;
	}
	if(SPIVal<0)
	{
		SPIVal=0;
	}

	DAC_SS_ddr = 1; //Set slave select for DAC to output
	DAC_SS = 1; //Set DAC slave select high then pull low for data transmission
	DAC_SS = 0;
	unsigned char comAddrByte;
	comAddrByte = (0b0011<<4);//Set command code to write to and update DAC
	comAddrByte |= (DACn & 0b00001111); //Add address code to send data

	unsigned char dataByteOne = SPIVal >> 2;
	unsigned char dataByteTwo = (SPIVal << 6) & 0b11000000;
	spiTransceive(comAddrByte);
	spiTransceive(dataByteOne);//send first 8 data bits
	spiTransceive(dataByteTwo);
	printf("SPIVal is %i\n\r", SPIVal);
	printf("0x%08X 0x%08X 0x%08X\n\r", comAddrByte, dataByteOne, dataByteTwo);
}

void initSPI()
{
	SPI_MISO_DDR = 0;// MISO set to input
	SPI_MISO =0; //Set MISO to low

	SPI_MOSI_DDR=1;
	SPI_MOSI =0; //MOSI set to output and low

	SPI_SCK_DDR=1;
	SPI_SCK=0; //SCK set to output and low

	DDRB |= (1<<DDB4);

	SPCR = 	 (0<<SPIE)| //Do not enable interrupt
			 (1<<SPE)|  //Enable SPI
			 (0<<DORD)|	//MSB Transmitted first
			 (1<<MSTR)|	//Select Master Mode
			 (0<<CPOL)| //Positive clock signal
			 (0<<CPHA)| //Send bit on rising edge of clock
			 (1<<SPR1)|	//Set clock to Fosc/64
			 (0<<SPR0);	//Set clock to Fosc/64

	SPSR =	(0<<SPI2X); //Do not double speed
}

unsigned char spiTransceive(BYTE data)
{
	SPDR = data;
	while(!((SPSR) & (1<<SPIF))); //Wait until data is done being sent
	printf("Data that was sent: %i\n\r",data);
	//recievedData = SPDR;
	//printf("",recievedData);
	return SPDR; //Return the data transmitted back by SPI device
}
