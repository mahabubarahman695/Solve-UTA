#include<iostream> 
#include<vector>

using namespace std;
int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >>n;
    for(int i=0; i<n; i++) {
    string name;
    long long force;
    cin >> name >> force;
    
    if(name=="Thor") {
        cout << "Y\n";
    }
    else {
        cout << "N\n";
    }
    }
    return 0;
}