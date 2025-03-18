/* ChangeLog :

 - Alpha 0.1        : Initial release
 - Alpha 0.2        : Added HighScore System
 - Alpha 0.3        : Added a Horizontal Version
 - Release 1.0 Rev0 : First Release/ improved rendering system (which is going to be further enhanced)
 - Release 1.0 Rev1 : Enhanced render system (Removed the Scan system because it's inefficient)
 - Release 1.1      : Added 2 player

*/

#include <EEPROM.h>

// Constants :

const uint8_t ROWS = 20;
const uint8_t COLS = 80;
const uint8_t NL = 10; // NewLine command
const uint8_t CR = 13; // Carriage Return command
const uint8_t ESC = 27;
const uint8_t BELL = 7;

// Variables :

uint8_t paddle1X = COLS - 3;
uint8_t paddle1Y = ROWS/2;
uint8_t ANSpaddle1X;
uint8_t ANSpaddle1Y;
uint8_t paddle2X = 3;
uint8_t paddle2Y = ROWS/2;
uint8_t ANSpaddle2X;
uint8_t ANSpaddle2Y;
uint8_t ballX = COLS - 4;
uint8_t ANSballX = 2;
uint8_t ballY = ROWS/2;
uint8_t ANSballY = 2;
int paddle1Pot;
int paddle2Pot;
int ANSpaddle1Pot;
int ANSpaddle2Pot;
int VballX = 0;
int VballY = 0;
int paddle1MoveFlag = 0;
int paddle2MoveFlag = 0;
uint8_t Speed = 20;
uint8_t Score = 0;
uint8_t Lifes = 3;
uint8_t HighScore;


// Booleans :

bool Frame = true;
bool debug = true;
bool exitloop = false;
bool start = false;
bool printstatsFlag = true;
bool Drawpaddle = true;


void setup() {
  Serial.begin(115200);
  ClearScreen();
  SetCursor(0, 0);
  Serial.println("ARDUIPONG / Ver 1.1 / 2 Player Version");
  Serial.println("Made By Abdelali221");
  Serial.print("Github : https://github.com/abdelali221/ArduiPong/");
  delay(2000);
  ClearScreen();
  PotentioRead();
}

void loop() {
  paddle1MoveFlag = 0;
  paddle2MoveFlag = 0;
  RenderGame();
  if (printstatsFlag || debug) {
    printstatsFlag = false;
    PrintGameStatus();
  }
  updateBallPos();
  delay(Speed);
}

void RenderGame(){
  if (Frame) {
    RenderBorders();
  }

  RenderObjects();
  Frame = false;
   
  if (Serial.available()) {
    SerialManage();
  }

  PotentioRead();
}

void RenderBorders() {
  for (int Y = 1; Y <= ROWS; Y++) {

    for (int X = 1; X <= COLS; X++) {

      if ( (X == 1) && (Y >= 1 && Y <= ROWS) ) {

        SetCursor(X, Y);

        if (!start) {
          delay(10);
        }

        Serial.print("|");

      } else if (Y == 1 || Y == ROWS) {
        SetCursor(X, Y);
        if (!start) {
          delay(10);
        }        

        Serial.print("-");

      }
    }
  }
}

void RenderObjects() {
  if (Drawpaddle) {
    Drawpaddle = false;
    for (int i = -1; i < 2; i++) {
      SetCursor(ANSpaddle1X, ANSpaddle1Y + i);
      Serial.print(" ");
    }
    for (int i = -1; i < 2; i++) {
      SetCursor(paddle1X, paddle1Y + i);
      Serial.print("[");
    }
    ANSpaddle1X = paddle1X;
    ANSpaddle1Y = paddle1Y;
    for (int i = -1; i < 2; i++) {
      SetCursor(ANSpaddle2X, ANSpaddle2Y + i);
      Serial.print(" ");
    }
    for (int i = -1; i < 2; i++) {
      SetCursor(paddle2X, paddle2Y + i);
      Serial.print("]");
    }
    ANSpaddle2X = paddle2X;
    ANSpaddle2Y = paddle2Y;
  }

  if (ANSballX != ballX || ANSballY != ballY) {
    SetCursor(ANSballX, ANSballY);
    Serial.print(" ");
    SetCursor(ballX, ballY);
    ANSballX = ballX;
    ANSballY = ballY;
    Serial.print("O");
  }
}

