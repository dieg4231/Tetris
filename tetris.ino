#include "SSD1306_minimal.h"
#include <avr/pgmspace.h>

#define ColCount 32
#define RowCount 16 

SSD1306_Mini oled;
uint8_t num_pza;

//Interaction memory
const int eepromAddress = 0x50;

static const byte font[] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // U+0020 (space)
0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00,    // U+002D (-)
0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00,    // U+002E (.)
0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00,    // U+002F (/)
0x7C, 0xC6, 0xCE, 0xDE, 0xF6, 0xE6, 0x7C, 0x00,    // U+0030 (0)48
0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xFC, 0x00,    // U+0031 (1)
0x78, 0xCC, 0x0C, 0x38, 0x60, 0xCC, 0xFC, 0x00,    // U+0032 (2)
0x78, 0xCC, 0x0C, 0x38, 0x0C, 0xCC, 0x00, 0x78,     // U+0033 (3)
0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x1E, 0x00,    // U+0034 (4)
0xFC, 0xC0, 0xF8, 0x0C, 0x0C, 0xCC, 0x78, 0x00,    // U+0035 (5)
0x38, 0x60, 0xC0, 0xF8, 0xCC, 0xCC, 0x78, 0x00,    // U+0036 (6)
0xFC, 0xCC, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x00,    // U+0037 (7)
0x78, 0xCC, 0xCC, 0x78, 0xCC, 0xCC, 0x78, 0x00,    // U+0038 (8)
0x78, 0xCC, 0xCC, 0x7C, 0x0C, 0x18, 0x70, 0x00,    // U+0039 (9)
0x30, 0x78, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0x00,    // U+0041 (A)65
0xFC, 0x66, 0x66, 0x7C, 0x66, 0x66, 0xFC, 0x00,    // U+0042 (B)
0x3C, 0x66, 0xC0, 0xC0, 0xC0, 0x66, 0x3C, 0x00,    // U+0043 (C)
0xF8, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00,    // U+0044 (D)
0xFE, 0x62, 0x68, 0x78, 0x68, 0x62, 0xFE, 0x00,    // U+0045 (E)
0xFE, 0x62, 0x68, 0x78, 0x68, 0x60, 0xF0, 0x00,    // U+0046 (F)
0x3C, 0x66, 0xC0, 0xC0, 0xCE, 0x66, 0x3E, 0x00,    // U+0047 (G)
0xCC, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0xCC, 0x00,    // U+0048 (H)
0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,    // U+0049 (I)
0x1E, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, 0x00,    // U+004A (J)
0xE6, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0xE6, 0x00,    // U+004B (K)
0xF0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xFE, 0x00,    // U+004C (L)
0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0x00,    // U+004D (M)
0xC6, 0xE6, 0xF6, 0xDE, 0xCE, 0xC6, 0xC6, 0x00,    // U+004E (N)
0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x00,    // U+004F (O)
0xFC, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xF0, 0x00,    // U+0050 (P)
0x78, 0xCC, 0xCC, 0xCC, 0xDC, 0x78, 0x1C, 0x00,    // U+0051 (Q)
0xFC, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0xE6, 0x00,    // U+0052 (R)
0x78, 0xCC, 0xE0, 0x70, 0x1C, 0xCC, 0x78, 0x00,    // U+0053 (S)
0xFC, 0xB4, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,    // U+0054 (T)
0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0x00,    // U+0055 (U)
0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00,    // U+0056 (V)
0xC6, 0xC6, 0xC6, 0xD6, 0xFE, 0xEE, 0xC6, 0x00,    // U+0057 (W)
0xC6, 0xC6, 0x6C, 0x38, 0x38, 0x6C, 0xC6, 0x00,    // U+0058 (X)
0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x30, 0x78, 0x00,    // U+0059 (Y)
0xFE, 0xC6, 0x8C, 0x18, 0x32, 0x66, 0xFE, 0x00,    // U+005A (Z)
0x00, 0x00, 0x00, 0x3C, 0x3C, 0xF0, 0xF0, 0x00,     // U+005A (Z)
0x00, 0x00, 0x00, 0xF0, 0xF0, 0x3C, 0x3C, 0x00,     // U+005A (Z)
0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0x00,    // U+005A (Z)
0x00, 0x00, 0x00, 0xFC, 0xFC, 0x30, 0x30, 0x00,     // U+005A (Z)
0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,    // U+005A (Z)
0x00, 0x30, 0x30, 0x30, 0x30, 0xF0, 0xF0, 0x00,     // U+005A (Z)
0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xF0, 0xF0, 0x00,    // U+005A (Z)

};

