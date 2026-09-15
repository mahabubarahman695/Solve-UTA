#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;

int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
     long long h;
     cin >> h;
     vector<long long>arr(h);
     for(int i=0; i<h;i++) {
        cin >> arr[i];
        if(arr[i] <arr[i-1]) {
            arr[i-1]=arr[i]-1;
            if(arr[i-1]<0) {
                cout << "-1" << endl;  
            }
        cout << arr[i];
        }
        
     }
     return 0;

}
