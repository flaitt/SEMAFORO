volatile unsigned int cont = 0;
unsigned char estado = 0; // estado = 0: LED apagado; estado = 1: LED aceso

void setup()
{
  // Configura o pino 13, ao qual está conectado o LED da placa, como saida
  pinMode(13,OUTPUT);

  cli(); // Desabilita interrupcoes globais
  configuracao_Timer0();
  sei(); // Habilita interrupcoes globais
}

void configuracao_Timer0(){
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Configuracao Temporizador 0 (8 bits) para gerar interrupcoes periodicas a cada 8ms no modo Clear Timer on Compare Match (CTC)
  // Relogio = 16e6 Hz
  // Prescaler = 1024
  // Faixa = 125 (contagem de 0 a OCR0A = 124)
  // Intervalo entre interrupcoes: (Prescaler/Relogio)*Faixa = (64/16e6)*(124+1) = 0.008s
  
  // TCCR0A – Timer/Counter Control Register A
  // COM0A1 COM0A0 COM0B1 COM0B0 – – WGM01 WGM00
  // 0      0      0      0          1     0
  TCCR0A = 0x02;

  // OCR0A – Output Compare Register A
  OCR0A = 124;

  // TIMSK0 – Timer/Counter Interrupt Mask Register
  // – – – – – OCIE0B OCIE0A TOIE0
  // – – – – – 0      1      0
  TIMSK0 = 0x02;
  
  // TCCR0B – Timer/Counter Control Register B
  // FOC0A FOC0B – – WGM02 CS02 CS01 CS0
  // 0     0         0     1    0    1
  TCCR0B = 0x05;
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

// Rotina de servico de interrupcao
ISR(TIMER0_COMPA_vect){
  // Insira aqui o codigo a ser executado pela interrupcao periodica
  cont++;  
}

void loop()
{
  // Se o LED esta apagado a 25*8=200ms ou mais, acende o LED
  if ((estado==0)&&(cont>=25)) {
    cont = 0;
    digitalWrite(13, HIGH);
    estado = 1;
  }
  
  // Se o LED esta aceso a 25*8=200ms ou mais, apaga o LED
  if ((estado==1)&&(cont>=25)) {
    cont = 0;
    digitalWrite(13, LOW);
    estado = 0;
  }
}