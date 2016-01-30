#include <Servo.h>

#define READ 1
#define DO 2
#define MAX_ROUND 50
#define NUMBER_OF_ACTIONS 4

#define LED_PIN 4
#define RELAY_PIN 2
#define SERVO_PIN 5
#define SOUND_PIN 3

#define LED_SENSOR_PIN 6
#define RELAY_SENSOR_PIN 9
#define SERVO_SENSOR_PIN 7
#define SOUND_SENSOR_PIN 8

Servo servoAction;

int lap = 0;
bool youLose = false;

int buttonPressed(int btn)
{
  if (Serial.available())
  {
    return Serial.read();
  }
  if(digitalRead(LED_SENSOR_PIN) == 0) return 49;
  if(digitalRead(RELAY_SENSOR_PIN) == 0) return 50;
  if(digitalRead(SERVO_SENSOR_PIN) == 0) return 51;
  if(digitalRead(SOUND_SENSOR_PIN) == 0) return 52;
  return -1;
}

bool led (int mode)
{
  Serial.println ("led");
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
}

bool relay (int mode)
{
  Serial.println ("relay");
  digitalWrite(RELAY_PIN, HIGH);
  delay(500);
  digitalWrite(RELAY_PIN, LOW);
}

bool servo (int mode)
{
  Serial.println ("servo");
  servoAction.write(100);
  delay(500);
  servoAction.write(40);
}

bool sound (int mode)
{
  Serial.println ("sound");
  tone(SOUND_PIN, 1000, 500);
}

void gameIsOver()
{
  Serial.println();
  Serial.println("----game is over----");
  Serial.println();
  Serial.println();
  delay(3000);
}

typedef bool (*GeneralFunction) (int);

struct Action
{
  GeneralFunction act;
  int sensor;
};

Action doActionsArray[NUMBER_OF_ACTIONS] =
{
  {led, 49},
  {relay, 50},
  {servo, 51},
  {sound, 52}
};

Action lapActions[MAX_ROUND];

void setup ()
{
  pinMode(SOUND_PIN, OUTPUT);
  digitalWrite(SOUND_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  servoAction.attach(5);
  servoAction.write(40);

  pinMode(SOUND_SENSOR_PIN, INPUT_PULLUP);
  pinMode(LED_SENSOR_PIN, INPUT_PULLUP);
  pinMode(RELAY_SENSOR_PIN, INPUT_PULLUP);
  pinMode(SERVO_SENSOR_PIN, INPUT_PULLUP);
  
  Serial.begin (115200);
  delay(2000);
}

void loop () {

  lapActions[lap] = doActionsArray[rand() % 4];
  (lap == MAX_ROUND) ? lap = 0 : ++lap;

  Serial.print ("lap ");
  Serial.println (lap);

  //show sequence to repeat
  for (int i = 0; i < lap; ++i)
  {
    lapActions[i].act(DO);
    delay(500);
  }

  //handle user actions

  //for each step
  for (int i = 0; i < lap; ++i)
  {
    Serial.println("repeat sequence");
    bool nextStep = false;

    //wait until any activity
    while (!youLose && !nextStep)
    {
      //check every button
      for (int j = 0; j < NUMBER_OF_ACTIONS; ++j)
      {
        //button is pressed?
        int btn = buttonPressed(doActionsArray[j].sensor);
        //Yes!
        if (btn != -1)
        {
          //button correct - check next step, no - user lose!
          (btn == lapActions[i].sensor) ? nextStep = true : youLose = true;
          lapActions[i].act(DO);
          //if user pressed any button - stop checking this step
          break;
        }
      }
    }
    //if user loses - don't check next steps
    if (youLose) break;
  }
  //I'm sorry
  if (youLose)
  {
    gameIsOver();
    lap = 0;
    youLose = false;
  }
}



