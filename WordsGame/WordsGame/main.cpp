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
    vector<vector<pair<char, bool>>> getBoard() const; 
    vector<char> getPlayableLetters() const;
    bool getEnd() const;
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
    for (int i = 0; i < b.at(0).size(); i++) { 
        gameOver = all_of(b.at(i).begin(), b.at(i).end(), [](const std::pair<char, bool>& p) {
            return p.first != ' ' || (p.first != ' ' && p.second == true);
            });
        if (!gameOver)
            break;
    }
    return gameOver;
}

//--------------------------------------------------------------------------------
// GET A VECTOR WITH ALL LETTERS IN PLAYABLE POSITIONS

vector<char> Board::getPlayableLetters() const{
    vector<char> playableLetters;
    bool playable;
    for (int i = 0; i < b.at(0).size(); i++) { // scroll through each line
        for (int j = 0; j < b.at(1).size(); j++) { // scroll through each column
            playable = true; // playable until proven otherwise
            if (isalpha(b.at(i).at(j).first)) { // true if it is a letter
                if (!b.at(i).at(j).second) { // true if it is a letter that was not played yet
                    if (i == 0) {
                        if (!isalpha(b.at(i + 1).at(j).first))
                            playable = false; // letter has nothing after in the column
                    }
                    else if (i == b.at(0).size() - 1) {
                        if (!isalpha(b.at(i - 1).at(j).first))
                            playable = false; // letter has nothing before in the column
                    }
                    else {
                        if ((isalpha(b.at(i - 1).at(j).first) == 0 && isalpha(b.at(i + 1).at(j).first) == 0)) 
                            playable = false; // letter has nothing before and after in the column
                    }
                    if (playable) {
                        for (int p = i; p > 0; p--) { // scroll up through the column
                            if (isalpha(b.at(i - (i - p) - 1).at(j).first)) { // true if it is a letter
                                if (!b.at(i - (i - p) - 1).at(j).second) { // true if it is a letter that was not played yet
                                    playable = false; // letter not playable once there is a letter before consecutive without being played
                                }
                            }
                            else {
                                break; // break as soon as it find a space
                            }
                        }
                    }
                    if (playable) { 
                        playableLetters.push_back(b.at(i).at(j).first); // add letter to the vector   
                    }
                    else { // is gonna check in the line if is valid
                        playable = true; // playable until proven otherwise
                        if (j == 0) {
                            if (!isalpha(b.at(i).at(j + 1).first))
                                playable = false; // letter has nothing after in the column
                        }
                        else if (j == b.at(1).size() - 1) {
                            if (!isalpha(b.at(i).at(j - 1).first))
                                playable = false; // letter has nothing before in the column
                        }
                        else {
                            if (isalpha(b.at(i).at(j - 1).first) == 0 && isalpha(b.at(i).at(j + 1).first) == 0) {
                                playable = false; // letter has nothing before and after in the line
                            }
                        }                      
                        if (playable) {
                            for (int l = j; l > 0; l--) { // scroll backwards through the line
                                if (isalpha(b.at(i).at(j - (j - l) - 1).first)) { // true if it is a letter
                                    if (!b.at(i).at(j - (j - l) - 1).second) { // true if it is a letter that was not played yet
                                        playable = false; // letter not playable once there is a letter before consecutive without being played
                                    }
                                }
                                else {
                                    break; // break as soon as it find a space
                                }
                            }
                        }
                        if (playable) {
                            playableLetters.push_back(b.at(i).at(j).first); // add letter to the vector   
                        }
                    }
                }
            }
        }
    }

    return playableLetters;
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
    vector<char> getLetters();
    void deleteLetters(int i);
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

    srand(static_cast<unsigned int>(time(0))); // use current time as seed for random generator
    random_shuffle(letters.begin(), letters.end()); // rearrange the elements in the vector "letters" randomly
}

//--------------------------------------------------------------------------------
// GETS AND SETS

vector<char> Bag::getLetters() {
    return letters;
}

//--------------------------------------------------------------------------------
// DELETE LETTERS FROM THE BAG

void Bag::deleteLetters(int i) {
    letters.erase(letters.begin() + i);
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
    bool checkIfCanPlay(Board& b, Bag& letterBag);
    bool validMoveExist(vector<char>& playableLetters);
    vector<char> readLetterToChange(int i);
    bool changeHand(Bag& letterbag);
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
        letterBag.deleteLetters(i);
    }
}

//--------------------------------------------------------------------------------
// CHECK IF CAN PLAY

