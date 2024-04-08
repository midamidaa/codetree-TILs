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
1185 1186 1187
*/

struct Query {
    int cmd;
    int t, x, n;
    string name;
};

bool cmp(Query a, Query b) {
    if (a.t != b.t) {
        return a.t < b.t;
    }
    return a.cmd < b.cmd;
}






//전체쿼리
vector<Query> queries;

//사람별 스시(100)
map<string, vector<Query> > people;

//사람별 입장타임(200)
map<string, int> entertime;
map<string, int> exittime;
set<string> names;
map<string, int> pos;

int shotcnt = 0;



int main() {

    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    // 여기에 코드를 작성해주세요.
    int l, q;
    cin >> l >> q;

    for (int i = 0; i < q; i++) {
        Query tmp;
        cin >> tmp.cmd >> tmp.t;
        if (tmp.cmd == 100) {
            cin >> tmp.x >> tmp.name;
        }
        else if (tmp.cmd == 200) {
            cin >> tmp.x >> tmp.name >> tmp.n;

        }

        queries.push_back(tmp);

        if (tmp.cmd == 100) {
            people[tmp.name].push_back(tmp);

        }
        else if (tmp.cmd == 200) {
            names.insert(tmp.name);
            entertime[tmp.name] = tmp.t;
            pos[tmp.name] = tmp.x;
        }
        


    }
    //사람마다

    set<string>::iterator iter;
    for (iter = names.begin(); iter != names.end(); iter++) {
        //사람별쿼리로 초밥 없어지는 시간계산
        string name = (*iter);
        for (int i = 0; i < people[name].size(); i++) {
            int chobaptime = people[name][i].t;//2
            long long chobappos = people[name][i].x;//2
            int persontime = entertime[name];//8
            int personpos = pos[name];//3
            int chobapx;

            if (chobaptime < persontime) {//초밥이먼저놓여짐

                //111쿼리추가
				chobappos += (persontime - chobaptime);
                chobappos %= l;//2 teddy가 왓을때 초밥위치
                if (personpos < chobappos) {
                    chobapx = persontime + personpos - chobappos + l;
                }
                else {
                    chobapx = persontime + personpos - chobappos;//3
                }


            }
            //동시에?
            else {//초밥이 사람보다 나중에 들어옴
                //없어지는 시간계산
                //초밥시간+l-(초밥위치-사람위치)
                if (personpos < chobappos) {
                    chobapx = chobaptime + personpos - chobappos + l;
                }
                else {
                    chobapx = chobaptime + personpos - chobappos;
                }


            }
            exittime[name] = chobapx;//2
            queries.push_back({ 111,chobapx,-1,-1,name});

        }
        
    }

    //222
    for (iter = names.begin(); iter != names.end(); iter++) {
        string name = (*iter);
        queries.push_back({ 222,exittime[name], -1, -1, name });

    }


    ///queries 정렬
    sort(queries.begin(), queries.end(), cmp);

 
    int curchobap = 0;
    int curpeople = 0;
    ///queries 순회
    for (int i = 0; i < queries.size(); i++) {
        if (queries[i].cmd == 100) {
            curchobap++;
        }
        else if (queries[i].cmd == 111) {//111이 중복
            curchobap--;
        }
        else if (queries[i].cmd == 200) {
            curpeople++;
        }
        else if (queries[i].cmd == 222) {
            curpeople--;
        }
        else if (queries[i].cmd == 300) {
            
            cout << curpeople << ' ' << curchobap << '\n';
        }
    }



    return 0;
}