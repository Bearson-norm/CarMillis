// Deklarasi pin untuk ESP32

#define IN1 32   // Motor 1 A
#define IN2 33   // Motor 1 B
#define IN3 25   // Motor 2 A
#define IN4 26   // Motor 2 B
#define ENA 14   // Motor 1 Enable (PWM)
#define ENB 12   // Motor 2 Enable (PWM)

// Konfigurasi PWM ESP32
const int freq = 5000;     // Frekuensi PWM
const int resolution = 8;  // Resolusi 8-bit (0-255)
const int pwmChannelA = 0; // Channel PWM 0 untuk motor A
const int pwmChannelB = 1; // Channel PWM 1 untuk motor B

// Variabel untuk millis()
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
int currentAction = 0;
bool isRunning = false;
int currentSpeed = 0;
unsigned long actionDuration = 0;

void setupPWM() {
  // Konfigurasi PWM untuk ESP32
  ledcAttachChannel(ENA, freq, resolution, pwmChannelA);
  ledcAttachChannel(ENB, freq, resolution, pwmChannelB);
}

// Fungsi untuk mengontrol motor maju
void motorMaju(int kecepatan) {
  ledcWrite(ENA, kecepatan);  // Mengatur kecepatan motor 1
  ledcWrite(ENB, kecepatan);  // Mengatur kecepatan motor 2
  digitalWrite(IN1, HIGH);            // Motor 1 maju
  digitalWrite(IN2, LOW);             // Motor 1 maju
  digitalWrite(IN3, HIGH);            // Motor 2 maju
  digitalWrite(IN4, LOW);             // Motor 2 maju
}

// Fungsi untuk mengontrol motor mundur
void motorMundur(int kecepatan) {
  ledcWrite(ENA, kecepatan);  // Mengatur kecepatan motor 1
  ledcWrite(ENB, kecepatan);  // Mengatur kecepatan motor 2
  digitalWrite(IN1, LOW);             // Motor 1 mundur
  digitalWrite(IN2, HIGH);            // Motor 1 mundur
  digitalWrite(IN3, LOW);             // Motor 2 mundur
  digitalWrite(IN4, HIGH);            // Motor 2 mundur
}

// Fungsi untuk mengontrol motor belok kiri
void motorBelokKiri(int kecepatan) {
  ledcWrite(ENA, kecepatan);  // Mengatur kecepatan motor 1
  ledcWrite(ENB, kecepatan);  // Mengatur kecepatan motor 2
  digitalWrite(IN1, HIGH);            // Motor 1 maju
  digitalWrite(IN2, LOW);             // Motor 1 maju
  digitalWrite(IN3, LOW);             // Motor 2 mundur
  digitalWrite(IN4, HIGH);            // Motor 2 mundur
}

// Fungsi untuk mengontrol motor belok kanan
void motorBelokKanan(int kecepatan) {
  ledcWrite(ENA, kecepatan);  // Mengatur kecepatan motor 1
  ledcWrite(ENB, kecepatan);  // Mengatur kecepatan motor 2
  digitalWrite(IN1, LOW);             // Motor 1 mundur
  digitalWrite(IN2, HIGH);            // Motor 1 mundur
  digitalWrite(IN3, HIGH);            // Motor 2 maju
  digitalWrite(IN4, LOW);             // Motor 2 maju
}

// Fungsi untuk mengontrol motor mundur kiri
void motorMundurKiri(int kecepatan) {
  ledcWrite(ENA, kecepatan);  // Mengatur kecepatan motor 1
  ledcWrite(ENB, kecepatan);  // Mengatur kecepatan motor 2
  digitalWrite(IN1, LOW);             // Motor 1 mundur
  digitalWrite(IN2, HIGH);            // Motor 1 mundur
  digitalWrite(IN3, LOW);             // Motor 2 mundur
  digitalWrite(IN4, HIGH);            // Motor 2 mundur
}

// Fungsi untuk mengontrol motor mundur kanan
void motorMundurKanan(int kecepatan) {
  ledcWrite(ENA, kecepatan);  // Mengatur kecepatan motor 1
  ledcWrite(ENB, kecepatan);  // Mengatur kecepatan motor 2
  digitalWrite(IN1, HIGH);            // Motor 1 maju
  digitalWrite(IN2, LOW);             // Motor 1 maju
  digitalWrite(IN3, HIGH);            // Motor 2 maju
  digitalWrite(IN4, LOW);             // Motor 2 maju
}

// Fungsi untuk menghentikan motor
void stopMotor() {
  digitalWrite(IN1, LOW);  // Motor 1 berhenti
  digitalWrite(IN2, LOW);  // Motor 1 berhenti
  digitalWrite(IN3, LOW);  // Motor 2 berhenti
  digitalWrite(IN4, LOW);  // Motor 2 berhenti
  ledcWrite(ENA, 0); // Matikan PWM motor 1
  ledcWrite(ENB, 0); // Matikan PWM motor 2
  isRunning = false;
}

// Fungsi untuk memulai aksi motor
void startAction(int action, int kecepatan, unsigned long durasi) {
  currentAction = action;
  currentSpeed = kecepatan;
  actionDuration = durasi;
  previousMillis = millis();
  isRunning = true;
  
  switch(action) {
    case 1: motorMaju(kecepatan); break;
    case 2: motorMundur(kecepatan); break;
    case 3: motorBelokKiri(kecepatan); break;
    case 4: motorBelokKanan(kecepatan); break;
    case 5: motorMundurKiri(kecepatan); break;
    case 6: motorMundurKanan(kecepatan); break;
    default: stopMotor(); break;
  }
}

void setup() {
  // Inisialisasi pin sebagai output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Setup PWM ESP32
  setupPWM();
  
  Serial.begin(115200);
  Serial.println("ESP32 Motor Control System Ready");
}

void loop() {
  currentMillis = millis();
  
  // Jika tidak ada aksi yang berjalan, mulai urutan berikutnya
  if (!isRunning) {
    // Urutan aksi yang akan dijalankan
    static int sequenceStep = 0;
    
    switch(sequenceStep) {
      case 0: 
        startAction(1, 255, 2000);    // Maju 2 detik
        Serial.println("Maju - Kecepatan 255");
        break;
      case 1: 
        startAction(3, 200, 1000);    // Belok kiri 1 detik
        Serial.println("Belok Kiri - Kecepatan 200");
        break;
      case 2: 
        startAction(2, 150, 3000);    // Mundur 3 detik
        Serial.println("Mundur - Kecepatan 150");
        break;
      case 3: 
        startAction(4, 255, 1500);    // Belok kanan 1.5 detik
        Serial.println("Belok Kanan - Kecepatan 255");
        break;
      case 4: 
        startAction(5, 180, 2000);    // Mundur kiri 2 detik
        Serial.println("Mundur Kiri - Kecepatan 180");
        break;
      case 5: 
        startAction(6, 150, 1000);    // Mundur kanan 1 detik
        Serial.println("Mundur Kanan - Kecepatan 150");
        break;
      case 6: 
        stopMotor(); 
        delay(1000);                 // Berhenti 1 detik
        Serial.println("Berhenti");
        break;
    }
    
    sequenceStep = (sequenceStep + 1) % 7; // Loop melalui urutan
  }
  
  // Periksa apakah waktu aksi saat ini sudah selesai
  if (isRunning && (currentMillis - previousMillis >= actionDuration)) {
    stopMotor();
    Serial.println("Aksi selesai");
  }
}
