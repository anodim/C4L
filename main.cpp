#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>

#define SAMPLE_DATA_FILES_MAX 3

using namespace std;

enum Etat {TO_SAMPLE,SAMPLE,TO_DIAG,DIAG,RESET,TO_MOL,MOL,TO_LAB,LAB};
enum MOL {A,B,C,D,E};


///////////////////////////////////////////// SAMPLE ///////////////////////////////////////////////////////////////////
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
        a_cost[0] = costA;
        a_cost[1] = costB;
        a_cost[2] = costC;
        a_cost[3] = costD;
        a_cost[4] = costE;
    }

    void setSample(int id, int health,int costA,int costB,int costC,int costD,int costE)
    {
        a_sampleId = id;
        a_health = health;
        a_cost[0] = costA;
        a_cost[1] = costB;
        a_cost[2] = costC;
        a_cost[3] = costD;
        a_cost[4] = costE;
    }

    void infoDebug()
    {
        cerr << "Sample id : " <<  a_sampleId << endl;
        //cerr << "Carried By : " << a_carriedBy << endl;
        //cerr << a_rank << endl;
        cerr << "Health gain : " << a_health << endl;
        cerr << "Cost ABCDE : " << a_cost[0] << " | " << a_cost[1] << " | " << a_cost[2] << " | " << a_cost[3] << " | " << a_cost[4] << endl;
    }

};

///////////////////////////////////////////////////// ROBOT ///////////////////////////////////////////////////////////
class Robot
{
public:
    string a_target;
    string a_prevModule;
    int a_eta;
    int a_score;
    int a_storage[5];
    int a_expertise[5];
    int a_moleculesCarry;
    int a_nbSlotInUse;
    int a_nbSlotIdentify;
    vector<Sample> a_sampleCarry;

    Robot()
    {

    }

    Robot(string target, int eta, int score, int storageA, int storageB, int storageC, int storageD, int storageE
          , int expertiseA, int expertiseB, int expertiseC, int expertiseD, int expertiseE)
    {
        setRobot(target,eta,score,storageA,storageB,storageC,storageD,storageE,expertiseA, expertiseB, expertiseC, expertiseD, expertiseE);
        init();
    }

    void setRobot(string target, int eta, int score, int storageA, int storageB, int storageC, int storageD, int storageE
                  , int expertiseA, int expertiseB, int expertiseC, int expertiseD, int expertiseE)
    {
        a_target = target;
        a_eta = eta;
        a_score = score;

        a_storage[0] = storageA;
        a_storage[1] = storageB;
        a_storage[2] = storageC;
        a_storage[3] = storageD;
        a_storage[4] = storageE;

        a_expertise[0] = expertiseA;
        a_expertise[1] = expertiseB;
        a_expertise[2] = expertiseC;
        a_expertise[3] = expertiseD;
        a_expertise[4] = expertiseE;

        a_moleculesCarry = a_storage[0] + a_storage[1] + a_storage[2] + a_storage[3] + a_storage[4];
    }

    void init()
    {
        a_moleculesCarry = 0;
        a_nbSlotInUse = 0;
        a_nbSlotIdentify = 0;
        a_prevModule = "START";
    }

// -1 si pas faisable
    int check(vector<Sample> sample,int available[5])
    {
        int sampleCanBeDone = -1;
        int nbMolTotal = 0;

        if(a_moleculesCarry == 10)
        {
         for(int i = 0; i<sample.size(); i++)
        {
          nbMolTotal = 0;
          if(sample[i].a_health != -1)
          {
            int nbCheck = 0;
            for(int j=0; j<5; j++)
            {
                if(sample[i].a_cost[j]<=a_storage[j]+a_expertise[j])
                {
                    nbCheck++;
                    nbMolTotal += sample[i].a_cost[j]-a_expertise[j];
                }
            }
            if(nbCheck == 5 && nbMolTotal <= 10)
            {
                sampleCanBeDone = i;
            }
          }
        }
        }
        else
        {
        for(int i = 0; i<sample.size(); i++)
        {
          nbMolTotal = 0;
          if(sample[i].a_health != -1)
          {
            int nbCheck = 0;
            for(int j=0; j<5; j++)
            {
                if(sample[i].a_cost[j]<=available[j]+a_storage[j]+a_expertise[j])
                {
                    nbCheck++;
                    nbMolTotal += sample[i].a_cost[j]-a_expertise[j];
                }
            }
            if(nbCheck == 5 && nbMolTotal <= 10)
            {
                sampleCanBeDone = i;
            }
          }
        }
        }


        return sampleCanBeDone;
    }

};

