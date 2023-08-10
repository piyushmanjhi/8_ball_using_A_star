/* Piyush Manjhi
   Problem2: 8 puzzle using a star search algorithm. The cost of a move in the 8-puzzle is equal to the number of the moved tile. That is, the cost of moving tile 1 is $1, the cost of moving tile 2 is $2, and so on.
*/

#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
using namespace std;

// declaring variable globally
#define n (int)3

using i64 = long long int;

// defining board, moves=cost, heuristic_function=h and mid state=prev_state
class state
{
public:
    int matrix[n][n], cost, h;
    state *prev_state;
    state()
    {
        cost = 0;
        h = 0;
        prev_state = NULL;
    }

    // this method is analyzing the curr state and matching it with goal,
    static int heuristic_function(state curr, state next_state)
    {
        int counter = 0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (curr.matrix[i][j] != next_state.matrix[i][j])
                    counter++;
        return counter;
    }

    // this function is analyzing the matrix and deciding if the curr state is
    // the goal state in that case process will end
    bool operator==(state a)
    {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (this->matrix[i][j] != a.matrix[i][j])
                    return false;
        return true;
    }

    // this method will display the each state
    void display()
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                cout << matrix[i][j] << " ";
            cout << endl;
        }
        cout << "Cost = " << cost << endl;
        cout << endl;
    }
};

// declaring global
vector<state> buffer_outpt; // to display output
bool lowerF(state a, state b) { return a.h < b.h; }

bool isinset(state a, vector<state> b)
{
    for (int i = 0; i < b.size(); i++)
        if (a == b[i])
            return true;
    return false;
}

//
void add_adjoin(state curr, state goal, int nxt_blk_i, int nxt_blk_j,
                int blank_i, int blank_j, vector<state> &set_open,
                vector<state> set_close)
{
    state new_curr_state = curr;
    swap(new_curr_state.matrix[nxt_blk_i][nxt_blk_j],
         new_curr_state.matrix[blank_i][blank_j]);
    if (!isinset(new_curr_state, set_close) &&
        !isinset(new_curr_state, set_open))
    {
        new_curr_state.cost =
            curr.cost +
            new_curr_state.matrix[blank_i][blank_j]; // cost cummulative
        new_curr_state.h = new_curr_state.cost +
                           state ::heuristic_function(new_curr_state, goal); // h
        state *temp = new state();
        *temp = curr;
        new_curr_state.prev_state = temp;
        set_open.push_back(new_curr_state);
    }
}

//
void adjoin(state curr, state goal, vector<state> &set_open,
            vector<state> &set_close)
{
    int i, j, blank_i, blank_j;

    // finding empty space
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (curr.matrix[i][j] == 0)
            {
                blank_i = i;
                blank_j = j;
                break;
            }

    i = blank_i, j = blank_j;
    if (i - 1 >= 0) // left
        add_adjoin(curr, goal, i - 1, j, blank_i, blank_j, set_open, set_close);
    if (i + 1 < n) // right
        add_adjoin(curr, goal, i + 1, j, blank_i, blank_j, set_open, set_close);
    if (j + 1 < n) // up
        add_adjoin(curr, goal, i, j + 1, blank_i, blank_j, set_open, set_close);
    if (j - 1 >= 0) // down
        add_adjoin(curr, goal, i, j - 1, blank_i, blank_j, set_open, set_close);
}

// transitioning curr next_state prev_state state; making new curr and
// forming prev_state
bool transition_state(state curr, vector<state> &prev_state)
{
    state *temp = &curr;
    while (temp != NULL)
    {
        prev_state.push_back(*temp);
        temp = temp->prev_state;
    }
    return true;
}

// main function, uses
bool astar(state start, state goal)
{
    vector<state> set_open;
    vector<state> set_close;

    state curr;
    start.cost = 0;

    // using formula f(n) = g(n) + h(n)
    start.h = start.cost + state ::heuristic_function(start, goal);
    set_open.push_back(start);

    // this loops runs until the set is empty. It looks for positions left, right,
    // top and down
    while (!set_open.empty())
    {
        sort(set_open.begin(), set_open.end(), lowerF);
        curr = set_open[0];
        if (curr == goal)
            return transition_state(curr, buffer_outpt);
        set_open.erase(set_open.begin());
        set_close.push_back(curr);
        adjoin(curr, goal, set_open, set_close);
    }
    return false;
}

//main function takes input for start state and goal state then calls astar function. lastly displays results
int main()
{
    state start, goal;

    // getting input
    cout << "Getting input from file" << endl;

    fstream myfile;

    // getting start state
    myfile.open("input.txt", ios::in);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            myfile >> start.matrix[i][j];
        }
    }

    cout << "Displaying start state: " << endl;
    // display input
    for (int i = 0; i < n; i++)
    {

        for (int j = 0; j < n; j++)
        {
            cout << " " << start.matrix[i][j];
        }
        cout << endl;
    }

    cout << endl;

    // reading goal state
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            myfile >> goal.matrix[i][j];

    // displaying goal state
    cout << "Displaying goal state: " << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << " " << goal.matrix[i][j];
        }
        cout << endl;
    }
    cout << endl;

    myfile.close();

    // calling astar method
    if (astar(start, goal) == 1)
    {
        for (int i = buffer_outpt.size() - 1; i >= 0; i--)
            buffer_outpt[i].display();
        cout << "We have reached the goal state." << endl;
    }
    else
        cout << "Failure: could not read goal state" << endl;
    return 0;
}