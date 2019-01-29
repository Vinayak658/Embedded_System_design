#include<lpc21xx.h>


int flag=0;
#define PLOCK 0x00000400
#define RS  0x00020000              /* RS  - P1.17                          */
#define RW  0X00040000              /* R/W - P1.18                          */    
#define EN  0X00080000              /* E   - P1.19                          */
#define CLR 0X00FE0000




void PORT_Initial(void)    
{
 IO1DIR  = 0x00FE0000;             			 /* LCD pins set as o/p                  */
 IO0DIR  = 0x002F0000;         
 PINSEL0 = 0x00000000;
 PINSEL1 = 0x05000000;
 PINSEL2 = 0x00000000;
}
 

int delay(unsigned int x)
{
 x=x*8000;
 while(x!=0)
 {
  x--;
 }
 return 0;
}

void LCD_Command(char command)
{
 int Temp;                           
 IO1CLR = CLR;                      /* Clearing the port pins               */
 IO1SET = EN;                       /* Enable pin high                      */
 IO1CLR = RS;                       /* RS=0 for command register            */
 IO1CLR = RW;                       /* R/W=0 for write                      */
 Temp   = (command & 0xF0) << 16;   /* Taking the first nibble of command   */
 IO1SET = IO1SET | Temp;            /* Writing it to data line              */
 delay(3);
 IO1CLR = EN;                       /* Enable pin low to give H-L pulse     */
}
 

void LCD_Command1(char command1)
{
 int Temp;
 IO1CLR = CLR;                      /* Clearing the port pins               */
 IO1SET = EN;                       /* Enable pin high                      */
 IO1CLR = RS;                       /* RS=0 for command register            */
 IO1CLR = RW;                       /* R/W=0 for write                      */
 Temp   = (command1 & 0xF0);        /* Taking the first nibble of command   */
 Temp   = Temp << 16;               /* Shift it 16 bits to left             */
 IO1SET = IO1SET | Temp;            /* Writing it to data line(P1.20-P1.23) */
 delay(3);
 IO1CLR = EN;                       /* Enable pin low to give H-L pulse     */
 
 IO1CLR = CLR;                      /* Clearing the port pins               */
 IO1SET = EN;                       /* Enable pin high                      */
 IO1CLR = RS;                       /* RS=0 for command register            */
 IO1CLR = RW;                       /* R/W=0 for write                      */
 Temp   = (command1 & 0x0F);        /* Taking the second nibble of command  */
 Temp   = Temp << 20;               /* Shift it 20 bits to left             */
 IO1SET = IO1SET | Temp;            /* Writing it to data line              */
 delay(3);
 IO1CLR = EN;                       /* Enable pin low to give H-L pulse     */
}
 

 void LCD_Data(char data)  
{
 int Temp;
 IO1CLR = CLR;                      /* Clearing the port pins               */
 IO1SET = EN;                       /* Enable pin high                      */
 IO1SET = RS;                       /* RS=1 for data register               */
 IO1CLR = RW;                       /* R/W=0 for write                      */
 Temp   = (data & 0xF0);            /* Taking the first nibble of data      */
 Temp   = Temp << 16;               /* Shift it 16 bits to left             */
 IO1SET = IO1SET | Temp;            /* Writing it to data line              */
 delay(3);
 IO1CLR = EN;                       /* Enable pin low to give H-L pulse     */
 
 IO1CLR = CLR;                      /* Clearing the port pins               */
 IO1SET = EN;                       /* Enable pin high                      */
 IO1SET = RS;                       /* RS=1 for data register               */
 IO1CLR = RW;                       /* R/W=0 for write                      */
 Temp   = (data & 0x0F);            /* Taking the second nibble of data     */
 Temp   = Temp << 20;               /* Shift it 20 bits to left             */
 IO1SET = IO1SET | Temp;            /* Writing it to data line              */
 delay(3);
 IO1CLR = EN;                       /* Enable pin low to give H-L pulse     */
}
 

void LCD_String(unsigned char *dat)
{
 while(*dat!='\0')                  /* Check for termination character      */
 {
  LCD_Data(*dat);                   /* Display the character on LCD         */
  dat++;                            /* Increment the pointer                */
 }
}
 

void LCD_Init(void)
{
 delay(15);
 LCD_Command(0x30);
 delay(15);    
 LCD_Command(0x30);
 delay(5);    
 LCD_Command(0x30);
 LCD_Command(0x20);
 LCD_Command1(0x28);
 LCD_Command1(0x01);                /* Clear display                        */
 LCD_Command1(0x06);                /* Auto increment                       */
 LCD_Command1(0x0C);                /* Cursor off                           */
 //LCD_Command1(0x80);								//cursor at home position
}


