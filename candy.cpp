#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    vector<long long> arr(n);
    for(int i=0; i< n; i++) {
        cin >> arr[i];
    }
    sort(arr.begin() , arr.end());
    long long bobble =0;
    long long aloo =0;
    int left = 0;
    int right = n-1;
    int round = 1;
    while(left < right) {
        if(round%2 != 0) {
            bobble += arr[right];
            right--;
            aloo+= arr[left];
            left++;
        }
        else{
          bobble += arr[left];
            left++;
            aloo+= arr[right];
            right--;
        }
        round++;
    }
    cout << bobble << " " << aloo << endl;
    return 0;

}