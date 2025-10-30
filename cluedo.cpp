#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <vector>
#include <graphics.h>

using namespace std;
class Card;
class Character;
class Player;
class Cpu;
class Room
{
public:
    string name;
    int index;//index on board
    int nPeop;//no of people in the room
    Room(string n, int ind)
    {
        name=n;
        index=ind;
        nPeop=0;
    }
};


class Card
{
    public:
    string name_of_card;
    string type;//weapon, character,room

        Card(){}
    Card(string nm,string tp)
    {
        name_of_card=nm;
        type=tp;
    }
};
class Character
{
public:
    int C[6];//allocated cards of the player
    string name;
    int stind;
    int pos;
    int color; //color code
    int roomPos;
    int currentRoom;
    Character(){};
    Character(string n, int st, int col)
    {
        name=n;
        stind=st;
        color=col;
        pos=stind;
        currentRoom=-1;
    }
    void setDetails(Character C)
    {
        name=C.name;
        stind=C.stind;
        color=C.color;
        pos=stind;
        currentRoom=-1;
    }

    void placeToken()
    {
        setcolor(color);
        if (pos<17)
            circle(20,20+pos*40,10);
        else if (pos<33)
            circle(20+40*(pos-16),660,10);
        else if (pos<49)
            circle(660,20+(49-pos)*40,10);
        else
            circle(60+(64-pos)*40,20,10);
    }
    void removeToken()
    {
        setcolor(3);
        if (pos<17)
            circle(20,20+pos*40,10);
        else if (pos<33)
            circle(20+40*(pos-16),660,10);
        else if (pos<49)
            circle(660,20+(49-pos)*40,10);
        else
            circle(60+(64-pos)*40,20,10);
    }


    void outOfRoom(Room r)
    {
        if (roomPos=0)
            pos++;
        r.nPeop--;
        currentRoom=-1;
    }



    };

class Cpu : public Character
{
public:
    int checkList[20];
    Cpu()
    {
    for(int i=0;i<20;i++)
        checkList[i]=0;
    }
    void suspect(int turn,Cpu a, Player b, int room, Card c[]);
    int accuse(int mur[], int acc[]);
    int checkAccuse(int mur[]);
    void rollDice(Room* rooms)
    {
//        cout<<"Current:"<<currentRoom<<endl;
        if(currentRoom!=-1)
        {
            outOfRoom(rooms[currentRoom]);
        }
        int val=((rand()%6)+1);
//        cout<<val<<endl;
        cout<<"Rolled:"<<val<<endl;

        //QT CODE FOR MOVEMENT
        int ch=0;
        for (int i=0;i<8;i++)
        {
//            cout<<"forLoop"<<endl;
           if ((pos<rooms[i].index && (rooms[i].index)<=(pos+val)%64) || (i==0 && pos+val>=64))
                {
                    if (rooms[i].nPeop==0)
                    {
                        removeToken();
                        pos=rooms[i].index;
                        currentRoom=i;
                        placeToken();
                        roomPos=0;
                    }
                    else
                    {
                        removeToken();
                        pos=rooms[i].index + 1;
                        placeToken();
                        roomPos=1;
                    }
                    rooms[i].nPeop++;
                }
        }
//        cout<<"Out";
        if (currentRoom==-1)
        {
//            cout<<"In";
            removeToken();
            pos+=val;
            pos=pos%64;
            placeToken();
//            cout<<"INNNNNNNNNNN";
        }
    }

   };
