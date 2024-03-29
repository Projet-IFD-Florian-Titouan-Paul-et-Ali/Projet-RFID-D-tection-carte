#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h> //dépend du modéle RFID

#define SS_PIN  10 //définition des pins
#define RTS_PIN  9 // ""

MFRC522 mfrc522(SS_PIN , RTS_PIN); //définition des pins du modèle RFID

int lock = 6; //pour une serrure électrique
//int serrure = 3; //pour serrure mécanique

//char st[20];

void setup() {
  //pinMode(serrure, OUTPUT);
  pinMode(lock, OUTPUT); 

  //initialisation série pour relevé les informations du moniteur

  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Test acces via TAG RFID");
  Serial.println();
}

void loop() {
  {
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  Serial.print("UID de tag :");
  String tag= "";
  byte caractere;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     tag.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     tag.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  tag.toUpperCase();

  // ici on va vérifier l'autorisation
  if (tag.substring(1) == "06 62 20 02") // le numero de tag est visible lorsqu on presente la carte ou le badge via le moniteur serie
  //il suffit d'insérer ci-dessus le tag que l'on souhaite authoriser ici on dit que si le tag = 06 62 20 02 est lu alors on affiche dans le moniteur serie
  // Tag verifie - Acces Autorisé et nous eteignons notre led pendant 3sec
  {
    digitalWrite(lock, HIGH);
    Serial.println("TAG verifie - Acces Autorise !");
    Serial.println();
    delay(3000);
    digitalWrite(lock, LOW);
    //dans le cas de la gache electrique
    //digitalWrite(serrure, LOW) 
    //*ou HIGH selon le mode de fonctionnement de la gache) ce qui nous laisserais 3 secondes pour ouvrir la porte
    }
 
  // ici on va vérifier un second badge (il est possible d'ajouter plusieurs "else if" il peut être interessant 
  //d'utiliser la fonction switch je vous invite à vous renseigner sur celle-ci.
 else if (tag.substring(1) == "D4 A9 42 DB") // le numero de tag est visible lorsqu on presente la carte ou le badge via le moniteur serie
  //il suffit d'insérer ci-dessus le tag que l'on souhaite authoriser ici on dit que si le tag = 06 62 20 02 est lu alors on affiche dans le moniteur serie
  // Tag verifie - Acces Autorisé et nous eteignons notre led pendant 3sec
  {
    digitalWrite(lock, HIGH);
    Serial.println("TAG verifie - Acces Autorise !");
    Serial.println();
    delay(3000);
    digitalWrite(lock, LOW);
    //dans le cas de la gache electrique
    //digitalWrite(serrure, LOW) 
    //*ou HIGH selon le mode de fonctionnement de la gache) ce qui nous laisserais 3 secondes pour ouvrir la porte
    }
  
  else
  //sinon si le Tag n'est pas valide
  {
    //on affiche Acces refuse !!!
    Serial.println("TAG inconnu - Acces refuse !!!");
    Serial.println();
    // on repete 5fois
    for (int i= 1; i<5 ; i++)
    {
      //LED clignotte ici rien à ajouter pour la gache puisque son etat ne doit pas changer la porte reste fermee
      digitalWrite(lock, HIGH);
      delay(200);
      digitalWrite(lock, LOW);
      delay(200);
    }
  }
  delay(1000);

  //CECI EST UN TEST
}
}