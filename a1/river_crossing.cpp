#include <iostream>
#include <vector>
#include <deque>
#include <unordered_set>
#include <fstream>
#include <string.h>
#include <set>
#include <stdlib.h>
#include <algorithm>
#include "TextTable.h"

struct State{
  State() : cost(0), parent(NULL), c(2), w(2), b(2), strs(2) {}
  int cost;
  struct State* parent;
  std::vector<int> c;
  std::vector<int> w;
  std::vector<bool> b;
  std::vector<std::string> strs;


  bool operator==(const State &s) const{
      return (!(c[0] != s.c[0] || c[1] != s.c[1] || w[0] != s.w[0] || w[1] != s.w[1] || b[0] != s.b[0]));
  }

  bool operator<(const State &s) const{
      return (s.cost > cost);
  }
  //return (m == anotherLine.m && c == anotherLine.c);


};

namespace std{
  template <>
  struct hash<State>
  {
    typedef State     argument_type;
    typedef std::size_t  result_type;

  //  result_type operator()(const MyClass & t) const
    result_type operator()(const State& s) const
    {
        return (hash<string>()(s.strs[0])) ^ (hash<string>()(s.strs[1]));
    }
  };
}

using namespace std;

void print_State(struct State s){

  TextTable t( '-', '|', '+' );

  t.add( to_string(s.cost) );
  t.add( "Left" );
  t.add( "Right" );
  t.endOfRow();

  t.add( "Chickens" );
  t.add( to_string(s.c[0]) );
  t.add( to_string(s.c[1]) );
  t.endOfRow();

  t.add( "Wolves" );
  t.add( to_string(s.w[0]) );
  t.add( to_string(s.w[1]) );
  t.endOfRow();

  t.add( "Boats" );
  if(s.b[0]){
    t.add("1");
    t.add("0");

  }else{
    t.add("0");
    t.add("1");

  }
  t.endOfRow();

  t.setAlignment( 2, TextTable::Alignment::RIGHT );
  cout << t <<endl;
}

void write_State(struct State s, string str){

  ofstream o;
//  std::ios::app
  o.open(str, ios::app);

  TextTable t( '-', '|', '+' );

  t.add( to_string(s.cost) );
  t.add( "Left" );
  t.add( "Right" );
  t.endOfRow();

  t.add( "Chickens" );
  t.add( to_string(s.c[0]) );
  t.add( to_string(s.c[1]) );
  t.endOfRow();

  t.add( "Wolves" );
  t.add( to_string(s.w[0]) );
  t.add( to_string(s.w[1]) );
  t.endOfRow();

  t.add( "Boats" );
  if(s.b[0]){
    t.add("1");
    t.add("0");

  }else{
    t.add("0");
    t.add("1");

  }
  t.endOfRow();

  t.setAlignment( 2, TextTable::Alignment::RIGHT );
  o << t <<endl;
}

void print(struct State s, string str){
  State st = s;
  deque<State> q;
  while (st.parent) {
    q.push_front(st);
    st = *st.parent;
  }
  q.push_front(st);
  for(auto i: q){
    write_State(i, str);
    print_State(i);
  }
}

vector<string> split(string str,string sep){
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    vector<string> arr;
    current=strtok(cstr,sep.c_str());
    while(current!=NULL){
        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }
    return arr;
}

struct State get_data(ifstream& f){

  State *s = new State;
  vector<string> v;
  string line;
  if (f.is_open()) {
    while(getline(f, line,'\n')){
      v.push_back(line);
    }
    f.close();
  }else{
    cerr << "Failure to open file" << '\n';
    exit(0);
  }
  vector<string> l = split(v[0], ",");
  vector<string> r = split(v[1], ",");
  s->strs[0] = v[0];
  s->strs[1] = v[1];
  s->c[0] = stoi(l[0]);
  s->c[1] = stoi(r[0]);
  s->w[0] = stoi(l[1]);
  s->w[1] = stoi(r[1]);
  s->b[0] = stoi(l[2]);
  s->b[1] = stoi(r[2]);
  s->parent = NULL;
  s->cost = 0;

  return *s;
}

//Checks whether or not a State is valid
bool check(struct State s){
  if( (s.w[0] > s.c[0] && s.c[0] > 0) || (s.w[1] > s.c[1] && s.c[1] > 0)|| s.c[0] < 0 || s.c[1] < 0 || s.w[0] < 0 || s.w[1] < 0 )
    return false;
  return true;
}

