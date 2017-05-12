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
  int a_score;
  int a_storage[5];


  Robot(string target,int score,int storageA, int storageB, int storageC, int storageD, int storageE)
  {
    a_target = target;
    a_score = score;
    a_storage[MOL::A] = storageA;
    a_storage[MOL::B] = storageB;
    a_storage[MOL::C] = storageC;
    a_storage[MOL::D] = storageD;
    a_storage[MOL::E] = storageE;
  }


  /** \brief Fonction donnant la fesabiliter de la molecule
   *
   * \param costA int cout de la molecule de type A
   * \param costB int cout de la molecule de type B
   * \param costC int cout de la molecule de type C
   * \param costD int cout de la molecule de type D
   * \param costE int cout de la molecule de type E
   * \return bool true si il est possible de faire l'aaemblage de la molecule, false sinon
   *
   */
  bool cmpStorageSample(int costA, int costB, int costC, int costD, int costE)
  {
    if(a_storage[MOL::A] >= costA &&
       a_storage[MOL::B] >= costB &&
       a_storage[MOL::C] >= costC &&
       a_storage[MOL::D] >= costD &&
       a_storage[MOL::E] >= costE)
    {
      return true;
    }
    else
      return false;
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

            r[i] = new Robot(target,score,storageA, storageB, storageC, storageD, storageE);
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
            cout << "CONNECT 2" << endl; //rank
            e = DIAG;
          break;

          case DIAG:
            cout << "GOTO DIAGNOSIS" << endl;
            e = DL;
          break;

          case DL:
            cout << "CONNECT " << sample->a_sampleId << endl;
            e = MOLECULE;
          break;

          case MOLECULE:
            cout << "GOTO MOLECULES" << endl;
            e = GET_MOL;
          break;

          case GET_MOL:
            for(int i=0;i<5;i++)
              cerr << r[0]->a_storage[i] << " ";
            cerr << endl;

            for(int i=0;i<5;i++)
              cerr << sample->a_cost[i] << " ";
            cerr << endl;

            if(r[0]->a_storage[0] < sample->a_cost[0])
              cout << "CONNECT A" << endl;
            else if(r[0]->a_storage[1] < sample->a_cost[1])
              cout << "CONNECT B" << endl;
            else if(r[0]->a_storage[2] < sample->a_cost[2])
              cout << "CONNECT C" << endl;
            else if(r[0]->a_storage[3] < sample->a_cost[3])
              cout << "CONNECT D" << endl;
            else if(r[0]->a_storage[4] < sample->a_cost[4])
              cout << "CONNECT E" << endl;
            else
            {
              e = ASM;
              cout << "GOTO LABORATORY" << endl;
            }
          break;

          case ASM:
            cout << "CONNECT " << sample->a_sampleId <<  endl;
            e = IDLE;
          break;
        }
    }
}
