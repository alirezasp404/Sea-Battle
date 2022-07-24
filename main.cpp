#include <iostream>
#include <fstream>
#include <cstring>
#include<ctime>

using namespace std;
struct Players {
    int shipsCoordinate[16][4];
    char name[15];
    char map[16][16];
    char result_Map[16][16];
    int targets;
    int score;
    int which_Row;
} first, second;
int selectedNumber;
bool firstCall = true;
bool exitFromGame = false;
int mapSize = 10;
int numOfUsers;
int numOfUsersInStart;
int numOfShips = 10;
int numOfCoordinate = 0;
int x, y, x1, y3, x2, y2;
int largestShip = 5;
int ships[10] = {5, 3, 3, 2, 2, 2, 1, 1, 1, 1};

void displayMenu(char[][15], int []);

void settings();

void displayMap(char[][16], bool);

void availableUsers(char [][15], int [], char [], const string &);

void putShips(const string &firstOrSecond, char[][16], int[][4]);

bool checkCoordinate(int &, char [][16], int [][4], bool);

void putShipsAuto(char [][16], int [][4]);

void playWithFriend(int &, char [], char[][16], char[][16], int &, int [][4]);

void startGame();

void exitGame(char [][15], int []);

int main() {
    startGame();
    return 0;
}

void startGame() {
    ifstream getUser;
    getUser.open("usersInfo.txt");
    if (!getUser.is_open()) {
        cout << "Error in opening file!";
        exit(0);
    }
    getUser >> numOfUsersInStart;
    numOfUsers = numOfUsersInStart;
    char usersInfo[numOfUsersInStart][15];
    int usersScore[numOfUsersInStart];
    for (int c = 1; c <= numOfUsersInStart; c++)
        getUser >> usersInfo[c] >> usersScore[c];
    getUser.close();

    for (int i = 0; i < numOfShips; i++)
        numOfCoordinate += ships[i];
    displayMenu(usersInfo, usersScore);

    while (true) {
        cout << "1.start game\n2.return to main menu\nEnter a number to continue :";
        cin >> selectedNumber;
        if (selectedNumber == 1 || selectedNumber == 2)
            break;
        else
            cout << "Wrong number!\n";
    }
    if (selectedNumber == 2)
        displayMenu(usersInfo, usersScore);
    first.targets = numOfCoordinate, second.targets = numOfCoordinate;
    first.score = 0, second.score = 0;
        while (true) {
            if (first.targets == 0) {
                cout << endl << first.name << " WON\n";
                second.score /= 2;
                usersScore[first.which_Row] += first.score;
                usersScore[second.which_Row] += second.score;
                break;
            } else if (second.targets == 0) {
                cout << endl << second.name << " WON\n";
                second.score /= 2;
                usersScore[second.which_Row] += second.score;
                usersScore[first.which_Row] += first.score;
                break;
            }
            playWithFriend(first.targets, first.name, second.result_Map, second.map, first.score,
                           second.shipsCoordinate);
            if (exitFromGame)
                break;
            playWithFriend(second.targets, second.name, first.result_Map, first.map, second.score,
                           first.shipsCoordinate);
            if (exitFromGame)
                break;
        }
        if (!exitFromGame) {
            cout << first.name << " you got " << first.score << " score";
            cout << second.name << " you got " << second.score << " score";
        }
    exitGame(usersInfo, usersScore);
}

void displayMenu(char usersInfo[][15], int usersScore[]) {
    string first_Or_Second = "First";
    int t;
    if (firstCall) {
        cout << char(201);
        for (t = 0; t < 43; t++)
            cout << char(205);
        cout << char(187) << endl;
        cout << "\t\tSEA BATTLE\n";
        firstCall = false;
    }
    cout << endl << "1.Play with a friend";
    cout << endl << "2.Settings";
    cout << endl << "3.Score board";
    cout << endl << "4.Exit";
    cout << endl << "Enter a number to continue :";
    cin >> selectedNumber;
    while (selectedNumber < 1 || selectedNumber > 4) {
        cout << "Please enter a number between 1-4 :";
        cin >> selectedNumber;
    }
    switch (selectedNumber) {
        case 1:
            for (t = 0; t < 2; t++, first_Or_Second = "Second") {
                while (true) {
                    cout << endl << first_Or_Second
                         << " player\n1.Choose from available users\n2.New user\nEnter a number to continue :";
                    cin >> selectedNumber;
                    if (selectedNumber == 1 || selectedNumber == 2)
                        break;
                    else
                        cout << "Wrong number!\n";
                }
                if (first_Or_Second == "First") {
                    availableUsers(usersInfo, usersScore, first.name, "First");
                    putShips(first_Or_Second, first.map, first.shipsCoordinate);
                } else {
                    availableUsers(usersInfo, usersScore, second.name, "Second");
                    putShips(first_Or_Second, second.map, second.shipsCoordinate);
                }
            }
            break;
        case 2:
            settings();
            displayMenu(usersInfo, usersScore);
            break;
        case 3:
            cout << "        Score board\nUsername            Score\n";
            for (t = 1; t <= numOfUsers; t++) {
                cout << t << "." << usersInfo[t];
                for (unsigned int b = 20 - strlen(usersInfo[t]); b > 0; b--)
                    cout << " ";
                cout << usersScore[t] << endl;
            }

            while (true) {
                cout << "Enter 1 to return to main menu :";
                cin >> selectedNumber;
                if (selectedNumber == 1)
                    break;
                else
                    cout << "Wrong number!\n";
            }
            if (selectedNumber == 1)
                displayMenu(usersInfo, usersScore);
            break;
        case 4:
            exitFromGame = true;
            exitGame(usersInfo, usersScore);
    }
}

