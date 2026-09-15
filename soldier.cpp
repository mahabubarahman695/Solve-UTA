#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int sol;
    cin >> sol;
    vector<int>arr(sol);
    for(int i=0; i<sol; i++) {
        cin >> arr[i];
    }
    sort(arr.begin(), arr.end());
    int minimum=1;;
    int checkpoint=0;
    for(int i=0; i<sol; i++) {
        if(arr[i]>=minimum) {
           checkpoint++;
            minimum++;
        }
       
    }
    cout << checkpoint << endl;


    return 0;
}