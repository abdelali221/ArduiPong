/* ChangeLog :

 - Alpha 0.1 : Initial release
 - Alpha 0.2 : Added HighScore System
 - Alpha 0.3 : Added a Horizontal Version

*/

#include <EEPROM.h>
// Constants :

const uint8_t ROWS = 20;
const uint8_t COLS = 50;
const uint8_t NL = 10; // NewLine command
const uint8_t CR = 13; // Carriage Return command
const uint8_t ESC = 27;
const uint8_t BELL = 7;

// Variables :

uint8_t ballX = COLS - 4;
int ANSballX;
uint8_t ballY = ROWS/2;
int ANSballY;
int VballX = -1;
int VballY = 1;
uint8_t ScanX = 0;
uint8_t ScanY = 0;
int paddleX = COLS - 3;
int paddleY = ROWS/2;
uint8_t Speed = 20;
uint8_t Score = 0;
uint8_t Lifes = 3;
uint8_t HighScore;
bool Frame = true;

// Booleans :

bool debug = false;
bool exitloop = false;
bool start = false;


void setup() {
  Serial.begin(115200);
  ClearScreen();
  SetCursor(0, 0);
  Serial.println("ARDUIPONG / alpha 0.3 / VERTICAL Version");
  Serial.println("Made By Abdelali221");
  Serial.print("Github : https://github.com/abdelali221/ArduiPong/");
  delay(2000);
  ClearScreen();
}

void loop() {
  SetCursor(0, 0);
  RenderGame();
  updateBallPos();
  delay(Speed);
}

void RenderGame(){

  for (ScanY = 0; ScanY <= ROWS; ScanY++) {
    for (ScanX = 0; ScanX <= COLS; ScanX++) {
      if ((ScanX == 0 && ScanY > 1 && ScanY < ROWS) && Frame) {
        SetCursor(ScanX, ScanY);
        Serial.print("|");
      } else if (ScanX == ballX && ScanY == ballY) {
        SetCursor(ANSballX, ANSballY);
        Serial.print(" ");
        SetCursor(ScanX, ScanY);
        ANSballX = ScanX;
        ANSballY = ScanY;
        Serial.print("O");
      } else if (ScanX == paddleX && (ScanY >= paddleY - 1 && ScanY <= paddleY + 1) ) {
        SetCursor(ScanX, ScanY);
        Serial.print("[");
      } else if ((ScanY == 0 || ScanY == ROWS) && Frame) {
        SetCursor(ScanX, ScanY);
        Serial.print("-");
      }
    }
    ReturnToline();
  }
  
  SetCursor(0, 21);
  if (!start) {
    Serial.print("Press SPACE to Start game / Score : ");
  } else {
    Serial.print("Press SPACE to end game / Score : ");
  }
  Serial.println(Score);
  Serial.print("Balls : ");
  for (int i = 0; i < Lifes; i++) {
    Serial.print("O");
  }

  Serial.print(" / Speed : ");
  if (Speed <= 100 && Speed > 90) {
    Serial.print("00");
  } else if (Speed <= 90 && Speed > 0) {
    Serial.print("0");
  }
  Serial.print(100 - Speed);
  if (debug) {
    PrintDebug();
  }
  Frame = false;
  if (Serial.available()) {
    SerialManage();
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
  Serial.print(Frame);
}

void updateBallPos() {
  if (!start) {
    ballX = paddleX - 1;
    ballY = paddleY;
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
        start = false;
        Lifes--;
      }
    } else if (ballX == 2 && VballX == -1) {
      Score++;
      VballX = 1;
    } 

    if (ballY == 2) {
        VballY = 1;
    } else if (ballY == ROWS - 1) {
      VballY = -1;
    }
  
    if (ballX == paddleX - 1 && (ballY >= paddleY - 1 && ballY <= paddleY + 1) ) {
      Serial.write(BELL);
      VballX = -1;
        if (ballY == 1) {      
        VballY = 1;
      } else if (ballY == ROWS - 1) {
        VballY = -1;
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
        if (paddleY > 3) {
          SetCursor(COLS - 3, paddleY + 1);
          Serial.print(" ");
          paddleY--;
        }
      break;

      case 's':
        if (paddleY < ROWS - 2) {
          SetCursor(COLS - 3, paddleY - 1);
          Serial.print(" ");
          paddleY++;
        }
      break;

      case '-':
        if (Speed < 100) {
          Speed++;
        }
      break;

      case '+':
        if (Speed > 0) {
          Speed--;
        }
      break;

      case ' ':
        if (!start) {
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

  paddleX = COLS - 3;
  paddleY = ROWS/2;
  ballX = paddleX - 1;
  ballY = paddleY;
  VballX = 0;
  VballY = 0;
  Score = 0;
  Lifes = 3;
  Frame = false;
  start = false;

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