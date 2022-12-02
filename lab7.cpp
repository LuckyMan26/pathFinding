#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>
#include<set>
#include<cmath>
#include<stack>
#include<time.h>
#define INFINITY 100000
using namespace std;
using namespace sf;
Color cl_of_path(255,255,255);
Color cl_of_cell(190,190,190);
Color cl_of_wall(40,40,40);
Color cl_of_visited_cells(255,0,0);
int minDist(int dist[],bool Tset[],int size){
 int minimum=INT_MAX,ind;

    for(int k=0;k<size;k++)
    {
        if(Tset[k]==false && dist[k]<=minimum)
        {
            minimum=dist[k];
            ind=k;
        }
    }
    return ind;
}
double calculateHValue(int row, int col, int dest,int num_of_cells)
{
    int x=dest/num_of_cells;
    int y=dest%num_of_cells;
    return ((double)(
        (row - x) * (row - x)
        + (col - y) * (col - y)));
}

typedef pair<double, pair<int, int> > pPair;
struct cell{
int index;
float x,y;
float f,g,h=-1;
int parent_i,parent_j=-1;
int size;
Color cl;
RectangleShape st;
cell(){
this->x=0;
this->y=0;
}
cell(float x,float y,int size,Color cl,int i){
this->x=x;
this->y=y;
this->size=size;
this->cl=cl;
st.setPosition(x,y);
st.setFillColor(cl_of_cell);
st.setSize(Vector2f(size,size));
st.setOutlineColor(Color::Black);
st.setOutlineThickness(2.0);
index=i;
}
void ChangeColor(Event event){
if(event.type==Event::MouseButtonPressed){
     if (event.mouseButton.button == sf::Mouse::Right)
    {
       this->cl=Color::Yellow;
       st.setFillColor(Color::Yellow);
    }
    if (event.mouseButton.button == sf::Mouse::Left)
    {
       this->cl=Color::Green;
       st.setFillColor(Color::Green);
    }
     if ((event.mouseButton.button == sf::Mouse::Middle)||((event.mouseButton.button == sf::Mouse::XButton1)))
    {
       this->cl=cl_of_wall;
       st.setFillColor(cl_of_wall);
    }
}
}
};
vector<int> tracePath(cell** cellDetails, int end,int num_of_cells)
{

    int row = end/num_of_cells;
    int col = end%num_of_cells;

    vector<int> Path;

    while (!(cellDetails[row][col].parent_i == row
             && cellDetails[row][col].parent_j == col)) {
        Path.push_back(row*num_of_cells+col);
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    Path.push_back(row*num_of_cells+col);

    return Path;
}
bool isValid(int row, int col,int num_of_cells,vector<cell>& v)
{

    return (row >= 0) && (row < num_of_cells) && (col >= 0)
           && (col < num_of_cells) && (v[row*num_of_cells+col].cl!=cl_of_wall);
}
struct node{
cell data;
node* next;
node(){
next=nullptr;
}
node(cell d){
data=d;
next=nullptr;
}
};
struct ListAdj{
int size;
node** arr;
ListAdj(){
size=0;
arr=new node*[size];
for(int i=0;i<size;i++){
    arr[i]=nullptr;
}
}
ListAdj(int s){
size=s;
arr=new node*[size];
for(int i=0;i<size;i++){
    arr[i]=nullptr;
}
}
void clear(){
arr=nullptr;
}
void print(){
node* temp=new node();
for(int i=0;i<size;i++){
    temp=arr[i];
    cout<<i<<" ";
    while(temp){
        cout<<temp->data.index<<" ";
        temp=temp->next;
    }
    cout<<endl;
}
}
void add_edge(cell beg,cell end){
node* temp=new node();


    temp=arr[beg.index];
    if(temp){
    while(temp->next){
       temp=temp->next;
    }
    node* tmp=new node(end);
    temp->next=tmp;
    }
    else{
            arr[beg.index]=new node(end);
    }


}
int find_min(int** dist,int& min_x,int& min_y,bool** sptSet,int size){
    int mini=INFINITY;
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            if(sptSet[i][j]==false && dist[i][j]<mini){
                mini=dist[i][j];
                min_x=i;
                min_y=j;
            }
}
vector<int> djikstra(int start,int end,vector<cell>& v,RenderWindow& window,int num_of_cells){
   bool** Spt=new bool*[size];
    for(int i=0;i<size;i++){
        Spt[i]=new bool[size];
    }

   float** cost=new float*[size];
   for(int i=0;i<size;i++){
    cost[i]=new float[size];
   }

   float* distance=new float[size];


   int pred[size];
   int visited[size],count,nextnode,i,j;
   float mindistance;
   for(int i=0;i<size;i++){
     for(int j=0;j<size;j++){
        cost[i][j]=INFINITY;
   }
   }
    for(int i=0;i<size;i++){
        distance[i]=INFINITY;

   }


   for(int i=0;i<size;i++){
    node* temp=new node();
    temp=arr[i];
    while(temp){
        if((abs((temp->data.index)/num_of_cells-i/num_of_cells)==1)&&(abs((temp->data.index)%num_of_cells-i%num_of_cells)==1)){
          cost[i][temp->data.index]=1.414;
        }
        else{
            cost[i][temp->data.index]=1;
        }
        temp=temp->next;
    }
   }
   for(i=0;i<size;i++) {
      distance[i]=cost[start][i];
      pred[i]=start;
      visited[i]=0;
   }
   distance[start]=0;
   visited[start]=1;
   int m=INFINITY;

    count=1;
   int found=0;

   while(count<size-1&&found!=1) {

            for(int i=0;i<size;i++){

                if(visited[i]==1&&v[i].cl!=cl_of_wall){
                    v[i].st.setFillColor(cl_of_visited_cells);
                    v[i].cl=cl_of_visited_cells;
                    window.draw(v[i].st);
                    RectangleShape r;
                    r.setFillColor(Color::Black);
                    r.setPosition(1000,0);
                    r.setSize(Vector2f(1000,1000));
                    window.draw(r);
                    window.display();
                }



            }

            if(visited[end]==true){

                found=1;
                break;
            }

      mindistance=100000.0;
      for(i=0;i<size;i++)

         if(distance[i]<mindistance&&!visited[i]) {
         mindistance=distance[i];
         nextnode=i;
      }
      visited[nextnode]=1;

      for(i=0;i<size;i++){
         if(!visited[i]){
      if(mindistance+cost[nextnode][i]<distance[i]) {
         distance[i]=mindistance+cost[nextnode][i];
         pred[i]=nextnode;
      }
        }
      }
      count++;
   }
   int path=end;
      vector<int> vec={};
      if(found==0){
        return {};
      }
      else{
      vec.push_back(v[start].index);
      do {
         path=pred[path];
         vec.push_back(v[path].index);
      }while(path!=start);
      vec.push_back(v[end].index);
    return vec;
      }
}
vector<int> Astar(vector<cell>& v,int start,int end,RenderWindow& window,int num_of_cells){
RectangleShape r;
r.setFillColor(Color::Black);
r.setPosition(1000,0);
r.setSize(Vector2f(1000,1000));

 bool** closedList=new bool*[num_of_cells];
    for(int i=0;i<num_of_cells;i++){
        closedList[i]=new bool[num_of_cells];
    }
 for(int i=0;i<num_of_cells;i++){
        for(int j=0;j<num_of_cells;j++){
            closedList[i][j]=false;
        }
    }

    cell** cellDetails=new cell*[num_of_cells];
    for(int i=0;i<num_of_cells;i++){
        cellDetails[i]=new cell[num_of_cells];
    }
    int i, j;

    for (i = 0; i < num_of_cells; i++) {
        for (j = 0; j < num_of_cells; j++) {
            cellDetails[i][j].f = INFINITY;
            cellDetails[i][j].g = INFINITY;
            cellDetails[i][j].h = INFINITY;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }

    i = start/num_of_cells, j = start%num_of_cells;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;


    set<pPair> openList;


    openList.insert(make_pair(0.0, make_pair(i, j)));


    bool foundDest = false;

    while (!openList.empty()) {
        pPair p = *openList.begin();

        openList.erase(openList.begin());


        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

        double gNew, hNew, fNew;


        if (isValid(i-1,j,num_of_cells,v)) {
            if (((i-1)==end/num_of_cells)&&(j==end%num_of_cells)) {

                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;

                return tracePath(cellDetails, end,num_of_cells);
                foundDest = true;

            }

            else if (closedList[i - 1][j] == false
                     && v[(i-1)*num_of_cells+j].cl!=cl_of_wall) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j, end,num_of_cells);
                fNew = gNew + hNew;


                if (cellDetails[i - 1][j].f == INFINITY
                    || cellDetails[i - 1][j].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j)));

                    cellDetails[i - 1][j].f = fNew;
                    cellDetails[i - 1][j].g = gNew;
                    cellDetails[i - 1][j].h = hNew;
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                }
                sf::sleep(milliseconds(5));
             v[(i-1)*num_of_cells+(j)].st.setFillColor(cl_of_visited_cells);
        v[(i-1)*num_of_cells+(j)].cl=cl_of_visited_cells;
        window.draw(v[(i-1)*num_of_cells+(j)].st);
        window.draw(r);
        window.display();
            }

        }



        if (isValid(i + 1, j,num_of_cells,v) == true) {
            if ((i + 1==end/num_of_cells)&&(j==end%num_of_cells)) {

                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;

                 return tracePath(cellDetails, end,num_of_cells);
                foundDest = true;

            }

            else if (closedList[i + 1][j] == false
                     &&v[(i+1)*num_of_cells+j].cl!=cl_of_wall) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j, end,num_of_cells);
                fNew = gNew + hNew;


                if (cellDetails[i + 1][j].f == INFINITY
                    || cellDetails[i + 1][j].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j)));

                    cellDetails[i + 1][j].f = fNew;
                    cellDetails[i + 1][j].g = gNew;
                    cellDetails[i + 1][j].h = hNew;
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                }
                 sf::sleep(milliseconds(5));
             v[(i+1)*num_of_cells+(j)].st.setFillColor(cl_of_visited_cells);
        v[(i+1)*num_of_cells+(j)].cl=cl_of_visited_cells;
        window.draw(v[(i+1)*num_of_cells+(j)].st);
        window.draw(r);
        window.display();
            }

        }


        if (isValid(i, j + 1,num_of_cells,v) == true) {
            if ((i==end/num_of_cells)&&((j+1)==end%num_of_cells) == true) {

                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;
                return tracePath(cellDetails, end,num_of_cells);
                foundDest = true;

            }


            else if (closedList[i][j + 1] == false
                     && v[i*num_of_cells+(j+1)].cl!=cl_of_wall) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j + 1, end,num_of_cells);
                fNew = gNew + hNew;


                if (cellDetails[i][j + 1].f == INFINITY
                    || cellDetails[i][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i, j + 1)));


                    cellDetails[i][j + 1].f = fNew;
                    cellDetails[i][j + 1].g = gNew;
                    cellDetails[i][j + 1].h = hNew;
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                }
                sf::sleep(milliseconds(5));
         v[i*num_of_cells+(j+1)].st.setFillColor(cl_of_visited_cells);
        v[(i)*num_of_cells+(j+1)].cl=cl_of_visited_cells;
        window.draw(v[(i)*num_of_cells+(j+1)].st);
        window.draw(r);
        window.display();
            }

        }


        if (isValid(i, j - 1,num_of_cells,v) == true) {
            if (i==end/num_of_cells && ((j-1)==end%num_of_cells)) {

                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;
                return tracePath(cellDetails, end,num_of_cells);
                foundDest = true;

            }


            else if (closedList[i][j - 1] == false
                     && v[i*num_of_cells+(j-1)].cl!=cl_of_wall) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j - 1, end,num_of_cells);
                fNew = gNew + hNew;


                if (cellDetails[i][j - 1].f == INFINITY
                    || cellDetails[i][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i, j - 1)));


                    cellDetails[i][j - 1].f = fNew;
                    cellDetails[i][j - 1].g = gNew;
                    cellDetails[i][j - 1].h = hNew;
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                }
                 sf::sleep(milliseconds(5));
              v[i*num_of_cells+(j-1)].st.setFillColor(cl_of_visited_cells);
        v[(i)*num_of_cells+(j-1)].cl=cl_of_visited_cells;
        window.draw(v[(i)*num_of_cells+(j-1)].st);
        window.draw(r);
        window.display();
            }

        }








     if (isValid(i - 1, j + 1,num_of_cells,v) == true&&isValid(i, j +1,num_of_cells,v)&&isValid(i-1, j,num_of_cells,v)) {
            if ((i - 1==end/num_of_cells)&&(j+1==end%num_of_cells) == true) {

                cellDetails[i - 1][j + 1].parent_i = i;
                cellDetails[i - 1][j + 1].parent_j = j;
                return tracePath(cellDetails, end,num_of_cells);
                foundDest = true;

            }


            else if (closedList[i - 1][j + 1] == false
                     && v[(i-1)*num_of_cells+(j+1)].cl!=cl_of_wall&&v[(i)*num_of_cells+(j+1)].cl!=cl_of_wall&&v[(i-1)*num_of_cells+(j)].cl!=cl_of_wall) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i - 1, j + 1, end,num_of_cells);
                fNew = gNew + hNew;


                if (cellDetails[i - 1][j + 1].f == INFINITY
                    || cellDetails[i - 1][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j + 1)));

                    cellDetails[i - 1][j + 1].f = fNew;
                    cellDetails[i - 1][j + 1].g = gNew;
                    cellDetails[i - 1][j + 1].h = hNew;
                    cellDetails[i - 1][j + 1].parent_i = i;
                    cellDetails[i - 1][j + 1].parent_j = j;
                }
                 sf::sleep(milliseconds(5));
             v[(i-1)*num_of_cells+(j+1)].st.setFillColor(cl_of_visited_cells);
        v[(i-1)*num_of_cells+(j+1)].cl=cl_of_visited_cells;
        window.draw(v[(i-1)*num_of_cells+(j+1)].st);
        window.draw(r);
        window.display();
            }

        }




        if (isValid(i - 1, j - 1,num_of_cells,v)  && isValid(i - 1, j,num_of_cells,v) && isValid(i, j - 1,num_of_cells,v)) {

            if (((i-1)==end/num_of_cells)&&((j-1)==end%num_of_cells)) {

                cellDetails[i - 1][j - 1].parent_i = i;
                cellDetails[i - 1][j - 1].parent_j = j;
                return tracePath(cellDetails, end,num_of_cells);
                foundDest = true;

            }


            else if (closedList[i - 1][j - 1] == false
                     && v[(i-1)*num_of_cells+(j-1)].cl!=cl_of_wall&& v[(i)*num_of_cells+(j-1)].cl!=cl_of_wall&& v[(i-1)*num_of_cells+(j)].cl!=cl_of_wall) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i - 1, j - 1, end,num_of_cells);
                fNew = gNew + hNew;


                if (cellDetails[i - 1][j - 1].f == INFINITY
                    || cellDetails[i - 1][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j - 1)));
                    cellDetails[i - 1][j - 1].f = fNew;
                    cellDetails[i - 1][j - 1].g = gNew;
                    cellDetails[i - 1][j - 1].h = hNew;
                    cellDetails[i - 1][j - 1].parent_i = i;
                    cellDetails[i - 1][j - 1].parent_j = j;
                }
                   sf::sleep(milliseconds(5));
             v[(i-1)*num_of_cells+(j-1)].st.setFillColor(cl_of_visited_cells);
        v[(i-1)*num_of_cells+(j-1)].cl=cl_of_visited_cells;
        window.draw(v[(i-1)*num_of_cells+(j-1)].st);
        window.draw(r);
        window.display();
            }

        }


        if (isValid(i + 1, j + 1,num_of_cells,v) == true &&isValid(i , j + 1,num_of_cells,v) && isValid(i + 1, j,num_of_cells,v)) {
            if ((i + 1==end/num_of_cells)&&(j+1==end%num_of_cells) == true) {

                cellDetails[i + 1][j + 1].parent_i = i;
                cellDetails[i + 1][j + 1].parent_j = j;
                printf("The destination cell is found\n");
                return tracePath(cellDetails, end,num_of_cells);
                foundDest = true;

            }

            else if (closedList[i + 1][j + 1] == false
                     && v[(i+1)*num_of_cells+(j+1)].cl!=cl_of_wall&& v[(i)*num_of_cells+(j+1)].cl!=cl_of_wall&& v[(i+1)*num_of_cells+(j)].cl!=cl_of_wall) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i + 1, j + 1, end,num_of_cells);
                fNew = gNew + hNew;


                if (cellDetails[i + 1][j + 1].f == INFINITY
                    || cellDetails[i + 1][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j + 1)));

                    cellDetails[i + 1][j + 1].f = fNew;
                    cellDetails[i + 1][j + 1].g = gNew;
                    cellDetails[i + 1][j + 1].h = hNew;
                    cellDetails[i + 1][j + 1].parent_i = i;
                    cellDetails[i + 1][j + 1].parent_j = j;
                }
                sf::sleep(milliseconds(5));
             v[(i+1)*num_of_cells+(j+1)].st.setFillColor(cl_of_visited_cells);
        v[(i+1)*num_of_cells+(j+1)].cl=cl_of_visited_cells;
        window.draw(v[(i+1)*num_of_cells+(j+1)].st);
        window.draw(r);
        window.display();
            }

        }


        if (isValid(i + 1, j - 1,num_of_cells,v) == true&&isValid(i, j - 1,num_of_cells,v)&&isValid(i + 1, j,num_of_cells,v) == true) {
            if ((i + 1==end/num_of_cells)&&(j-1==end%num_of_cells) == true) {
                cellDetails[i + 1][j - 1].parent_i = i;
                cellDetails[i + 1][j - 1].parent_j = j;

                return tracePath(cellDetails, end,num_of_cells);
                foundDest = true;

            }


            else if (closedList[i + 1][j - 1] == false
                     && v[(i+1)*num_of_cells+(j-1)].cl!=cl_of_wall&& v[(i+1)*num_of_cells+(j)].cl!=cl_of_wall&& v[(i)*num_of_cells+(j-1)].cl!=cl_of_wall) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i + 1, j - 1, end,num_of_cells);
                fNew = gNew + hNew;


                if (cellDetails[i + 1][j - 1].f == INFINITY
                    || cellDetails[i + 1][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j - 1)));

                    cellDetails[i + 1][j - 1].f = fNew;
                    cellDetails[i + 1][j - 1].g = gNew;
                    cellDetails[i + 1][j - 1].h = hNew;
                    cellDetails[i + 1][j - 1].parent_i = i;
                    cellDetails[i + 1][j - 1].parent_j = j;
                }
                 sf::sleep(milliseconds(5));
                v[(i+1)*num_of_cells+(j-1)].st.setFillColor(cl_of_visited_cells);
                v[(i+1)*num_of_cells+(j-1)].cl=cl_of_visited_cells;
            window.draw(v[(i+1)*num_of_cells+(j-1)].st);
            window.draw(r);
        window.display();
            }
        }

    }

    if (foundDest == false)
    return {};
        cout<<"Failed to find the Destination Cell\n";


}
};
ListAdj convert_map_to_graph(vector<cell>& v,int num_of_cells){
 ListAdj g(num_of_cells*num_of_cells);

    for(int i=0;i<v.size();i++){
        if(((v[i].index/num_of_cells)<num_of_cells)&&(v[i].cl!=cl_of_wall)&&((v[i+num_of_cells].cl!=cl_of_wall))&&(i+num_of_cells<v.size())){
                g.add_edge(v[i],v[i+num_of_cells]);
        }

         if((((v[i].index)/num_of_cells)>0)&&(v[i].cl!=cl_of_wall)&&((v[i-num_of_cells].cl!=cl_of_wall))&&(i-num_of_cells>0)){
            g.add_edge(v[i],v[i-num_of_cells]);
        }
        if(((v[i].index%num_of_cells)<num_of_cells-1)&&(v[i].cl!=cl_of_wall)&&((v[i+1].cl!=cl_of_wall))&&((i+1<v.size()))){
        g.add_edge(v[i],v[i+1]);
        }
         if(((v[i].index%num_of_cells)>0)&&(v[i].cl!=cl_of_wall)&&((v[i-1].cl!=cl_of_wall))&&(i-1>0)){
        g.add_edge(v[i],v[i-1]);
        }


        if(((v[i].index/num_of_cells)<num_of_cells)&&(v[i].cl!=cl_of_wall)&&((v[i+num_of_cells].cl!=cl_of_wall))&&(i+num_of_cells+1<v.size())){
               if(((v[i].index%num_of_cells)<num_of_cells-1)&&(v[i].cl!=cl_of_wall)&&((v[i+1].cl!=cl_of_wall))&&(i+num_of_cells+1<v.size())){
        g.add_edge(v[i],v[i+num_of_cells+1]);
        }
        }
        if(((v[i].index/num_of_cells)<num_of_cells)&&(v[i].cl!=cl_of_wall)&&((v[i+num_of_cells].cl!=cl_of_wall))&&(i+num_of_cells<v.size())){
                if(((v[i].index%num_of_cells)>0)&&(v[i].cl!=cl_of_wall)&&((v[i-1].cl!=cl_of_wall))&&(i-1>0)){
        g.add_edge(v[i],v[i+num_of_cells-1]);
        }
        }
         if((((v[i].index)/num_of_cells)>0)&&(v[i].cl!=cl_of_wall)&&((v[i-num_of_cells].cl!=cl_of_wall))&&(i-num_of_cells-1>0)){
             if(((v[i].index%num_of_cells)>0)&&(v[i].cl!=cl_of_wall)&&((v[i-1].cl!=cl_of_wall))&&(i-1-num_of_cells>0)){
        g.add_edge(v[i],v[i-num_of_cells-1]);
        }
        }
         if((((v[i].index)/num_of_cells)>0)&&(v[i].cl!=cl_of_wall)&&((v[i-num_of_cells].cl!=cl_of_wall))&&(i-num_of_cells>0)){
            if((((v[i].index)%num_of_cells)<num_of_cells-1)&&(v[i].cl!=cl_of_wall)&&((v[i+1].cl!=cl_of_wall))&&(i+1<v.size())){
        g.add_edge(v[i],v[i-num_of_cells+1]);
        }
        }
    }
    return g;

}
ListAdj create_maze(vector<cell>& v,ListAdj g,int num_of_cells,int current,bool* visited){
visited[current]=true;
ListAdj g1(num_of_cells*num_of_cells);
stack<int> s;
s.push(current);
vector<int> vec;
node* temp=new node();
while(!s.empty()){
    vec.clear();

    current=s.top();

    s.pop();
    temp=g.arr[current];

    while(temp){


            if(!visited[temp->data.index]){
                vec.push_back(temp->data.index);

        }
            temp=temp->next;

    }
    if(!vec.empty()){
        s.push(current);

        int x=rand()%vec.size();
       for (auto i = 0; i < vec.size(); ++i)
      {
          visited[vec[i]]=true;

        if (i != x) s.push(vec[i]);

        if(current-vec[i]==2){
            g1.add_edge(v[current],v[current-1]);
            g1.add_edge(v[vec[i]],v[vec[i]+1]);
            g1.add_edge(v[current-1],v[current]);
            g1.add_edge(v[vec[i]+1],v[vec[i]]);
        }
        else if(current-vec[i]==-2){
            g1.add_edge(v[current],v[current+1]);
            g1.add_edge(v[vec[i]],v[vec[i]-1]);
            g1.add_edge(v[current+1],v[current]);
            g1.add_edge(v[vec[i]-1],v[vec[i]]);
        }
         else if(current-vec[i]==2*num_of_cells){
            g1.add_edge(v[current],v[current-num_of_cells]);
            g1.add_edge(v[vec[i]],v[vec[i]+num_of_cells]);
            g1.add_edge(v[current-num_of_cells],v[current]);
            g1.add_edge(v[vec[i]+num_of_cells],v[vec[i]]);
        }
         else if(current-vec[i]==-2*num_of_cells){
            g1.add_edge(v[current],v[current+num_of_cells]);
            g1.add_edge(v[vec[i]],v[vec[i]-num_of_cells]);
            g1.add_edge(v[current+num_of_cells],v[current]);
            g1.add_edge(v[vec[i]-num_of_cells],v[vec[i]]);
        }
      }
      s.push(vec[x]);




    }
    }
return g1;

}
ListAdj transform_map(vector<cell>& v,int num_of_cells){
ListAdj g1(num_of_cells*num_of_cells);
for(int i=0;i<num_of_cells;i+=2){
    for(int j=0;j<num_of_cells;j++){
        v[i+j*num_of_cells].st.setFillColor(cl_of_wall);
        v[i+j*num_of_cells].cl=cl_of_wall;

    }
}
for(int i=1;i<num_of_cells;i+=2){
    for(int j=0;j<num_of_cells;j++){
        v[i*num_of_cells+j].st.setFillColor(cl_of_wall);
        v[i*num_of_cells+j].cl=cl_of_wall;

    }
}

for(int i=0;i<v.size();i++){
    if(((v[i].index/num_of_cells)<num_of_cells-1)&&(v[i].cl!=cl_of_wall)&&((v[i+2*num_of_cells].cl!=cl_of_wall))){
                g1.add_edge(v[i],v[i+2*num_of_cells]);
        }

         if((((v[i].index)/num_of_cells)>1)&&(v[i].cl!=cl_of_wall)&&((v[i-2*num_of_cells].cl!=cl_of_wall))){
            g1.add_edge(v[i],v[i-2*num_of_cells]);
        }
        if(((v[i].index%num_of_cells)<num_of_cells-2)&&(v[i].cl!=cl_of_wall)&&((v[i+2].cl!=cl_of_wall))){
        g1.add_edge(v[i],v[i+2]);
        }
         if(((v[i].index%num_of_cells)>1)&&(v[i].cl!=cl_of_wall)&&((v[i-2].cl!=cl_of_wall))){
        g1.add_edge(v[i],v[i-2]);
        }
}

return g1;
}
void draw_map(ListAdj g,int num_of_cells,vector<cell>& v){
node* temp=new node();
for(int i=0;i<num_of_cells*num_of_cells;i++){
    temp=g.arr[i];
    while(temp){

        v[i].st.setFillColor(cl_of_cell);
        v[i].cl=cl_of_cell;

        v[temp->data.index].st.setFillColor(cl_of_cell);
        v[temp->data.index].cl=cl_of_cell;


        temp=temp->next;
    }
}
}
void clear(vector<cell>& v,RenderWindow& window){
for(int i=0;i<v.size();i++){
    v[i].st.setFillColor(cl_of_cell);
    v[i].cl=cl_of_cell;
    window.draw(v[i].st);
}
}
int main(){


    srand(time(NULL));
    int index_of_target=-1;
    int start=-1;
    int size_of_window_x=1500;
    int size_of_window_y=1000;
    int size_x=1000;
    int size_y=1000;
    CircleShape st(50.f);
    int num_of_cells;
    int size=20;
    int count=0;

    st.setFillColor(cl_of_cell);
    RenderWindow window(sf::VideoMode(size_of_window_x, size_of_window_y), "SFML works!");
    vector<cell> v;
     for(int i=0;i<size_y;i+=1.5*size){
                num_of_cells++;
                for(int j=0;j<size_x;j+=1.5*size){
                    cell c(j,i,size,cl_of_cell,count);
                    count++;
                    v.push_back(c);
                }
            }
sf::Text Text1;
sf::Text Text2;
sf::Text Text3;
sf::Text Text4;
sf::Text Text5;
sf::Text Text6;
sf::Text Text7;
sf::Text Text8;
sf::Text Text9;
sf::Font font;
sf::Texture texture;
texture.loadFromFile("yellow.png");
sf::Sprite sprite;
sprite.setTexture(texture);
sprite.setTextureRect(sf::IntRect(10, 10, 30, 30));
sprite.setPosition(1050, 425);

sf::Texture texture1;
texture1.loadFromFile("index.png");
sf::Sprite sprite1;
sprite1.setTexture(texture1);
sprite1.setTextureRect(sf::IntRect(10, 10, 30, 30));
sprite1.setPosition(1050, 525);

sf::Texture texture2;
texture2.loadFromFile("black.png");
sf::Sprite sprite2;
sprite2.setTexture(texture2);
sprite2.setTextureRect(sf::IntRect(10, 10, 30, 30));
sprite2.setPosition(1050, 625);

sf::Texture texture3;
texture3.loadFromFile("white.jpg");
sf::Sprite sprite3;
sprite3.setTexture(texture3);
sprite3.setTextureRect(sf::IntRect(10, 10, 30, 30));
sprite3.setPosition(1050, 725);

sf::Texture texture4;
texture4.loadFromFile("red.jpg");
sf::Sprite sprite4;
sprite4.setTexture(texture4);
sprite4.setTextureRect(sf::IntRect(10, 10, 30, 30));
sprite4.setPosition(1050, 825);
if (!font.loadFromFile("arial.ttf"))
{

}
  Text1.setStyle( sf::Text::Bold );
  Text1.setFont(font);
  Text1.setString( "Dijkstra algorithm" );
  Text1.setFillColor( sf::Color::Green );
  Text1.setCharacterSize( 48 );
  Text1.setPosition(1050,0);

  Text2.setStyle( sf::Text::Bold );
  Text2.setFont(font);
  Text2.setString( "A*   algorithm" );
  Text2.setFillColor( sf::Color::Green );
  Text2.setCharacterSize( 48 );
  Text2.setPosition(1050,100);

  Text3.setStyle( sf::Text::Bold );
  Text3.setFont(font);
  Text3.setString( "Create random maze" );
  Text3.setFillColor( sf::Color::Green );
  Text3.setCharacterSize( 48 );
  Text3.setPosition(1020,200);

  Text4.setStyle( sf::Text::Bold );
  Text4.setFont(font);
  Text4.setString( "         Clear     ");
  Text4.setFillColor( sf::Color::Green );
  Text4.setCharacterSize( 48 );
  Text4.setPosition(1020,300);

  Text5.setStyle( sf::Text::Bold );
  Text5.setFont(font);
  Text5.setString( "   finish (right click)");
  Text5.setFillColor( sf::Color::Green );
  Text5.setCharacterSize( 48 );
  Text5.setPosition(1050,400);

  Text6.setStyle( sf::Text::Bold );
  Text6.setFont(font);
  Text6.setString( "   start  (left click)");
  Text6.setFillColor( sf::Color::Green );
  Text6.setCharacterSize( 48 );
  Text6.setPosition(1050,500);

  Text7.setStyle( sf::Text::Bold );
  Text7.setFont(font);
  Text7.setString( "     wall  (middle)   ");
  Text7.setFillColor( sf::Color::Green );
  Text7.setCharacterSize( 48 );
  Text7.setPosition(1050,600);

  Text8.setStyle( sf::Text::Bold );
  Text8.setFont(font);
  Text8.setString( "     path     ");
  Text8.setFillColor( sf::Color::Green );
  Text8.setCharacterSize( 48 );
  Text8.setPosition(1050,700);

  Text9.setStyle( sf::Text::Bold );
  Text9.setFont(font);
  Text9.setString( "    visited   cells     ");
  Text9.setFillColor( sf::Color::Green );
  Text9.setCharacterSize( 48 );
  Text9.setPosition(1050,800);
    while (window.isOpen())
    {

        Event event;
        for(int i=0;i<v.size();i++){
            window.draw(v[i].st);
        }

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
            }
            if (event.type == sf::Event::KeyPressed){
                  if (event.key.code == sf::Keyboard::Enter){
                    ListAdj g(num_of_cells*num_of_cells);
                    g=convert_map_to_graph(v,num_of_cells);
                    cout<<"Finding path"<<endl;
                    vector<int> vec;
                    vec=g.djikstra(start,index_of_target,v,window,num_of_cells);
                    window.display();
                    if(vec.size()==1){
                        cout<<"No path"<<endl;
                    }
                    else{


                    for(int i=1;i<vec.size()-1;i++){
                        v[vec[i]].st.setFillColor(cl_of_path);

                    }
                    v[start].st.setFillColor(Color::Green);
                    v[start].cl=Color::Green;
                    v[index_of_target].st.setFillColor(Color::Yellow);
                    v[index_of_target].cl=Color::Yellow;
                  }
            }
            }
            if(event.type==Event::MouseButtonPressed){

            Vector2i pos=Mouse::getPosition(window);

            for(int i=0;i<v.size();i++){
                if((v[i].x<=pos.x)&&((v[i].x+size+2.0>=pos.x))&&(v[i].y<=pos.y)&&((v[i].y+size+2.0>=pos.y))&&(v[i].cl!=cl_of_wall)){
                if(event.mouseButton.button == sf::Mouse::Right&&index_of_target!=-1){
                        v[index_of_target].st.setFillColor(cl_of_cell);
                        index_of_target=i;
                        v[i].ChangeColor(event);

                }
                 else if(event.mouseButton.button == sf::Mouse::Right&&index_of_target==-1){
                        index_of_target=i;
                        v[i].ChangeColor(event);


                }
                 else if(event.mouseButton.button == sf::Mouse::Left&&start!=-1){
                        v[start].st.setFillColor(cl_of_cell);
                        start=i;
                        v[i].ChangeColor(event);
                }
                 else if(event.mouseButton.button == sf::Mouse::Left&&start==-1){

                        start=i;
                        v[i].ChangeColor(event);
                }
                else{
                    v[i].ChangeColor(event);
                }
                }

            }
            if((size_x+50<pos.x)&&(pos.x<size_of_window_x-50)&&(0<pos.y)&&(pos.y<48)){

                  if(start==-1||index_of_target==-1){
                    cout<<"Enter start and finish"<<endl;
                  }
                  else{
                  ListAdj g(num_of_cells*num_of_cells);
                   g=convert_map_to_graph(v,num_of_cells);
                    cout<<"Finding path"<<endl;

                    window.display();
                    vector<int> vec={};
                    vec=g.djikstra(start,index_of_target,v,window,num_of_cells);
                    window.display();
                    if(vec.size()<=1){
                        cout<<"No path"<<endl;
                    }
                    else{


                    for(int i=1;i<vec.size()-1;i++){
                         v[vec[i]].st.setFillColor(cl_of_path);

                    }
                    v[start].st.setFillColor(Color::Green);
                    v[start].cl=Color::Green;
                    v[index_of_target].st.setFillColor(Color::Yellow);
                    v[index_of_target].cl=Color::Yellow;
                  }
            }
            }
              if((size_x+50<pos.x)&&(pos.x<size_of_window_x-50)&&(100<pos.y)&&(pos.y<100+48)){

                  if(start==-1||index_of_target==-1){
                    cout<<"Enter start and finish"<<endl;
                  }
              else{
                    ListAdj g(num_of_cells*num_of_cells);
                    g=convert_map_to_graph(v,num_of_cells);
                    cout<<"Finding path"<<endl;
                    vector<int> vec;
                    vec=g.Astar(v,start,index_of_target,window,num_of_cells);
                    window.display();
                    if(vec.size()<=1){
                        cout<<"No path"<<endl;
                    }
                    else{

                    for(int i=1;i<vec.size()-1;i++){
                        v[vec[i]].st.setFillColor(cl_of_path);

                    }
                    v[start].st.setFillColor(Color::Green);
                    v[start].cl=Color::Green;
                    v[index_of_target].st.setFillColor(Color::Yellow);
                    v[index_of_target].cl=Color::Yellow;
                  }

            }
            }
            if((size_x+50<pos.x)&&(pos.x<size_of_window_x-50)&&(200<pos.y)&&(pos.y<200+48)){
                  bool* visited=new bool[num_of_cells*num_of_cells];

                  for(int i=0;i<num_of_cells*num_of_cells;i++){
                    visited[i]=false;
                  }




ListAdj g1(num_of_cells*num_of_cells);
ListAdj g2(num_of_cells*num_of_cells);
g1=transform_map(v,num_of_cells);
            int x,y;
                  x=rand()%num_of_cells;
                y=rand()%num_of_cells;
                  while(v[x*num_of_cells+y].cl==cl_of_wall){
                    x=rand()%num_of_cells;
                    y=rand()%num_of_cells;
                  }
g2=create_maze(v,g1,num_of_cells,x*num_of_cells+y,visited);

draw_map(g2,num_of_cells,v);
g1.clear();
g2.clear();

            }
              if((size_x+50<pos.x)&&(pos.x<size_of_window_x-50)&&(300<pos.y)&&(pos.y<300+48)){
                clear(v,window);
                start=-1;
                index_of_target=-1;

            }
        }

        }
        window.draw(Text1);
        window.draw(Text2);
        window.draw(Text3);
        window.draw(Text4);
        window.draw(Text5);
        window.draw(Text6);
        window.draw(Text7);
        window.draw(Text8);
        window.draw(Text9);
        window.draw(sprite);
        window.draw(sprite1);
        window.draw(sprite2);
        window.draw(sprite3);
        window.draw(sprite4);
        window.display();
        window.clear();
    }

}
