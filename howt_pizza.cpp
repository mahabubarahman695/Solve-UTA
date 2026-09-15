#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    long long n;
    cin >> n;
    vector<long long> arr(n);
    for(int i=0; i<n; i++) {
        cin >> arr[i];
    }
    long long total=0;
    sort(arr.begin() , arr.end());
    for(int i=0; i<n; i++) {
         int current=arr[i]-i;
         total+=current;
    }
    cout << total <<endl;
}