// ConnectFour.cpp : This file contains the 'main' function. Program execution begins and ends there.
/*
    Joseph Pearce           Game Developer
    Blog: https://highpixelstudio.wordpress.com/
    Website: https://highpixellabs.itch.io/

*/
#include <array>
#include <iostream>
#include <string>
#include <list>
#include <iterator>
using namespace std;

bool gameOver = false;
int maxPlayers = 1;
int currPlayer = 0;
string inGameArray[6][7];

std::list<std::array<int, 2>> player1Moves;
std::list<std::array<int, 2>> player2Moves;

void PrintGrid(string a[7][7], int size, string realArray[6][7])
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i == 0)
            {
                a[i][j] = j;
                cout << " " << j + 1 << " ";
            }
            else
            {
                if (realArray[i - 1][j].empty())
                {
                    a[i][j] = "[ ]";
                }
                else
                {
                    a[i][j] = realArray[i - 1][j];
                }
                cout << "" << a[i][j] << "";
            }

        }
        cout << "\n";
    }
}

void SwitchTurn()
{
    currPlayer++;
    if (currPlayer > 1)
    {
        currPlayer = 0;
    }
}

void SetGameOver(bool setValue)
{
    gameOver = setValue;
}

bool FollowThePath(int pattern[2], string tokenInputed, string gameArray[6][7], int currSpot[2])
{
    int connected = 1;
    for (int x = 1; x <= 4; x++)
    {
        int setX = -pattern[0] + currSpot[0];
        int setY = pattern[1] + currSpot[1];
        if (setX >= 0 && setX < 6 && setY >= 0 && setY < 7)
        {
            if (gameArray[setX][setY] == tokenInputed)
            {
                connected++;
                currSpot[0] = setX;
                currSpot[1] = setY;
                if (connected >= 4)
                {
                    break;
                }
            }
            else
            {
                connected = 0;
                break;
            }
        }
        else
        {
            break;
        }
    }
    if (connected >= 4)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CheckTheBoardForMatch(string tokenInputed, string gameArray[6][7], int pos[2])
{
    for (int x = pos[0] - 1; x <= pos[0] + 1; x++)
    {
        for (int y = pos[1] - 1; y <= pos[1] + 1; y++)
        {
            if (x >= 0 && x < 6 && y >= 0 && y < 7 && gameArray[x][y] == tokenInputed && !gameOver)
            {
                int myX = pos[0];
                int myY = pos[1];
                if (myX != x || myY != y)
                {
                    string t = gameArray[x][y];
                    int diffX = myX - x;
                    int diffY = y - myY;
                    int patternSet[2] = { diffX, diffY };
                    int currSpot[2] = { myX, myY };
                    bool foundConnection = FollowThePath(patternSet, tokenInputed, gameArray, currSpot);
                    if (foundConnection)
                    {
                        SetGameOver(true);
                        cout << "\n" << "===== GAME OVER! =====" << "\n";
                    }
                }
            }
        }
    }
}


bool AddToBoard(int slot, int player, string realArray[6][7], string token)
{
    bool isFull = false;
    int lowestX = 0;
    int rowX = 6;
    for (int i = 0; i < rowX; i++)
    {
        if (realArray[i][slot].empty())
        {
            lowestX = i;
        }
        else
        {
            if (i == 0)
            {
                isFull = true;
            }
        }
    }
    cout << "\n";
    if (!isFull)
    {
        realArray[lowestX][slot] = "[" + token + "]";
        int position[2] = { lowestX,slot };
        if (currPlayer == 0)
        {
            player1Moves.push_front({ {position[0], position[1]}});
            /*for (const auto& arr : player1Moves)
                for (const auto& v : arr)
                    cout << v << '\n';*/
        }
        else
        {
            player2Moves.push_front({ {position[0], position[1]} });
            /*for (const auto& arr : player2Moves)
                for (const auto& v : arr)
                    cout << v << '\n';*/
        }
        CheckTheBoardForMatch(realArray[lowestX][slot], realArray, position);
        return true;
    }
    else
    {
        cout << "Selected Slot is full, please select another spot" << "\n";
        return false;
    }
}

void GameOverInfo(string gameArray[6][7])
{
    char restart;
    cout << "Do you want to play again? (y/n)";
    while (!(cin >> restart) && (restart == 'y' || restart == 'n'))
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Please enter y or n. " << "\n\n";
        cout << "Do you want to play again? (y/n)" << "\n";
    }
    if (restart == 'y')
    {
        gameOver = false;
        currPlayer = 0;
        memset(gameArray, 0, sizeof(gameArray[0][0]) * 6 * 7);
        player1Moves.clear();
        player2Moves.clear();
    }
    else if (restart == 'n')
    {
        cout << "Thanks for playing!!!" << "\n\n";
        
    }
    else
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Please enter y or n. " << "\n\n";
        GameOverInfo(gameArray);
    }
}

