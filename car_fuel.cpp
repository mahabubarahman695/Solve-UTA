#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n,k;
    cin >> n >> k;
    vector<long long>arr(n);
    for(int i=0; i<n; i++) {
        cin >> arr[i];
    }
    long long total=0;
    sort(arr.begin() , arr.end());
    if(n*10<k) {
        cout << -1 << endl;
        return 0;
    }
    for(int i=0; i<n;i++) {
           
        if(k==0){ break;}
        long long buy = min(10,k);
        total+=buy*arr[i];
        k-=buy;
        }
    cout << total<< endl;
    return 0;
 }