void availableUsers(char usersInfo[][15], int usersScore[], char playerName[15], const string &first_Or_Second) {
    int g;
    int selectedUser;
    if (selectedNumber == 1 && numOfUsersInStart != 0) {
        cout << "\nAvailable users :\n";
        for (g = 1; g <= numOfUsersInStart; g++)
            cout << g << "." << usersInfo[g] << endl;

        while (true) {
            cout << "choose a user to continue :";
            cin >> selectedUser;
            if (selectedUser >= 1 && selectedUser <= numOfUsersInStart)
                break;
            else
                cout << "Wrong number!\n";
        }
        strcpy(playerName, usersInfo[selectedUser - 1]);
    } else {
        if (selectedNumber == 1 && numOfUsersInStart == 0)
            cout << "there is not any user!\n";
        cout << "Enter your name :";
        cin >> playerName;
        for (g = 1; g <= numOfUsersInStart; g++) {
            if (strcmp(usersInfo[g], playerName) == 0 ||
                (first_Or_Second == "Second" && strcmp(first.name, playerName) == 0)) {
                cout << "Please enter another Name :";
                cin >> playerName;
                g = 0;
            }
        }
        numOfUsers++;
        if (first_Or_Second == "First") {
            first.which_Row = numOfUsers;
        } else
            second.which_Row = numOfUsers;
        strcpy(usersInfo[numOfUsers], playerName);
        usersScore[numOfUsers] = 0;
    }
}

void settings() {
    while (true) {
        cout << "\n1.Ships\n2.Map size\nEnter a number to continue :";
        cin >> selectedNumber;
        if (selectedNumber == 1 || selectedNumber == 2)
            break;
        else
            cout << "Wrong number!\n";
    }
    switch (selectedNumber) {
        case 1:
            while (true) {
                cout << "Enter number of ships :";
                cin >> numOfShips;
                if (numOfShips > 0)
                    break;
                else
                    cout << "Wrong number!\n";
            }
            while (true) {
                int s;
                cout << "Enter length of ships :";
                for (s = 0; s < numOfShips; s++) {
                    cin >> ships[s];
                    if (s == numOfShips - 1)
                        break;
                    else if (ships[s] <= 0)
                        cout << "Wrong length!\n";
                }
                largestShip = ships[0];
                for (int f = 0; f < numOfShips; f++)
                    if (largestShip < ships[f])
                        largestShip = ships[f];
                break;
                case 2:
                    while (true) {
                        cout << "Enter new Map size(at most 16) :";
                        cin >> mapSize;
                        if (mapSize <= 16)
                            break;
                        else
                            cout << "Wrong mapSize!\n";
                    }
                break;
                default:
                    settings();
            }
    }
}

void displayMap(char Map[][16], bool clear = false) {
    cout << endl << "      ";
    int h;
    for (h = 1; h <= mapSize; h++) {
        cout << h << "     ";
        if (h < 9)
            cout << " ";
    }
    cout << "\b\bX" << endl;
    for (int a = 0; a < mapSize; a++) {
        if (a < 9)
            cout << " ";
        cout << a + 1 << " ";
        for (int b = 0; b < mapSize; b++) {
            cout << "  " << Map[a][b] << "  ";
            if (b != mapSize - 1)
                cout << " " << char(179);
        }
        cout << endl << "   ";
        if (a != mapSize - 1) {
            for (int c = 0; c < mapSize * 7; c++)
                cout << char(196);
            cout << endl;
        }
    }
    cout << "\b\nY\n\n";
    if (clear) {
        long long timer = time(nullptr) + 3;
        while (true) {
            if (time(nullptr) == timer) {
                for (h = 0; h < 100; h++)
                    cout << endl;
                break;
            }
        }
    }
}

