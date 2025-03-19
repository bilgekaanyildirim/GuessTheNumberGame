#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <ctime>
#include <mutex>
#include <vector>
#include <random>

using namespace std;

int targetNum;
vector<int> scores;
mutex myMutex;
int winner;
int roundNum;
int ranUp, ranDown;
int playerNum;
bool isFirst = true;

/* Begin: code taken from threads16.cpp */
int random_range(const int & min, const int & max)
{
    static mt19937 generator(time(0));
    uniform_int_distribution<int> distribution(min,max);
    return distribution(generator);
}
/* End: code taken from threads16.cpp */

// Player thread function
void PlayerThread(int id, tm * ptm)
{
    // In order to start at the same time, waits 3 seconds after game started
    this_thread::sleep_until(chrono::system_clock::from_time_t(mktime(ptm)));

    int num;

    // Works until a player wins the round
    while (winner == -1)
    {
        myMutex.lock();

        num = random_range(ranDown, ranUp); // guessing number

        // checking the guess is the target number
        if (num != targetNum && winner == -1)
        {
            cout << "Player with id " <<  id << " guessed "  << num << " incorrectly at: " << put_time(ptm, "%X") << endl;
        }
        else if (winner == -1)
        {
            cout << "Player with id " <<  id << " guessed "  << num << " correctly at: " << put_time(ptm, "%X") << endl;
            winner = id;
        }
        myMutex.unlock();

        // Sleep 1 sec for other guess
        this_thread::sleep_for(chrono::seconds(1));

        time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now());

        ptm = localtime(&tt);
    }
}

// Host thread function
void HostThread(struct tm * ptm)
{
    for (int j = 0; j < roundNum; ++j)
    {
        myMutex.lock();

        winner = -1;

        if (!isFirst)
        {
            cout << endl << "---------------------------------------------------" << endl;
            cout << "Round " << j + 1 << " started at: " << put_time(ptm, "%X") << endl;
        }

        // Determining target number
        targetNum = random_range(ranDown, ranUp);
        cout << "Target is " << targetNum << endl << endl;
        myMutex.unlock();

        isFirst = false;

        // wait until a player win the round and then increase the score of the player
        while (winner == -1)
        {}
        scores[winner]++;
    }
}

int main()
{
    cout << "Please enter number of players" << endl;
    cin >> playerNum;

    while (playerNum < 1)
    {
        cout << "Number of players cannot be lower than 1!" << endl;
        cout << "Please enter number of players" << endl;
        cin >> playerNum;
    }

    cout << "Please enter number of rounds" << endl;
    cin >> roundNum;

    while (roundNum < 1)
    {
        cout << "Number of rounds cannot be lower than 1!" << endl;
        cout << "Please enter number of rounds" << endl;
        cin >> roundNum;
    }

    cout << "Please enter the randomization range" << endl;
    cin >> ranDown >> ranUp;

    while (ranUp < ranDown)
    {
        cout << "Lower bound has to be smaller than or equal to higher bound" << endl;
        cout << "Please enter the randomization range" << endl;
        cin >> ranDown >> ranUp;
    }

    scores.resize(playerNum, 0);

    time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now());

    tm *ptm = localtime(&tt);

    cout << endl << "---------------------------------------------------" << endl;
    cout << "Game started at: " << put_time(ptm, "%X") << endl;
    cout << "Round 1 will start 3 seconds later" << endl << endl;

    ptm->tm_sec += 3;

    thread hostT (HostThread, ptm);

    vector<thread> threads(playerNum);

    for (int i = 0; i < playerNum; i++)
    {
        threads[i] = thread(PlayerThread, i, ptm);
    }

    hostT.join();

    for (int i = 0; i < playerNum; i++)
    {
        threads[i].join();
    }

    cout << endl << "Game is over!" << endl;
    cout << "Leaderboard:" << endl;

    for (int i = 0; i < playerNum; ++i)
    {
        cout << "Player " << i << " has won " << scores[i] << " times." << endl;
    }
    delete ptm;
}

