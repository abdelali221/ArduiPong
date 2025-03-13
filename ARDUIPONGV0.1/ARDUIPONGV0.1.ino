const uint8_t ROWS = 20;
const uint8_t COLS = 70;
const uint8_t NL = 10; // NewLine command
const uint8_t CR = 13; // Carriage Return command
const uint8_t ESC = 27;

uint8_t ballX = COLS/2;
uint8_t ballY = ROWS/2;
int VballX = -1;
int VballY = 1;
int ScanX = 0;
int ScanY = 0;
int paddleX = COLS - 3;
int paddleY = ROWS/2;
int Speed = 100;
int Score = 0;

void setup() {
  Serial.begin(115200);
  ClearScreen();
  Serial.print("ARDUIPONG / alpha 0.1");
  delay(2000);
  ClearScreen();
}

void loop() {
  Render();
  updateBallPos();
  if (Serial.available()) {
    SerialManage();
  }
  CursorReset();
  delay(Speed);
}

void Render(){
  for (ScanY = 0; ScanY <= ROWS; ScanY++) {
    if (ScanY == 0 || ScanY == ROWS) {
      for (ScanX = 0; ScanX < COLS; ScanX++) {
        Serial.print("-");
      }
    } else {
      for (ScanX = 0; ScanX <= COLS; ScanX++) {
        if (ScanX == 0 || ScanX == COLS) {
          Serial.print("|");
        } else if (ScanX == ballX && ScanY == ballY) {
          Serial.print("#");
        } else if (ScanX == paddleX && (ScanY >= paddleY - 1 && ScanY <= paddleY + 1) ) {
          Serial.print("[");
        } else {
          Serial.print(" ");
        }
      }
    }
    ReturnToline();
  }
  Serial.print("Score : ");
  Serial.println(Score);
}

void updateBallPos() {
  if (ballX == COLS - 2 && VballX == 1) {
    LooseScreen();
  } else if (ballX == 2 && VballX == -1) {
    Score++;
    VballX = 1;
  } 

  if (ballY == 1) {
    VballY = 1;
  } else if (ballY == ROWS - 1) {
    VballY = -1;
  }
  
  if (ballX == paddleX - 1) {
    if (ballY == paddleY - 1) {
      VballX = -1;
      VballY = -1;
    } else if (ballY == paddleY) {
      VballX = -1;
      VballY = VballY;
    } else if (ballY == paddleY + 1) {
      VballX = -1;
      VballY = 1;
    }
  }
  ballX = ballX + VballX;
  ballY = ballY + VballY;
}

void ReturnToline() {
  Serial.write(CR);
  Serial.write(NL);
}

void ClearScreen() {
  Serial.write(ESC); // ESC command (Required to send the Clear Screen instruction)
  Serial.print("[2J"); // Clears the Terminal
}
void CursorReset() {
  Serial.write(ESC); // ESC command (Required to send the Clear Screen instruction)
  Serial.print("[0;0f"); // Sets the Cursor to 0;0
}

void SerialManage() {
  if (Serial.available() > 0) {
    char chr = Serial.read();
    switch (chr) {
      case 'z':
        if (paddleY > 2) {
          paddleY--;
        }
      break;

      case 's':
        if (paddleY < ROWS - 2) {
          paddleY++;
        }
      break;
    }
  }
}

void LooseScreen() {
  ClearScreen();
  Serial.println(" You Lost!");
  Serial.print("Score : ");
  Serial.println(Score);
  Serial.print("Press the Reset button to restart");
  Halt();
}

void Halt() {
  Halt();
}