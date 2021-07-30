#define DEBUG
#define PNP

#define ARDUINOJSON_ENABLE_PROGMEM 0
#include <ArduinoJson.h>

#include "Pulse.h"
#include "StepperDriver.h"
#include "StepperLocator.h"
#include "doublecheck.h"
#include "src/encoder/encoder.h"
#include "src/pid/persist-target.h"
#include "src/pid/position.h"
#include "src/pid/velocity.h"

const auto MAX_MAIN_ARM_SPEED = 25000L;
const auto MIN_MAIN_ARM_LOC = 3156L;
const auto MAX_MAIN_ARM_LOC = 28760L;
const auto HALF_MAIN_ARM_LOC = (MIN_MAIN_ARM_LOC + MAX_MAIN_ARM_LOC) / 2;

const auto mainLimiter = PB5, verticalLimiter = PB3, horizontalLimiter = PB4;

StepperDriver mainArm(PB1, PB0);
StepperDriver conveyor(PA3, PA2);
StepperLocator verticalArm(PB6, PB7);
StepperLocator horizontalArm(PB8, PB9);

encoder<int32_t> mainArmEnc(PA7, PA6);
encoder<int32_t> conveyorEnc(PA1, PA0);

position_pid<double> outer;
velocity_pid<double> inner(0, -1, 0);

auto &HostSerial = Serial;

#ifdef DEBUG
auto &DebugSerial = Serial3;
#endif

double realMainArmLoc = 0, conveyorLoc = 0,
       wantMainArmLoc = MIN_MAIN_ARM_LOC * 2;

bool echo = false;

StaticJsonDocument<200> doc;

double i, o, spd;

#ifdef PNP
bool pluging = false;
#endif

void onMainArmChangeA() { mainArmEnc.onChangeA(); }
void onMainArmChangeB() { mainArmEnc.onChangeB(); }
void onConveyorPtChangeA() { conveyorEnc.onChangeA(); }
void onConveyorPtChangeB() { conveyorEnc.onChangeB(); }

void pid_isr() {
  int32_t mainArmLoc = mainArmEnc.read();
#ifdef PNP
  if (!pluging) {
#endif
    auto diff = (realMainArmLoc - mainArmLoc) * 1000;

    o = outer(mainArmLoc, wantMainArmLoc);
    i = inner(diff, o);

    spd = i;

    if (realMainArmLoc < MIN_MAIN_ARM_LOC) {
      mainArm.speed(-MAX_MAIN_ARM_SPEED);
    } else if (realMainArmLoc > MAX_MAIN_ARM_LOC) {
      mainArm.speed(MAX_MAIN_ARM_SPEED);
    } else {
      mainArm.speed(constrain(spd, -MAX_MAIN_ARM_SPEED, MAX_MAIN_ARM_SPEED));
    }
#ifdef PNP
  }
#endif
  realMainArmLoc = mainArmLoc;
}

