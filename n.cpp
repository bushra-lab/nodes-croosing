#include <iostream>
#include <fstream>
#include <string>
#include<sstream>
#include <vector>
#include <cstdio>
#include <cmath>

void function_(int number_of_rows);

using namespace std;

int get_number_of_lines(const string& path){
    ifstream f;
    f.open(path);
    string line;
    int number_of_lines = 0;
    if ( f.is_open() ) {
        while (f) {
            getline(f, line);
            number_of_lines++;
        }
    }
    return number_of_lines;
}

void initialize_col(vector<double> &columns, const string& path, const int& number_of_rows) {
    vector<double> temp_line;
    ifstream f;
    f.open(path);
    string line;
    double x;
    int num_of_items_pushed = 0;
    if (f.is_open() ) {
        while ( f ) {
            getline (f, line);
            stringstream s(line); // Used for breaking words
            string word; // to store individual words
            while (s >> word){
                stringstream geek(word);
                geek >> x;
                temp_line.push_back(x);
            }
            if(!temp_line.empty()) {
                for (int i = 0; i < temp_line.size(); i++) {
                    columns[(i * number_of_rows) + num_of_items_pushed] = temp_line[i];
                }
                num_of_items_pushed++;
            }
            temp_line.clear();
        }
        f.close();
    }
    else {
        cout << "Couldn't open file\n";
    }
}

void swap(vector<double>& columns, const int& i, const int& j, const int& number_of_rows){
    double swap;
    for(int dim = 0; dim < 5; dim++){
        swap = columns[dim * number_of_rows + i];
        columns[dim * number_of_rows + i] = columns[dim * number_of_rows + j];
        columns[dim * number_of_rows + j] = swap;
    }
}

int partition (vector<double>& columns, const int& low, const int& high, const int& number_of_rows){
    int pivot = columns[4 * number_of_rows + (random() % number_of_rows)]; // pivot
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++){
        if (columns[4 * number_of_rows + j] < pivot){
            i++;
            swap(columns, i,  j, number_of_rows);
        }
    }
    swap(columns, i + 1,  high, number_of_rows);
    return (i + 1);
}

void quickSort(vector<double>& columns, const int& low, const int& high, const int& number_of_rows){
    if (low < high){
        int pi = partition(columns, low, high, number_of_rows);
        printf("pi: %d\n", pi);
        quickSort(columns, low, pi - 1, number_of_rows);
        quickSort(columns, pi + 1, high, number_of_rows);
    }
}

int main() {
    int number_of_rows = get_number_of_lines("/Users/bushra/Downloads/crossing36(1).dat");
    vector<double> cols(5 * number_of_rows);
    initialize_col(cols, "/Users/bushra/Downloads/crossing36(1).dat", number_of_rows);
    FILE *fptr = fopen("/Users/bushra/Downloads/output.dat", "w");
    double max_fifth_col = 0.;
    double min_fifth_col = 999999.9;
    vector<double> chunk;
    for(int i = 0; i < number_of_rows; i++){
        for(int k = 0; k < number_of_rows; k++){
            double dist = sqrt(((cols[i] - cols[k]) * (cols[i] - cols[k]))
                               + ((cols[number_of_rows + i] - cols[number_of_rows + k]) * (cols[number_of_rows + i] - cols[number_of_rows + k]))
                               + ((cols[(number_of_rows << 1) + i] - cols[(number_of_rows << 1) + k]) * (cols[(number_of_rows << 1) + i] - cols[(number_of_rows << 1) + k])));
            if(cols[(number_of_rows << 2) + k] > max_fifth_col)
                max_fifth_col = cols[(number_of_rows << 2) + k]; // If there is a number greater than the
            // current value of max then max becomes that number
            if(min_fifth_col > cols[(number_of_rows << 2) + k])
                min_fifth_col = cols[(number_of_rows << 2) + k]; // If there is a number less than min
            // then min gets that value
            if(dist  < 0.05 and cols[(number_of_rows << 2) + k] < 13.17 and cols[(number_of_rows << 2) + k] > 13.1599){

                for(int ctr = 0; ctr < 5; ctr++){
                    chunk.push_back(cols[(ctr * number_of_rows) + k]);
                }
            }
        }
        if(chunk.size() / 5 > 3){
            for(int index = 0; index < chunk.size(); index = index + 5){
                fprintf(fptr, "%lf %lf %lf %lf %lf\n", cols[index], cols[index + 1], cols[index + 2],
                        cols[index + 3], cols[index +  4]);
            }
        }
        chunk.clear();
        fprintf(fptr, "\n");
    }
    cout << max_fifth_col << " " << min_fifth_col << endl;
    fclose(fptr);
//    cols.clear();
//    number_of_rows = get_number_of_lines("/Users/mokazemi-93/Downloads/output.dat");
//    cols.clear();
//    function_(number_of_rows);
    return 0;
}

void function_(int number_of_rows) {
    FILE *fptr;
    vector<double> cols(5 * number_of_rows);
    initialize_col(cols, "/Users/bushra/Downloads/output.dat", number_of_rows);

    cout << "going for quickSort" <<  endl;

    vector<double> v = {3., 4., 2., 1., 5.};

    quickSort(v, 0, 0, 1);


    fptr = fopen("/Users/bushra/Downloads/output_2.dat", "w");
    for(int i = 0; i < number_of_rows; i++){
        fprintf(fptr, "%lf %lf %lf %lf %lf\n", cols[i], cols[number_of_rows + i], cols[(number_of_rows << 1) + i], cols[3 * number_of_rows + i], cols[(number_of_rows << 2) + i]);
        printf("%lf %lf %lf %lf %lf\n", cols[i], cols[number_of_rows + i], cols[(number_of_rows << 1) + i], cols[3 * number_of_rows + i], cols[(number_of_rows << 2) + i]);
    }
    fclose(fptr);
}