class Player: public Character
{
public:
    int sus[3];
    void suspect(Cpu a, Cpu b, int room);
    int accuse(int arr[]);
    void rollDice(Room* rooms)
    {
//        cout<<"Current Start:"<<currentRoom<<endl;

        if(currentRoom!=-1)
        {
            outOfRoom(rooms[currentRoom]);
        }

        int val=((rand()%6)+1);
        cout<<"Rolled:"<<val<<endl;
        //QT CODE FOR MOVEMENT
        int ch=0;
        for (int i=0;i<8;i++)
        {
//            cout<<"forLoop"<<endl;
           if ((pos<rooms[i].index && (rooms[i].index)<=(pos+val)%64) || (i==0 && pos+val>=64))
            {
                cout<<"Press 1 to enter "<<rooms[i].name<<" else 0"<<endl;
                cout<<"Choice:";
                cin>>ch;
                cout<<endl;
                if (ch==1)
                {
                    if (rooms[i].nPeop==0)
                    {
                        removeToken();
                        pos=rooms[i].index;
                        currentRoom=i;
                        placeToken();
                        roomPos=0;
                    }
                    else
                    {
                        removeToken();
                        pos=rooms[i].index + 1;
                        placeToken();
                        roomPos=1;
                    }
                    rooms[i].nPeop++;

                }
                else
                {
                    ch=0;
                    removeToken();
                    pos++;
                    pos=pos%64;
                    break;
                }
            }

        }
        if (ch==0)
        {
            removeToken();
            pos+=val;
            pos=pos%64;
            placeToken();
        }
//        cout<<"Current End:"<<currentRoom<<endl;
    }

//    friend class Cpu;
};

int Player::accuse(int mur[])//A=MURDERER LIST
{
    int choice;
    int c=0;
    int acc[3];
    cout<<"GIVE YOUR ACCUSATION:"<<endl;
    c1:
    cout<<"Select a Weapon(1-6)"<<endl;
    cin>>choice;
    if(choice<7 && choice>0)
        acc[0]=choice-1;
    else
    {
        cout<<"Enter a valid choice for Weapon"<<endl;
        goto c1;
    }
    c2:
    cout<<"Select a Character(7-12)"<<endl;
    cin>>choice;
    if(choice<13 && choice>6)
        acc[1]=choice-1;
    else
    {
        cout<<"Enter a valid choice for Character"<<endl;
        goto c2;
    }
    c3:
    cout<<"Select a Room(13-20)"<<endl;
    cin>>choice;
    if(choice<21 && choice>12)
        acc[2]=choice-1;
    else
    {
        cout<<"Enter a valid choice for Room"<<endl;
        goto c3;
    }

    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            if(mur[i]==acc[j])
                c++;

    if(c==3)
        return 1;
    else
        return 0;
}

void Player::suspect(Cpu a, Cpu b, int room)
{
    int choice;
    vector <int> s;
    vector <int> s1;
    cout<<"ENTER YOUR SUGGESTIONS!"<<endl;
    c1:
    cout<<"Select a Weapon(1-6)"<<endl;
    cin>>choice;
    if(choice<7 && choice>0)
        sus[0]=choice-1;
    else
    {
        cout<<"Enter a valid choice for weapon"<<endl;
        goto c1;
    }
    c2:
    cout<<"Select a Character(7-12)"<<endl;
    cin>>choice;
    if(choice<13 && choice>6)
        sus[1]=choice-1;
    else
    {
        cout<<"Enter a valid choice for Character"<<endl;
        goto c2;
    }
    sus[2]=room;
    for(int i=0; i<6; i++)
        for(int j=0; j<3; j++)
        {
            if(a.C[i]==sus[j])
            {
//                cout<<a.C[i]<<endl;
                s.push_back(a.C[i]);
                break;
            }
        }

    if(s.empty())
    {
        for(int i=0; i<6; i++)
            for(int j=0; j<3; j++)
            {
                if(b.C[i]==sus[j])
                {
                    cout<<b.C[i]<<endl;
                    s1.push_back(b.C[i]);
                    break;
                }
            }
            if(s1.empty())
                {
                    cout<<"NOBODY SHOWS A CARD!"<<endl;
                }
            else//OTHER CPU DISPLAYS CARD to CPU
                {
                    cout<<"OTHER PLAYER SHOWS A CARD!:";
                    cout<<s1[rand()%s1.size()];
                }

    }
    else//OTHER PLAYER WILL SHOW
    {
        cout<<"Card Shown:"<<s[rand()%s.size()]-1<<endl;
    }
}

