// blackjack test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*
   Blackjack is a card game where the objective is to have a hand with a higher value than the dealer's hand, without exceeding a total value of 21.
   The dealer deals two cards face up to each player and one card face up to themselves.
   The player must decide whether to "hit" (take another card) or "stand" (keep their current hand) in order to get as close to a total of 21 as possible without exceeding it.
   Once the player has finished their turn, the dealer reveals their face-down card and must hit until their hand totals 17 or more.
   If the dealer busts (goes over 21), all remaining players win. If not, the player with a hand closest to 21 without going over wins the game.
*/
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

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
        cout << "Welcome to Blackjack!" << endl;
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
    int getBet() {
        int bet;
        while (true) {
            cout << "How much would you like to bet? (Minimum bet is $10)" << endl;
            cin >> bet;
            if (bet >= 10 && bet <= playerMoney) {
                return bet;
            }
            else {
                cout << "Invalid bet amount. Please enter a bet between $10 and $" << playerMoney << "." << endl;
            }
        }
    }
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
                cin >> choice;
                if (choice == 'h' || choice == 's') {
                    break;
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
    Game game(500);
    game.startGame();
    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