void PrintDebug() {

  Serial.print(" / POS X : ");

  if (ballX < 10) {
    Serial.print("0");
  }

  Serial.print(ballX);
  Serial.print(" / POS Y : ");

  if (ballY < 10) {
    Serial.print("0");
  }
  
  Serial.println(ballY);
  Serial.print("Vx : ");

  if (VballX >= 0) {
    Serial.print("+");
  }

  Serial.print(VballX);
  Serial.print(" / Vy : ");

  if (VballY >= 0) {
    Serial.print("+");
  }

  Serial.println(VballY);
  Serial.println(Frame);
  Serial.print("POT 1 : "); 
  Serial.print((paddle1Pot*ROWS) / 1023);
  Serial.print(" / ");
  Serial.print(paddle1Pot);
  Serial.print(" / POT 2 : ");
  Serial.print((paddle2Pot*ROWS) / 1023);
  Serial.print(" / ");
  Serial.print(paddle2Pot);

}

void updateBallPos() {
  if (!start) {
    ballX = COLS/2;
    ballY = ROWS/2;
    VballX = 0;
    VballY = 0;
  } else {
    if (ballX == COLS - 1 && VballX == 1) {
      ballX = COLS - 3;
      ballY = ROWS/2;      
      if (Lifes < 1) {
        GameOverScreen();
      } else {
        ClearScreen();
        Drawpaddle = true;
        printstatsFlag = true;
        start = false;
        Lifes--;
      }
    } else if (ballX == 2 && VballX == -1) {
      printstatsFlag = true;
      Score++;
      VballX = 1;
    } 

    if (ballY == 2) {
        VballY = 1;
    } else if (ballY == ROWS - 1) {
      VballY = -1;
    }
  
    if (ballX == paddle1X - 1 && (ballY >= paddle1Y - 1 && ballY <= paddle1Y + 1) ) {
      Serial.write(BELL);
      VballX = -1;
        if (ballY == 1) {      
        VballY = 1;
      } else if (ballY == ROWS - 1) {
        VballY = -1;
      } else {
        if (paddle1MoveFlag != 0) {
          VballY = paddle1MoveFlag;
        }
      }
    } if (ballX == paddle2X + 1 && (ballY >= paddle2Y - 1 && ballY <= paddle2Y + 1) ) {
      Serial.write(BELL);
      VballX = 1;
        if (ballY == 1) {      
        VballY = 1;
      } else if (ballY == ROWS - 1) {
        VballY = -1;
      } else {
        if (paddle2MoveFlag != 0) {
          VballY = paddle2MoveFlag;
        }
      }
    }
  }

  ballX = ballX + VballX;
  ballY = ballY + VballY;

  if (ballX > COLS - 1 || ballY > ROWS - 1) {
    ClearScreen();
    Serial.println("ERROR! INVALID BALL POSITION WAS DETECTED!!!");
    PrintDebug();
    Reset();
  }
}

void ReturnToline() {
  Serial.write(CR);
  Serial.write(NL);
}

void ClearScreen() {
  Frame = true;
  Serial.write(ESC); // ESC command (Required to send the Clear Screen instruction)
  Serial.print("[2J"); // Clears the Terminal
}
void CursorReset() {
  Serial.write(ESC); // ESC command (Required to send the Clear Screen instruction)
  Serial.print("[0;0f"); // Sets the Cursor to 0;0
}

void SetCursor(int x, int y) {
  Serial.write(ESC); // ESC command (Required to send the Clear Screen instruction)
  Serial.print("[");
  Serial.print(y);
  Serial.print(";");
  Serial.print(x);
  Serial.print("f");
}