int Cpu::checkAccuse(int mur[])
{
    int acc[3];
    int counter=0,val=0;
    for(int i=0;i<6;i++)
    {
        if(checkList[i]==3)
        {
        counter=counter +1;
        acc[0]=i;
         break;
        }
    }
    for(int i=6;i<12;i++)
    {
        if(checkList[i]==3)
        {
        counter=counter +1;
        acc[1]=i;
         break;
        }
    }
    for(int i=12;i<20;i++)
    {
        if(checkList[i]==3)
        {
        counter=counter +1;
        acc[2]=i;
         break;
        }
    }
    if(counter==3)
        {val=accuse(mur, acc);
        return val;
        }
    else
        return 0;

}

int Cpu::accuse(int mur[], int acc[])//WHEN 1 DEF IN EACCH CAT//A=MURDERER LIST
{
    int c=0;
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            if(mur[i]==acc[j])
                c++;
    if(c==3)
        return 2;
    else
        return 1;
}
void Cpu::suspect(int turn,Cpu a, Player b, int room, Card d[])
{
    //chooses suspects
    //ROOM AND SUGGESTION SHOULD HAVE W,C,R
    int w=0;
    int c=0;
    int sus[3];
    sus[2]=room+12;

    for(int i=0; i<20; i++)
    {
        if(checkList[i]==1)
        {
            if(i<6 && c==0)
            {
                sus[0]=i;
                c++;
                i=5;
                continue;
            }
            else if(i<12 && w==0)
            {
                sus[1]=i;
                w++;
                break;
            }
        }
    }
//cout<<"1  =";
    if(w==0 || c==0)
    {
        for(int i=0; i<20; i++)
            if(checkList[i]==0)
            {
                if(i<6 && c==0)
                {
                    sus[0]=i;
                    c++;
                    i=5;
                    continue;
                }
                else if(i<12 && w==0)
                {
                    sus[1]=i;
                    w++;
                    break;
                }
            }
     }
    for(int i=0; i<3; i++)
        if(checkList[sus[i]]<1)
            checkList[sus[i]]=1;

    //logic for suspect list

    vector <int> s;//matching
    vector <int> s1;
    if(turn==2)//next person, on left, is User (b)
    {
        int choice;
        //suspect cards matching with player cards
        cout<<"Cpu Suggested:";
        for(int val:sus)
            cout<<d[val].name_of_card<<" ";
            cout<<endl;


        for(int i=0; i<3; i++)//checklist of other cpu updated
            if(a.checkList[sus[i]]<1)
                a.checkList[sus[i]]=1;

        for(int i=0; i<6; i++)
            for(int j=0; j<3; j++)
            {
                if(b.C[i]==sus[j])
                {
//                    cout<<b.C[i]<<endl;
                    s.push_back(b.C[i]);
                    break;
                }
            }



        if(s.empty())
        {
        for(int i=0; i<6; i++)
            for(int j=0; j<3; j++)
            {
                if(a.C[i]==sus[j])
                {
                    cout<<a.C[i]<<endl;
                    s1.push_back(a.C[i]);
                    break;
                }
            }
            if(s1.empty())
                {for(int i=0; i<3;i++)
                    if(checkList[sus[i]]<2)
                        checkList[sus[i]]=3;
                }
            else//OTHER CPU DISPLAYS CARD to CPU
                {
                    cout<<"OTHER PLAYER SHOWS A CARD!"<<endl;
                    checkList[s1[rand()%s1.size()]]=2;
                }

        }
        else
        {   int valid=1;
            cout<<"Enter Card to show:"<<endl;
            for(int val: s)
            cout<<val<<" ";
            cout<<endl;
            while(valid)//Handling proper input
            {
                cin>>choice;
                for(int i=0; i<s.size() ;i++)
                    if(choice==s[i])
                {
                    cout<<"Choice:"<<choice<<endl;
                    checkList[s[i]]=2;
                    valid=0;
                    break;
                }
                if(valid==1)
                    cout<<"Enter a valid choice!"<<endl;

            }
        }
    }
    else
    {
        for(int i=0; i<3; i++)
            for(int j=0; j<5; j++)
            {
                if(a.C[j]==sus[i])
                {
                    cout<<a.C[j]<<endl;
                    s.push_back(a.C[j]);
                    break;
                }
            }
        cout<<"Cpu hows card to the other Cpu"<<endl;
        if(s.empty())//CHECK FOR USER add 3 FOR DEFINITE
        {
            checkList[a.C[rand()%5]]=2;
        }
        else
        {
            checkList[s[rand()%s.size()]]=2;
        }
    }
}