/* /////////////////// FONCTION ///////////////////////// */


//////////////////////////////////////////////////// MAIN ///////////////////////////////////////////////////////////////
int main()
{
    Etat e = TO_SAMPLE;

    Robot me;
    Robot ai;
    Robot *bot[2];
    bot[0] = &me;
    bot[1] = &ai;

    Sample sample[SAMPLE_DATA_FILES_MAX];

    int available[5];

    int sampleCanBeDone = -1;

    /* INIT */
    int projectCount;
    cin >> projectCount;
    cin.ignore();
    for (int i = 0; i < projectCount; i++)
    {
        int a,b,c,d,e;
        cin >> a >> b >> c >> d >> e;
        cin.ignore();
    }

    me.init(); /* nbSlot & */

    ///////////////////////////////// game loop ////////////////////////////////////////
    while (1)
    {
      // long clkDep = clock();

        string target;
        int eta, score;
        int storageA, storageB, storageC, storageD, storageE;
        int expertiseA, expertiseB, expertiseC, expertiseD, expertiseE;

        for (int i = 0; i < 2; i++)
        {
            cin >> target >> eta >> score >> storageA >> storageB >> storageC >> storageD >> storageE >> expertiseA >> expertiseB >> expertiseC >> expertiseD >> expertiseE;

            if(i == 0)
              me.setRobot(target,eta,score,storageA, storageB, storageC, storageD, storageE,expertiseA, expertiseB, expertiseC, expertiseD, expertiseE);
        }

        int availableA, availableB, availableC, availableD, availableE;
        int sampleCount;

        cin >> availableA >> availableB >> availableC >> availableD >> availableE;

        available[0] = availableA;
        available[1] = availableB;
        available[2] = availableC;
        available[3] = availableD;
        available[4] = availableE;

        cin >> sampleCount;

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
                sample[mySamples].setSample(sampleId,health,costA,costB,costC,costD,costE);
                mySamples++;
            }
        }

        ///////////////////////////// MACHINE ETAT /////////////////////////////////////
        int delay;
        switch(e)
        {
        case TO_SAMPLE:
            cout << "GOTO SAMPLES" << endl;
            if(me.a_eta == 1)
            {
                e = SAMPLE;
                me.a_prevModule = "SAMPLES";
            }
            break;

        case RESET: // pas check
            cout << "CONNECT " << me.a_sampleCarry[0].a_sampleId << endl;
            me.a_sampleCarry.erase(me.a_sampleCarry.begin());

            me.a_nbSlotIdentify--;
            me.a_nbSlotInUse--;
            if(me.a_sampleCarry.size() == 0)
            {
                e = TO_SAMPLE;
            }
            break;

        case SAMPLE:

            if(me.a_nbSlotInUse == 0) // faire la selection pour avoir un de chaque
                cout << "CONNECT 2" << endl;
            else if(me.a_nbSlotInUse == 1)
                cout << "CONNECT 2" << endl;
            else if(me.a_nbSlotInUse == 2)
                cout << "CONNECT 3" << endl;

            me.a_nbSlotInUse++;
            if(me.a_nbSlotInUse == 3)
            {
                e = TO_DIAG;
            }
            break;

        case TO_DIAG:
            cout << "GOTO DIAGNOSIS" << endl;
            if(me.a_eta == 1)
            {
                e = DIAG;
                me.a_prevModule = "DIAGNOSIS";
                delay = 1;
            }
            break;

        case DIAG:
            bool cmdUse2;
            cmdUse2 = false;
            if(me.a_nbSlotIdentify <3)
            {
              cout << "CONNECT " << sample[me.a_nbSlotIdentify].a_sampleId << endl;
              cmdUse2 = true;
              me.a_nbSlotIdentify++;
            }

            if(me.a_nbSlotIdentify == 3)
            {

                me.a_sampleCarry.clear();
                for(int i=0;i<3;i++)
                  me.a_sampleCarry.push_back(sample[i]);

                sampleCanBeDone = me.check(me.a_sampleCarry,available);
                if(sampleCanBeDone != -1)
                {
                    e = TO_MOL;
                }
                else
                {
                    e = RESET;
                }


                if(cmdUse2 == false)
                {
                  cout << "WAIT chg d etat" << endl;
                }
            }
            break;

        case TO_MOL:
            cout << "GOTO MOLECULES" << endl;
            if(me.a_eta == 1)
            {
                e = MOL;
                me.a_prevModule = "MOLECULES";
            }
            break;

        case MOL:
            bool cmdUse;
            cmdUse = false;

            for(int i=0; i<5; i++) /* Recuperation des molecules nessesaire pour le sample */
            {
              if(cmdUse == false)
                {
                    if(me.a_storage[i] + me.a_expertise[i] < me.a_sampleCarry[sampleCanBeDone].a_cost[i])
                    {
                        if(available[i]>0 && me.a_moleculesCarry < 10)
                        {
                            cout << "CONNECT " << (char)('A'+i) << endl;
                            me.a_storage[i]++;
                            me.a_moleculesCarry++;
                            cmdUse = true;
                        }

                    }
                }
            }

            /* Test de faisabiliter des echantillons*/
              if(me.a_storage[0] + me.a_expertise[0] >= me.a_sampleCarry[sampleCanBeDone].a_cost[0] &&
                      me.a_storage[1] + me.a_expertise[1]>= me.a_sampleCarry[sampleCanBeDone].a_cost[1] &&
                      me.a_storage[2] + me.a_expertise[2]>= me.a_sampleCarry[sampleCanBeDone].a_cost[2] &&
                      me.a_storage[3] + me.a_expertise[3]>= me.a_sampleCarry[sampleCanBeDone].a_cost[3] &&
                      me.a_storage[4] + me.a_expertise[4]>= me.a_sampleCarry[sampleCanBeDone].a_cost[4])
              {
                  e = TO_LAB;
              }

            if(cmdUse == false) /* cas 1 plus de molecule dispo cas 2: ...*/
            {
                sampleCanBeDone = me.check(me.a_sampleCarry,available);
                if(sampleCanBeDone == -1)
                {
                    e = TO_DIAG;
                }

                if(me.a_moleculesCarry == 10)
                {
                  cout << "WAIT chg de sample ?" << endl;
                }

            //if(e != TO_DIAG){
                cerr << sampleCanBeDone  <<" " << me.a_moleculesCarry << endl << endl;
                for(int i=0;i<5;i++)
                  cerr << me.a_sampleCarry[sampleCanBeDone].a_cost[i] << endl;
                cout << "WAIT Samples </3" << endl;
              //  }

            }

            break;

        case TO_LAB:
            cout << "GOTO LABORATORY" << endl;
            if(me.a_eta == 1)
            {
                e = LAB;
                me.a_prevModule = "LABORATORY";
            }
            break;

        case LAB:
            cout << "CONNECT " << me.a_sampleCarry[sampleCanBeDone].a_sampleId <<  endl; /* Validation du sample */

            for(int i=0; i<5; i++) /* Mise a jour du nombre de molecule */
            {
                cerr << me.a_sampleCarry[sampleCanBeDone].a_cost[i] << endl;
                me.a_moleculesCarry -= me.a_sampleCarry[sampleCanBeDone].a_cost[i] - me.a_expertise[i];// il faut prendre en compte l'xp
            }


            if(me.a_moleculesCarry <0)
            {
                me.a_moleculesCarry = 0;
                cerr << "La maj du nb de molecule a echouer" << endl;
            }

            me.a_nbSlotIdentify--;
            me.a_nbSlotInUse--;
            me.a_sampleCarry.erase(me.a_sampleCarry.begin()+sampleCanBeDone);

            sampleCanBeDone = me.check(me.a_sampleCarry,available);
            if(sampleCanBeDone != -1)
                e = TO_MOL;
            else
                e = TO_SAMPLE;
            break;
        }
       // cerr << ((float)(clock()-clkDep)/CLOCKS_PER_SEC)*1000 << endl;
    }
}
