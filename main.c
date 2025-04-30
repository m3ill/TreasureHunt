//Mertcan İlhan 230101052

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <math.h>
#include <string.h>

#define UNEXPLORED '?'
#define VISITED '='
#define PLAYER 'P'
#define TREASURE 'T'
#define TRAP 'X'
#define SIZETABLE 5

typedef struct {
    char name[20];
    int LocX, LocY;
    int health;
    int FoundTrasure;
    int move;
    int point;
}PlayerProp;

typedef struct {
    char name[20];
    int moves;
}PlayerScore;

void CreateTable(char*** Table, char*** VisiableTable);
void DisplayTable(char **TableVisiable, PlayerProp* Player);
void PlaceTrasure(char*** Table);
void PlaceTrap(char*** Table);
void PlayerMovement(char*** TableVisiable,char **Table,PlayerProp* Player);
void SavePointDatas(char playerName[20],int move);
void CheckIsFinished(PlayerProp *Player);
void DisplayHint(char **table,PlayerProp Player);
void DisplayLeaderboard();
void SaveGameState(char **,char **, PlayerProp*);
void LoadGameState(char*** Table,char*** TableVisiable,PlayerProp* Player);
void GamePrepare(char*** Table,char*** TableVisiable);

int main(int argc, char const *argv[]) {

    PlayerProp player;
    player.LocX = 0;
    player.LocY = 0;
    player.move = 0;
    player.FoundTrasure = 0;
    player.health = 1;


    char** Table;
    char** TableVisiable;


    if (argc == 1) {
        printf("Please provide minimum one parameters\n");
        exit(1);
    }
    strcpy (player.name, argv[1]);


        if (argc == 2) {
            GamePrepare(&Table, &TableVisiable);
        }
        else if (strcmp(argv[2] ,"leaders")== 0) {
            DisplayLeaderboard();
            GamePrepare(&Table,&TableVisiable);
        }
        else if (strcmp(argv[2] ,"load")== 0)
            LoadGameState(&Table,&TableVisiable,&player);
        else {
            GamePrepare(&Table,&TableVisiable);
        }




    printf("your name is %s\n", player.name);



    DisplayTable(TableVisiable,&player);

    while (1) {
        DisplayHint(Table,player);
        PlayerMovement(&TableVisiable,Table,&player);
        player.move++;
        printf("\n");
        DisplayTable(TableVisiable,&player);
        printf("\n");
        CheckIsFinished(&player);
        printf("press p to save game and exit\n");
    }



    return 0;
}


