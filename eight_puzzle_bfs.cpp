#include <iostream>
#include <queue>
#include <unordered_set>
#include <map>
#include <string>
using namespace std;

struct State
{
	vector< vector<int> > board;
	int blank_row, blank_column;
	string id;
	void set_id()
	{
		id = "";
		for(int i = 0; i < 3; ++i)
		{
			for(int j = 0; j < 3; ++j)
				id += to_string(board[i][j]);
		}
	}
};

struct Rule
{
	function<State(State)> apply;
	function<bool(State)> check_condition;
};

unordered_set<string> closed_set, open_set;
map<string, string> parent;
bool finished = false;

vector<Rule> problem_rules = { 
	
	{ [](State s) -> State { swap<int>(s.board[s.blank_row][s.blank_column], s.board[s.blank_row][s.blank_column - 1]); s.blank_column--; return s; }, [](State s) -> bool { return s.blank_column > 0; } },
	{ [](State s) -> State { swap<int>(s.board[s.blank_row][s.blank_column], s.board[s.blank_row][s.blank_column + 1]); s.blank_column++; return s; }, [](State s) -> bool { return s.blank_column < 2; } },
	{ [](State s) -> State { swap<int>(s.board[s.blank_row][s.blank_column], s.board[s.blank_row - 1][s.blank_column]); s.blank_row--; return s; }, [](State s) -> bool { return s.blank_row > 0; } },
	{ [](State s) -> State { swap<int>(s.board[s.blank_row][s.blank_column], s.board[s.blank_row + 1][s.blank_column]); s.blank_row++; return s; }, [](State s) -> bool { return s.blank_row < 2; } }

};

void display_board(State s)
{
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			if(s.board[i][j] == 0) cout << "_ ";
			else cout << s.board[i][j] << " ";
		}
		cout << endl;
	}
}

bool is_goal_state(State s) 
{  
	int count = 1;
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			if(s.board[i][j] != count % 9) return false;
			count++;
		}	
	}
	return true;
}

bool in_closed_set(State s) { return closed_set.find(s.id) != closed_set.end(); }
bool in_open_set(State s) { return open_set.find(s.id) != open_set.end(); }
void add_to_closed_set(State s) { closed_set.insert(s.id); }
void add_to_open_set(State s){ open_set.insert(s.id); }
void set_parent(State s, State p) { parent[s.id] = p.id; }

vector<State> generate_neighbours(State s)
{
	vector<State> neighbours;
	for(Rule rule: problem_rules)
	{
		if(rule.check_condition(s))
		{
			State new_state = rule.apply(s);
			new_state.set_id();
			if(!in_closed_set(new_state) && !in_open_set(new_state)) neighbours.push_back(new_state);
		}
	}
	return neighbours;
}

void traceback(string s)
{
	if(s == "$") return;
	else traceback(parent[s]);
	int count = 0;
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			if(s[count] == '0') cout << "_ ";
			else cout << s[count] << " ";
			count++;
		}
		cout << "\n";
	}
	cout << "\n";
}

void print_solution(State state) 
{ 
	cout << "Solution path:\n";
	traceback(state.id); 
}

void solve(State start)
{
	queue<State> open;
 	open.push(start);
 	parent[start.id] = "$";
 	while(!open.empty())
 	{
 		State current_state = open.front();
 		open.pop();
 		add_to_closed_set(current_state);
 		if(is_goal_state(current_state)) 
 		{
 			print_solution(current_state);
 			return;
 		}
 		vector<State> neighbours = generate_neighbours(current_state);
 		for(State neighbour: neighbours) 
 		{
 			set_parent(neighbour, current_state);
 			open.push(neighbour);
 			add_to_open_set(neighbour);	
 		}
 	}
}

int main()
{
 	State start = { {{0, 1, 3}, {4, 2, 5}, {7, 8, 6}}, 0, 0, "" };
 	start.set_id();
 	solve(start);
	return 0;
}