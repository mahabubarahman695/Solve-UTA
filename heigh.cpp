#include<iostream>
#include<algorithm>
#include<vector>
long long get_cost(long long n,long long y) {
    long long total_unit=n*y;
    long long cost=0;

    if(total_unit <= 50) {
        cost=(total_unit*10);
    }
    else if(total_unit <= 80) {
        cost=(50*10)+(total_unit-50)*15;
    }
    else if(total_unit <= 100) {
        cost=(50*10)+(30*15)+(total_unit-80)*20;
    }
    else {
        cost=cost=(50*10)+(30*15)+(20*20)+(total_unit-100)*25;
    }
    return cost;
}
using namespace std;
int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    long long x,y;
    cin >> x>> y;
    long long low=0, high=x/y;
    long long ans=0;
    while(low<=high) {
        long long mid = low+(high-low)/2;
        if(get_cost(mid,y)<=x) {
           ans=mid;
           low=mid+1;
        }
        else{
            high=mid-1;
        }
    }
    cout << ans<<endl;
    return 0;

    
}