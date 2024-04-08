#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <fstream>

using namespace std;
/*

int main() {
    
    ifstream fina;
    ifstream finc;

    fina.open("a.txt");
    finc.open("c.txt");
    int cnt = 0;
    int a1, a2, c1, c2;
    cout << "와일문전";
    while ((fina >> a1 >> a2) &&(finc >> c1 >> c2) ){

        cnt++;
        if ((a1 != c1) || (a2 != c2)) {
            cout << cnt <<'\n';
            
        }
    }
    fina.close();
    finc.close();


    
}

*/

int board[51][51] = { 0, };//루돌프는 -1
int santascore[31] = { 0, };
pair<int, int> santapos[31];
int santaparalyzed[31] = { 0, };//기절한 턴 기록
int santaout[31] = { 0, };//1이면 탈락산타 
pair<int, int> rudolfmove[8] = { {-1,-1}, {-1,0},{-1,1},
                                  {0, -1}, {0,1},
                                {1,-1},{1,0},{1,1} };
pair<int, int> santamove[4] = {  {-1,0},
                                  {0, -1}, {0,1},
                                {1,0}};


int main() {

    int n, m, p, c, d;
    cin >> n >> m >> p >> c >> d;
    int rr, rc;
    cin >> rr >> rc;
    int score = 0;
    int available_santa = p;
    for (int i = 1; i <= p; i++) {
        int sr, sc,no;
        cin >> no;
        cin >> sr >> sc;
        board[sr][sc] = no;
        santapos[no].first = sr;
        santapos[no].second = sc;
    }
    //산타모두 탈락하면 break 

    for (int i = 0; i < m; i++) {

        //가장 가까운 산타 찾음 탈락한 산타 빼고 (max 30)
        int closestsanta;
        int closestdistance = (n + 1)* (n + 1)+ (n + 1)* (n + 1);
        for (int j = 1; j <= p; j++) {

            if (santaout[j] == 0) {
                int distance = (santapos[j].first - rr) * (santapos[j].first - rr) + (santapos[j].second - rc) * (santapos[j].second - rc);
                if (distance < closestdistance) {
                    closestdistance = distance;
                    closestsanta = j;
                }
                else if (distance == closestdistance) {//거리가 같으면 r이큰순으로
                    if (santapos[closestsanta].first == santapos[j].first) {
                        if (santapos[closestsanta].second < santapos[j].second) {
                            closestsanta = j;

                        }
                    }
                    else if (santapos[closestsanta].first < santapos[j].first) {
                        closestsanta = j;


                    }

                }
            }
        }
        int santax = santapos[closestsanta].first;
        int santay = santapos[closestsanta].second;

        int rudir;
        int rux;
        int ruy;
        closestdistance = (n + 1) * (n + 1) + (n + 1) * (n + 1);

        //가장 가까워지는 방향 찾음(max 8) 기록
        for (int j = 0; j < 8; j++) {

            int newr = (rudolfmove[j].first + rr);
            int newc = (rudolfmove[j].second + rc);
            int newd = (newr - santax) * (newr - santax) + (newc - santay)*(newc - santay);
            if (newd < closestdistance) {
                rudir = j;
                rux = newr;
                ruy = newc;
            }


        }


        //루돌프 돌진함(rr rc 업뎃)
        int collidesanta=0;
        if (board[rux][ruy] != 0) {
            collidesanta = board[rux][ruy];
            santascore[collidesanta] += c;
            board[rux][ruy] = -1;
            rr = rux;
            rc = ruy;
        }
        //충돌확인
        // 점수반영 c
        // 밀려남(바뀐위치에 번호표시)
        if (collidesanta) {//충돌이일어낫음
             // 탈락확인-탈락한산타 보드에서 빼고 산타수 1줄임
            // 기절기록
            santaparalyzed[collidesanta] = i;
            board[santapos[collidesanta].first][santapos[collidesanta].second] = 0;
            int newr = santapos[collidesanta].first + rudolfmove[rudir].first * c;
            int newc = santapos[collidesanta].second + rudolfmove[rudir].second * c;
            if ((newr < 1) || (newr > n) || (newc < 1) || (newc >> n)) {//탈락함
                available_santa--;
                if (available_santa <= 0)break;
                santaout[collidesanta] = 1;
            }
            else {
                santapos[collidesanta].first = newr;
                santapos[collidesanta].second = newc;


                // 상호작용
                // 연쇄작용(탈락확인,산타수업뎃) 보드업뎃 santapos업뎃

                while (board[newr][newc]) {
                    
                    int oldsanta = board[newr][newc];
                    board[newr][newc] = collidesanta;
                    santapos[collidesanta].first = newr;
                    santapos[collidesanta].second = newc;
                    newr = santapos[oldsanta].first + rudolfmove[rudir].first;
                    newc = santapos[oldsanta].second + rudolfmove[rudir].second;
                    santapos[oldsanta].first = newr;
                    santapos[oldsanta].second = newc;
                    if ((newr < 1) || (newr > n) || (newc < 1) || (newc >> n)) {//탈락함
                        available_santa--;
                        if (available_santa <= 0)break;
                        santaout[collidesanta] = 1;
                        break;
                    }
                    collidesanta = oldsanta;


                }
                
                if (available_santa <= 0)break;


            }

        }


       
        
        // 
        // 
        //산타 움직임 탈락, 기절산타빼고 - 이때 기절턴에서 2 지낫으면 기절 풀어주고 움직임
        
        for (int j = 1; j <= p; j++) {
            

            if (santaout[j])continue;
            if (santaparalyzed[j]) {
                if (i - santaparalyzed[j] < 2) {
                    continue;
                }
                else {
                    santaparalyzed[j] = 0;
                }

            }
            int santar = santapos[j].first;
            int santac = santapos[j].second;

            // 가장 가까워지는 방향 max(4) 기록- 산타가잇는칸, 게임판 밖 빼고,움직이지않을수도 잇음
            int closestd = (santar - rr) * (santar - rr) + (santac - rc) * (santac - rc);
            int sandir=-1;
            int collider = 0;
            

            for (int k = 0; k < 4; k++) {
                int newr = (santamove[k].first + santar);
                int newc = (santamove[k].second + santac);

                if (newr>n||newr<1||newc<1||newr>n) {//보드밖으로 나감
                    continue;
                }
                else if (board[newr][newc]==-1) {//루돌프랑 충돌
                    sandir = k;
                    collider = j;
                    break;



                }
                else if (board[newr][newc]) {//산타가있음
                    continue;
                }
                

                int newd = (newr - rr) * (newr - rr) + (newc - rc) * (newc - rc);
                if (newd < closestd) {
                    sandir = k;
                    santar = newr;
                    santac = newc;
                }


            }
            //sandir=-1인경우
            if (sandir == -1)continue;
            // 
            
            //루돌프랑 충돌한경우
             // 점수반영 d, 기절기록
        // 충돌후 밀려난위치 계산, 탈락확인, 밀려나는방향기록
        // 밀려난위치에 번호표시와 동시에 상호작용확인
        // 밀려남 탈락확인,산타수업뎃
        // 밀려난위치가 0일때까지
            if (collider) {
                //rr rc
                santascore[collider]+=d;
                santaparalyzed[collider] = i;

                int newr = santapos[collider].first + santamove[sandir].first * d*-1;
                int newc = santapos[collider].second + santamove[sandir].second * d*-1;
                if ((newr < 1) || (newr > n) || (newc < 1) || (newc >> n)) {//탈락함
                    available_santa--;
                    santaout[collidesanta] = 1;

                    if (available_santa <= 0)break;
                }

                else {
                    santapos[collider].first = newr;
                    santapos[collider].second = newc;


                    // 상호작용
                    // 연쇄작용(탈락확인,산타수업뎃) 보드업뎃 santapos업뎃

                    while (board[newr][newc]) {

                        int oldsanta = board[newr][newc];
                        board[newr][newc] = collider;
                        santapos[collider].first = newr;
                        santapos[collider].second = newc;
                        newr = santapos[oldsanta].first + santamove[sandir].first*-1;
                        newc = santapos[oldsanta].second + santamove[sandir].second*-1;
                        santapos[oldsanta].first = newr;
                        santapos[oldsanta].second = newc;
                        if ((newr < 1) || (newr > n) || (newc < 1) || (newc >> n)) {//탈락함
                            available_santa--;
                            if (available_santa <= 0)break;
                            santaout[collider] = 1;
                            break;
                        }
                        collider=oldsanta;


                    }

                    if (available_santa <= 0)break;


                }



            }
            



            //충돌하지않고움직인경우 
            //상호작용없음
            else {
                santapos[j].first = santar;
                santapos[j].second = santac;
                board[santar][santac] = j;

            }

            

            





        }

        if (available_santa <= 0)break;
        for (int j = 1; j <= p; j++) {
            if (santaout[j] == 0) {
                santascore[j]++;
            }

        }

        //산타수만큼 점수 up
   }
    
   for (int i = 1; i <= p; i++) {
       cout << santascore[i] << ' ';
   }

    return 0;
}