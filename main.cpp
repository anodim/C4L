#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define NULL 0

using namespace std;

enum Etat {IDLE,SAMP,DIAG,DL,MOLECULE,GET_MOL,LAB,ASM};
enum MOL {A,B,C,D,E};

class Sample
{
public:
    int a_sampleId;
    int a_health;
    int a_cost[5];


    Sample(int id, int health,int costA,int costB,int costC,int costD,int costE)
    {
        a_sampleId = id;
        a_health = health;
        a_cost[MOL::A] = costA;
        a_cost[MOL::B] = costB;
        a_cost[MOL::C] = costC;
        a_cost[MOL::D] = costD;
        a_cost[MOL::E] = costE;

    }
};

class Robot
{
public:
    string a_target;
    int a_eta;
    int a_score;
    int a_storage[5];
    int a_expertise[5];

    Robot(string target, int eta, int score, int storageA, int storageB, int storageC, int storageD, int storageE)
    {
        a_target = target;
        a_eta = eta;
        a_score = score;
        a_storage[MOL::A] = storageA;
        a_storage[MOL::B] = storageB;
        a_storage[MOL::C] = storageC;
        a_storage[MOL::D] = storageD;
        a_storage[MOL::E] = storageE;
    }
};

int main()
{
    Etat e = IDLE;

    int projectCount;
    cin >> projectCount;
    cin.ignore();
    for (int i = 0; i < projectCount; i++)
    {
        int a,b,c,d,e;
        cin >> a >> b >> c >> d >> e;
        cin.ignore();
    }

    // game loop
    while (1)
    {
        Robot *r[2];
        Sample *sample;

        int bestGain = -1;
        int idBestGain = -1;

        string target;
        int eta, score;
        int storageA, storageB, storageC, storageD, storageE;
        int expertiseA, expertiseB, expertiseC, expertiseD, expertiseE;

        for (int i = 0; i < 2; i++)
        {
            cin >> target >> eta >> score >> storageA >> storageB >> storageC >> storageD >> storageE >> expertiseA >> expertiseB >> expertiseC >> expertiseD >> expertiseE;
            cin.ignore();

            r[i] = new Robot(target,eta,score,storageA, storageB, storageC, storageD, storageE);
        }

        int availableA, availableB, availableC, availableD, availableE;
        int sampleCount;

        cin >> availableA >> availableB >> availableC >> availableD >> availableE;
        cin.ignore();

        cin >> sampleCount;
        cin.ignore();

        for (int i = 0; i < sampleCount; i++)
        {
            int sampleId;
            int carriedBy;
            int rank;
            string expertiseGain;
            int health;
            int costA, costB, costC, costD, costE;
            cin >> sampleId >> carriedBy >> rank >> expertiseGain >> health >> costA >> costB >> costC >> costD >> costE;
            cin.ignore();
            /*
                        cerr << sampleId << endl;
                        cerr << carriedBy << endl;
                        cerr << rank << endl;
                        cerr << health << endl;
                        cerr << costA << " " << costB << " " << costC << " " << costD << " " << costE << endl;
            */
            if(carriedBy == 0)
            {
                sample = new Sample(sampleId,health,costA,costB,costC,costD,costE);
            }
        }

        switch(e)
        {
        case IDLE:
            cout << "GOTO SAMPLES" << endl;
            e = SAMP;
            break;

        case SAMP:
            if(r[0]->a_eta == 0)
            {
                cout << "CONNECT 2" << endl; //rank
                e = DIAG;
            }
            else
              cout << "->" << endl;
            break;

        case DIAG:
            cout << "GOTO DIAGNOSIS" << endl;
            e = DL;
            break;

        case DL:
            if(r[0]->a_eta == 0)
            {
                cout << "CONNECT " << sample->a_sampleId << endl;
                e = MOLECULE;
            }
            else
            {
              cout << "-> ->" << endl;
            }
            break;

        case MOLECULE:
            cout << "GOTO MOLECULES" << endl;
            e = GET_MOL;
            break;

        case GET_MOL:
            if(r[0]->a_eta == 0)
            {

                for(int i=0;i<5;i++)
                  cerr << r[0]->a_storage[i] << " ";
                cerr << endl;

                for(int i=0;i<5;i++)
                  cerr << sample->a_cost[i] << " ";
                cerr << endl;


                if(r[0]->a_storage[0] < sample->a_cost[0] && availableA>0)
                    cout << "CONNECT A" << endl;
                else if(r[0]->a_storage[1] < sample->a_cost[1] && availableB>0)
                    cout << "CONNECT B" << endl;
                else if(r[0]->a_storage[2] < sample->a_cost[2] && availableC>0)
                    cout << "CONNECT C" << endl;
                else if(r[0]->a_storage[3] < sample->a_cost[3] && availableD>0)
                    cout << "CONNECT D" << endl;
                else if(r[0]->a_storage[4] < sample->a_cost[4] && availableE>0)
                    cout << "CONNECT E" << endl;
                else
                {
                    if(r[0]->a_storage[0] >= sample->a_cost[0] &&
                       r[0]->a_storage[1] >= sample->a_cost[1] &&
                       r[0]->a_storage[2] >= sample->a_cost[2] &&
                       r[0]->a_storage[3] >= sample->a_cost[3] &&
                       r[0]->a_storage[4] >= sample->a_cost[4])
                    {
                      e = ASM;
                      cout << "GOTO LABORATORY" << endl;
                    }
                    else
                    {
                      cout << "WAIT" << endl;
                    }
                }
            }
            else
            {
              cout << "-> -> ->" << endl;
            }
            break;

        case ASM:
            if(r[0]->a_eta == 0)
            {
              cout << "CONNECT " << sample->a_sampleId <<  endl;
              e = IDLE;
            }
            else
            {
              cout << "WAIT" << endl;
            }

            break;
        }
    }
}
