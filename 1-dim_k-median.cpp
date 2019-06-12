#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int median_index(int i, int j){
    if((j-i) % 2 == 0){
        return (i+j)/2;
    }
    else{
        return (i+j+1)/2;
    }
}


int cluster_cost(int lower, int upper, vector<int> numbers, vector<int> prefix_sums){
    int m_ind = median_index(lower,upper);

    if((upper-lower) % 2 == 0){
        return ( (prefix_sums[upper] - prefix_sums[m_ind]) - (prefix_sums[m_ind-1] - prefix_sums[lower-1]) );
        }
    else{
        return ( (prefix_sums[upper] - prefix_sums[m_ind]) - (prefix_sums[m_ind-1] - prefix_sums[lower-1]) + numbers[m_ind] );
    }

}


vector<int> prefix_sums(vector<int> numbers){
    vector<int> ps(numbers.size());
    for (int i = 1; i < numbers.size(); ++i) {
        ps[i] = ps[i-1] + numbers[i];
    }
    return ps;
}


void initialize_1_medians(vector<vector<int>> &D, vector<int> numbers, vector<int> pre_sums){
    for(int j = 2; j < D[0].size(); ++j){
        D[1][j] = cluster_cost(1,j,numbers,pre_sums);
    }
    return;
}


int get_min_cost(vector<vector<int>> D, vector<int> numbers, vector<int> pre_sums,int i, int m){
    int min = numeric_limits<int>::max();
    int index_min = 0;

    for (int j = 0; j <= m; ++j) {
        int cost = D[i-1][j-1] + cluster_cost(j,m,numbers,pre_sums);
        if(cost < min){
            min = cost;
            index_min = j;
        }
    }
    return min;
}



void fill_matrix(vector<vector<int>> &D, vector<int> numbers, vector<int> pre_sums){
    for (int i = 2; i < D.size(); ++i) {
        for (int m = 1; m < D[0].size(); ++m) {
            D[i][m] = get_min_cost(D,numbers,pre_sums,i,m);
        }
    }
}




int main() {

    int num_centers, num_points;


    cin >> num_centers;
    cin >> num_points;
    vector<int> numbers(num_points+1);

    for (int i = 0; i < num_points; ++i) {
        int number;
        cin >> number;
        numbers[i+1] = number;
    }

    vector<vector<int>> D(num_centers+1,vector<int>(num_points+1));



    sort(numbers.begin(),numbers.end());
    vector<int> pre_sums = prefix_sums(numbers);
    initialize_1_medians(D,numbers,pre_sums);
    fill_matrix(D,numbers,pre_sums);

    cout << 25*num_points - D[num_centers][num_points];
    return EXIT_SUCCESS;
}
