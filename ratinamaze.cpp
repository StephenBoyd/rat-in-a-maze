//Edwin Warner and Stephen Boyd
//Rat in a maze using depth-first search.
//This actually tends to find one of the longest possible paths.
//uncomment line 226 to actually watch the rat's movements.

#include <cstdlib>
#include <vector>
#include <stack>
#include <iostream>
#include <string>

using namespace std;

class Maze{

private:
  struct point{
    int x;
    int y;
    bool open;
    bool start;
    bool end;
    bool current;
    bool visited;
    bool onPath;
  };

  void fill(){
    point tempCoor;
    for(int i = 0; i < graph.size(); i++){
      for(int j = 0; j < graph[i].size(); j++){	
        tempCoor.x = i;
        tempCoor.y = j;
        tempCoor.open = false;
        tempCoor.start = false;
        tempCoor.end = false;
        tempCoor.current = false;
        tempCoor.visited = false;
        tempCoor.onPath = false;
        graph[i][j] = tempCoor;
      }
    }
  }

  void printCoor(const struct point c){
    string toPrint;
    string blue  = "\033[44m\033[34m #";  //for closed points
    string yellow = "\033[43m\033[30m ."; //for open points
    string white = "\033[47m\033[30m x"; //visited
    string rat  = "\033[42m\033[32m O"; //location of the generator
    string start = "\033[0m A";
    string end = "\033[0m B";
    string red = "\033[41m\033[30m o";
    c.open ? toPrint = yellow : toPrint = blue;
    if (c.visited) toPrint = white;
    if (c.onPath) toPrint = red;
    if (c.start) toPrint = start;
    if (c.end) toPrint = end;
    if (c.current) toPrint = rat;
    cout << toPrint;
  }

  void generateMaze(int rows, int cols){
    point tempCoor;
    int halfWidth = (rows / 2) + 2;
    tempCoor.x = cols / 8;
    tempCoor.y = rows / 2;
    int direction;
      srand(time(NULL));

    for(int i = 0; i < (rows * 5); i++){
      direction = rand() % 4;
      if (i == 0) direction = 2;
      switch(direction){
        case 0: // up
          for (int q = 0; q < rand() % halfWidth; q++){
            if (tempCoor.y <= 1) tempCoor.y = 1;
            else {
              tempCoor.y--;
              graph[tempCoor.y][tempCoor.x].open = true;
            }
          }
          break;
        case 1: // down
          for (int q = 0; q < rand() % halfWidth; q++){
            if (tempCoor.y >= rows - 2) tempCoor.y = rows - 2;
            else {
              tempCoor.y++;
              graph[tempCoor.y][tempCoor.x].open = true;
            }
          }
          break;
        case 2:	 // right
          for (int q = 0; q < rand() % halfWidth; q++){
            if (tempCoor.x >= cols - 2) tempCoor.x = cols - 2;
            else {
              tempCoor.x++;
              graph[tempCoor.y][tempCoor.x].open = true;
            }
          }
          break;
        case 3: // left
          for (int q = 0; q < rand() % halfWidth; q++){
            if (tempCoor.x <= 1) tempCoor.x = 1;
            else {
              tempCoor.x--;
              graph[tempCoor.y][tempCoor.x].open = true;
            }
          }
          break;
        default:
          break;
      }
      graph[tempCoor.y][tempCoor.x].open = true;

    }
  }

  void animate(point rat, int microseconds){
    graph[rat.x][rat.y].current = true;
    printMaze();
    usleep(microseconds);
    graph[rat.x][rat.y].current = false;
  }
  
  point findStart(){
    for(int i = 0; i < graph.size(); i++){
      for(int j = 0; j < graph[i].size(); j++){	
        if (graph[i][j].open){
          graph[i][j].start = true;
          return graph[i][j];
        }
      }
    }
  }

  point findEnd(){
    for(int i = graph.size() - 1; i > 0; i--){
      for(int j = graph[i].size() - 1; j > 0; j--){	
        if (graph[i][j].open){
          graph[i][j].end = true;
          return graph[i][j];
        }
      }
    }
  }


public:	
  point start;
  point end;
  vector < vector <point> > graph;

  Maze(int size){
    int rows = size;
    int cols = size;
    graph.resize(rows);
    for(int i = 0; i < rows; i++){
      graph[i].resize(cols);
    }
    fill();
    generateMaze(rows, cols);
    start = findStart();
    end = findEnd();
  }


  void printMaze(){
    for(int i = 0; i < graph.size(); i++){
      for(int j = 0; j < graph[i].size(); j++){	
        printCoor(graph[i][j]);
      }
      cout << "\033[0m" <<endl;
    }
    cout << endl;
  }

  int numberVertices;
  int iteration;
  bool done;
  
  void navigate(){
    iteration = 0;
    done = false;
    depth = 0;
    dfsearch(start);
  }
  
  
  stack<point> path;
  stack<point> pathInOrder;

  void printPoint(point p){
    cout << "x: " << p.x << " y: " << p.y << endl;
    graph[p.x][p.y].onPath = true;
  }

  void printPath(int q = 0){
    while(!path.empty()){
      pathInOrder.push(path.top());
      path.pop();
    }
    cout << "printing path... " << endl;
    while(!pathInOrder.empty()){
      //cout << "stack size: " << pathInOrder.size() << "    ";
      printPoint(pathInOrder.top());
      pathInOrder.pop();
    }
    printMaze();
  }

  int depth;
  void dfsearch(point node){
    if (done) return;
    if (node.visited) return;
    iteration++;
    depth++;
    if (node.end){
      cout << "reached the end" << endl;
      path.push(end);
      cout << " length = " << depth << endl;
      done = true;
      return;
    }
    graph[node.x][node.y].visited = true;
    //animate(node, 200000);
    path.push(node);
    if (graph[node.x-1][node.y].open && !graph[node.x-1][node.y].visited){
      dfsearch(graph[node.x-1][node.y]);
      if (done) return;
      depth--;
      path.pop();
    }
    if (graph[node.x][node.y-1].open && !graph[node.x][node.y-1].visited){
      dfsearch(graph[node.x][node.y-1]);
      if (done) return;
      depth--;
      path.pop();
    }
    if (graph[node.x+1][node.y].open && !graph[node.x+1][node.y].visited){
      dfsearch(graph[node.x+1][node.y]);
      if (done) return;
      depth--;
      path.pop();
    }
    if (graph[node.x][node.y+1].open && !graph[node.x][node.y+1].visited){
      dfsearch(graph[node.x][node.y+1]);
      if (done) return;
      depth--;
      path.pop();
    }
  }

};


int main(){
  int size = 18;
  Maze mymaze(size);
  mymaze.printMaze();
  cout << "start: " << mymaze.start.x << ", " << mymaze.start.y << endl;
  cout << "end: " << mymaze.end.x << ", " << mymaze.end.y << endl;
  mymaze.navigate();
  mymaze.printPath();
  cout << "start: " << mymaze.start.x << ", " << mymaze.start.y << endl;
  cout << "end: " << mymaze.end.x << ", " << mymaze.end.y << endl;
  return 0;
}