void SerialManage() {
  if (Serial.available() > 0) {
    char chr = Serial.read();

    switch (chr) {
      case 'z':
        if (paddle1Y > 3) {
          SetCursor(paddle1X, paddle1Y + 1);
          Serial.print(" ");
          paddle1Y--;
          paddle1MoveFlag = -1;
          Drawpaddle = true;
        }
      break;

      case 's':
        if (paddle1Y < ROWS - 2) {
          SetCursor(paddle1X, paddle1Y - 1);
          Serial.print(" ");
          paddle1Y++;
          paddle1MoveFlag = 1;
          Drawpaddle = true;
        }
      break;

      case '-':
        if (Speed < 200) {
          printstatsFlag = true;
          Speed++;
        }
      break;

      case '+':
        if (Speed > 0) {
          printstatsFlag = true;
          Speed--;
        }
      break;

      case ' ':
        if (!start) {
          printstatsFlag = true;
          Drawpaddle = true;
          VballX = -1;
          VballY = -1;
          start = true;
          ClearScreen();
        } else {
          GameOverScreen();
          start = false;
        }
      break;
    }
  }
}

void GameOverScreen() {
  ClearScreen();
  CursorReset();
  HighScore = EEPROM.read(0);
  Serial.println(" Game Over!");

  if (HighScore < Score) {
    HighScore = Score;
    EEPROM.write(0, Score);
    Serial.println("New High Score!!");
  }

  Serial.print("Score : ");
  Serial.println(Score);
  Serial.print("High Score : ");
  Serial.println(HighScore);
  Serial.print("Press ENTER to restart");
  updateBallPos();
  Reset();
  exitloop = false;
}

void Reset() {

  paddle1X = COLS - 3;
  paddle1Y = ROWS/2;
  ballX = paddle1X - 1;
  ballY = paddle1Y;
  VballX = 0;
  VballY = 0;
  Score = 0;
  Lifes = 3;
  Frame = false;
  start = false;
  printstatsFlag = true;
  Drawpaddle = true;

  while (!exitloop) {
    if (Serial.available()) {
      char chr = Serial.read();

      if (chr == NL || chr == CR) {
        ClearScreen();
        exitloop = true;
      }
    }
  }
}

void PrintGameStatus() {
  SetCursor(1, ROWS + 2);

  if (!start) {
    Serial.print("Press SPACE to Start game / Score : ");
  } else {
    Serial.print("Press SPACE to end game / Score : ");
  }

  Serial.println(Score);
  Serial.print("Balls : ");

  for (int i = 0; i < Lifes; i++) {
    Serial.print("O ");
  }

  Serial.print(" / Speed : ");

  if (Speed <= 200 && Speed > 190) {
    Serial.print("00");
  } else if (Speed <= 190 && Speed > 100 ) {
    Serial.print("0");
  }

  Serial.print(200 - Speed);

  if (debug) {
    PrintDebug();
  }
}

void PotentioRead() {
  paddle1Pot = analogRead(A0);
  paddle2Pot = analogRead(A1);

  if ( (ANSpaddle1Pot + 10 <= paddle1Pot || ANSpaddle1Pot - 10 >= paddle1Pot) && (paddle1Pot > 100 && paddle1Pot < 1023) ) {
    Drawpaddle = true;
    if (paddle1Pot > 890) {
      paddle1Y = ROWS - 2;
    } else if (paddle1Pot < 160) {
       paddle1Y = 3;
    } else {
      paddle1Y = ((paddle1Pot*ROWS) / 1000);
    }    
    ANSpaddle1Pot = paddle1Pot;
  }
  if ( (ANSpaddle2Pot + 10 <= paddle2Pot || ANSpaddle2Pot - 10 >= paddle2Pot) && (paddle2Pot > 100 && paddle2Pot < 1023) ) {
    Drawpaddle = true;
    if (paddle2Pot > 890) {
      paddle2Y = ROWS - 2;
    } else if (paddle2Pot < 160) {
       paddle2Y = 3;
    } else {
      paddle2Y = ((paddle2Pot*ROWS) / 1000);
    }    
    ANSpaddle2Pot = paddle2Pot;
  }

}