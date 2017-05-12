#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define NULL 0

#define SAMPLE_DATA_FILES_MAX 3

using namespace std;

enum Etat {IDLE,SAMP,DIAG,DL,MOLECULE,PRE_MOL,GET_MOL,LAB,ASM,RESET};
enum MOL {A,B,C,D,E};

class Sample
{
public:
    int a_sampleId;
    int a_health;
    int a_cost[5];

    Sample()
    {

    }

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

    void init(int id, int health,int costA,int costB,int costC,int costD,int costE)
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
    int a_moleculesCarry;

    Robot()
    {

    }

    Robot(string target, int eta, int score, int storageA, int storageB, int storageC, int storageD, int storageE)
    {
        init(target,eta,score,storageA,storageB,storageC,storageD,storageE);
        a_moleculesCarry = 0;
    }

    void init(string target, int eta, int score, int storageA, int storageB, int storageC, int storageD, int storageE)
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

    Robot me;
    Robot ai;

    Sample sample[SAMPLE_DATA_FILES_MAX];

    int available[5];

    int nbSampleCarryUp = 0;
    int nbIdentify = 0;
    int sampleFaisable = -1;

    int projectCount;
    cin >> projectCount;
    cin.ignore();
    for (int i = 0; i < projectCount; i++)
    {
        int a,b,c,d,e;
        cin >> a >> b >> c >> d >> e;
        cin.ignore();
    }

    me.a_moleculesCarry = 0;
    // game loop
    while (1)
    {
        string target;
        int eta, score;
        int storageA, storageB, storageC, storageD, storageE;
        int expertiseA, expertiseB, expertiseC, expertiseD, expertiseE;

        for (int i = 0; i < 2; i++)
        {
            cin >> target >> eta >> score >> storageA >> storageB >> storageC >> storageD >> storageE >> expertiseA >> expertiseB >> expertiseC >> expertiseD >> expertiseE;
            cin.ignore();

            if(i==0)
            {
                me.init(target,eta,score,storageA, storageB, storageC, storageD, storageE);
            }
            else
            {
                ai.init(target,eta,score,storageA, storageB, storageC, storageD, storageE);
            }


        }

        int availableA, availableB, availableC, availableD, availableE;
        int sampleCount;

        cin >> availableA >> availableB >> availableC >> availableD >> availableE;
        cin.ignore();

        available[0] = availableA;
        available[1] = availableB;
        available[2] = availableC;
        available[3] = availableD;
        available[4] = availableE;

        cin >> sampleCount;
        cin.ignore();

        int mySamples = 0;
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

            if(carriedBy == 0)
            {

                /*
                cerr << sampleId << endl;
                cerr << carriedBy << endl;
                cerr << rank << endl;
                cerr << health << endl;
                cerr << costA << " " << costB << " " << costC << " " << costD << " " << costE << endl;
                /**/
                sample[mySamples].init(sampleId,health,costA,costB,costC,costD,costE);
                mySamples++;
            }
        }

        int sampleCanBeDone;
        bool sampleComplete;
        bool outDone;

        switch(e)
        {
        case IDLE:
            cout << "GOTO SAMPLES" << endl;
            e = SAMP;
            break;

        case RESET:
            cout << "CONNECT " << sample[0].a_sampleId << endl;
            nbIdentify--;
            nbSampleCarryUp--;
            if(nbIdentify == 0)
                e = IDLE;
            break;

        case SAMP:
            if(me.a_eta == 0)
            {
                if(nbSampleCarryUp == 0)
                    cout << "CONNECT 2" << endl; //rank
                else if(nbSampleCarryUp == 1)
                    cout << "CONNECT 1" << endl; //rank
                else
                    cout << "CONNECT 3" << endl;

                nbSampleCarryUp++;
                if(nbSampleCarryUp == 3)
                {
                    e = DIAG;
                }
            }
            else
                cout << "WAIT ->" << endl;
            break;

        case DIAG:
            cout << "GOTO DIAGNOSIS" << endl;
            e = DL;
            break;

        case DL:
            if(me.a_eta == 0)
            {
                cout << "CONNECT " << sample[nbIdentify].a_sampleId << endl;
                nbIdentify++;
                if(nbIdentify == 3)
                    e = MOLECULE;
            }
            else
            {
                cout << "WAIT -> ->" << endl;
            }
            break;

        case MOLECULE:
            sampleCanBeDone = -1;

            for(int i = 0; i<SAMPLE_DATA_FILES_MAX; i++)
            {
                int nbCheck = 0;
                for(int j=0; j<5; j++)
                {
                    if(sample[i].a_cost[j]<=available[j]+me.a_storage[j])
                    {
                        nbCheck++;
                    }
                }
                if(nbCheck == 5)
                {
                    sampleCanBeDone = i;
                }
            }

            if(sampleCanBeDone == -1)
            {
                //cout << "WAIT cant be done "<< endl;
                cout << "GOTO DIAGNOSIS" << endl;
                e = RESET;
            }
            else
            {
                cout << "GOTO MOLECULES" << endl;
                e = GET_MOL;
            }
            break;

        case PRE_MOL:
            if(me.a_eta == 0)
            {
                e = GET_MOL;
            }
            else
            {
                cout << "WAIT ->MOLECULE(" << me.a_eta << ")" <<  endl;
            }
            break;

        case GET_MOL:

            /*
                            for(int i=0;i<5;i++)
                              cerr << me.a_storage[i] << " ";
                            cerr << endl;

                            for(int i=0;i<5;i++)
                              cerr << sample[nbSampleCarryUp].a_cost[i] << " ";
                            cerr << endl;
            /**/

            if(me.a_eta == 0)
            {
                outDone = false;

                for(int i=0; i<5; i++)
                {
                    if(me.a_storage[i] < sample[sampleCanBeDone].a_cost[i] && available[i]>0 && me.a_moleculesCarry < 10 && outDone == false)
                    {
                        cout << "CONNECT " << (char)('A'+i) << endl;
                        me.a_moleculesCarry++;
                        outDone = true;
                    }
                }

                sampleComplete = false;

                for(int i=0; i<SAMPLE_DATA_FILES_MAX; i++)
                {
                    if(sampleComplete == false &&
                            me.a_storage[0] >= sample[i].a_cost[0] &&
                            me.a_storage[1] >= sample[i].a_cost[1] &&
                            me.a_storage[2] >= sample[i].a_cost[2] &&
                            me.a_storage[3] >= sample[i].a_cost[3] &&
                            me.a_storage[4] >= sample[i].a_cost[4])
                    {
                        e = ASM;
                        sampleComplete = true;
                        sampleFaisable = i;
                        cout << "GOTO LABORATORY" << endl;
                    }
                }

                if(sampleComplete == false && outDone == false)
                {
                    cout << "WAIT pas de recup ni de sample completer" << endl; // faire quelque chose de plus inteligent dans le future
                }
            }
            else
            {
                cout << "WAIT ->MOLECULE(" << me.a_eta << ")" <<  endl;
            }
            break;

        case ASM:
            if(me.a_eta == 0)
            {
                cout << "CONNECT " << sample[sampleFaisable].a_sampleId <<  endl;
                for(int i=0; i<5; i++)
                    me.a_moleculesCarry -= sample[sampleFaisable].a_cost[i];

                nbIdentify--;
                nbSampleCarryUp--;
                e = IDLE;
            }
            else
            {
                cout << "WAIT eta: " << me.a_eta << endl;
            }

            break;
        }
    }
}