void CreateTable(char ***Table, char*** VisiableTable) {
    //defining Background Table's size
    *Table = (char**)malloc(SIZETABLE * sizeof(char*));
    for (int i = 0; i < 5; i++) {
        (*Table)[i] = (char*)malloc(SIZETABLE * sizeof(char));
    }

    //defining Front Table's size
    *VisiableTable = (char**)malloc(SIZETABLE * sizeof(char*));
    for (int i = 0; i < 5; i++) {
        (*VisiableTable)[i] = (char*)malloc(SIZETABLE * sizeof(char));
    }


    //fill the background table
    for (int i = 0; i < SIZETABLE; i++ ) {
        for (int j = 0; j < SIZETABLE; j++ ) {
            (*Table)[i][j] = VISITED;
        }
    }

    //fill the front table
    for (int i = 0; i < SIZETABLE; i++ ) {
        for (int j = 0; j < SIZETABLE; j++ ) {
            if (i == 0 && j == 0)
                (*VisiableTable)[i][j] = VISITED;
            else
                (*VisiableTable)[i][j] = UNEXPLORED;
        }
    }

    //print table for test


}
void DisplayTable(char **TableVisiable, PlayerProp* Player) {
    for (int i = 0; i < SIZETABLE; i++) {
        for (int j = 0; j < SIZETABLE; j++) {
            if (Player->LocX == j && Player->LocY == i) {
                printf(" %c " , PLAYER);
            }else {
                printf(" %c " , TableVisiable[i][j]);
            }
        }
        printf("\n");
    }
}
void PlaceTrasure(char*** Table) {
    int TrasureX,TrasureY;
    srand(time(NULL));
    for (int i = 0; i < 3; i++) {
        do {
            TrasureX = rand() % SIZETABLE;
            TrasureY = rand() % SIZETABLE;
        }while((*Table)[TrasureX][TrasureY] != VISITED || (TrasureX == 0 && TrasureY == 0));
        (*Table)[TrasureX][TrasureY] = TREASURE;
    }
}
void PlaceTrap(char*** Table) {
    int TrapX,TrapY;
    srand(time(NULL));
    for (int i = 0; i < 3; i++) {
        do {
            TrapX = rand() % SIZETABLE;
            TrapY = rand() % SIZETABLE;
        }while((*Table)[TrapX][TrapY] != VISITED || (TrapX == 0 && TrapY == 0));
        (*Table)[TrapX][TrapY] = TRAP;
    }
}
void PlayerMovement(char*** TableVisiable,char** Table,PlayerProp* Player) {
    char keyCode;
    int tempX, tempY;
        printf("You can move with (w/a/s/d) button");
        keyCode = getch();

        switch (keyCode) {
            case 'a':
                if (Player->LocX > 0)
                    Player->LocX--;
                break;
            case 's':
                if (Player->LocY < SIZETABLE - 1)
                    Player->LocY++;
                break;
            case 'd':
                if (Player->LocX < SIZETABLE - 1)
                    Player->LocX++;
                break;
            case 'w':
                if (Player->LocY > 0)
                    Player->LocY--;
                break;
            case 'p':
                SaveGameState(Table,*TableVisiable,Player);
                printf("%s's game is saved successfully",Player->name);
                exit(0);

        }

    if (Table[Player->LocY][Player->LocX] == TRAP && (*TableVisiable)[Player->LocY][Player->LocX] != TRAP)
        Player->health--;
    else if(Table[Player->LocY][Player->LocX] == TREASURE && (*TableVisiable)[Player->LocY][Player->LocX] != TREASURE)
        Player->FoundTrasure++;

    (*TableVisiable)[Player->LocY][Player->LocX] = Table[Player->LocY][Player->LocX];

    printf("Founded Trasure : %d \n",Player->FoundTrasure);


}
void CheckIsFinished(PlayerProp* Player) {
    if (Player->FoundTrasure == 3) {
        printf("Player wins\n");
        printf("%s finished in %d moves",Player->name,Player->move);
        SavePointDatas(Player->name,Player->move);
        DisplayLeaderboard();
        getchar();
        exit(0);
    }else if (Player->health == 0) {
        printf("Player Defeated");

        getchar();
        exit(0);
    }
}
void SavePointDatas(char playerName[20],int move) {
    FILE* fp;
    fp = fopen("leaderboard.txt","a");
    if(fp == NULL) {
        printf("Error in opening file");
        exit(0);
    }

    fprintf(fp,"%s %d\n",playerName,move);
    fclose(fp);
}
void DisplayHint(char **table,PlayerProp Player) {

    int amountTrap = 0,amountTreasure = 0;
    int dx[4] = {-1,1,0,0};
    int dy[4] = {0,0,-1,1};

    for (int i = 0; i < 4; i++) {
        int nx = Player.LocX + dx[i];
        int ny = Player.LocY + dy[i];
        if (nx >= 0 && nx < SIZETABLE && ny >= 0 && ny < SIZETABLE) {
            if (table[ny][nx] == TREASURE) {
                amountTreasure++;
            }else if (table[ny][nx] == TRAP) {
                amountTrap++;
            }
        }
    }


    printf("there are %d traps and %d treasure around Player.\n", amountTrap, amountTreasure);
}
void DisplayLeaderboard() {

    PlayerScore scores[100];
    FILE* fp = fopen("leaderboard.txt","r+");
    if(fp == NULL) {
        printf("Error in opening file");
        exit(0);
    }
    int count = 0;

    while (fscanf(fp,"%s %d\n",scores[count].name,&scores[count].moves) == 2) {
        count++;
        if(count >= 100) break;
    }

    fclose(fp);

    for (int i = 0; i < count-1; i++) {
        for (int j = i+1; j < count; j++) {
            if (scores[i].moves > scores[j].moves) {
                int temp = scores[i].moves;
                scores[i].moves = scores[j].moves;
                scores[j].moves = temp;
            }
        }
    }

    printf("\n=== Leaderboard ===\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s - %d move\n", i + 1, scores[i].name, scores[i].moves);
    }
    printf("========================\n");
}
void SaveGameState(char** Table, char **visiableTable,PlayerProp* player) {
    char filename[30];
    sprintf(filename,"%s_Save.txt",player->name);

    FILE* fp = fopen(filename,"w");
    if(fp == NULL) {
        printf("Error in opening file");
        exit(0);
    }

    fprintf(fp,"%s\n",player->name);
    fprintf(fp,"%d %d\n",player->LocX,player->LocY);
    fprintf(fp,"%d\n",player->FoundTrasure);

    //save unvisiable table
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            fprintf(fp,"%c ",Table[i][j]);
        }
        fprintf(fp,"\n");
    }
    //save visiable Table
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            fprintf(fp,"%c ",visiableTable[i][j]);
        }
        fprintf(fp,"\n");
    }

    fclose(fp);
    fscanf(stdout,"Game saved succesfully");
}
void LoadGameState(char*** Table,char*** TableVisiable,PlayerProp* Player) {
    *Table = (char**)malloc(SIZETABLE * sizeof(char*));
    for (int i = 0; i < 5; i++) {
        (*Table)[i] = (char*)malloc(SIZETABLE * sizeof(char));
    }

    //defining Front Table's size
    *TableVisiable = (char**)malloc(SIZETABLE * sizeof(char*));
    for (int i = 0; i < 5; i++) {
        (*TableVisiable)[i] = (char*)malloc(SIZETABLE * sizeof(char));
    }

    char filename[30];
    sprintf(filename,"%s_Save.txt",Player->name);

    FILE* fp = fopen(filename,"r");
    if (fp == NULL) {
        printf("Error in opening file");
        exit(0);
    }

    char savedPlayer[20];
    fscanf(fp,"%s",Player->name);
    fscanf(fp, "%d %d", &Player->LocX, &Player->LocY);
    fscanf(fp, "%d ", &Player->FoundTrasure);

    fscanf(fp,"\n");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            fscanf(fp,"%c ", &((*Table)[i][j]));
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            fscanf(fp, "%c ",&((*TableVisiable)[i][j]));
        }
    }
    fclose(fp);
    printf("%s 's game loaded succesfully\n",Player->name);
}
void GamePrepare(char*** Table,char*** TableVisiable){
    CreateTable(Table, TableVisiable);
    PlaceTrasure(Table);
    PlaceTrap(Table);
}