class Play
{
public:
    vector <int> cards;
    int murderer[3];

        Player U;
        Cpu F;
        Cpu S;
        int turn;
        Play(){}
        void movement(){};
    void allocmurderer()
    {
        for(int i=0;i<20;i++)
        {
            cards.push_back(i);
        }
        murderer[0]=rand()%6;
       // sleep(3);
        murderer[1]=(rand()%6)+6;
        murderer[2]=(rand()%8)+12;
        for(int i=0;i<3;i++)
        cout<<murderer[i]<<endl;
        for(int i=0;i<20;i++)
        {
            for(int j=0;j<3;j++)
            {
            if(cards[i]==murderer[j])
            {
                for(int k=i;k<20-j;k++)
                {
                    cards[k]=cards[k+1]; //should pop here only
                }
            }
        }
        }
        for(int i=0;i<3;i++)
        {
            cards.pop_back();
        }
//        cout<<"Murderer allocated!"<<endl;
    }
    int checkCard(int ind,int counter)
    {
      for(int i=0;i<18-counter;i++)
      {
          if(cards[i]==ind)
              return 0;
              else
                return 1;// return 1 oly if card not found
      }
    }

    void allocate()
    {
    srand(time(0));
        int val,x=0;
        for(int i=0;i<6;i++)
        {

            val=rand()%20;
             while(checkCard(val,x))
               {
//                   sleep(1);
                  val=rand()%20;
//                  val=rand()%20;

               }
               if(i<2)
               U.C[i]=val;
               else if(i<4)
                U.C[i]=val+6;
               else
                 U.C[i]=val+12;

//               cout<<"Allocated card is: "<<U.C[i]<<endl;
               x=x+1;
               for(int p=0;p<18-x;p++)
      {
          if(cards[p]==val)
               {
                for(int k=p;k<18-x;k++)
                {
                    cards[k]=cards[k+1];
                }
            cards.pop_back();
            break;
        }
      }
//      cout<<"Player0"<<endl;
        }
        for(int i=0;i<6;i++)
        {
            val=rand()%20;
             while(checkCard(val,x))
               {
                  val=rand()%20;
               }
               F.C[i]=val;
//                cout<<"Allocated card is: "<<U.C[i]<<endl;
               x=x+1;
          for(int p=0;p<18-x;p++)
      {
          if(cards[p]==val)
               {
                for(int k=p;k<18-x;k++)
                {
                    cards[k]=cards[k+1];
                }
            cards.pop_back();
            break;
        }
        }
//         cout<<"Player1"<<endl;
        }
        for(int i=0; i<6;i++)
            F.checkList[F.C[i]]=2;


                 for(int i=0;i<5;i++)
        {
            val=rand()%20;
             while(checkCard(val,x))
               {
                  val=rand()%20;
               }
               S.C[i]=val;
//                cout<<"Allocated card is: "<<S.C[i]<<endl;
               x=x+1;
               for(int p=0;p<18-x;p++)
      {
          if(cards[p]==val)
               {
                for(int k=p;k<18-x;k++)
                {
                    cards[k]=cards[k+1];
                }
            cards.pop_back();
        }
        }
//        cout<<"Player2"<<endl;
        }
         for(int i=0; i<5;i++)
        {
          S.checkList[S.C[i]]=2;
//          cout<<"Check"<<endl;
        }

        /////////////////////////////////////////////////////

//        cout<<"Murderer: "<<endl;
//        for(int i=0;i<3;i++)
//        {
//            cout<<murderer[i]<<endl;
//        }
//         cout<<"User: "<<endl;
//        for(int i=0;i<6;i++)
//        {
//            cout<<U.C[i]<<endl;
//        }
//        cout<<"Player 1: "<<endl;
//        for(int i=0;i<6;i++)
//        {
//            cout<<F.C[i]<<endl;
//        }
//        cout<<"Player 2: "<<endl;
//        for(int i=0;i<5;i++)
//        {
//            cout<<S.C[i]<<endl;
//        }
//        for(int i=0;i<20;i++)
//        {
//            cout<<cards[i]<<endl;
//        }
    }

};
void pressTC()
{
    cout<<"Press to continue..."<<endl;
    cin.get();
}
 int main()
    {
        int turn = 0;
        //Character Initialization
        Character characters[]={Character("Mrs. Peacock",4,1),Character("Colonel Mustard",12,14),Character("Professor Plum",20,5),Character("Miss Scarlet",29,4),Character("Mrs. White",36,15),Character("Mr. Green",45,2)};
 //Room Initialization
        Room rooms[]={Room("Kitchen",0),Room("Dining Room",8),Room("Lounge",16),Room("Hall",24),Room("Study",32),Room("Library",40),Room("Billiard Room",48),Room("Ball Room",56)};

        //Card Initialization
        srand(time(0));
        Card C[]={Card("Dagger", "Weapon"),Card("Candlestick","Weapon"),Card("Rope","Weapon"),Card("Lead Pipe","Weapon"),Card("Revolver","Weapon"),Card("Wrench","Weapon"),
        Card("Mustard","Character"),Card("Plum","Character"),Card("White","Character"),Card("Scarlet","Character"),Card("Green","Character"),Card("Peacock","Character"),
        Card("Hall","Room"), Card("Lounge","Room"), Card("Dining Room","Room"), Card("Kitchen","Room"), Card("Ballroom","Room"), Card("Billiard Room","Room"), Card("Library","Room"), Card("Study","Room")};
        Play P;
        P.allocmurderer();
        P.allocate();
        ///////////////////////////////////
       /*while(1)
        {
            if(turn==0)
            {
            cout<<"Roll Die"<<endl;
            cin.get();*/

        system("cls");
    const std::string blue = "\033[1;34m";
    const std::string black = "\033[30m";
    const std::string red = "\033[31m";
const std::string green = "\033[32m";
const std::string yellow= "\033[33m";
const std::string magenta = "\033[35m";
const std::string cyan= "\033[36m";
const std::string white= "\033[37m";
cout<<"\033[1m";
cout <<yellow<< R"(
  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.        .----------------.  .----------------.
    | .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |      | .--------------. || .--------------. |
    | | _____  _____ | || |  _________   | || |   _____      | || |     ______   | || |     ____     | || | ____    ____ | || |  _________   | |      | |  _________   | || |     ____     | |
    | ||_   _||_   _|| || | |_   ___  |  | || |  |_   _|     | || |   .' ___  |  | || |   .'    `.   | || ||_   \  /   _|| || | |_   ___  |  | |      | | |  _   _  |  | || |   .'    `.   | |
    | |  | | /\ | |  | || |   | |_  \_|  | || |    | |       | || |  / .'   \_|  | || |  /  .--.  \  | || |  |   \/   |  | || |   | |_  \_|  | |      | | |_/ | | \_|  | || |  /  .--.  \  | |
    | |  | |/  \| |  | || |   |  _|  _   | || |    | |   _   | || |  | |         | || |  | |    | |  | || |  | |\  /| |  | || |   |  _|  _   | |      | |     | |      | || |  | |    | |  | |
    | |  |   /\   |  | || |  _| |___/ |  | || |   _| |__/ |  | || |  \ `.___.'\  | || |  \  `--'  /  | || | _| |_\/_| |_ | || |  _| |___/ |  | |      | |    _| |_     | || |  \  `--'  /  | |
    | |  |__/  \__|  | || | |_________|  | || |  |________|  | || |   `._____.'  | || |   `.____.'   | || ||_____||_____|| || | |_________|  | |      | |   |_____|    | || |   `.____.'   | |
    | |              | || |              | || |              | || |              | || |              | || |              | || |              | |      | |              | || |              | |
    | '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |      | '--------------' || '--------------' |
     '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'       '----------------'  '----------------'
                                         .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.
                                        | .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |
                                        | |     ______   | || |   _____      | || | _____  _____ | || |  _________   | || |  ________    | || |     ____     | |
                                        | |   .' ___  |  | || |  |_   _|     | || ||_   _||_   _|| || | |_   ___  |  | || | |_   ___ `.  | || |   .'    `.   | |
                                        | |  / .'   \_|  | || |    | |       | || |  | |    | |  | || |   | |_  \_|  | || |   | |   `. \ | || |  /  .--.  \  | |
                                        | |  | |         | || |    | |   _   | || |  | '    ' |  | || |   |  _|  _   | || |   | |    | | | || |  | |    | |  | |
                                        | |  \ `.___.'\  | || |   _| |__/ |  | || |   \ `--' /   | || |  _| |___/ |  | || |  _| |___.' / | || |  \  `--'  /  | |
                                        | |   `._____.'  | || |  |________|  | || |    `.__.'    | || | |_________|  | || | |________.'  | || |   `.____.'   | |
                                        | |              | || |              | || |              | || |              | || |              | || |              | |
                                        | '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |
                                         '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'
   )" <<endl;
   cout<<endl;
