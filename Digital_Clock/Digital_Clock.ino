

int hour = 5;
int minute = 0;
int second = 0;

int a, b;
int c, d;
int e, f;


bool flag = true;

const bool num[17][7] = {
    {1, 1, 1, 1, 1, 1, 0}, //0
    {0, 1, 1, 0, 0, 0, 0}, //1
    {1, 1, 0, 1, 1, 0, 1}, //2
    {1, 1, 1, 1, 0, 0, 1}, //3
    {0, 1, 1, 0, 0, 1, 1}, //4
    {1, 0, 1, 1, 0, 1, 1}, //5
    {1, 0, 1, 1, 1, 1, 1}, //6
    {1, 1, 1, 0, 0, 0, 0}, //7
    {1, 1, 1, 1, 1, 1, 1}, //8
    {1, 1, 1, 1, 0, 1, 1}, //9
    {1, 1, 1, 0, 1, 1, 1}, //A
    {0, 0, 1, 1, 1, 1, 1}, //B
    {1, 0, 0, 1, 1, 1, 0}, //C
    {0, 1, 1, 1, 1, 0, 1}, //D
    {1, 0, 0, 1, 1, 1, 1}, //E
    {1, 0, 0, 0, 1, 1, 1}, //F
    {0, 0, 0, 0, 0, 0, 0}  //NULL
};
void multiplax(int bit1, int bit2, int bit3, int bit4, int plexTime = 125)
{
  for (int j = 0; j < plexTime; j++)
  {

    /////////////////////////////Phase 1////////////////////////////
    digitalWrite(A1, HIGH);
    for (int i = 0; i < 7; i++)
    {
      digitalWrite(i, num[bit1][i]);
    }
    for (int i = 0; i < 7; i++)
    {
      digitalWrite(7 + i, num[bit3][i]);
    }
    delay(2);
    //all off
    for (int i = 0; i <= 7; i++)
    {
      PORTD = ~(1 << i) & PORTD;
    }
    for (int i = 0; i <= 5; i++)
    {
      PORTB = ~(1 << i) & PORTB;
    }
    //
    digitalWrite(A1, LOW);

    //////////////////////////////Phase 2//////////////////////////
    digitalWrite(A2, HIGH);
    for (int i = 0; i < 7; i++)
    {
      digitalWrite(i, num[bit2][i]);
    }
    for (int i = 0; i < 7; i++)
    {
      digitalWrite(7 + i, num[bit4][i]);
    }
    delay(2);
    //all off
    for (int i = 0; i <= 7; i++)
    {
      PORTD = ~(1 << i) & PORTD;
    }
    for (int i = 0; i <= 5; i++)
    {
      PORTB = ~(1 << i) & PORTB;
    }
    //
    digitalWrite(A2, LOW);
  }
}
void effect()
{
  for (int i = 0; i < 13; i++)
  {
    multiplax(i, i + 1, i + 2, i + 3, 200);
  }

  multiplax(16, 16, 16, 16, 150);
  multiplax(8, 8, 8, 8, 200);
  multiplax(16, 16, 16, 16, 150);
}
void setup()
{

  //Serial.begin(9600);
  for (int i = 0; i < 14; i++)
  {
    pinMode(i, OUTPUT);
  }
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, OUTPUT);
  pinMode(A5, INPUT_PULLUP);
  cli(); //stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1 = 0;  //initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624; // = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei(); //allow interrupts

} //end setup

ISR(TIMER1_COMPA_vect)
{
  flag = false;
}

void loop()
{
  int bitFlagdot=true;
  effect();
  while (true)
  {

    if (flag == false)
    {
      second++;
    }
    if (second >= 60)
    {
      second = 0;
      minute++;
    }
    if (minute >= 60)
    {
      tone(A0,1200,400);
      minute = 0;
      hour++;
    }
    if (hour >= 13)
    {
      hour = 1;
    }
    int chour = hour;
    int cminute = minute;
    int csecond = second;

    b = chour % 10;
    chour /= 10;
    a = chour % 10;

    d = cminute % 10;
    cminute /= 10;
    c = cminute % 10;

    /*f = csecond % 10;
    csecond /= 10;
    e = csecond % 10;*/

    flag = true;

    //////abcd declared here
    do
    {
      
      if (a == 0)
      {
        a = 16;
      }
      /*if (c == 0)
      {
        c = 16;
      }
      if (e == 0)
      {
        e = 16;
      }*/

      if(bitFlagdot==true)
      {
        digitalWrite(A4,HIGH);
        bitFlagdot=false;
      }else{
        digitalWrite(A4,LOW);
        bitFlagdot=true;
      }
      if(digitalRead(A3)==LOW)
      {
        hour++;
      }
      
      if(digitalRead(A5)==LOW)
      {
        minute++;
      }
      
      multiplax(a, b, c, d);
      /* Serial.print(a);
      Serial.print("-");
      Serial.print(b);
      Serial.print("-");
      Serial.print(c);
      Serial.print("-");
      Serial.print(d);
      Serial.println("-");
      delay(10); */
    } while (flag == false);
  }
}