void putShips(const string &firstOrSecond, char map[][16], int shipsCoordinate[][4]) {
    int counter = 0;
    if (firstOrSecond == "Bot")
        putShipsAuto(map, shipsCoordinate);
    else {
        while (true) {
            cout << "1.Auto\n2.Normal\nEnter a number to continue :";
            cin >> selectedNumber;
            if (selectedNumber == 1 || selectedNumber == 2)
                break;
            else
                cout << "Wrong number!\n";
        }
        if (selectedNumber == 1)
            putShipsAuto(map, shipsCoordinate);
        else {
            cout << "please enter numbers :\n";
            for (int l = 0; l < numOfShips; l++) {
                bool check = true;
                displayMap(map);
                cout << "ship 1" << char(158) << ships[l] << endl;
                if (ships[l] == 1) {
                    cout << "Y :";
                    cin >> y;
                    cout << "X :";
                    cin >> x;
                    y--;
                    x--;
                    if (!checkCoordinate(l, map, shipsCoordinate, true))
                        l--;
                    else {
                        map[y][x] = 'S';
                        shipsCoordinate[counter][0] = l;
                        shipsCoordinate[counter][1] = y;
                        shipsCoordinate[counter][2] = x;
                        counter++;
                    }
                } else {
                    int u;
                    while (true) {
                        cout << "Y1 :";
                        cin >> y3;
                        cout << "X1 :";
                        cin >> x1;
                        cout << "Y2 :";
                        cin >> y2;
                        cout << "X2 :";
                        cin >> x2;
                        y3--, y2--, x1--, x2--;
                        if (x2 < x1) swap(x2, x1);
                        if (y2 < y3) swap(y2, y3);
                        if (x2 == x1 && y2 - y3 + 1 == ships[l]) {
                            x = x2;
                            for (u = y3; u <= y2; u++) {
                                y = u;
                                if (!checkCoordinate(l, map, shipsCoordinate, true)) {
                                    check = false;
                                    l--;
                                    break;
                                }
                            }
                            if (check) {
                                for (u = y3; u <= y2; u++) {
                                    map[u][x2] = 'S';
                                    shipsCoordinate[counter][0] = l;
                                    shipsCoordinate[counter][1] = u;
                                    shipsCoordinate[counter][2] = x2;
                                    counter++;
                                }
                            }
                            break;
                        } else if (y2 == y3 && x2 - x1 + 1 == ships[l]) {
                            y = y2;
                            for (u = x1; u <= x2; u++) {
                                x = u;
                                if (!checkCoordinate(l, map, shipsCoordinate, true)) {
                                    check = false;
                                    l--;
                                    break;
                                }
                            }
                            if (check) {
                                for (u = x1; u <= x2; u++) {
                                    map[y2][u] = 'S';
                                    shipsCoordinate[counter][0] = l;
                                    shipsCoordinate[counter][1] = y2;
                                    shipsCoordinate[counter][2] = u;
                                    counter++;
                                }
                            }
                            break;
                        } else {
                            cout << "\nWrong coordinate!\nTry again\n";
                            l--;
                            break;
                        }
                    }
                }
            }
        }
    }
}

bool checkCoordinate(int &l, char map[][16], int shipsCoordinate[][4], bool warning) {
    bool checkCoordinate;
    bool checkCoordinate1 = (y < 0 || y > mapSize - 1 || x < 0 || x > mapSize - 1);
    bool checkCoordinate2 = (map[y - 1][x - 1] == 'S' || map[y - 1][x + 1] == 'S' ||
                             map[y + 1][x + 1] == 'S' || map[y + 1][x - 1] == 'S');
    bool checkCoordinate3 = (map[y][x - 1] == 'S' || map[y][x + 1] == 'S' ||
                             map[y + 1][x] == 'S' || map[y - 1][x] == 'S');
    if (checkCoordinate1) {
        if (warning)cout << "\nWrong coordinate!\nTry again\n";
        return false;
    } else if (checkCoordinate2 || checkCoordinate3) {
        checkCoordinate = false;
        if (checkCoordinate3 && !checkCoordinate2) {
            for (int k = 0; k < numOfCoordinate; k++) {
                if (shipsCoordinate[k][0] == l) {
                    if (shipsCoordinate[k][1] == y) {
                        if (shipsCoordinate[k][2] == x - 1 || shipsCoordinate[k][2] == x + 1) {
                            checkCoordinate = true;
                            break;
                        }
                    } else if (shipsCoordinate[k][2] == x) {
                        if (shipsCoordinate[k][1] == y - 1 || shipsCoordinate[k][1] == y + 1) {
                            checkCoordinate = true;
                            break;
                        }
                    }
                }
            }
        }
        if (!checkCoordinate) {
            if (warning)cout << "\nWrong coordinate! you must put ships with one space between\nTry again";
            return false;
        }
    }
    return true;
}