//Checks if two States are equivalent
bool match(struct State s1, struct State s2){
  if(s1.c[0] != s2.c[0] || s1.c[1] != s2.c[1] || s1.w[0] != s2.w[0] || s1.w[1] != s2.w[1] || s1.b[0] != s2.b[0])
    return false;
  return true;
}

//Generates specified expansion
struct State step(struct State& init, int c, int w){

  State* s = new State();
  int l;
  int r;
  if(init.b[0]){
    l = 0;
    r = 1;
  }else{
    l = 1;
    r = 0;
  }

  s->c[l] = init.c[l] - c;
  s->w[l] = init.w[l] - w;
  s->c[r] = init.c[r] + c;
  s->w[r] = init.w[r] + w;


  s->b[l] = false;
  s->b[r] = true;
  s->cost = init.cost + 1;
  s->parent = &init;

  return *s;
}

//Generates all valid successor States of a given State
vector<State> expand(struct State& init){
  vector<State>* States= new vector<State>;
  int c = 2;
  int w = 0;
  for(int i=0; i<5; i++){
    struct State* s = new State();
    *s = step(init, c, w);
    if(check(*s)){
      States->push_back(*s);
    }
    if(i%2 == 0){
      c--;
    }else{
      w++;
    }
  }
  return *States;
}

State bfs(struct State& init, struct State& goal){

  //add init to frontier
  deque<State> frontier;
  vector<State>* children = new vector<State>;
  frontier.push_back(init);

  //make empty explored list
  unordered_set<State> exp;
  unordered_set<State> frnt;
  frnt.insert(init);

  int c = 0;
  int nodes = 1;

  while(1){
    if(frontier.empty()){
      cout<<"No solution"<<endl;
      cout<<"Nodes created: "<<nodes<<endl;
      cout<<"Nodes visited: "<<c<<endl;
      return init;
    }

    //pop frontier
    State *s = new State();//now pointer is different each time
    *s = frontier.front();
    frontier.pop_front();

    //check if goal
    if(match(*s, goal)){
      cout<<"BFS done."<<endl;
      cout<<"Nodes created: "<<nodes<<endl;
      cout<<"Nodes visited: "<<c<<endl;

      return *s;
    }

    //add to explored
    exp.insert(*s);

    //expand
    *children = expand(*s);

    //add children only if not explored or in frontier
    for(auto st: *children){
      if(exp.find(st) == exp.end() && frnt.find(st) == frnt.end() ){
        frontier.push_back(st);
        frnt.insert(st);
        nodes++;
        if(nodes%10000 == 0){
          cout<<nodes<<endl;
        }
      }else if(!(exp.find(st) == exp.end())){
      //  cout<<"skipped"<<endl;
        if(exp.find(st)->cost > st.cost){
          cout<<"update"<<endl;
          exp.erase(exp.find(st));
          exp.insert(st);
        }
      }
    }
    // for(auto st: *children){
    //   if(exp.find(st) == exp.end() && frnt.find(st) == frnt.end() ){
    //     frontier.push_back(st);
    //     frnt.insert(init);
    //     nodes++;
    //     if(nodes%10000 == 0){
    //       cout<<nodes<<endl;
    //     }
    //   }else{
    //   //  cout<<"skipped"<<endl;
    //     if(exp.find(st)->cost > st.cost){
    //       cout<<"update"<<endl;
    //       exp.erase(exp.find(st));
    //       exp.insert(st);
    //     }
    //     continue;
    //   }
    // }


    c++;
  }
  return init;
}

State dfs(struct State& init, struct State& goal){

  //add init to frontier
  deque<State> frontier;
  vector<State>* children = new vector<State>;
  frontier.push_back(init);

  //make empty explored list
  unordered_set<State> exp;
  unordered_set<State> frnt;
  frnt.insert(init);

  int c = 0;
  int nodes = 1;

  while(1){
    if(frontier.empty()){
      return init;
    }

    //pop frontier
    State *s = new State();//now pointer is different each time
    *s = frontier.front();
    frontier.pop_front();

    //check if goal
    if(match(*s, goal)){
      cout<<"DFS done."<<endl;
      cout<<"Nodes created: "<<nodes<<endl;
      cout<<"Nodes visited: "<<c<<endl;
      return *s;
    }

    //add to explored
    exp.insert(*s);

    //expand
    *children = expand(*s);

    //add children only if not explored or in frontier
    for(auto st: *children){
      if(exp.find(st) == exp.end() && frnt.find(st) == frnt.end() ){
        frontier.push_front(st);
        frnt.insert(st);
        nodes++;
      }else if(!(exp.find(st) == exp.end())){
        if(exp.find(st)->cost > st.cost){
          cout<<"update"<<endl;
          exp.erase(exp.find(st));
          exp.insert(st);
        }
      }
    }
    c++;
  }
  return init;
}