/* 
	Piezas del tetris papaw 
		para la figura z en orientacion normal seria el numero 99 que en en binario en un
		int de 4 bytes se reprecentaria 0000 0000 0110 0011 y si lo apilamos en grupos de cuatro
		veremos la figura, todas las figuras del tetris se pueden guardar en una matriz de 4x4
		por eso se representa con un int 

			 0000
			 0000
			 0110
			 0011
*/

uint16_t pz[7][4] = {
						/* Z */{99, 306},
						/* S */{54, 561},
						/* 0 */{51},
						/* T */{114, 305, 39, 562},
						/* I */{8738, 240},
						/* L */{1094, 116, 1570, 23},
						/*_| */{550, 71, 1604, 113}
					};



// Variables globales ???

int8_t numero;
int8_t indice;
uint8_t M[17]; // tablero de 8 X 16 y el reglon 17 esta todo lleno pues sera la base de nuestro tablero
uint8_t pieza[4]; // la pieza actual de juego
int8_t x, y;  // La posicion de la pieza de arriba
uint8_t next_number; // la siguiente figurita
int8_t t;


unsigned char getFlash( const unsigned char * mem, unsigned int idx  )
{
  return pgm_read_byte( &(mem[idx]) );
}
 
void printChar( char ch , byte inverse)
{
    char data[8];
    unsigned char i;
    if(ch > 64)
     	i = ch-51;  
    else
    	i = ch-44;
    
    data[0] = getFlash(font, i * 8 );
    data[1] = getFlash(font, i * 8 + 1);
    data[2] = getFlash(font, i * 8 + 2);
    data[3] = getFlash(font, i * 8 + 3);
    data[4] = getFlash(font, i * 8 + 4);
    data[5] = getFlash(font, i * 8 + 5);
    data[6] = getFlash(font, i * 8 + 6);
    data[7] = getFlash(font, i * 8 + 7);    
    
    Wire.beginTransmission(SlaveAddress);
    Wire.send(GOFi2cOLED_Data_Mode); // data mode
    for(int j=0; j<8;j++)
      Wire.send( inverse? ~data[j]:data[j] );
    Wire.endTransmission();
}

void printString( char * pText,char x,char y,byte inverse )
{
  unsigned char i;
  unsigned char len = strlen( pText );
  for (i = 0; i < len; i++)
  {
	oled.cursorTo(y,7-x-i);
	 printChar( pText[i],inverse); 
  }
}

/*
  se carga una piza en 4 bytes
  0000 0000     0000 0000
  0000 0000     0000 0000
  0000 0000     0000 0110
  0000 0000     0000 0011
*/

void load(uint8_t numero, uint8_t indice)
{
   uint8_t i; 
   for( i = 0; i < 4; i++)
   {    
   		pieza[i] = 0;
    	pieza[i] = (pz[numero][indice] >> (4 * i) ) & 15 ;
   } 
}

uint8_t setrot(int8_t xx,uint8_t y)
{ 
  int8_t i,j;
  uint8_t renglon=0;
  renglon = pieza[0]|pieza[1]|pieza[2]|pieza[3];
  if(xx<0)
  {
    for( i = 0; i > xx; i--)
    { 
      if((renglon%2)==1){ return 0;}
      renglon  >>= 1;
      pieza[0] >>= 1;
      pieza[1] >>= 1;
      pieza[2] >>= 1;
      pieza[3] >>= 1;
    }
    
    }
   else{ for( i = 0; i < xx; i++)
    { 
      if(renglon >= 128){ return 0;}
      renglon  <<= 1;
      pieza[0] <<= 1;
      pieza[1] <<= 1;
      pieza[2] <<= 1;
      pieza[3] <<= 1;
    }
   }
  for(i = y, j = 0; j < 4 ; i--, j++)
  {
    if( (M[i] & pieza[j]) != 0 )
      return 0;
  }
  for(i = y,j = 0; i > y-4 ; i--, j++)
      M[i]|=pieza[j];
  return 1;
}


void rotar()
{
  if (numero == 2) ;  
  else if( (numero == 0 || numero == 1 || numero == 4) && indice+1 > 1) indice = 0;
  else if( (numero == 3 || numero == 5 || numero == 6) && indice+1 > 3) indice = 0;
  else indice++;
  
  load(numero, indice);
 
  if(!setrot(x,y))
  {
    
    if(indice==0)
    { if (numero == 2) ;  
      else if( (numero == 0 || numero == 1 || numero == 4) ) indice = 1;
      else if( (numero == 3 || numero == 5 || numero == 6) ) indice = 3;
    }
    else
      indice--;
      
    load(numero,indice);
    if(!setrot(x,y))  delay(300);    
  }  
}