void putShipsAuto(char map[][16], int shipsCoordinate[][4]) {
    //ship 1*5
    map[0][0] = 'S';
    shipsCoordinate[0][0] = 0;
    shipsCoordinate[0][1] = 0;
    shipsCoordinate[0][2] = 0;
    map[1][0] = 'S';
    shipsCoordinate[1][0] = 0;
    shipsCoordinate[1][1] = 1;
    shipsCoordinate[1][2] = 0;
    map[2][0] = 'S';
    shipsCoordinate[2][0] = 0;
    shipsCoordinate[2][1] = 2;
    shipsCoordinate[2][2] = 0;
    map[3][0] = 'S';
    shipsCoordinate[3][0] = 0;
    shipsCoordinate[3][1] = 3;
    shipsCoordinate[3][2] = 0;
    map[4][0] = 'S';
    shipsCoordinate[4][0] = 0;
    shipsCoordinate[4][1] = 4;
    shipsCoordinate[4][2] = 0;
    //ship 1*3
    map[7][0] = 'S';
    shipsCoordinate[5][0] = 1;
    shipsCoordinate[5][1] = 7;
    shipsCoordinate[5][2] = 0;
    map[8][0] = 'S';
    shipsCoordinate[6][0] = 1;
    shipsCoordinate[6][1] = 8;
    shipsCoordinate[6][2] = 0;
    map[9][0] = 'S';
    shipsCoordinate[7][0] = 1;
    shipsCoordinate[7][1] = 9;
    shipsCoordinate[7][2] = 0;
    //ship 1*3
    map[9][6] = 'S';
    shipsCoordinate[8][0] = 2;
    shipsCoordinate[8][1] = 9;
    shipsCoordinate[8][2] = 6;
    map[9][7] = 'S';
    shipsCoordinate[9][0] = 2;
    shipsCoordinate[9][1] = 9;
    shipsCoordinate[9][2] = 7;
    map[9][8] = 'S';
    shipsCoordinate[10][0] = 2;
    shipsCoordinate[10][1] = 9;
    shipsCoordinate[10][2] = 8;
    //ship 1*2
    map[5][5] = 'S';
    shipsCoordinate[11][0] = 3;
    shipsCoordinate[11][1] = 5;
    shipsCoordinate[11][2] = 5;
    map[5][6] = 'S';
    shipsCoordinate[12][0] = 3;
    shipsCoordinate[12][1] = 5;
    shipsCoordinate[12][2] = 6;
    //ship 1*2
    map[7][6] = 'S';
    shipsCoordinate[13][0] = 4;
    shipsCoordinate[13][1] = 7;
    shipsCoordinate[13][2] = 6;
    map[7][7] = 'S';
    shipsCoordinate[14][0] = 4;
    shipsCoordinate[14][1] = 7;
    shipsCoordinate[14][2] = 7;
    //ship 1*2
    map[1][6] = 'S';
    shipsCoordinate[19][0] = 9;
    shipsCoordinate[19][1] = 1;
    shipsCoordinate[19][2] = 6;
    map[2][6] = 'S';
    shipsCoordinate[20][0] = 9;
    shipsCoordinate[20][1] = 2;
    shipsCoordinate[20][2] = 6;
     //ship 1*1
    map[2][9] = 'S';
    shipsCoordinate[15][0] = 5;
    shipsCoordinate[15][1] = 2;
    shipsCoordinate[15][2] = 9;
    //ship 1*1
    map[4][9] = 'S';
    shipsCoordinate[16][0] = 6;
    shipsCoordinate[16][1] = 4;
    shipsCoordinate[16][2] = 9;
    //ship 1*1
    map[8][2] = 'S';
    shipsCoordinate[17][0] = 7;
    shipsCoordinate[17][1] = 8;
    shipsCoordinate[17][2] = 2;
//  ship 1*1
    map[9][4] = 'S';
    shipsCoordinate[18][0] = 8;
    shipsCoordinate[18][1] = 9;
    shipsCoordinate[18][2] = 4;
}

