#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    using Iter = std::vector<int>::iterator;
    using PIter = std::pair<Iter,Iter>;
    struct comp {
        bool operator()(PIter p1,PIter p2) {
            return *p1.first > *p2.first;
        }
    };

    vector<int> smallestRange(vector<vector<int>>& nums) {    
        priority_queue<PIter,vector<PIter>,comp> pq;
        int hi = -10e5 - 1;
        int lo = 10e5 + 1;
        for(auto && e:nums) {
            hi = max(hi,e[0]);
            lo = min(lo,e[0]);
            pq.push({e.begin(),e.end()});
        }
        vector<int> ans = {lo,hi};
        while(true) {
            auto p = pq.top();
            pq.pop();
            ++(p.first);
            if(p.first == p.second) {
                break;
            }
            pq.push(p);
            hi = max(hi,*p.first);
            lo = *pq.top().first;
            if(hi - lo < ans[1] - ans[0]) {
                ans = {lo,hi};
            }
        }
        return ans;
    }
};

int main() {


}