void Delay(unsigned long val)
{
 while(val>0)
 {
  val--;
 }
}
void U0Write(char data)
{
	//while ( !(U0LSR & THRE ) );           // wait till the THR is empty
	
	U0THR = data;
}

/*__irq void Ext_ISR(void) 								// Interrupt Service Routine-ISR 
{
		
	
	PLL0CON = 0x01; 
	PLL0CFG = 0x24; 
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
	while( !( PLL0STAT & PLOCK ));
	PLL0CON = 0x03;
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
  VPBDIV = 0x00; 											// PCLK is same as CCLK i.e 60Mhz
	Delay(200000);
	U0TER = (1<<7);
  
	LCD_Command1(0x8D);
	LCD_Data('w');
	IO0SET |=(1<<6);	
	flag=1;
	
	if(ILR & 0X01)
	{//ILR         = 0X01;
	 EXTINT |= 0x04;}
	
	if(EXTINT & 0x04)
	{EXTINT |= 0x04;
	//ILR         = 0X01;
		} 		 								//clear interrupt
	Delay(40000000);
	//IO0CLR |=(1<<6); //VICVectAddr = 0; 	 								// End of interrupt execution
	VICVectAddr = 0x0f;
}*/

void RTC_int(void)__irq
{
  //Delay(2000000);
  flag        = 1;                         
  LCD_Command1(0x8D);
	LCD_Data('w');
   IO0SET |=(1<<6);													//motor on
	 LCD_Command1(0xC5);
	 LCD_String("on");
  ILR         = 0X01;                      
	
 VICVectAddr = 0X00000000;
}

/*void init_ext_interrupt()  						// Initialize Interrupt
{
  EXTMODE = 0x04; 										//Edge sensitive mode on EINT2
  
  EXTPOLAR |= (0x04); 	         		 //rising Edge Sensitive
  INTWAKE |= (1<<2)| (1<<15);   		 //wake by eint2 and RTC
  PINSEL0 |= 0x0C000;            			//Select Pin function P0.7 as EINT2
  
                                            initialize the interrupt vector 
  //VICIntSelect &= ~ ((1<<16)| (1<<13));        			// EINT2 selected as IRQ 16
  //VICVectAddr5 = (unsigned int)Ext_ISR; 	// address of the ISR
  //VICVectCntl5 = (1<<5) | 16| (1<<13) ;			
  //VICIntEnable = (1<<16)|(1<<13);           			// EINT2 interrupt enabled

  //EXTINT &= ~(0x4);	
	*/

 void RTC_Init()
{
 ILR  = 0X03;                             /* Interrupt from counter increment block  */
 CCR  = 0X11;                             /* RTC Time counters enable                */
 CIIR = 0X0A;                             /* Interrupt generated in each minute and day     */
 SEC  = 0x00;                             /* Load initial value into seconds register*/
 MIN  = 0x00;                             /* Load initial value into minutes register*/
 HOUR = 0x00;                             /* Load initial value into hour register   */
 DOM  = 0x12;
 AMR = 0xFF;
 VICIntEnable = 0x00002000;               /* RTC interrupt enabled                   */
 VICVectCntl0 = 0x0000002D;               /* RTC interrupt assigned to slot6        */
 VICVectAddr0 = (unsigned)RTC_int;
}


