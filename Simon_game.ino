#define READ 1
#define DO 2
#define MAX_ROUND 50
#define NUMBER_OF_ACTIONS 4

int lap = 0;
bool youLose = false;

int buttonPressed(int btn)
{
  if (Serial.available())
  {
    return Serial.read();
  }
  return -1;
}

bool led (int mode)
{
  Serial.println ("led");
}

bool relay (int mode)
{
  Serial.println ("relay");
}

bool servo (int mode)
{
  Serial.println ("servo");
}

bool sound (int mode)
{
  Serial.println ("sound");
}

void gameIsOver()
{
  Serial.println("----game is over----");
  Serial.println();
  Serial.println();
  Serial.println();
  youLose = false;
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
bool doneLapActions[MAX_ROUND] = { false };

void setup ()
{
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
          //if user pressed any button - stop checking this step
          break;
        }
      }
    }
    //if user loses - don't check next steps
    if (youLose) break;
  }

  if (youLose)
  {
    gameIsOver();
    lap = 0;
  }
}