void setup() {
  // Setup limiters
  pinMode(mainLimiter, INPUT_PULLUP);
  pinMode(verticalLimiter, INPUT_PULLUP);
  pinMode(horizontalLimiter, INPUT_PULLUP);

  // Setup encoders
  attachInterrupt(mainArmEnc.A, onMainArmChangeA, CHANGE);
  attachInterrupt(mainArmEnc.B, onMainArmChangeB, CHANGE);

  attachInterrupt(conveyorEnc.A, onConveyorPtChangeA, CHANGE);
  attachInterrupt(conveyorEnc.B, onConveyorPtChangeB, CHANGE);

  // Setup Timer 2
  Timer2.pause();
  Timer2.setPeriod(10);  // in microseconds
  Timer2.refresh();

  // Setup Pulse ISR on Channel 1 @ 100kHz
  Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  Timer2.setCompare(TIMER_CH1, 1);
  Timer2.attachInterrupt(TIMER_CH1, Pulse::isr);

  // Update frequency in Pulse
  Pulse::updateGlobalfrequency(1000000 / 10 / 1);

  Timer2.resume();

  HostSerial.begin(115200);
#ifdef DEBUG
  DebugSerial.begin(115200);
#endif

  mainArm.setSpeed(MAX_MAIN_ARM_SPEED);
  verticalArm.setSpeed(1000);
  horizontalArm.setSpeed(1000);

  // Reset
  uint8_t reset_state = 0b000;
  mainArm.cw();
  verticalArm.ccw();
  horizontalArm.cw();

  while (0b111 != reset_state) {
    if (bitRead(reset_state, 0) == 0 && LOW == doublecheck(mainLimiter)) {
      mainArm.step(0);
      mainArmEnc.reset();
      bitSet(reset_state, 0);
    }

    if (bitRead(reset_state, 1) == 0 && LOW == doublecheck(verticalLimiter)) {
      verticalArm.step(0);
      verticalArm.updateLocation(0);
      bitSet(reset_state, 1);
    }

    if (bitRead(reset_state, 2) == 0 && LOW == doublecheck(horizontalLimiter)) {
      horizontalArm.step(0);
      horizontalArm.updateLocation(0);
      bitSet(reset_state, 2);
    }
  }

  // Move to mid point
  mainArm.ccw();

  while (mainArmEnc.read() < MIN_MAIN_ARM_LOC) {
    /* empty */;
  }

  mainArm.step(0);

  // Setup Timer 2
  Timer2.pause();

  // Setup PID updater on Channel 2 @ 1kHz
  Timer2.setMode(TIMER_CH2, TIMER_OUTPUTCOMPARE);
  Timer2.setCompare(TIMER_CH2, 100);
  Timer2.attachInterrupt(TIMER_CH2, pid_isr);

  Timer2.resume();
}

void loop() {
  if (HostSerial.available()) {
#ifdef DEBUG
    DeserializationError error = deserializeJson(doc, HostSerial);
#else
    DeserializationError error = deserializeMsgPack(doc, HostSerial);
#endif

    if (!error) {
      JsonObject msg = doc.as<JsonObject>();

      if (!msg.isNull()) {
        if (msg.containsKey("setpid")) {
#ifdef PNP
          pluging = true;
          outer = position_pid<double>(msg["setpid"][0].as<double>(),
                                       msg["setpid"][1].as<double>(),
                                       msg["setpid"][2].as<double>());
          inner = velocity_pid<double>(msg["setpid"][3].as<double>(),
                                       msg["setpid"][4].as<double>(),
                                       msg["setpid"][5].as<double>());
          pluging = false;
#endif
        }

        if (msg.containsKey("setwantpt")) {
          wantMainArmLoc = msg["setwantpt"].as<signed int>();
        }

        if (msg.containsKey("setmainarmspd")) {
          mainArm.setSpeed(msg["setmainarmspd"]);
        }

        if (msg.containsKey("setmainarmstep")) {
          mainArm.step(msg["setmainarmstep"].as<signed int>());
        }

        if (msg.containsKey("setconveyorspd")) {
          conveyor.speed(msg["setconveyorspd"]);
        }

        if (msg.containsKey("setconveyorstep")) {
          conveyor.step(msg["setconveyorstep"].as<signed int>());
        }

        if (msg.containsKey("setecho")) {
          echo = msg["setecho"];
        }
      }
#ifdef DEBUG
      else {
        DebugSerial.println(F("document is not an object"));
      }
#endif

      doc.clear();
    }
#ifdef DEBUG
    else {
      DebugSerial.print(F("deserializeJson() failed: "));
      DebugSerial.println(error.c_str());
    }
#endif
  }

  if (echo) {
    doc["now"] = millis();
    doc["realpt"] = realMainArmLoc;
    doc["conveyorspd"] = conveyorEnc.read();

#ifdef DEBUG
    serializeJson(doc, HostSerial);
#else
    serializeMsgPack(doc, HostSerial);
#endif

    HostSerial.println();
    doc.clear();
  }
  // cutPt += 20;
}
