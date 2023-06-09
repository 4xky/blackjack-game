//Group 2 Final Project
//Kyle Willhite, Hakeem Fessuh, Bosoon Karimi
//CSC-132-01
//Professor Kmir 

//Purpose: Source code for users to play the Black-jack Card game using C++ code

/*
   Blackjack is a card game where the objective is to have a hand with a higher value than the dealer's hand, without exceeding a total value of 21.
   The dealer deals two cards face up to each player and one card face up to themselves.
   The player must decide whether to "hit" (take another card) or "stand" (keep their current hand) in order to get as close to a total of 21 as possible without exceeding it.
   Once the player has finished their turn, the dealer reveals their face-down card and must hit until their hand totals 17 or more.
   If the dealer busts (goes over 21), all remaining players win. If not, the player with a hand closest to 21 without going over wins the game.
*/

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Blackjack test.cpp : This file contains the 'main' function. Program execution begins and ends there.

//Libraries needeed to run code are all listed here
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
//for easier code management
using namespace std;

//Created a class to establish cards that can hold integer values within the game
class Card {
private:
    int value;
    string name;
public:
    Card(int v, string n) {
        value = v;
        name = n;
    }
    int getValue() {
        return value;
    }
    void displayCard() {
        cout << name << endl;
    }
};
//Created a class, used void functions, and for loops to establish the deck that holds the cards and the values that will randomly be stored within them
class Deck {
private:
    vector<Card> cards;
public:
    void generateDeck() {
        string suits[] = { "Hearts", "Diamonds", "Clubs", "Spades" };
        string faceCards[] = { "Jack", "Queen", "King", "Ace" };
        for (int s = 0; s < 4; s++) {
            for (int i = 2; i <= 10; i++) {
                Card c(i, to_string(i) + " of " + suits[s]);
                cards.push_back(c);
            }
            for (int j = 0; j < 4; j++) {
                Card c(10, faceCards[j] + " of " + suits[s]);
                cards.push_back(c);
            }
            Card c(11, "Ace of " + suits[s]);
            cards.push_back(c);
        }
    }
    void shuffleDeck() {
        srand(time(NULL));
        random_shuffle(cards.begin(), cards.end());
    }
    Card dealCard() {
        Card c = cards.back();
        cards.pop_back();
        return c;
    }
};
//Created a class with loops, if statements, and void functions being used to obtain and display a randomly generated hand
//Seperated aces from the rest of the deck since they require user input if obtained
class Hand {
private:
    vector<Card> cards;
public:
    void addCard(Card c) {
        cards.push_back(c);
    }
    int getHandValue() {
        int value = 0;
        int aces = 0;
        for (int i = 0; i < cards.size(); i++) {
            value += cards[i].getValue();
            if (cards[i].getValue() == 11) {
                aces++;
            }
        }
        while (value > 21 && aces > 0) {
            value -= 10;
            aces--;
        }
        return value;
    }
    void displayHand(bool showFirstCard) {
        for (int i = 0; i < cards.size(); i++) {
            if (i == 0 && !showFirstCard) {
                cout << "Hidden Card" << endl;
            }
            else {
                cards[i].displayCard();
            }
        }
    }
};
//Created a class to call 2 randomly generated hands, inputted bets, and players given default money amount of $500 to begin the game
//Gameplay functions included below
class Game {
private:
    Deck deck;
    Hand playerHand;
    Hand dealerHand;
    int playerMoney;
    int betAmount;
public:
    Game(int startingMoney) {
        playerMoney = startingMoney;
    }
    void startGame() {
        while (playerMoney > 0) {
            cout << "You currently have $" << playerMoney << "." << endl;
            betAmount = getBet();
            deck.generateDeck();
            deck.shuffleDeck();
            playerHand = Hand();
            dealerHand = Hand();
            playerHand.addCard(deck.dealCard());
            dealerHand.addCard(deck.dealCard());
            playerHand.addCard(deck.dealCard());
            dealerHand.addCard(deck.dealCard());
            bool playerBust = playerTurn();
            if (!playerBust) {
                dealerTurn();
            }
            determineWinner(playerBust);
        }
        cout << "You are out of money. Game over!" << endl;
    }
//Loops and if statements to establish play, bet amount, player money, and associated errors 
    int getBet() {
        int bet;
        while (true) {
            cout << "How much would you like to bet? (Minimum bet is $10)" << endl;
            cout << "Type 0 to quit out of the game." << endl; //Added output message if user decides to quit the game.
            cin >> bet;
            if (bet == 0) { //Check if bet equals 0 before checking the other conditions to ensure the program exits immediatly if the player wants to quit, without accepting a bet of 0 as a valid bet. 
                cout << "You have quit the game thanks for playing!" << endl; //If input is 0 the program will exit. 
                exit(0); //exit() function is used to terminate program immediatly refardless of where it is in program.
            }
            else if (bet >= 10 && bet <= playerMoney) {
                return bet;
            } 
            else {
                cout << "Invalid bet amount. Please enter a bet between $10 and $" << playerMoney << "." << endl;
            }
        }
    }
   //Player game play function using if statments and loops to allow the user to input choices and play.
    bool playerTurn() {
        while (true) {
            cout << "Your hand:" << endl;
            playerHand.displayHand(false);
            cout << "Dealer's hand:" << endl;
            dealerHand.displayHand(true);
            cout << "Your hand value is: " << playerHand.getHandValue() << endl;
            char choice;
            while (true) {
                cout << "Would you like to hit (h) or stand (s)?" << endl;
                cout << "Type q to quit out of the game" <<endl; //Added output message if user decides to quit the game.
                cin >> choice;
                if (choice == 'h' || choice == 's') {
                    break;
                } else if (choice == 'q' || choice == 'Q'){ //If the input is lower/upppercase q program will exit
                    cout <<"You have quit the game thanks for playing!" << endl; //If input is 0 the program will exit.
                    exit(0); //exit() function is used to terminate program immediatly refardless of where it is in program.
                }
                else {
                    cout << "Invalid input. Please enter 'h' for hit or 's' for stand." << endl;
                }
            }
            if (choice == 'h') {
                playerHand.addCard(deck.dealCard());
                int handValue = playerHand.getHandValue();
                if (handValue > 21) {
                    cout << "Bust! Your hand value is " << handValue << "." << endl;
                    return true;
                }
            }
            else {
                break;
            }
        }
        return false;
    }
   //The dealer algorithm play function which outputs to the user the neccessary dealer information for gameplay
    void dealerTurn() {
        while (dealerHand.getHandValue() < 17) {
            dealerHand.addCard(deck.dealCard());
        }
        cout << "Dealer's hand:" << endl;
        dealerHand.displayHand(false);
        cout << "Dealer's hand value is: " << dealerHand.getHandValue() << endl;
    }
    void determineWinner(bool playerBust) {
        int playerValue = playerHand.getHandValue();
        int dealerValue = dealerHand.getHandValue();
        if (playerBust) {
            cout << "Dealer wins!" << endl;
            playerMoney -= betAmount;
        }
        else if (dealerValue > 21) {
            cout << "Player wins!" << endl;
            playerMoney += betAmount;
        }
        else if (dealerValue > playerValue) {
            cout << "Dealer wins!" << endl;
            playerMoney -= betAmount;
        }
        else if (dealerValue == playerValue) {
            cout << "Push!" << endl;
        }
        else {
            cout << "Player wins!" << endl;
            playerMoney += betAmount;
        }
    }
};

int main() {
    /*While loop that will display output including options for the user to play or quit the game.
    if statments inside of our loop that will either start the game, quit the game, or if none of user inputs match criteria than the program will go back to the beginning of the loop until one of the conditions are met.*/
    while (true) {
        cout << "Welcome to Blackjack!" <<endl;
        cout <<"Blackjack Menu:" <<endl;
        cout << "P. Play" <<endl;
        cout << "Q. Quit" <<endl;
        char decide;
        cin >> decide;
        if (decide == 'p' || decide == 'P') {
            Game game(500);
            game.startGame();
        } else if (decide == 'q' || decide == 'Q') {
            cout << "You have quit the game thanks for playing!" <<endl;
            return 0;
        } else {
            cout << "Invalid option, Please try again." <<endl;
        }
    }
    
}