system("pause");
     system("cls");
 cout <<white<<R"(


                                                           "   Dr. Black invited you and five other people to his mansion for dinner
                                                                   but blackmailed you all into supporting his nefarious scheme.
                                                                              Later that night, he is found dead!     "
    )";
    system("pause");
     cout<<"\033[40m";
    cout<<"\033[30m";
    system("cls");
     cout<<white<<R"(


                        "                               At the time of the murder, present in his house were six of his guests -


                                                                     Miss. Scarlett :  the stock femme fatale character. Represented by Red token.

                                                                     Colonel Mustard : a retired military officer with a distinguished career. Represented by Yellow token.

                                                                     Mrs. White : the chief domestic servent. Represented by White token.

                                                                     Mr. Green : the local clergyman. Represented by Green token.

                                                                     Professor Plum : an intelligent yet absent-minded academic. Represented by Purple token.

                                                                     Mrs. Peacock : a stylish widowed socialite. Represented  by a Blue token.   "

                    )";
                    system("pause");
                    system("cls");
                    cout<<white<<R"(


                                                     Ironically enough, the house also housed the same number of weapons as its inmates -

                                                                                     Dagger
                                                                                     Candlestick
                                                                                     Rope
                                                                                     Lead Pipe
                                                                                     Revolver
                                                                                     Wrench.
         )"<<endl;
         system("pause");
                    system("cls");
                    cout <<white<<R"(


                                                     The house, a sprawling gothic mansion had the following rooms -

                                                                           The Hall
                                                                           The Lounge
                                                                           The Dining Room
                                                                           The Kitchen
                                                                           The Ballroom
                                                                           The Billiard Room
                                                                           The Library
                                                                           The Study

                 )"<<endl;
                 system("pause");
                    system("cls");
                    cout<<white<<R"(



                                    Dive into the murder mystery and be the first to figure out the murderer, the murder weapon and the crime room.
                                    By eliminating possibilities,  narrow down the suspects until you are able to identify the murderer successfully.
                                    Strategize and navigate through the clues and come up with the right combination before anyone else does.


                                           That's how you WIN the game. Remember, even though you can make as many number suggestions,
                       you can only make one accusation which would be the deciding factor of whether you WIN or are OUT. So pay attention to the clues and…

)"<< endl;
 system("pause");
                    system("cls");
                     cout<<"\033[0m";
