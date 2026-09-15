#include<iostream>
#include<string>
using namespace std;
int main () {
      ios_base::sync_with_stdio(false);
      cin.tie(NULL);
      string alphabet;
      while(cin >> alphabet) {
        int n;
        cin>> n;
        string msg="";
        for(int i=0; i<n;i++) {
            int bulb_no;
            cin >> bulb_no;
            msg+=alphabet[bulb_no-1];
            
        }
        cout << msg << endl;
      }
      return 0;
    }