int main()
{  PORT_Initial();
	 RTC_Init();                              /* Initialize RTC module                   */
   LCD_Init(); 
	// init_ext_interrupt();  // Initialize Interrupt
	LCD_Command1(0x80);
	LCD_String("Time");
	LCD_Command1(0xBF);
	LCD_String("pump");
	
			
	IO0DIR &= ~((1<<9));  // setting pin 9 as input
	IO0DIR |= (1<<8) ; // setting pin 8 as output 
	IO0DIR |= (1<<21);
	IO0DIR |= (1<<6) ; // setting pin 6 as output
	PINSEL1|= (1<<26);
	AD0CR = (0x1200004);
	
	//IO0DIR &= ~((1<<5));  // setting pin 5 as input
	IO0DIR &= ~((1<<14));
	int mstatus=0;
	T0CTCR = 0x0;  //initalizing the timer
	T0PR = 0x0E4E1BF; //for 1 sec delay
	T0TCR = 0x02; // reset the timer
	T0TCR = 0x00;
	int time;
	int working=0;
	int count=0;
	int previous=0;
	
	PINSEL0|= (0X05);				// prog rx and tx pins                          // programming the u0art
	U0FCR |= (0X05); 				// reset fit and enable it
	U0LCR |= (0X083); 			//8 bits data 1 stop bit no parity
	U0FDR |= (0X010);     	// setting the mulval and divval value
	U0DLL |= (0X061);
	U0DLM |=  (0X00);
	U0LCR &=  (0X03); 
	
	int reg = 0x000;
	int totseconds = 0x00;
	int minutes = 0x00;
	int remseconds = 0x00;
	int convert = 0x00;
	int convert1 = 0x00;
	char t;
	int reg2=0;
	
	
	while(1)
	{
	U0TER = (0<<7);
	time=T0TC;
	count= IO0PIN;
	count= (count & (1<<9) ); 
		
	if ((count & (1<<9)) && (working==0))
	{IO0SET |=(1<<8);
	//T0CTCR = 0x0;  //initalizing the timer
	//T0PR = 0x0E4E1BF; //for 1 sec delay
	 T0TCR = 0x02; // reset the timer
	 T0TCR = 0x01;    //enable timer
	 working=1;   		
	
	IO0SET |= (1<<21);
	Delay(20000000);
	IO0CLR |= (1<<21);
	Delay(50000000);
	
	
	}
	time=T0TC;
	count= IO0PIN;
	count= (count & (1<<9) ); 
	
	if((count & (1<<9)) && (working == 1) )
	{ if( (time<15))
		{ previous=previous+T0TC;
			T0TCR = 0x02;  //reset timer
			T0TCR = 0x01; //Enable timer
		
	// IO0SET |= (1<<21);
	 Delay(20000000);
	 //IO0CLR |= (1<<21);
	  Delay(50000000);
	
	
	
			} 
		
	}
	time=T0TC;
	count= IO0PIN;
	count= (count & (1<<9) ); 
	
	 if( (time >= 15))	
	{ if(  (~(count & (1<<9))) && (working==1) )
		{working=0;
		previous=0;
		IO0CLR |= (1<<8); 	 
		T0TCR = 0x02;  //reset timer
		T0TCR = 0x00;
		U0TER = (0<<7);
		 	 
		IO0DIR |= (1<<21);
	  IO0SET |= (1<<21);
		Delay(1000000);
		IO0CLR |= (1<<21);
		if (flag == 0)
		{
		LCD_Command1(0x8D);
		LCD_Data('n' );
		//PCON= 0x02;   //entering power saver  mode
		}
		}
	}
	if (!(0x00 & T0PC) && (T0TCR & 0x01))
			{reg = T0TC + previous ;
			if(reg2!=T0TC)
			{
			totseconds = (reg);
			minutes = totseconds / 60;
			remseconds =  totseconds%60;
			
			convert = minutes%10;				//writing minutes
			convert1 = minutes/10;
			
			convert1= convert1+48;
			t=convert1;
			LCD_Command1(0x85);
			LCD_Data(t);
			 U0Write(t);
			convert=convert+48;
			t=convert;
			LCD_Command1(0x86);
			LCD_Data(t);
			 U0Write(t);
			
			LCD_Command1(0x87);
			LCD_Data(':');
			
			U0Write(' ');								//writing seconds
			convert = remseconds%10;
			convert1 = remseconds/10;
			
			convert1= convert1+48;
			t=convert1;
			LCD_Command1(0x88);
			LCD_Data(t);
			 U0Write(t);
			convert=convert+48;
			t=convert;
			LCD_Command1(0x89);
			LCD_Data(t);
			 U0Write(t);
			  U0Write('\r');
			 reg2=T0TC;
		 }
			
	}

	
	
	mstatus= IO0PIN;
	mstatus= (mstatus & (1<<5));
	
	
		
	if (!(IO0PIN & (1<<14)) ) //manual overide of motor 
	{IO0SET |=(1<<6);													//motor on 
		flag=1;
		Delay(9000000);
		 LCD_Command1(0xC6);
		 LCD_String(" on");
	}
	if(flag==1)
	{LCD_Command1(0xC6);
	 LCD_String(" on");}
	
  	AD0CR = (0x1200004);
	 if(AD0STAT & (1<<2))
		{  unsigned int result;
			result = AD0GDR;
			result &= (0xFFC0);
			result= (result>>6);
			if(result<=(0x092))
			{IO0CLR |=(1<<6);													//motor off
	     flag=0;
			 LCD_Command1(0xC5);
			 LCD_String("off");
				
				   if (working==0)
		     { LCD_Command1(0x8D);
			     LCD_Data('n');
					//ILR         = 0X01;
			    //PCON= 0x02;   //entering power saver  mode }
		       }
			}}
			
				/*if(result>(0X36B) && result<=(0X400))
				{IO0SET |=(1<<6);													//motor on
	       flag=1;
				}
			}*/

	    totseconds = (SEC);			
			minutes =  totseconds%60;
			
			convert = minutes%10;				//writing minutes
			convert1 = minutes/10;
			
			convert1= convert1+48;
			t=convert1;
			LCD_Command1(0x8B);
			LCD_Data(t);
			 
			convert=convert+48;
			t=convert;
			LCD_Command1(0x8C);
			LCD_Data(t);
			
	
	
}
}