cout<<R"(


                                                                       Good Luck, Sherlock! )";
cout<<endl;
system("pause");
system("cls");

    int n;
    cout<<"Select your character"<<endl;
    cout<<R"( 0 : Mrs. Peacock
              1 : Colonel Mustard
              2 : Professor Plum
              3 : Miss Scarlet
              4 : Mrs. White
              5 : Mr. Green)"<<endl;
              cin>>n;
             P.U.setDetails(characters[n]);
              system("pause");
              system("cls");
    cout<<"Select you opponent 1"<<endl;
    jump :
        int j;
    cout<<R"( 0 : Mrs. Peacock
              1 : Colonel Mustard
              2 : Professor Plum
              3 : Miss Scarlet
              4 : Mrs. White
              5 : Mr. Green)"<<endl;
              cin>>j;
    for (int i=0;i<6;i++)
    {
        if (j==n){
            cout<<"Character already selected\n";
            cout<<"Select other character ";
            goto jump;
            }
    }
   P.F.setDetails(characters[j]);
    system ("pause");
    system("cls");
    cout<<"Select you opponent 2"<<endl;
    jumpi :
        int k;
    cout<<R"( 0 : Mrs. Peacock
              1 : Colonel Mustard
              2 : Professor Plum
              3 : Miss Scarlet
              4 : Mrs. White
              5 : Mr. Green)"<<endl;
              cin>>k;
    for (int i=0;i<6;i++)
    {
        if (k==n||k==j){
            cout<<"Character already selected\n";
            cout<<"Select other character ";
            goto jumpi;
            }
    }
   P.S.setDetails(characters[k]);

    //board display code
    initwindow(1000,700);
    setbkcolor(3);
    cleardevice();
    rectangle(0,0,680,680);
    rectangle(0,0,80,80);
    rectangle(600,0,680,80);
    rectangle(0,600,80,680);
    rectangle(600,600,680,680);
    rectangle(320,0,400,80);
    rectangle(0,320,80,400);
    rectangle(320,600,400,680);
    rectangle(600,320,680,400);
    line(40,80,40,320);
    line(40,400,40,600);
    line(640,80,640,320);
    line(640,400,640,600);
    line(80,40,320,40);
    line(400,40,600,40);
    line(80,640,320,640);
    line(400,640,600,640);
    for (int i=0;i<6;i++)
    {
        line(0,80+40*i,40,80+40*i);
    }
    for (int i=8;i<13;i++)
    {
        line(0,80+40*i,40,80+40*i);
    }
    for (int i=0;i<6;i++)
    {
        line(640,80+40*i,680,80+40*i);
    }
    for (int i=8;i<13;i++)
    {
        line(640,80+40*i,680,80+40*i);
    }
    for (int i=0;i<6;i++)
    {
        line(80+40*i,0,80+40*i,40);
    }
    for (int i=8;i<13;i++)
    {
        line(80+40*i,0,80+40*i,40);
    }
    for (int i=0;i<6;i++)
    {
        line(80+40*i,640,80+40*i,680);
    }
    for (int i=8;i<13;i++)
    {
        line(80+40*i,640,80+40*i,680);
    }
    outtextxy(10,30,"Kitchen");
    outtextxy(330,30,"Ball Room");
    outtextxy(600,30,"Billiard Room");
    outtextxy(0,350,"Dining Room");
    outtextxy(610,350,"Library");
    outtextxy(10,630,"Lounge");
    outtextxy(330,630,"Hall");
    outtextxy(610,630,"Study");
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 5);
    setcolor(YELLOW);
    outtextxy(180 + 15, 180 +150, "CLUEDO");
    P.U.placeToken();
    P.F.placeToken();
    P.S.placeToken();
     P.allocmurderer();
    P.allocate();

    setcolor(15);
    settextstyle(9,0,1);
    outtextxy(700,20,"Your cards:\n");
    outtextxy(700,40,"Name                Type\n");
    for (int i = 0; i < 6; i++) {

    string x = C[P.U.C[i]].name_of_card;
    string y = C[P.U.C[i]].type;
    char* ptr1 = const_cast<char*>(x.c_str());
    char* ptr2 = const_cast<char*>(y.c_str());
    outtextxy(700, 60 + i * 20, ptr1);
    outtextxy(860, 60 + i * 20, ptr2);
}


        system("cls");

    int choice,ch,win=0,fobj=1,sobj=1;
    while(1)
    {
        cout<<"Your turn!"<<endl;
        P.turn=0;
        cout<<"Select: "<<endl;
        cout<<" 1 : Accuse\n";
        cout<<" 2 : Roll dice\n";
        cin>>choice;
        if(choice==1)
            {
                win=P.U.accuse(P.murderer);
                if(win==1)
                {
                    cout<<"Well done, Sherlock! You win!!"<<endl;
                    cout<<"GAME OVER!!"<<endl;
                    exit(0);
                }
                else
                {cout<<"Oops! Wrong Guess! You lose!"<<endl;
                cout<<"GAME OVER!!"<<endl;
                exit(0);
                }
            }
        else
            {
            P.U.rollDice(rooms);
//            system("cls");
//            cout<<"Current ROom outside"<<P.U.currentRoom<<endl;
            if(P.U.currentRoom!=-1)
            {
                cout<<"Press 1 to suggest\nPress 2 to accuse\n";
                cout<<"Choice:";
                cin>>ch;
                if (ch==2)
                {
                    P.U.accuse(P.murderer);

                }
                else if(P.U.currentRoom>-1)
                    P.U.suspect(P.F,P.S,P.U.currentRoom);
            }
            }
        if(fobj==1)
        {
        sleep(2);
        cout<<"Opponent 1's turn!"<<endl;
        P.turn=1;

        fobj=fobj+P.F.checkAccuse(P.murderer);
        if(fobj==3)
            {
                cout<<"Opponent 1 wins!"<<endl;
                cout<<"GAME OVER"<<endl;
                exit(0);
            }

             P.F.rollDice(rooms);
//            system("cls");

        if(P.F.currentRoom>-1){
//                cout<<"suggest1"<<endl;
                P.F.suspect(P.turn,P.S,P.U,P.F.currentRoom,C);
        }}
        if(sobj==1)
        {
        sleep(2);
        cout<<"Opponent 2's turn!"<<endl;
        P.turn=2;
        sobj=sobj+P.S.checkAccuse(P.murderer);
        if(sobj==3)
            {
                cout<<"Opponent 2 wins!"<<endl;
                cout<<"GAME OVER"<<endl;
                exit(0);
            }
        P.S.rollDice(rooms);
//            system("cls");

//        cout<<"Noob"<<endl;
        if(P.S.currentRoom>-1)
        {
//            cout<<"suggest"<<endl;
                P.S.suspect(P.turn,P.F,P.U,P.S.currentRoom,C);
        }
        }
//        cout<<fobj<<sobj<<endl;
        if((fobj==2)&&(sobj==2))
        {
              cout<<"Your opponents failed! You win!!"<<endl;
              cout<<"GAME OVER!!"<<endl;
              exit(0);
        }
//        cout<<"Loop\n"<<endl;
//            getch();

        }
    getch();
    closegraph();

    return 0;


}




