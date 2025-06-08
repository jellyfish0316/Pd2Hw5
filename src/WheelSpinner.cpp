#include "WheelSpinner.h"


void show_wheel(const vector<string>& problems, int current) {
    system("clear");
    cout << "🎰 題目轉盤 🎰" << endl << endl;
    for (int i = 0; i < problems.size(); ++i) {
        if (i == current) {
            cout << "👉 [" << problems[i] << "] 👈" << endl;
        } else {
            cout << "   " << problems[i] << endl;
        }
    }
}

int spin_wheel(const vector<string>& problems, mt19937& rng) {
    int n = problems.size();
    uniform_int_distribution<int> dist_start(0, n - 1);
    uniform_int_distribution<int> dist_spin(20, 49);  // 轉動次數

    int pos = dist_start(rng);
    int total_spin = dist_spin(rng);

    for (int i = 0; i < total_spin; ++i) {
        pos = (pos + 1) % n;
        show_wheel(problems, pos);
        this_thread::sleep_for(chrono::milliseconds(80 + i * 5));
    }
    return pos;
}