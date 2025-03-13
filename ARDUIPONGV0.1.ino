const uint8_t ROWS = 20;
const uint8_t COLS = 50;
const uint8_t NL = 10; // NewLine command
const uint8_t CR = 13; // Carriage Return command
const uint8_t ESC = 27;

uint8_t ballX = COLS/2;
uint8_t ballY = ROWS/2;
int VballX = 1;
int VballY = 0;
int ScanX = 0;
int ScanY = 0;
int paddleX = COLS - 3;
int paddleY = ROWS/2;
int Speed = 50;


void setup() {
  Serial.begin(115200);
  ClearScreen();
  Serial.print("ARDUIPONG / alpha 0.1");
  delay(5000);
  ClearScreen();
}

void loop() {
  Render();
  updateBallPos();
  if (Serial.available()) {
    SerialManage();
  }
  ClearScreen();
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
  Serial.print(ballX);
  Serial.println(VballX);
  Serial.print(ballY);
  Serial.println(VballY);
}

void updateBallPos() {
  if (ballX == COLS - 2 && VballX == 1) {
    VballX = -1;
  } else if (ballX == 2 && VballX == -1) {
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
  Serial.print("[2J"); // Clear the terminal
}

void SerialManage() {
  char chr = Serial.read();

  switch (chr) {
    case 'z':
      paddleY--;
    break;

    case 's':
      paddleY++;
    break;
  }
}