void LoopThroughPastMoves(int playerSelected, string playerToken, list<std::array<int, 2>> allMoves, string gameArray[6][7])
{
    for (auto const& i : allMoves) 
    {
        //cout << to_string(i[0]) << " | " << to_string(i[1]) <<"\n";
        int currMove[2] = { i[0], i[1] };
        string realToken = "[" + playerToken + "]";
        CheckTheBoardForMatch(realToken, gameArray, currMove);
        if (gameOver)
        {
            break;
        }
    }
}

void PrintLogo()
{
    cout << R"(
 $$$$$$\                                                      $$\           $$\   $$\ 
$$  __$$\                                                     $$ |          $$ |  $$ |
$$ /  \__| $$$$$$\  $$$$$$$\  $$$$$$$\   $$$$$$\   $$$$$$$\ $$$$$$\         $$ |  $$ |
$$ |      $$  __$$\ $$  __$$\ $$  __$$\ $$  __$$\ $$  _____|\_$$  _|        $$$$$$$$ |
$$ |      $$ /  $$ |$$ |  $$ |$$ |  $$ |$$$$$$$$ |$$ /        $$ |          \_____$$ |
$$ |  $$\ $$ |  $$ |$$ |  $$ |$$ |  $$ |$$   ____|$$ |        $$ |$$\             $$ |
\$$$$$$  |\$$$$$$  |$$ |  $$ |$$ |  $$ |\$$$$$$$\ \$$$$$$$\   \$$$$  |            $$ |
 \______/  \______/ \__|  \__|\__|  \__| \_______| \_______|   \____/             \__|
)" 
        << "\n"
        <<"Made by HighPixelLabs" 
        << "\n\n";
}

int main()
{
    int size = 7;
    string showArray[7][7];
    string choices[2] = { "O", "X" };
    int selectedSlot;
    PrintLogo();
    while (!gameOver)
    {
        PrintGrid(showArray, size, inGameArray);
        cout << "Player " << currPlayer + 1 << " (" + choices[currPlayer] + ")," << " Choose a slot: ";
        while (!(cin >> selectedSlot))
        {
            
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter valid slot between 1 and 7. " << "\n\n";
            PrintGrid(showArray, size, inGameArray);
            cout << "Player " << currPlayer + 1 << " (" + choices[currPlayer] + ")," << " Choose a slot: ";
        }
        if (selectedSlot > 0 && selectedSlot <= size)
        {
            string selectedToken = choices[currPlayer];
            bool isBoardFull = AddToBoard(selectedSlot - 1, currPlayer, inGameArray, selectedToken);
            if (isBoardFull && !gameOver)
            {
                if (currPlayer == 0)
                {
                    LoopThroughPastMoves(currPlayer, selectedToken, player1Moves, inGameArray);
                }
                else
                {
                    LoopThroughPastMoves(currPlayer, selectedToken, player2Moves, inGameArray);
                }
            }

            if (isBoardFull && !gameOver)
            {
                SwitchTurn();
            }
            if (gameOver)
            {
                PrintGrid(showArray, size, inGameArray);
                cout << "Player " + to_string(currPlayer + 1) << " (" + choices[currPlayer] + ") " + "is the winner!" <<"\n\n";
                GameOverInfo(inGameArray);

            }
        }
        else
        {
            cout << "Invalid input. Please enter valid slot between 1 and 7. " << "\n\n";
        }
    }
}
