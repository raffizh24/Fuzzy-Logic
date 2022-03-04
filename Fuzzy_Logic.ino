#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Fuzzy.h>
//WiFi
char auth[] = "SlDaMbUvN8Y_dwKtUQBXElUPEfYA5dIy";
char ssid[] = "Raffi";
char pass[] = "Rn123456";
//Sensor
#define DHTPIN D2          
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Sensor tidak terbaca!");
    return;
  }
  Fuzzy *fuzzy = new Fuzzy();
  // FuzzyInput Suhu
  FuzzySet *dingin = new FuzzySet(0, 0, 15, 25);
  FuzzySet *normal = new FuzzySet(20, 25, 25, 30);
  FuzzySet *panas = new FuzzySet(25, 35, 50, 50);
  // FuzzyInput Kelembaban
  FuzzySet *kering = new FuzzySet(0, 0, 15, 35);
  FuzzySet *seimbang = new FuzzySet(25, 50, 50, 75);
  FuzzySet *basah = new FuzzySet(65, 85, 100, 100);
  // FuzzyOutput AC
  FuzzySet *ACmati = new FuzzySet(0, 0, 0.25, 0.5);
  FuzzySet *ACnyala = new FuzzySet(0.5, 0.75, 1, 1);
  // FuzzyOutput Kipas
  FuzzySet *KAmati = new FuzzySet(0, 0, 0.25, 0.5);
  FuzzySet *KAnyala = new FuzzySet(0.5, 0.75, 1, 1);
  // FuzzyInput Suhu
  FuzzyInput *suhu = new FuzzyInput(1);
  suhu->addFuzzySet(dingin);
  suhu->addFuzzySet(normal);
  suhu->addFuzzySet(panas);
  fuzzy->addFuzzyInput(suhu);
  // FuzzyInput Kelembaban
  FuzzyInput *kelembaban = new FuzzyInput(2);
  kelembaban->addFuzzySet(kering);
  kelembaban->addFuzzySet(seimbang);
  kelembaban->addFuzzySet(basah);
  fuzzy->addFuzzyInput(kelembaban);
  // FuzzyOutput
  FuzzyOutput *AC = new FuzzyOutput(1);
  AC->addFuzzySet(ACmati);
  AC->addFuzzySet(ACnyala);
  fuzzy->addFuzzyOutput(AC);
  // FuzzyOutput
  FuzzyOutput *KA = new FuzzyOutput(2);
  KA->addFuzzySet(KAmati);
  KA->addFuzzySet(KAnyala);
  fuzzy->addFuzzyOutput(KA);
  // Building FuzzyRule1
  FuzzyRuleAntecedent *IFdinginANDkering = new FuzzyRuleAntecedent();
  IFdinginANDkering->joinWithAND(dingin,kering);
  FuzzyRuleConsequent *thenACmatiKAmati1 = new FuzzyRuleConsequent();
  thenACmatiKAmati1->addOutput(ACmati);
  thenACmatiKAmati1->addOutput(KAmati);
  FuzzyRule *fuzzyRule1 = new FuzzyRule(1, IFdinginANDkering, thenACmatiKAmati1);
  fuzzy->addFuzzyRule(fuzzyRule1);
  // Building FuzzyRule2
  FuzzyRuleAntecedent *IFdinginANDseimbang2 = new FuzzyRuleAntecedent();
  IFdinginANDseimbang2->joinWithAND(dingin,seimbang);
  FuzzyRuleConsequent *thenACmatiKAmati2 = new FuzzyRuleConsequent();
  thenACmatiKAmati2->addOutput(ACmati);
  thenACmatiKAmati2->addOutput(KAmati);
  FuzzyRule *fuzzyRule2 = new FuzzyRule(2, IFdinginANDseimbang2, thenACmatiKAmati2);
  fuzzy->addFuzzyRule(fuzzyRule2);
  // Building FuzzyRule3
  FuzzyRuleAntecedent *IFdinginANDbasah3 = new FuzzyRuleAntecedent();
  IFdinginANDbasah3->joinWithAND(dingin,basah);
  FuzzyRuleConsequent *thenACmatiKAnyala3 = new FuzzyRuleConsequent();
  thenACmatiKAnyala3->addOutput(ACmati);
  thenACmatiKAnyala3->addOutput(KAnyala);
  FuzzyRule *fuzzyRule3 = new FuzzyRule(3, IFdinginANDbasah3, thenACmatiKAnyala3);
  fuzzy->addFuzzyRule(fuzzyRule3);
  // Building FuzzyRule4
  FuzzyRuleAntecedent *IFnormalANDkering4 = new FuzzyRuleAntecedent();
  IFnormalANDkering4->joinWithAND(normal,kering);
  FuzzyRuleConsequent *thenACmatiKAmati4 = new FuzzyRuleConsequent();
  thenACmatiKAmati4->addOutput(ACmati);
  thenACmatiKAmati4->addOutput(KAmati);
  FuzzyRule *fuzzyRule4 = new FuzzyRule(4, IFnormalANDkering4, thenACmatiKAmati4);
  fuzzy->addFuzzyRule(fuzzyRule4);
  // Building FuzzyRule5
  FuzzyRuleAntecedent *IFnormalANDseimbang5 = new FuzzyRuleAntecedent();
  IFnormalANDseimbang5->joinWithAND(normal,seimbang);
  FuzzyRuleConsequent *thenACmatiKAmati5 = new FuzzyRuleConsequent();
  thenACmatiKAmati5->addOutput(ACmati);
  thenACmatiKAmati5->addOutput(KAmati);
  FuzzyRule *fuzzyRule5 = new FuzzyRule(5, IFnormalANDseimbang5, thenACmatiKAmati5);
  fuzzy->addFuzzyRule(fuzzyRule5);
  // Building FuzzyRule6
  FuzzyRuleAntecedent *IFnormalANDbasah6 = new FuzzyRuleAntecedent();
  IFnormalANDbasah6->joinWithAND(normal,basah);
  FuzzyRuleConsequent *thenACmatiKAnyala6 = new FuzzyRuleConsequent();
  thenACmatiKAnyala6->addOutput(ACmati);
  thenACmatiKAnyala6->addOutput(KAnyala);
  FuzzyRule *fuzzyRule6 = new FuzzyRule(6, IFnormalANDbasah6, thenACmatiKAnyala6);
  fuzzy->addFuzzyRule(fuzzyRule6);
  // Building FuzzyRule7
  FuzzyRuleAntecedent *IFpanasANDkering = new FuzzyRuleAntecedent();
  IFpanasANDkering->joinWithAND(panas,kering);
  FuzzyRuleConsequent *thenACnyalaKAmati = new FuzzyRuleConsequent();
  thenACnyalaKAmati->addOutput(ACnyala);
  thenACnyalaKAmati->addOutput(KAmati);
  FuzzyRule *fuzzyRule7 = new FuzzyRule(7, IFpanasANDkering, thenACnyalaKAmati);
  fuzzy->addFuzzyRule(fuzzyRule7);
  // Building FuzzyRule8
  FuzzyRuleAntecedent *IFpanasANDseimbang8 = new FuzzyRuleAntecedent();
  IFpanasANDseimbang8->joinWithAND(panas,seimbang);
  FuzzyRuleConsequent *thenACnyalaKAmati8 = new FuzzyRuleConsequent();
  thenACnyalaKAmati8->addOutput(ACnyala);
  thenACnyalaKAmati8->addOutput(KAmati);
  FuzzyRule *fuzzyRule8 = new FuzzyRule(8, IFpanasANDseimbang8, thenACnyalaKAmati8);
  fuzzy->addFuzzyRule(fuzzyRule8);
  // Building FuzzyRule9
  FuzzyRuleAntecedent *IFpanasANDbasah9 = new FuzzyRuleAntecedent();
  IFpanasANDbasah9->joinWithAND(panas,basah);
  FuzzyRuleConsequent *thenACnyalaKAnyala9 = new FuzzyRuleConsequent();
  thenACnyalaKAnyala9->addOutput(ACnyala);
  thenACnyalaKAnyala9->addOutput(KAnyala);
  FuzzyRule *fuzzyRule9 = new FuzzyRule(9, IFpanasANDbasah9, thenACnyalaKAnyala9);
  fuzzy->addFuzzyRule(fuzzyRule9);
  fuzzy->setInput(1, t);
  fuzzy->setInput(2, h);
  fuzzy->fuzzify();
  Serial.print("Suhu Sensor = ");
  Serial.print(t);
  Serial.print(", Kelembaban Sensor =");
  Serial.println(h);
  float outputAC = fuzzy->defuzzify(1);
  float outputKA = fuzzy->defuzzify(2);
  Serial.print("Status: ");
  Serial.print(" AC = ");
  Serial.print(outputAC);
  Serial.print(", Kipas = ");
  Serial.println(outputKA);
  String statusAC;
  String statusKipas;
  if(outputAC>0.5){
    digitalWrite(D3, LOW);
    statusAC="ON";
  }else{
    digitalWrite(D3, HIGH);
    statusAC="OFF";
  }
  if(outputKA>0.5){
    digitalWrite(D4, LOW);
    statusKipas="ON";
  }else{
    digitalWrite(D4, HIGH);
    statusKipas="OFF";
  }
  Blynk.virtualWrite(V3, statusAC);
  Blynk.virtualWrite(V4, statusKipas);
  Blynk.virtualWrite(V5, t); //suhu virtual 5
  Blynk.virtualWrite(V6, h); //kelembaban virtual 6
}
void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  timer.setInterval(5000L, sendSensor);
}
void loop()
{
  Blynk.run();
  timer.run();
}
