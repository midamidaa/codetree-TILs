#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <fstream>

using namespace std;


int board[51][51] = { 0, };//루돌프는 -1

int score[31] = { 0, };
pair<int, int> pos[31];
pair<int, int> rudolf;
int kijul[31] = { 0, };
int talak[31] = { 0, };
int santanum;

int dr[4] = { -1,0,1,0 };
int dc[4] = { 0,1,0,-1 };

int rdr = 0;
int rdc = 0;

int n, m, p, c, d;

bool istalak(int x, int y) {
    if (x <= 0 || x > n || y <= 0 || y > n) return true;
    else return false;
}

int main() {

    cin >> n >> m >> p >> c >> d;
    cin >> rudolf.first >> rudolf.second;
    santanum = p;

    board[rudolf.first][rudolf.second] = -1;

    for (int i = 0; i < p; i++) {
        int pn;
        int sr, sc;
        cin >> pn >> sr >> sc;

        board[sr][sc] = pn;
        pos[pn].first = sr;
        pos[pn].second = sc;

    }

    for (int i = 1; i <= m; i++) {
        rdr = 0;
        rdc = 0;
        int closest = 20000;
        int idx = 0;
        for (int j = 1; j <= p; j++) {
            if (talak[j] == 0) {
               // cout << j << "번산타거리" << (pos[j].first - rudolf.first) * (pos[j].first - rudolf.first) + (pos[j].second - rudolf.second) * (pos[j].second - rudolf.second) << "\n";
                if ((pos[j].first - rudolf.first) * (pos[j].first - rudolf.first) + (pos[j].second - rudolf.second) * (pos[j].second - rudolf.second) < closest) {
                    idx = j;
                    closest = (pos[j].first - rudolf.first) * (pos[j].first - rudolf.first) + (pos[j].second - rudolf.second) * (pos[j].second - rudolf.second);
                }
                else if ((pos[j].first - rudolf.first) * (pos[j].first - rudolf.first) + (pos[j].second - rudolf.second) * (pos[j].second - rudolf.second) == closest) {
                    if (pos[idx].first < pos[j].first) {
                        closest = (pos[j].first - rudolf.first) * (pos[j].first - rudolf.first) + (pos[j].second - rudolf.second) * (pos[j].second - rudolf.second);

                        idx = j;

                    }
                    else if (pos[idx].first == pos[j].first) {
                        if (pos[idx].second < pos[j].second) {
                            idx = j;

                        }
                    }
                }
            }
        }
        //cout << "가장 가까운 산타: " << idx << '\n';

        board[rudolf.first][rudolf.second] = 0;


        if (pos[idx].first > rudolf.first) {
            rudolf.first++;
            rdr = 1;
        }
        else if (pos[idx].first < rudolf.first) {
            rudolf.first--;
            rdr = -1;
        }
        if (pos[idx].second > rudolf.second) {
            rudolf.second++;
            rdc = 1;
        }
        else if (pos[idx].second < rudolf.second) {
            rudolf.second--;
            rdc = -1;
        }

        //cout << "이동: " << rdr << rdc << '\n';


        if (board[rudolf.first][rudolf.second]) {
            kijul[idx] = i;
            score[idx] += c;
            board[rudolf.first][rudolf.second] = -1;
            if (istalak(rudolf.first + rdr * c, rudolf.second + rdc * c)) {
                santanum--;
                talak[idx] = 1;
                if (santanum == 0)break;
            }

            pos[idx].first = rudolf.first + rdr * c;
            pos[idx].second = rudolf.second + rdc * c;


            while (board[pos[idx].first][pos[idx].second]) {

                int tmp = board[pos[idx].first][pos[idx].second];
                board[pos[idx].first][pos[idx].second] = idx;
                idx = tmp;
                if (istalak(pos[idx].first + rdr, pos[idx].second + rdc)) {
                    santanum--;
                    talak[idx] = 1;
                    if (santanum == 0)break;
                }

                pos[idx].first = pos[idx].first + rdr;
                pos[idx].second = pos[idx].second + rdc;

                if (board[pos[idx].first][pos[idx].second] == 0) {

                    board[pos[idx].first][pos[idx].second] = idx;
                    break;
                }


            }
            board[pos[idx].first][pos[idx].second] = idx;

            
            if (santanum == 0)break;



        }
        else {
            board[rudolf.first][rudolf.second] = -1;
        }

       // cout << "boardafterrudolf\n";

        //for (int j = 1; j <= n; j++) {
        //    for (int k = 1; k <= n; k++) {
        //        cout << board[j][k] << ' ';
        //    }
        //    cout << '\n';
        //}

        for (int j = 1; j <= p; j++) {
           // cout << j << "번산타이동\n";

            if (kijul[j] != 0) {
                if (i-kijul[j] >= 2) {
                    kijul[j] = 0;
                    //cout << j << "번산타기절풀림\n";

                }
                else continue;
            }
            if (talak[j] == 1)continue;

            int dxdy = -1;

            int rudis = (pos[j].first - rudolf.first) * (pos[j].first - rudolf.first) + (pos[j].second - rudolf.second) * (pos[j].second - rudolf.second);
            int clodis = 1000000;
            for (int k = 3; k >= 0; k--) {
                if (istalak(pos[j].first + dr[k], pos[j].second + dc[k]))continue;
                if (board[pos[j].first + dr[k]][pos[j].second + dc[k]] > 0)continue;

                int dis = (pos[j].first + dr[k] - rudolf.first) * (pos[j].first + dr[k] - rudolf.first) + (pos[j].second + dc[k] - rudolf.second) * (pos[j].second + dc[k] - rudolf.second);
                if (rudis > dis) {
                    if (dis <= clodis) {
                        clodis = dis;
                        dxdy = k;
                    }

                }

            }
            if (dxdy == -1) {
                //cout << j << "번산타안움직임\n";
                continue;
            }




            if (board[pos[j].first + dr[dxdy]][pos[j].second + dc[dxdy]] == 0) {
                board[pos[j].first][pos[j].second] = 0;

                board[pos[j].first + dr[dxdy]][pos[j].second + dc[dxdy]] = j;

                pos[j].first += dr[dxdy];
                pos[j].second += dc[dxdy];
                continue;
            }


            //루돌프충돌

            score[j] += d;
            kijul[j] = i;
            //밀려남
            //cout << j << "번산타루돌프충돌\n";

            
            if (istalak(rudolf.first-d*dr[dxdy], rudolf.second-d*dc[dxdy])) {
                santanum--;
                talak[j] = 1;
                board[pos[j].first][pos[j].second] = 0;
                //cout << j << "번산타탈락\n";
                if (santanum == 0) {
                    break;
                }
                continue;
            }
            else {
                board[pos[j].first][pos[j].second] = 0;
                pos[j].first = rudolf.first-d * dr[dxdy];
                pos[j].second = rudolf.second-d * dc[dxdy];

            }

            int ii = j;

            while (board[pos[ii].first][pos[ii].second]) {
                int tmp = board[pos[ii].first][pos[ii].second];

                board[pos[ii].first][pos[ii].second] = ii;
                ii = tmp;

                pos[ii].first -= dr[dxdy];
                pos[ii].second -= dc[dxdy];

                if (istalak(pos[ii].first, pos[ii].second)) {
                    santanum--;
                    talak[ii] = 1;
                    //cout << ii << "번산타탈락\n";
                    break;
                }

            }
            board[pos[ii].first][pos[ii].second] = ii;


            if (santanum == 0)break;



        }




        for (int j = 1; j <= p; j++) {
            if (talak[j] == 0)score[j]++;
        }

        //cout << i << "번턴점수\n";
       /* for (int i = 1; i <= p; i++) {
            cout << score[i] << ' ';
        }*/
       /* cout << "\n탈락현황\n";
        for (int i = 1; i <= p; i++) {
            cout << talak[i] << ' ';
        }
        cout << "\n기절현황\n";
        for (int i = 1; i <= p; i++) {
            cout << kijul[i] << ' ';
        }
        cout << '\n'<<"boardafterturn\n";

        for (int j = 1; j <= n; j++) {
            for (int k = 1; k <= n; k++) {
                cout << board[j][k] << ' ';
            }
            cout << '\n';
        }
        cout << '\n';*/
    }


    for (int i = 1; i <= p; i++) {
        cout << score[i] << ' ';
    }

    return 0;
}