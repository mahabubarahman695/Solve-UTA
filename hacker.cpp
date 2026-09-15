#include<iostream>
#include <vector>
#include<algorithm>
using namespace std;
int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    vector<int > m(n);
    for(int i=0; i<n;i++) {
        cin >> m[i];
    }
   int found=0;
   for(int i=0;i<n-1;i++) {
      if(m[i]==m[i+1]) {
           found++;
           break;
      }
   }
   if(found) {
    cout<<"Cheater detected\n";
   }
   else{
    cout<< "All clear\n";
   }
    return 0;
}