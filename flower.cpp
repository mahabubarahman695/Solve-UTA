#include<iostream>
#include<vector>
#include<algorithm>


using namespace std;
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
     int n, k, m;
     cin >> n >> k >>m;
     vector<int> arr(n);
     for(int i=0; i< n; i++) {
        cin >> arr[i];
     }
     sort(arr.begin() ,arr.end());
     int cost=0;
     
     vector<long long>selected;

     for(int i=0; i< n; i++) {
          selected.push_back(arr[i]);
            cost+=arr[i];

          }
     }
       for(int i=0; i< n; i++) {
       
              expensive=arr[i];

          }
     }

    return 0;
}