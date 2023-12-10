//Group 12:
// - Bárbara Teixeira (UP202209742)
// - Diogo Bastos (UP202202425)
// - Sara Vidal (UP202202427)

// WORD GAMES
//================================================================================
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
//--------------------------------------------------------------------------------
using namespace std;
//================================================================================
// COLOR CODES / ANSI ESCAPE SEQUENCES
//================================================================================
// TEXT COLOR
#define NO_COLOR "\033[0m"
#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define LIGHTRED "\033[1;31m"
#define WHITE "\033[1;37m"
// BACKGROUND COLOR
#define BLACK_B "\033[0;40m"
#define RED_B "\033[0;41m"
#define GREEN_B "\033[0;42m"
#define WHITE_B "\033[1;47m"

//================================================================================
//================================================================================
// USER DEFINED CLASSES
//================================================================================
//================================================================================

//================================================================================
//CLASSE BOARD
//================================================================================

class Board {
public:
    //Board(int numLins, int numCols);
    Board(const string& filename); // constructor
    void show() const; // const method (NOTE: const methods cannot modify the attributes)
    vector<vector<pair<char, bool>>> getBoard() const; // const method
    bool getEnd() const; // const method
private:
    vector<vector<pair<char, bool>>> b; //False --> letter not covered / True --> letter covered
};

//--------------------------------------------------------------------------------
// CONSTRUCTORS

Board::Board(const string& filename) {
    ifstream f(filename); // open an input file stream to read from a file with the name specified in 'filename'
    if (!(f.is_open())) { // 'is_open()' test whether the file is already open, if is not an error message will show
        cerr << "File not found!" << endl;
        exit(1); // stop the program with an exit code != 0 
    }

    string dummy, boardSize; // 'dummy' is a temporary string to discard certain values 
    getline(f, boardSize); // read the first line of the file 'board.txt' an save its content in the string 'boardSize'
    istringstream iss(boardSize); // initialize string stream 'iss' with the string 'boardSize' 
    int numLins, numCols;
    iss >> dummy >> dummy >> dummy >> numLins >> dummy >> numCols; // read values from 'boardSize' and store integers 'numLins' and 'numCols' 
    b = vector<vector<pair<char, bool>>>(numLins, vector<pair<char, bool>>(numCols, make_pair(' ', false))); // create a bidimensional matrix 'b' 
    // with dimensions 'numLins' by 'numCols';
    // initialize each element as a pair<char, bool> 
    // with a space character (' ') and false
// jump to line 5 (where the board starts)
    for (int i = 0; i < 4; ++i)
        getline(f, dummy);

    // read the board content
    for (int i = 0; i < numLins; ++i)
    {
        getline(f, dummy); // read line of the board

        for (int j = 2; j < dummy.size(); j += 2)
        {
            if (isalpha(dummy[j]))
            {
                // place the letter on the board
                b[i][j / 2 - 1] = make_pair(dummy[j], false);
            }
        }
    }
}

//--------------------------------------------------------------------------------
// GETS AND SETS

vector<vector<pair<char, bool>>> Board::getBoard() const {
    return b;
}

//--------------------------------------------------------------------------------
// UNDERSTAND WHEN THE GAME IS OVER

bool Board::getEnd() const {
    bool gameOver = true;
    for (int i = 0; i <= b.at(0).size(); i++) {
        gameOver = all_of(b.at(i).begin(), b.at(i).end(), [](const std::pair<char, bool>& p) {

            return p.first != ' ' || (p.first != ' ' && p.second == true);
            });
        if (!gameOver)
            break;
    }
    return gameOver;
}

//--------------------------------------------------------------------------------
// SHOW

void Board::show() const {

    cout << endl;
    cout << BLACK_B << WHITE << " ";
    cout << " ";
    for (size_t j = 0; j < b.at(0).size(); j++)
        cout << (char)('a' + j) << ' ';
    cout << endl;

    for (size_t i = 0; i < b.size(); i++)
    {
        cout << BLACK_B << WHITE << (char)('A' + i) << ' ';
        for (size_t j = 0; j < b.at(i).size(); j++)
            cout << BLACK << WHITE_B << b.at(i).at(j).first << ' ';
        cout << endl;
    }
    cout << NO_COLOR << BLACK_B;
    cout << endl;
}

//================================================================================
// CLASS BAG
//================================================================================

class Bag
{
public:
    Bag(const Board& b);
    vector<char>& getLetters();
    void showLetters() const;
private:
    vector<char> letters;
};

//--------------------------------------------------------------------------------
// CONSTRUCTORS

Bag::Bag(const Board& b)
{
    // read a line
    for (int i = 0; i < b.getBoard().at(0).size(); ++i)
    {
        // read a column
        for (int j = 0; j < b.getBoard().at(1).size(); j += 1)
        {
            // if the cell board contains a letter, adds it to the vector "letters" in the bag
            if (isalpha(b.getBoard().at(i).at(j).first)) { // "isalpha()" returns 0 if the character is not an alphabet letter
                letters.push_back(b.getBoard().at(i).at(j).first); // create a list with each letter on the board
            }
        }
    }
    random_shuffle(letters.begin(), letters.end()); // rearrange the elements in the vector "letters" randomly
}

//--------------------------------------------------------------------------------
// GETS AND SETS

