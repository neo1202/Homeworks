#include<iostream>
#include<algorithm>
#include<cstring>
using namespace std;
 
const int setLen = 10000;

class Card
{
private:
    char suit;
    int rank;
    static int Spoint; 
    static int Hpoint; 
    static int Dpoint; 
    static int Cpoint; 
public: 
    Card();
    Card(char suit, int rank);
    char getSuit();
    int getRank();
    void printNum();
    friend class Deck;
    //bool isBiggerThan(const Card& c) const ;
    bool operator>(const Card& c) const ;

    static int getSpoint();
    static void setSpoint(int color);
    static int getHpoint();
    static void setHpoint(int color);
    static int getDpoint();
    static void setDpoint(int color);
    static int getCpoint();
    static void setCpoint(int color);
};
class Deck
{
private:
    Card* mycards;
    int cardCnt;
public:
    Deck();
    ~Deck();
    void addCard(Card a);
    void printDeck() const ;
    Card getCard(int);
    Deck(const Deck &deck);
    void sortCardsDesc();
};

int Card::Spoint = -1;
int Card::Hpoint = -1;
int Card::Dpoint = -1;
int Card::Cpoint = -1;

int main(){
    Deck player;
    char url[setLen]; //存字串 
    char delim[] = ",";//用他切  
    int howmany = 0;
    cin >> url;
    char* start = strtok(url, delim);
    while(start != nullptr)
    {
        howmany++; //總共多少牌 
        int number = static_cast<int>(*(start+1)-'0');
        if(*(start+1)=='1' and *(start+2)=='0'){
            number = 10;
        }
        else if(*(start+1) == 'A'){
            number = 1;
        }
        else if(*(start+1) == 'J'){
            number = 11;
        }
        else if(*(start+1) == 'Q'){
            number = 12;
        }
        else if(*(start+1) == 'K'){
            number = 13;
        }
        Card a = Card(*start, number);
        player.addCard(a);
        //cout<<a.getSuit()<<" "<<a.getRank()<<endl;
        start = strtok(nullptr, delim);
    }
    cin>>url;
    for(int i=0; i<4; i++){
        if(url[i]=='S'){
            Card::setSpoint(4-i);
        }
        if(url[i]=='H'){
            Card::setHpoint(4-i);
        }
        if(url[i]=='D'){
            Card::setDpoint(4-i);
        }
        if(url[i]=='C'){
            Card::setCpoint(4-i);
        }
    }
    player.sortCardsDesc(); 
    player.printDeck();
    return 0;
}
void Deck::sortCardsDesc(){
    for(int i=cardCnt-1 ; i>0 ; i--){
        for(int j=0 ; j<i ; j++){
            if(!(mycards[j]>mycards[j+1])){
                Card tmp = mycards[j];
                mycards[j] = mycards[j+1];
                mycards[j+1] = tmp;
            }
        }
    }
}
Card::Card()
{
    suit = 0;
    rank = 0;
}
Card::Card(char suit, int rank)
{
    this->suit = suit;
    this->rank = rank;
}
char Card::getSuit(){
    return suit;
} 
int  Card::getRank(){
    return rank;
} 
bool Card::operator>(const Card& c) const
{
    int me = this->rank;
    int you = c.rank;
    if(me == 1){
        me = 14;
    }
    if(you == 1){
        you = 14;
    }
    if(me > you){
        return true;
    }
    else if(me < you){
        return false;
    }
    else{
        if(this->suit=='S') me = Spoint;
        if(this->suit=='H') me = Hpoint;
        if(this->suit=='D') me = Dpoint;
        if(this->suit=='C') me = Cpoint;
        if(c.suit=='S') you = Spoint;
        if(c.suit=='H') you = Hpoint;
        if(c.suit=='D') you = Dpoint;
        if(c.suit=='C') you = Cpoint;
        if(me > you){
            return true;
        }
        else{
            return false;
        }
    }
    return 0;
}
Deck::Deck() //最多52張牌 
{
    mycards = new Card[60];
    cardCnt = 0;
}
Deck::~Deck(){
    delete []mycards;
    mycards = nullptr;
}
Deck::Deck(const Deck& deck)
{
    cardCnt = deck.cardCnt;
    mycards = new Card[cardCnt];
    for (int i = 0; i < cardCnt; i++)
        mycards[i] = deck.mycards[i];
}
void Deck::addCard(Card a){
    mycards[cardCnt] = a;
    cardCnt++; //從0開始
}
void Deck::printDeck() const
{
    cout<<mycards[0].getSuit();
    mycards[0].printNum();
    for(int i = 1 ; i<cardCnt ; i++){
        cout<<",";
        cout<<mycards[i].getSuit();
        mycards[i].printNum();
    }
    cout<<endl;
}
void Card::printNum()
{
    if(rank == 1){
        cout<<"A";
    }
    else if(rank == 11){
        cout<<"J";
    }
    else if(rank == 12){
        cout<<"Q";
    }
    else if(rank == 13){
        cout<<"K";
    }
    else{
        cout<<rank;
    }
}
int Card::getSpoint(){
    return Spoint;
}
int Card::getHpoint(){
    return Hpoint;
}
int Card::getDpoint(){
    return Dpoint;
}
int Card::getCpoint(){
    return Cpoint;
}
void Card::setSpoint( int point ){
    Spoint = point ;
}
void Card::setHpoint( int point ){
    Hpoint = point ;
}
void Card::setDpoint( int point ){
    Dpoint = point ;
}
void Card::setCpoint( int point ){
    Cpoint = point ;
}