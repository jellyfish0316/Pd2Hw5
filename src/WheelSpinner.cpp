
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

int spin_wheel(const vector<string>& problems) {
    int n = problems.size();
    int pos = rand() % n;
    int total_spin = rand() % 30 + 20;  // 轉動次數

    for (int i = 0; i < total_spin; ++i) {
        pos = (pos + 1) % n;
        show_wheel(problems, pos);
        this_thread::sleep_for(chrono::milliseconds(80 + i * 5));  // 越來越慢
    }
    return pos;
}
/*
int main() {
    srand(time(0));

    vector<string> problems = {
        "Problem 01", "Problem 02", "Problem 03", "Problem 04", "Problem 05",
        "Problem 06", "Problem 07", "Problem 08", "Problem 09", "Problem 10"
    };

    cout << "按 Enter 開始轉盤...";
    cin.get();

    int result = spin_wheel(problems);

    cout << "\n🎉 恭喜抽到題目：" << problems[result] << "！" << endl;
    cout << "📄 檔案路徑：/problems/" << result + 1 << "/in.txt /out.txt" << endl;
    cout << "🪄 Magic Number: " << rand() % 1000 + 1 << endl;

    return 0;
}*/
