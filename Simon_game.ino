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

bool led(int mode)
{
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
}

bool relay(int mode)
{
  digitalWrite(RELAY_PIN, HIGH);
  delay(500);
  digitalWrite(RELAY_PIN, LOW);
}

bool servo(int mode)
{
  servoAction.write(100);
  delay(500);
  servoAction.write(40);
}

bool sound (int mode)
  tone(SOUND_PIN, 1000, 500);
  delay(500);
}

void gameIsOver()
{
  for(int i = 0; i < 3; ++i)
  {
    servoAction.write(100);
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    tone(SOUND_PIN, 1000, 500);
    delay(500);
    servoAction.write(40);
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    delay(500);
  }
  delay(2000);
}

typedef bool (*PointerFunction) (int);

struct Action
{
  PointerFunction act;
  int sensorPin;
};

Action doActionsArray[NUMBER_OF_ACTIONS] =
{
  {led, LED_SENSOR_PIN},
  {relay, RELAY_SENSOR_PIN},
  {servo, SERVO_SENSOR_PIN},
  {sound, SOUND_SENSOR_PIN}
};

PointerFunction lapActions[MAX_ROUND];

void setup ()
{
  pinMode(SOUND_PIN, OUTPUT);
  digitalWrite(SOUND_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  servoAction.attach(SERVO_PIN);

  pinMode(SOUND_SENSOR_PIN, INPUT_PULLUP);
  pinMode(LED_SENSOR_PIN, INPUT_PULLUP);
  pinMode(RELAY_SENSOR_PIN, INPUT_PULLUP);
  pinMode(SERVO_SENSOR_PIN, INPUT_PULLUP);

  delay(2000);

  gameIsOver();
}

void loop () {

  lapActions[lap] = doActionsArray[rand() % 4].act;
  (lap == MAX_ROUND) ? lap = 0 : ++lap;

  //show sequence to repeat
  for (int i = 0; i < lap; ++i)
  {
    lapActions[i](DO);
    delay(500);
  }

  //handle user actions

  //for each step
  for (int i = 0; i < lap; ++i)
  {
    bool nextStep = false;

    //wait until any activity
    while (!youLose && !nextStep)
    {
      //check every button
      for (int j = 0; j < NUMBER_OF_ACTIONS; ++j)
      {
        //if button is pressed
        if(digitalRead(doActionsArray[j].sensorPin) == 0)
        {
          //do action for this button
          doActionsArray[j].act(DO);
          //go to next step if is it correct button. otherwise, game is over
          (doActionsArray[j].act == lapActions[i]) ? nextStep = true : youLose = true;
          //anyway break this step
          break;
        }
      }
    }
    //if user loses - don't check next steps
    if (youLose) break;
  }
  
  //I'm sorry );
  if (youLose)
  {
    gameIsOver();
    lap = 0;
    youLose = false;
  }
  delay(1000);
}