void playWithFriend(int &targets, char playerName[15], char resultMap[][16], char enemyMap[][16], int &score,
                    int shipsCoordinate[][4]) {
    bool checkCoordinate;
    displayMap(resultMap);
    while (true) {
        cout << " Enter your target coordinate\nX :";
        cin >> x;
        cout << "Y :";
        cin >> y;
        x--;
        y--;
        checkCoordinate = (resultMap[y][x] == 'W' || resultMap[y][x] == 'E' || resultMap[y][x] == 'C' ||
                           y < 0 || y >= mapSize || x < 0 || x >= mapSize);
        if (checkCoordinate)
            cout << "Wrong coordinate!\n";
        else
            break;
    }
    if (enemyMap[y][x] == 'S') {
        score++;
        targets--;
        resultMap[y][x] = 'E';
        int number, d;
        bool check = true;
        for (d = 0;; d++) {
            if (shipsCoordinate[d][1] == y && shipsCoordinate[d][2] == x) {
                shipsCoordinate[d][3] = 1;
                number = shipsCoordinate[d][0];
                break;
            }
        }
        for (d = 0; d < numOfCoordinate; d++) {
            if (shipsCoordinate[d][0] == number && shipsCoordinate[d][3] != 1) {
                check = false;
                break;
            }
        }
        if (check) {
            int lengthOfShip = 0;
            for (int f = 0; f < numOfCoordinate; f++) {
                if (shipsCoordinate[f][0] == number) {
                    lengthOfShip++;
                    y3 = shipsCoordinate[f][1];
                    x1 = shipsCoordinate[f][2];
                    resultMap[y3][x1] = 'C';
                    char *result[8] = {&resultMap[y3][x1 - 1], &resultMap[y3][x1 + 1],
                                       &resultMap[y3 + 1][x1], &resultMap[y3 - 1][x1], &resultMap[y3 - 1][x1 - 1],
                                       &resultMap[y3 - 1][x1 + 1], &resultMap[y3 + 1][x1 + 1],
                                       &resultMap[y3 + 1][x1 - 1]};
                    for (int g = 0; g < 8; g++) {
                        if (*result[g] != 'E')
                            *result[g] = 'W';
                    }
                }
            }
            if (lengthOfShip == 1)
                score += 25;
            else if (lengthOfShip == 2)
                score += 12;
            else if (lengthOfShip == 3)
                score += 8;
            else if (lengthOfShip == 5)
                score += 5;
            else {
                score += 5 * largestShip / lengthOfShip;
            }

        }
        if (targets != 0) {
            cout << "\ayou can shoot again\n";
            playWithFriend(targets, playerName, resultMap, enemyMap, score, shipsCoordinate);
        } else {
            displayMap(resultMap);
            while (true) {
                cout << "Enter 1 to continue or Enter -1 to exit :";
                cin >> selectedNumber;
                if (selectedNumber == 1 || selectedNumber == -1)
                    break;
                else
                    cout << "Wrong number!\n";
            }
            if (selectedNumber == -1)
                exitFromGame = true;
            else
                for (int h = 0; h < 100; h++)
                    cout << endl;
        }
    } else {
        resultMap[y][x] = 'W';
        displayMap(resultMap);
        while (true) {
            cout << "Enter 1 to continue or Enter -1 to exit :";
            cin >> selectedNumber;
            if (selectedNumber == 1 || selectedNumber == -1)
                break;
            else
                cout << "Wrong number!\n";
        }
        if (selectedNumber == -1)
            exitFromGame = true;
        else
            for (int h = 0; h < 100; h++)
                cout << endl;
    }
}

void exitGame(char usersInfo[][15], int usersScore[]) {
    if (exitFromGame) {
        while (true) {
            cout << "Are you sure to exit?(1.Yes 2.No) :";
            cin >> selectedNumber;
            if (selectedNumber == 1 || selectedNumber == 2)
                break;
            else
                cout << "Wrong number!";
        }
        if (selectedNumber == 1 || !exitFromGame) {
            for (int j = 1; j <= numOfUsers; j++) {
                if (usersScore[j] < usersScore[j + 1]) {
                    swap(usersScore[j], usersScore[j + 1]);
                    swap(usersInfo[j], usersInfo[j + 1]);
                }
            }
            ofstream writeUsers;
            writeUsers.open("usersInfo.txt", ofstream::out);
            writeUsers.clear();
            writeUsers << numOfUsers << endl;
            for (int i = 1; i <= numOfUsers; i++)
                writeUsers << usersInfo[i] << " " << usersScore[i] << endl;
            writeUsers.close();
            cout << endl << char(200);
            for (int t = 0; t < 43; t++)
                cout << char(205);
            cout << char(188);
            exit(0);
        }
    }
}