void displayblock(int8_t x ){
  char i = 0;
  int j=0;
 
  Wire.beginTransmission(SlaveAddress);
  Wire.send(GOFi2cOLED_Data_Mode);            // data mode
  //for(i=0;i<8;i++)
    if(x)
      {
      Wire.send( 0x00 );
      Wire.send( 0x7f );
      Wire.send( 0x7f );
      Wire.send( 0x7f );
      Wire.send( 0x7f );
      Wire.send( 0x7f );
      Wire.send( 0x7f );
      Wire.send( 0x7f );}
    else 
      {Wire.send( 0x00 );
      Wire.send( 0x00 );
      Wire.send( 0x00 );
      Wire.send( 0x00 );
      Wire.send( 0x00 );
      Wire.send( 0x00 );
      Wire.send( 0x00 );
      Wire.send( 0x00 );}
         
            
  Wire.endTransmission(); 
  
}


void printM()
{
	oled.cursorTo(128,0);
	unsigned char var,x ;
	unsigned char contador, inicio = 128; // 2^(8-1) = 128

  for(char j=0;j<8;j++)
	  for(char i=0;i<16;i++)
	  {
	    var = M[i];
	    if(i==0) {oled.cursorTo(8,j);continue;}
	       // El contador se desplaza un bit a la derecha cada ciclo
	          if( (128>>j) & var) // Si contador AND var == 1
	              displayblock(1);
	          else
	              displayblock(0);
	  }
}


int set(int8_t xx,int8_t y)
{
  uint8_t renglon=0;
  renglon = pieza[0]|pieza[1]|pieza[2]|pieza[3];
  
  if(xx>0)
  {
    for(uint8_t i = 0; i < xx; i++)
    { 
      if(renglon >= 128){return 0;}
      renglon  <<= 1;
      
        if( ((pieza[0] << 1) & M[y])  ) return 0;
        if( ((pieza[1] << 1) & M[y-1])  ) return 0;
        if( ((pieza[2] << 1) & M[y-2])  ) return 0;
        if( ((pieza[3] << 1) & M[y-3])  ) return 0;
      
      pieza[0] <<= 1;
      pieza[1] <<= 1;
      pieza[2] <<= 1;
      pieza[3] <<= 1;
    }
  }else
  {
    for(int8_t i = 0; i > xx; i--)
    { 
      if( (renglon % 2) == 1)return 0;
      renglon  >>= 1;

      if( ((pieza[0] >> 1) & M[y])  ) return 0;
        if( ((pieza[1] >> 1) & M[y-1])  ) return 0;
        if( ((pieza[2] >> 1) & M[y-2])  ) return 0;
        if( ((pieza[3] >> 1) & M[y-3])  ) return 0;

      pieza[0] >>= 1;
      pieza[1] >>= 1;
      pieza[2] >>= 1;
      pieza[3] >>= 1;
    }
  }

  for(int8_t i = y, j = 0; j < 4 ; i--, j++)
  {
    if( (M[i] & pieza[j]) != 0 )
      return 0;
  }
  for(int8_t i = y,j = 0; i > y-4 ; i--, j++)
      M[i]|=pieza[j];
  return 1;
  
}

void unset(int8_t y)
{
  for(int8_t i = y, j = 0 ; i > y-4; i--, j++)
      M[i] ^= pieza[j];
}

int8_t down(){
  if( set(0, y+1)  )
  { 
    y++;
    return 1;
  }
  else
  {
    set(0, y);
    return 0;
  }
}

int8_t left(){
  if( set(1, y) )
  {
    x++;
    return 1;
  }else
  {
    set(0, y);
    return 0;
  }
}

int8_t right(){
  if( set(-1, y) )
  {
    x--;
    return 1;
  }else
  {
    set(0, y);
    return 0;
  }
}

void delete_row()
{
  int8_t yy=y;
  for(int8_t j = yy; j > 0; j--)
    while(M[j] == 255)
      {
        for(int8_t i = j; i > 0; i--)
        {
          M[i]=M[i-1];
          
        } 
        M[0]=0;
      }
}

void die( unsigned char r, unsigned char c ){

  Wire.beginTransmission(SlaveAddress);
    Wire.send(GOFi2cOLED_Data_Mode);            // data mode

    Wire.send( 0xFF );
    Wire.send( 0x81 );
    Wire.send( 0x81 );
    Wire.send( 0xFF );
  Wire.endTransmission();
}




void setup()
{

  randomSeed(analogRead(3));
  pinMode(3,INPUT);//derecha
  pinMode(4,INPUT);//giro
  pinMode(1,INPUT);//izquierda
  oled.init(0x3c);
  oled.clear();
  


}

void  drawImage( const unsigned char * img, unsigned char col, unsigned char row, unsigned char w, unsigned char h ){
  unsigned int i,k,data;

  oled.cursorTo(0,0);
 //clipArea( col, row, w, h);
  for (i=0;i< (w*h);i++)
  {
      data= getFlash( img, i);
      
      Wire.beginTransmission(SlaveAddress);
      Wire.send(GOFi2cOLED_Data_Mode);            // data mode
      Wire.send( ~data );
      Wire.endTransmission();

  }

  
}