bool Hand::checkIfCanPlay(Board& b, Bag& letterbag) {
    vector<char> playableLetters = b.getPlayableLetters();
    if (validMoveExist(playableLetters)) { // it is true if there is a valid move so the player can play
        return true;
    }
    else { // if there is no valid moves it asks for substitutions in the hand                                              
        cout << BLUE << "You have no play options!" << endl << NO_COLOR;
        cout << " " << endl;
        vector<char> selectedLetters;
        int aux = 0;
        if (letterbag.getLetters().size() == 0) { // true if the bag is empty
            cout << BLUE << "The bag is empty, you cannot change letters, your turn has passed!" << endl << NO_COLOR;
            return false; // the player is not gonna play
        }
        else if (letterbag.getLetters().size() == 1) { // true if the bag has only 1 letter
            cout << BLUE << "You can only exchange one letter from your hand with the bag!" << endl << NO_COLOR;
            aux = 1;
        }
        else {
            cout << BLUE << "You can exchange one or two letters from your hand with the bag!" << endl << NO_COLOR;
            aux = 2;
        }
        if (aux != 0) {
            bool isValid = false;
            do {
                if (aux == 1)
                    selectedLetters = readLetterToChange(0); // can return one letter or QUIT or PASS
                else
                    selectedLetters = readLetterToChange(1); // can return one or two letters or QUIT or PASS
                if (selectedLetters.at(0) == 'P' && selectedLetters.at(1) == 'A' && selectedLetters.at(2) == 'S' && selectedLetters.at(3) == 'S') {
                    cout << BLUE << "You passed your turn!" << endl << NO_COLOR;
                    return false;
                }
                else if (selectedLetters.at(0) == 'Q' && selectedLetters.at(1) == 'U' && selectedLetters.at(2) == 'I' && selectedLetters.at(3) == 'T') {
                    //deletePlayer(); // it returns true if the player was deleted correctely
                    cout << BLUE << "You quit the game!" << endl << NO_COLOR;
                    return false;
                }
                else
                    isValid = changeHand(letterbag); // do the substitution and it returns true if the substitution was completed correctely        
            } while (!isValid);
            if (validMoveExist(playableLetters)) { // it is true if there is a valid move so the player can play
                return true;
            }
            else {
                cout << BLUE << "Still have no play options!" << endl << NO_COLOR;
                return false; // the player is not gonna play
            } 
        }
    }
}

//--------------------------------------------------------------------------------
// READ LETTERS FROM HAND TO CHANGE

vector<char> Hand::readLetterToChange(int i) {
    string input;
    do {
        if(i == 0) // Can only change one letter;
            cout << "Which hand letter do you want to change (QUIT/PASS)? " << endl;
        else // Can change one or two letters;
            cout << "Which hand letter/s do you want to change (QUIT/PASS)? " << endl;
        if (getline(cin, input)) {
            if (input == "QUIT" || input == "PASS") { // check if the person wants to quit or pass
                return { input[0], input[1], input[2], input[3] };
            }
            else {
                if (i == 0) { // to the case where there is only one letter in the bag
                    if (input.size() == 1)
                        return { input[0] };
                    else
                        cout << RED << "You can only choose one letter from your hand! " << endl << NO_COLOR;
                }
                else { // to the case where there is two or more letters in the bag
                    if (input.size() == 1) // if input is only one letter
                        return { input[0] };
                    else if (input.size() == 3 && input[1] == ' ') // if input is two letters separeted bu a space
                        return { input[0], input[2] };
                    else
                        cout << RED << "You can only choose one or two letter from your hand separated by space! " << endl << NO_COLOR;
                }
            }                
        }
    } while (true);
}

//--------------------------------------------------------------------------------
// CHANGE LETTERS BETWEEN HAND AND BAG

bool Hand::changeHand(Bag& letterbag) {

}


//--------------------------------------------------------------------------------
// CHECK IF THERE IS A VALID MOVE

bool Hand::validMoveExist(vector<char>& playableLetters) {
    for (char ch1 : playableLetters) {
        for (char ch2 : playerHand) {
            if (ch1 == ch2) {
                return true; // common character found between the player's hand and the playable letters on the board
            }
        }
    }
    return false;
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
    Hand getHand() const;
    void play(Board& b, Bag& letterBag);
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

Hand Player::getHand() const {
    return hand_;
}

//--------------------------------------------------------------------------------
// PLAY LETTERS

void Player::play(Board& b, Bag& letterBag) {
    bool isValid = false;
    string letter;
    do {
        hand_.showHand();
        cout << "In which position do you want to play (Lc)? ";
        getline(cin, letter);

    } while (!isValid);
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
    vector<Player> getListPlayers() const;
    void showPlayers() const;
private:
    vector<Player> playersList;
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
// GETS AND SETS

vector <Player> ListPlayer::getListPlayers() const {
    return playersList;
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
                // clean input buffer
                cin.ignore(1000, '\n');
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
        for (int i = 0; i < listPlayer.getListPlayers().size(); i++) {
            listPlayer.getListPlayers().at(i).getHand().showHand();
            if (listPlayer.getListPlayers().at(i).getHand().checkIfCanPlay(b, letterBag)) {
                listPlayer.getListPlayers().at(i).play(b, letterBag);
            }
            if (!b.getEnd())
                break;
        }
    }

    return 0;

    
}