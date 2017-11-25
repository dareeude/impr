#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define FILENAME "data.txt"
#define ENTRIES 790
#define LINELENGTH 150
#define NAMELENGTH 50
#define SECSINHOUR 3600
#define SECSINMIN 60

typedef struct {
  char raceName[NAMELENGTH];  /* name of race, probably enumerate this */
  char firstName[NAMELENGTH]; /* only lower case and capitalized */
  char lastName[NAMELENGTH]; /* specified as all caps in file */
  int age;
  char team[NAMELENGTH];
  char nationality[NAMELENGTH]; /* 3-char */
  int placement; /* 0 should reflect DNF*/
  char raceTime[NAMELENGTH]; /* create function for representing secs as nicely formatted time */
  int raceTimeSec;
  int points;
} entry;

int readData(entry data[]);
void printData(entry data[]);
int convertTime(char string[]);
void printRange(entry data[], int entries, int age, char nationality[]);
/* point system
2 points for entering into any race
(ridersFinished - placement in race)/17 points, if race is finished without dnf and otl
1st +8, 2nd +5 and 3rd +3 points */
void calculatePoints(entry data[]);
void splitNames(entry data[], char string[]);
int enumeratePlacement(char string[]);

/* assignments
  (X) find and print all BEL below 23 years
  ( ) find and print all danish racers that have attended one or more races. Sort these after teams, secondly alphabetically on firstName
  ( ) print the 10 highest scoring riders, sort by points, secondly by age (youngest), thirdly alphabetically on lastName
  ( ) find, for each race, the team with most riders DNF or OTL
  ( ) find the nation, that did best in the races. sort by own choice
  ( ) find, in each race, the meadian raceTime, without DNF or OTL, a higher time is preferred, relative to M, than a lower time.
*/

int main(int argc, char *argv[]) {
  /* should use malloc() */
  entry data[ENTRIES];
  int entries = readData(data), optAge;
  char option, optNation[2], testString[] = "Ben O'CONNOR";

  if (argc > 2) {
    printf("Too many arguments supplied!\n");
    return EXIT_FAILURE;
  } else if(argc == 2 && !strcmp(argv[1], "--print")) {
      printRange(data, entries, 23, "BEL");
  } else {
    printf("1: Print all entries\n");
    printf("2: Find racer by nationality and max-age\n");
    printf("3: Debug some stuff\n");
    printf("Choose an option: ");
    scanf(" %c", &option);
    if (option == '1') {
      printData(data);
    }
    if (option == '2') {
      printf("Choose nationality, e.g. DEN or GBR: ");
      scanf(" %s", optNation);
      printf("Choose max-age: ");
      scanf(" %d", &optAge);
      printRange(data, entries, optAge, optNation);
    } if (option == '3') {
      splitNames(data, testString);
    }
  }

  return EXIT_SUCCESS;
}

int readData(entry data[]) {
  int i=0;
  char buffer[LINELENGTH], placement[5];
  FILE *ifp;
  ifp = fopen(FILENAME, "r");

  while(fgets(buffer, LINELENGTH, ifp) != NULL) {
    sscanf(buffer, "%s \" %s %[^\"]\" %d %[A-Z] %[A-Z] %[0-9DNFOTL] %[0-9:]",
    data[i].raceName,
    data[i].firstName,
    data[i].lastName,
    &(data[i].age),
    data[i].team,
    data[i].nationality,
    placement, /* replaced with placeholder */
    data[i].raceTime);

    data[i].placement = enumeratePlacement(placement);
    data[i].raceTimeSec = convertTime(data[i].raceTime);
    i++;
  }
  fclose(ifp);
  return i;
}

int enumeratePlacement(char string[]) {
  if (!strcmp(string, "DNF")) return 0;
  else if (!strcmp(string, "OTL")) return -1;
  else return atoi(string);
  return -2;
}

void splitNames(entry data[], char string[]) {
  int i=0, n=1, stringLength = strlen(string);
  char string1[20], string2[20];

  for (i=0; i<=stringLength; i++)
  if( (isupper(string[i]) && ((isupper(string[n]) || string[n] == '\'' )) ) ) {
    strncpy(string1, string, i);
    string1[i] = '\0';
    strcpy(string2, &string[i]);
    break;
  } else {
    n++;
  }
}

void printData(entry data[]) {
  int i;
  printf("##### PRINTING DATA #####\n");
  for (i=0; i<ENTRIES; i++) {
    printf("raceName: %s\n", data[i].raceName);
    printf("firstName: %s\t", data[i].firstName);
    printf("lastName: %s\t", data[i].lastName);
    printf("age: %d\n", data[i].age);
    printf("team: %s\t", data[i].team);
    printf("nationality: %s\t", data[i].nationality);
    printf("placement: %d\t", data[i].placement);
    printf("raceTime: %s\t", data[i].raceTime),
    printf("raceTime: %d\n\n", data[i].raceTimeSec);
  }
}

void printRange(entry data[], int entries, int age, char nationality[]) {
  int i;
  printf("matching for age: %d and nationality: %s\n", age, nationality);
  for (i=0; i<entries; i++) {
    if ((data[i].age < age) && !strcmp(data[i].nationality, nationality)) {
      printf("match found for: %s %s, age %d from %s\n", data[i].firstName, data[i].lastName, data[i].age, data[i].nationality);
    }
  }
}

int convertTime(char string[]) {
  int h=0, m=0, s=0;
  sscanf(string, "%d:%d:%d", &h, &m, &s);
  return h*SECSINHOUR + m*SECSINMIN + s;
}