vector<char>& Bag::getLetters() {
    return letters;
}

//--------------------------------------------------------------------------------
// SHOW

void Bag::showLetters() const
{
    cout << "List of letters in the bag:" << endl;
    for (char letter : letters) {
        cout << letter << ' ';
    }
    cout << endl << endl;
}

//================================================================================
// CLASS HAND
//================================================================================

class Hand
{
public:
    Hand();
    Hand(int handBegin, Bag& letterBag);
    void showHand() const;
private:
    vector<char> playerHand;
};

//--------------------------------------------------------------------------------
// CONSTRUCTORS

Hand::Hand() {
    // nothing to do here, the vector<char> playerHand will be created empty automatically
}

Hand::Hand(int handBegin, Bag& letterBag) {
    for (int i = 0; i < handBegin; i++) {
        char randomLetter = letterBag.getLetters().at(i);
        playerHand.push_back(randomLetter);
        letterBag.getLetters().erase(letterBag.getLetters().begin() + i);
    }
}

//--------------------------------------------------------------------------------
// SHOW

void Hand::showHand() const {
    cout << "List of letters in the hand: ";
    for (char letter : playerHand) {
        cout << letter << ' ';
    }
    cout << endl;
}

//================================================================================
// CLASS PLAYER
//================================================================================

class Player
{
public:
    Player(int id, int handBegin, Bag& letterBag);
    int getId() const;
    string getName() const;
    void showPlayer() const;
private:
    int id_;
    int points_;
    string name_;
    Hand hand_;
};

//--------------------------------------------------------------------------------
// CONSTRUCTORS

Player::Player(const int id, const int handBegin, Bag& letterBag) {
    bool isValid = false;
    string name;
    do {
        cout << "Enter the name of the Player " << id << ": ";
        if (getline(cin, name)) {
            if (!name.empty()) {
                isValid = true;
                id_ = id;
                points_ = 0;
                name_ = name;
                Hand hand(handBegin, letterBag);
                hand_ = hand;
            }
        }
        if (!isValid) {
            cin.clear();
            // clean input buffer
            cin.ignore(1000, '\n');
            cout << RED << "Invalid name!" << endl;
        }
        cout << NO_COLOR;
    } while (!isValid);
}

//--------------------------------------------------------------------------------
// GETS AND SETS

int Player::getId() const {
    return id_;
}

string Player::getName() const {
    return name_;
}

//--------------------------------------------------------------------------------
// SHOW

void Player::showPlayer() const {
    cout << "Player " << id_ << endl;
    cout << " -> Name: " << name_ << endl;
    cout << " -> ";
    hand_.showHand();
    cout << " -> Points: " << points_ << endl;
    cout << endl;
}

//================================================================================
// CLASS LISTPLAYER
//================================================================================

class ListPlayer
{
public:
    ListPlayer(int handBegin, Bag& letterBag);
    void showPlayers() const;
private:
    vector <Player> playersList;
};

//--------------------------------------------------------------------------------
// CONSTRUCTORS

ListPlayer::ListPlayer(int handBegin, Bag& letterBag) {
    bool isValid = false;
    int num;
    do {
        cout << "How many players are goin to play? ";
        if (cin >> num) {
            if (num >= 2 && num <= 4) {
                isValid = true;
                // clean input buffer
                cin.ignore(1000, '\n');
            }
        }
        if (!isValid) {
            cin.clear();
            // clean input buffer
            cin.ignore(1000, '\n');
            cout << RED << "The number should be betwwen 2 and 4!" << endl;
        }
        cout << NO_COLOR;
    } while (!isValid);

    for (int i = 1; i <= num; i++) {
        Player p(i, handBegin, letterBag);
        playersList.push_back(p);
    }
    cout << endl;
}

//--------------------------------------------------------------------------------
// SHOW

void ListPlayer::showPlayers() const {
    cout << "List of players in the game:" << endl;
    for (Player player : playersList) {
        player.showPlayer();
    }
    cout << endl;
}

//================================================================================
//================================================================================
// HELP FUNCTIONS
//================================================================================
//================================================================================

// Read the number of letters initially taken by each player
void readHandBegin(int& handBegin, Bag& letterBag) {
    bool isValid = false;
    do {
        cout << "How many letters should be initially taken by each player? ";
        if (cin >> handBegin) {
            if (handBegin <= (letterBag.getLetters().size() / 4)) {
                isValid = true;
            }
        }
        if (!isValid) {
            cin.clear();
            // clean input buffer
            cin.ignore(1000, '\n');
            cout << RED << "The maximum number of letters should be under " << (letterBag.getLetters().size() / 4) << "!" << endl;
        }
        cout << NO_COLOR << endl;
    } while (!isValid);

}

//================================================================================
//================================================================================
// MAIN
//================================================================================
//================================================================================

int main() {
    cout << "WORDS GAME\n\n";

    string filename;
    cout << "File name: ";
    cin >> filename;
    Board b(filename);
    b.show();

    Bag letterBag(b);
    letterBag.showLetters();

    int handBegin;
    readHandBegin(handBegin, letterBag);

    ListPlayer listPlayer(handBegin, letterBag);
    listPlayer.showPlayers();

    while (!b.getEnd()) {

        cout << "Hello World!!!!";

    }

    return 0;
}