State ids(struct State& init, struct State& goal){
  int n = 2099999999;
  // int n = 13;
  int c = 0;
  int nodes = 1;
  int cap = 1;
  while(cap<n){
    cap++;
    //add init to frontier
    deque<State> frontier;
    vector<State>* children = new vector<State>;
    frontier.push_back(init);

    //make empty explored list
    unordered_set<State> exp;
    unordered_set<State> frnt;
    frnt.insert(init);


    while(1){
      if(frontier.empty() ){
        return init;
      }

      //pop frontier
      State *s = new State();//now pointer is different each time
      *s = frontier.front();
      frontier.pop_front();

      //check if goal
      if(match(*s, goal)){
        cout<<"IDS done."<<endl;
        cout<<"Nodes created: "<<nodes<<endl;
        cout<<"Nodes visited: "<<c<<endl;

        return *s;
      }

      //add to explored
      exp.insert(*s);

      //expand
      *children = expand(*s);

      //add children only if not explored or in frontier
      for(auto st: *children){
        if(exp.find(st) == exp.end() && frnt.find(st) == frnt.end() ){
          frontier.push_front(st);
          frnt.insert(st);
          nodes++;
        }else if(!(exp.find(st) == exp.end())){
          if(exp.find(st)->cost > st.cost){
            cout<<"update"<<endl;
            exp.erase(exp.find(st));
            exp.insert(st);
          }
        }
      }
      c++;
    }
  }
  return init;

}
//
// State astar(struct State& init, struct State& goal){
//
// }


int bfs_eval(struct State& init, struct State& goal){

  //add init to frontier
  deque<State> frontier;
  vector<State>* children = new vector<State>;
  frontier.push_back(init);

  //make empty explored list
  unordered_set<State> exp;
  unordered_set<State> frnt;
  frnt.insert(init);


  int c = 0;
  int nodes = 1;


  while(1){
    if(frontier.empty()){
      return -1;
    }

    //pop frontier
    State *s = new State();//now pointer is different each time
    *s = frontier.front();
    frontier.pop_front();

    //check if goal
    if(match(*s, goal)){
      // cout<<"BFS done."<<endl;
      // cout<<"Nodes created: "<<nodes<<endl;
      // cout<<"Nodes visited: "<<c<<endl;

      return c;
    }

    //add to explored
    exp.insert(*s);

    //expand
    *children = expand(*s);

    //add children only if not explored or in frontier
    for(auto st: *children){
      if(exp.find(st) == exp.end() && frnt.find(st) == frnt.end() ){
        frontier.push_back(st);
        frnt.insert(init);
        nodes++;
      }else if(!(exp.find(st) == exp.end())){
        if(exp.find(st)->cost > st.cost){
          cout<<"update"<<endl;
          exp.erase(exp.find(st));
          exp.insert(st);
        }
      }
    }
    c++;
  }
  return -1;
}

int get_mode(string str){
  if(str == "bfs"){
    return 1;
  }else if(str == "dfs"){
    return 2;
  }else if(str == "iddfs"){
    return 3;
  }else{
    return 0;
  }
}

int eval(struct State& curr, struct State& goal){
  int lc = abs(curr.c[0] - goal.c[0]);
  int lw = abs(curr.w[0] - goal.w[0]);
  int lb = abs(curr.b[0] - goal.b[0]);
  int rc = abs(curr.c[1] - goal.c[1]);
  int rw = abs(curr.w[1] - goal.w[1]);
  int rb = abs(curr.b[1] - goal.b[1]);
  //

  return lc+rc+lw+rw+lb+rb;
  // int n =  max(lc,lw, rc, rw);
  // return n+ rb;
  // // return lc+rc+lw+rw;
  // return lc+lw+lb;
  //State s = bfs_eval(curr, goal);
  //return bfs_eval(curr, goal);


}

int main(int argc, char const *argv[]) {

  ifstream f(argv[1]);
  ifstream g(argv[2]);
  string mode = argv[3];
  string of = argv[4];

  State init = get_data(f);
  State goal = get_data(g);
  State res;

  switch(get_mode(mode)){
    case 1:
      res = bfs(init, goal);
      // print_State(bf);
    break;
    case 2:
      res = dfs(init, goal);
      // print_State(df);
    break;
    case 3:
      res = ids(init, goal);
      // print_State(id);
    break;
    case 0:
      cout<<"Algorithm not found."<<endl;
  }
//  print_State(res);
  print(res,of);



  return 0;
}