void printImage(int numImg)
{
    byte data[16];
      oled.cursorTo(0,0);
      for(int ddress=1024*numImg; ddress < (1024*numImg)+1024 ;ddress+=16)
        {
         Wire.beginTransmission(eepromAddress);
         TinyWireM.send((int)(ddress >> 8));   // MSB
         TinyWireM.send((int)(ddress & 0xFF)); // LSB
         TinyWireM.endTransmission();
         TinyWireM.requestFrom(eepromAddress,16);
         for(byte Q=0;Q<16;Q++)
            data[Q] = TinyWireM.receive();
         
         TinyWireM.beginTransmission(SlaveAddress);
         TinyWireM.send(GOFi2cOLED_Data_Mode);            // data mode
         for(byte Q=0;Q<16;Q++)
             TinyWireM.send(~data[Q]);
         TinyWireM.endTransmission();
      }
}



void loop()
{
	int f=0;
	char pzas[18];
  	char new_pza[1];
    uint8_t i;
    // Limpiado tablero
    for(i = 0; i < 16; i++)
      M[i] = 0;
    M[16]=255;
    // Mostrando tablero
    printM();
  
    numero = random(7); //  
  	indice = 0;
  	next_number = random(7); // Siguiente pieza
  	load(numero, indice);
  
  	x=3; // Cordenadas donde aparecera la pieza
  	y=3;
 
  while(digitalRead(1)==LOW)  // ANIMACION en lo que empieza el juego
  {
      printImage(0);
      delay(100);
      printImage(1);
      delay(100);
  }
  oled.clear();
  
  num_pza = 0;  
  t = 0;  // Tiempo al aumentar aumenta la velocidad del juego la medida no es tiempo

  while(set(x,y)) 
  {
    num_pza++;
    //Imprimir parte de arriba
  	sprintf(pzas,"%d",num_pza);
  	printString(pzas,0,0,0);
  	new_pza[0] = 91 + next_number;
  	printString(new_pza,4,0,0);

    do
    {
      printM();
      i = 50 - t;
      while(i--)
      {
          if(digitalRead(1))
          {
            while(digitalRead(1));
            delay(200 - 2 * t);
            if(digitalRead(1))
            { 
            	unset(y); 
              	while(down())
            	{
            		printM();
               		unset(y);
               	}
               	i = 0;
            }
            else
            {
            	unset(y);   
            	rotar();
            	printM();
            }
            delay(60);
          }
          if(digitalRead(3))
          {
            unset(y);   
            left();
            printM();
            delay(60);
          } 
          if(digitalRead(4))
          {
            unset(y);   
            right();
            printM();
            delay(60);
          }   
          delay(10);
      }
      unset(y);
    }while(down());   
    delete_row();
    numero = next_number;
    // Siguiente pieza
    next_number = random(7);

    if(num_pza%10==0 && t <=50) // Cada 10 piezas aumenta la velocidad 
      t += 5;
    indice = 0;

    // Posision nueva pieza
    x = 3;
    y = 3;
    // Carga de pieza
    load(numero, indice);
  }

  //Al perder Animacion loser 
  printImage(2);
  delay(200);
  printImage(3);
  delay(200);
  printImage(4);
  delay(200);
  printImage(5);
  
  //Mostramos scroe
  sprintf(pzas,"SCORE",num_pza);
  printString( pzas,0,110,1);
  sprintf(pzas,"%d",num_pza);
  printString( pzas,14,110,1);

  while(digitalRead(1)==LOW) // Pausa en lo que se aprienta el boton para continuar
  {
     delay(50);
  }
  

} 







/*
//Comandos de pantalla


      Wire.beginTransmission(SlaveAddress);
      Wire.send( 0x26 );
      Wire.send( 0x00 );
      Wire.send( 0x05 );
      Wire.send( 0x05 );
      Wire.send( 0x07 );
      Wire.send( 0x2F );
      Wire.endTransmission();


 	 Wire.beginTransmission(SlaveAddress);
 	 Wire.send( 0x26 );
 	 Wire.send( 0x2E );
  	 Wire.endTransmission();
	 delay(100);
	 oled.sendCommand(GOFi2cOLED_Display_Off_Cmd); 
	 delay(100);
	 oled.sendCommand(GOFi2cOLED_Display_On_Cmd);  

	 oled.sendCommand(Set_Contrast_Cmd);  

	   //contract control
 oled.sendCommand(0x00);
 delay(100);


 oled.sendCommand(Set_Contrast_Cmd);    //contract control
 oled.sendCommand(0xcf);

*/